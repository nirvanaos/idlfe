#include "Builder.h"
#include "Enum.h"
#include "SafeInt/SafeInt.hpp"
#include "decNumber.h"
extern "C" {
#include <decNumber/decPacked.h>
}
#include <stdexcept>
#include <limits>
#include <cfenv>
#include <assert.h>

using namespace std;

namespace AST {

Variant::~Variant ()
{
	clear ();
}

Variant::Variant (Variant&& src) noexcept :
	Type (move (src))
{
	_move (src);
	src.reset ();
}

Variant& Variant::operator = (const Variant& src)
{
	clear ();
	copy (src);
	return *this;
}

Variant& Variant::operator = (Variant&& src) noexcept
{
	clear ();
	_move (src);
	src.reset ();
	return *this;
}

void Variant::_move (const Variant& src)
{
	if ((is_const_ref_ = src.is_const_ref_))
		val_.const_ref = src.val_.const_ref;
	else
		memcpy (&val_, &src.val_, sizeof (val_));
}

void Variant::clear () noexcept
{
	if (!is_const_ref_) {
		switch (dereference_type ().kind ()) {
			case Kind::STRING:
				val_.s.~basic_string ();
				break;
			case Kind::WSTRING:
				val_.ws.~basic_string ();
				break;
		}
	}
	reset ();
}

void Variant::copy (const Variant& src)
{
	Type::copy (src);
	if ((is_const_ref_ = src.is_const_ref_))
		val_.const_ref = src.val_.const_ref;
	else {
		switch (dereference_type ().kind ()) {
			case Kind::STRING:
				new (&val_.s) string (src.val_.s);
				break;

			case Kind::WSTRING:
				new (&val_.ws) wstring (src.val_.ws);
				break;

			default:
				memcpy (&val_, &src.val_, sizeof (val_));
		}
	}
}

Variant::Variant (bool v) noexcept :
	Type (BasicType::BOOLEAN),
	is_const_ref_ (false)
{
	val_.b = v;
}

Variant::Variant (char v) noexcept :
	Type (BasicType::CHAR),
	is_const_ref_ (false)
{
	val_.ui = v;
}

Variant::Variant (wchar_t v) noexcept :
	Type (BasicType::WCHAR),
	is_const_ref_ (false)
{
	val_.ui = v;
}

Variant::Variant (int32_t v) noexcept :
	Type (BasicType::LONG),
	is_const_ref_ (false)
{
	val_.i = v;
}

Variant::Variant (uint32_t v) noexcept :
	Type (BasicType::ULONG),
	is_const_ref_ (false)
{
	val_.ui = v;
}

Variant::Variant (int64_t v) noexcept :
	Type (BasicType::LONGLONG),
	is_const_ref_ (false)
{
	val_.i = v;
}

Variant::Variant (uint64_t v) noexcept :
	Type (BasicType::ULONGLONG),
	is_const_ref_ (false)
{
	val_.ui = v;
}

Variant::Variant (float v) noexcept :
	Type (BasicType::FLOAT),
	is_const_ref_ (false)
{
	val_.d = v;
}

Variant::Variant (double v) noexcept :
	Type (BasicType::DOUBLE),
	is_const_ref_ (false)
{
	val_.d = v;
}

Variant::Variant (long double v) noexcept :
	Type (BasicType::LONGDOUBLE),
	is_const_ref_ (false)
{
	val_.d = v;
}

Variant::Variant (const _decNumber& v) noexcept :
	Type (v.digits, -v.exponent),
	is_const_ref_ (false)
{
	int scale;
	decPackedFromNumber (val_.fixed, bcd_length (v.digits), &scale, &v);
	assert (fixed_scale () == scale);
}

Variant::Variant (const Constant& constant) noexcept :
	Type (constant),
	is_const_ref_ (true)
{
	val_.const_ref = &constant;
}

Variant::Variant (const EnumItem& item) noexcept :
	Type (&item.enum_type ()),
	is_const_ref_ (false)
{
	val_.enum_item = &item;
}

const Variant& Variant::dereference_const () const noexcept
{
	const Variant* v = this;
	while (v->is_const_ref_) {
		v = v->val_.const_ref;
	}
	return *v;
}

[[noreturn]] void Variant::throw_out_of_range ()
{
	throw range_error ("Value out of range.");
}

uint8_t Variant::to_octet () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	uint8_t ret;
	BasicType bt = v.dereference_type ().basic_type ();
	if (bt == BasicType::OCTET)
		ret = (uint8_t)v.val_.ui;
	else if (is_signed (bt) ? SafeCast (v.val_.i, ret) : SafeCast (v.val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

char Variant::to_char () const
{
	assert (is_integer ());
	if (dereference_type ().basic_type () == BasicType::CHAR)
		return (char)dereference_const ().val_.ui;
	else
		return (char)to_octet ();
}

wchar_t Variant::to_wchar () const
{
	assert (is_integer ());
	if (dereference_type ().basic_type () == BasicType::WCHAR)
		return (wchar_t)dereference_const ().val_.ui;
	else
		return (wchar_t)to_unsigned_short ();
}

uint16_t Variant::to_unsigned_short () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	uint16_t ret;
	BasicType bt = v.dereference_type ().basic_type ();
	if (bt == BasicType::USHORT)
		ret = (uint16_t)v.val_.ui;
	else if (is_signed (bt) ? SafeCast (v.val_.i, ret) : SafeCast (v.val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

int16_t Variant::to_short () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	int16_t ret;
	BasicType bt = v.dereference_type ().basic_type ();
	if (bt == BasicType::SHORT)
		ret = (int16_t)v.val_.i;
	else if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint32_t Variant::to_unsigned_long () const
{
	assert (is_integer ());
	uint32_t ret;
	if (basic_type () == BasicType::SHORT)
		ret = (uint32_t)val_.ui;
	else if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

int32_t Variant::to_long () const
{
	assert (is_integer ());
	int32_t ret;
	if (basic_type () == BasicType::SHORT)
		ret = (int32_t)val_.i;
	else if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint64_t Variant::to_unsigned_long_long () const
{
	assert (is_integer ());
	uint64_t ret;
	if (is_signed (basic_type ())) {
		if (SafeCast (val_.i, ret))
			throw_out_of_range ();
	} else
		ret = val_.ui;
	return ret;
}

int64_t Variant::to_long_long () const
{
	assert (is_integer ());
	int64_t ret;
	if (!is_signed (basic_type ())) {
		if (SafeCast (val_.ui, ret))
			throw_out_of_range ();
	} else
		ret = val_.i;
	return ret;
}

void Variant::check_fp ()
{
	int ex = fetestexcept (FE_ALL_EXCEPT);
	if (ex & FE_OVERFLOW)
		throw overflow_error ("Conversion overflow.");
	else if (ex & FE_UNDERFLOW)
		throw underflow_error ("Conversion underflow.");
}

float Variant::to_float () const
{
	assert (is_floating_pt ());
	float ret;
	feclearexcept (FE_ALL_EXCEPT);
	if (basic_type () == BasicType::FLOAT)
		ret = (float)val_.d;
	else {
		ret = (float)val_.d;
		check_fp ();
	}
	return ret;
}

double Variant::to_double () const
{
	assert (is_floating_pt ());
	double ret;
	feclearexcept (FE_ALL_EXCEPT);
	if (basic_type () == BasicType::DOUBLE)
		ret = (double)val_.d;
	else {
		ret = (double)val_.d;
		check_fp ();
	}
	return ret;
}

long double Variant::to_long_double () const
{
	assert (is_floating_pt ());
	feclearexcept (FE_ALL_EXCEPT);
	return val_.d;
}

void Variant::as_decNumber (_decNumber& dn) const noexcept
{
	assert (kind () == Type::Kind::FIXED);
	int scale = fixed_scale ();
	decPackedToNumber (val_.fixed, bcd_length (), &scale, &dn);
	assert (fixed_scale () == scale);
}

}
