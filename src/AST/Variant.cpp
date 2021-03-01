#include "Variant.h"
#include "SafeInt/SafeInt.hpp"
extern "C" {
#include <decNumber/decNumber.h>
#include <decNumber/decPacked.h>
}
#include <stdexcept>
#include <limits>
#include <assert.h>

using namespace std;

namespace AST {

Variant::Variant (Variant&& src) noexcept :
	Type (move (src))
{
	memcpy (&val_, &src.val_, sizeof (val_));
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
	memcpy (&val_, &src.val_, sizeof (val_));
	src.reset ();
	return *this;
}

void Variant::clear () noexcept
{
	switch (kind ()) {
		case Kind::STRING:
			val_.s.~basic_string ();
			break;
		case Kind::WSTRING:
			val_.ws.~basic_string ();
			break;
	}
	reset ();
}

void Variant::copy (const Variant& src)
{
	Type::copy (src);
	switch (kind ()) {
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

Variant::Variant (bool v) noexcept :
	Type (BasicType::BOOLEAN)
{
	val_.b = v;
}

Variant::Variant (char v) noexcept :
	Type (BasicType::CHAR)
{
	val_.ui = v;
}

Variant::Variant (wchar_t v) noexcept :
	Type (BasicType::WCHAR)
{
	val_.ui = v;
}

Variant::Variant (int32_t v) noexcept :
	Type (BasicType::LONG)
{
	val_.i = v;
}

Variant::Variant (uint32_t v) noexcept :
	Type (BasicType::ULONG)
{
	val_.ui = v;
}

Variant::Variant (int64_t v) noexcept :
	Type (BasicType::LONGLONG)
{
	val_.i = v;
}

Variant::Variant (uint64_t v) noexcept :
	Type (BasicType::ULONGLONG)
{
	val_.ui = v;
}

Variant::Variant (double v) noexcept :
	Type (BasicType::DOUBLE)
{
	val_.d = v;
}

Variant::Variant (long double v) noexcept :
	Type (BasicType::LONGDOUBLE)
{
	val_.d = v;
}

Variant::Variant (const _decNumber& v) noexcept :
	Type (v.digits, -v.exponent)
{
	int scale;
	decPackedFromNumber (val_.fixed, bcd_length (v.digits), &scale, &v);
	assert (fixed_scale () == scale);
}

[[noreturn]] void Variant::throw_out_of_range ()
{
	throw range_error ("Value out of range.");
}

uint8_t Variant::to_octet () const
{
	assert (is_integer ());
	uint8_t ret;
	if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint16_t Variant::to_unsigned_short () const
{
	assert (is_integer ());
	uint16_t ret;
	if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

int16_t Variant::to_short () const
{
	assert (is_integer ());
	int16_t ret;
	if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint32_t Variant::to_unsigned_long () const
{
	assert (is_integer ());
	uint32_t ret;
	if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
		throw_out_of_range ();
	return ret;
}

int32_t Variant::to_long () const
{
	assert (is_integer ());
	int32_t ret;
	if (is_signed (basic_type ()) ? SafeCast (val_.i, ret) : SafeCast (val_.ui, ret))
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

}
