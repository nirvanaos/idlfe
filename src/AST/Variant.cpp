#include "Variant.h"
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

Variant::Variant (char v) :
	Type (BasicType::CHAR)
{
	val_.ui = v;
}

Variant::Variant (wchar_t v) :
	Type (BasicType::WCHAR)
{
	val_.ui = v;
}

Variant::Variant (unsigned long v) :
	Type (BasicType::ULONG)
{
	val_.ui = v;
}

Variant::Variant (unsigned long long v) :
	Type (BasicType::ULONGLONG)
{
	val_.ui = v;
}

Variant::Variant (double v) :
	Type (BasicType::DOUBLE)
{
	val_.d = v;
}

Variant::Variant (long double v) :
	Type (BasicType::LONGDOUBLE)
{
	val_.d = v;
}

Variant::Variant (const _decNumber& v) :
	Type (v.digits, -v.exponent)
{
	int scale;
	decPackedFromNumber (val_.fixed, bcd_length (v.digits), &scale, &v);
	assert (fixed_scale () == scale);
}

}
