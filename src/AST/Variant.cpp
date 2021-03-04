#include "Builder.h"
#include "Constant.h"
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
	val_.plain = src.val_.plain;
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
	val_.plain = src.val_.plain;
	src.reset ();
	return *this;
}

void Variant::clear () noexcept
{
	if (!val_.v.constant) {
		switch (dereference_type ().kind ()) {
			case Kind::STRING:
				val_.v.u.s.~basic_string ();
				break;
			case Kind::WSTRING:
				val_.v.u.ws.~basic_string ();
				break;
		}
	}
	reset ();
}

void Variant::copy (const Variant& src)
{
	Type::copy (src);
	if (!(val_.v.constant = src.val_.v.constant)) {
		switch (dereference_type ().kind ()) {
			case Kind::STRING:
				new (&val_.v.u.s) string (src.val_.v.u.s);
				break;

			case Kind::WSTRING:
				new (&val_.v.u.ws) wstring (src.val_.v.u.ws);
				break;

			default:
				val_.plain = src.val_.plain;
		}
	}
}

Variant::Variant (bool v) noexcept :
	Type (BasicType::BOOLEAN),
	val_ (v)
{}

Variant::Variant (char v) noexcept :
	Type (BasicType::CHAR),
	val_ ((uint64_t)v)
{}

Variant::Variant (wchar_t v) noexcept :
	Type (BasicType::WCHAR),
	val_ ((uint64_t)v)
{}

Variant::Variant (int32_t v) noexcept :
	Type (BasicType::LONG),
	val_ ((int64_t)v)
{}

Variant::Variant (uint32_t v) noexcept :
	Type (BasicType::ULONG),
	val_ ((uint64_t)v)
{}

Variant::Variant (int64_t v) noexcept :
	Type (BasicType::LONGLONG),
	val_ ((int64_t)v)
{}

Variant::Variant (uint64_t v) noexcept :
	Type (BasicType::ULONGLONG),
	val_ (v)
{}

Variant::Variant (float v) noexcept :
	Type (BasicType::FLOAT),
	val_ ((long double)v)
{}

Variant::Variant (double v) noexcept :
	Type (BasicType::DOUBLE),
	val_ ((long double)v)
{}

Variant::Variant (long double v) noexcept :
	Type (BasicType::LONGDOUBLE),
	val_ (v)
{}

Variant::Variant (const _decNumber& v) noexcept :
	Type (v.digits, -v.exponent)
{
	int scale;
	decPackedFromNumber (val_.v.u.fixed, bcd_length (v.digits), &scale, &v);
	assert (fixed_scale () == scale);
}

Variant::Variant (const Constant& constant) noexcept :
	Type (constant),
	val_ (constant)
{}

Variant::Variant (const EnumItem& item) noexcept :
	Type (&item.enum_type ()),
	val_ (item)
{}

const Variant& Variant::dereference_const () const noexcept
{
	const Variant* v = this;
	while (const Constant* c = v->val_.v.constant) {
		v = c;
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
		ret = (uint8_t)v.val_.v.u.ui;
	else if (is_signed (bt) ? SafeCast (v.val_.v.u.i, ret) : SafeCast (v.val_.v.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

char Variant::to_char () const
{
	assert (is_integer ());
	if (dereference_type ().basic_type () == BasicType::CHAR)
		return (char)dereference_const ().val_.v.u.ui;
	else
		return (char)to_octet ();
}

wchar_t Variant::to_wchar () const
{
	assert (is_integer ());
	if (dereference_type ().basic_type () == BasicType::WCHAR)
		return (wchar_t)dereference_const ().val_.v.u.ui;
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
		ret = (uint16_t)v.val_.v.u.ui;
	else if (is_signed (bt) ? SafeCast (v.val_.v.u.i, ret) : SafeCast (v.val_.v.u.ui, ret))
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
		ret = (int16_t)v.val_.v.u.i;
	else if (is_signed (bt) ? SafeCast (v.val_.v.u.i, ret) : SafeCast (v.val_.v.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint32_t Variant::to_unsigned_long () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	uint32_t ret;
	BasicType bt = v.dereference_type ().basic_type ();
	if (bt == BasicType::ULONG)
		ret = (uint32_t)v.val_.v.u.ui;
	else if (is_signed (bt) ? SafeCast (v.val_.v.u.i, ret) : SafeCast (v.val_.v.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

int32_t Variant::to_long () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	int32_t ret;
	BasicType bt = v.dereference_type ().basic_type ();
	if (bt == BasicType::LONG)
		ret = (int32_t)v.val_.v.u.i;
	else if (is_signed (bt) ? SafeCast (v.val_.v.u.i, ret) : SafeCast (v.val_.v.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint64_t Variant::to_unsigned_long_long () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	uint64_t ret;
	if (is_signed (v.dereference_type ().basic_type ())) {
		if (SafeCast (v.val_.v.u.i, ret))
			throw_out_of_range ();
	} else
		ret = v.val_.v.u.ui;
	return ret;
}

int64_t Variant::to_long_long () const
{
	assert (is_integer ());
	const Variant& v = dereference_const ();
	int64_t ret;
	if (!is_signed (v.dereference_type ().basic_type ())) {
		if (SafeCast (v.val_.v.u.ui, ret))
			throw_out_of_range ();
	} else
		ret = v.val_.v.u.i;
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
	feclearexcept (FE_ALL_EXCEPT);
	float ret = (float)dereference_const ().val_.v.u.d;
	check_fp ();
	return ret;
}

double Variant::to_double () const
{
	assert (is_floating_pt ());
	feclearexcept (FE_ALL_EXCEPT);
	double ret = (double)dereference_const ().val_.v.u.d;
	check_fp ();
	return ret;
}

long double Variant::to_long_double () const
{
	assert (is_floating_pt ());
	feclearexcept (FE_ALL_EXCEPT);
	return dereference_const ().val_.v.u.d;
}

void Variant::as_decNumber (_decNumber& dn) const noexcept
{
	assert (dereference_type ().kind () == Type::Kind::FIXED);
	int scale = fixed_scale ();
	decPackedToNumber (val_.v.u.fixed, bcd_length (), &scale, &dn);
	assert (fixed_scale () == scale);
}

}
