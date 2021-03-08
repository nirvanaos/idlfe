/*
* Nirvana IDL Front End Library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  silver.popov@google.com
*/
#include "../include/AST/Constant.h"
#include "../include/AST/Enum.h"
#include "Builder/SafeInt/SafeInt.hpp"
#include "Builder/decNumber.h"
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

Variant::Variant (Variant&& src) noexcept
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
	switch (type_) {
		case VT::STRING:
			val_.u.s.~basic_string ();
			break;
		case VT::WSTRING:
			val_.u.ws.~basic_string ();
			break;
	}
	reset ();
}

void Variant::copy (const Variant& src)
{
	switch (src.type_) {
		case VT::STRING:
			new (&val_.u.s) string (src.val_.u.s);
			break;
		case VT::WSTRING:
			new (&val_.u.ws) wstring (src.val_.u.ws);
			break;
		default:
			val_.plain = src.val_.plain;
	}
	type_ = src.type_;
}

const Variant& Variant::dereference_const () const noexcept
{
	const Variant* v = this;
	while (v->vtype () == VT::CONSTANT) {
		v = &v->as_constant ();
	}
	return *v;
}

[[noreturn]] void Variant::throw_out_of_range ()
{
	throw range_error ("Value out of range.");
}

uint8_t Variant::to_octet () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	uint8_t ret;
	if (is_signed () ? SafeCast (v.val_.u.i, ret) : SafeCast (v.val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint16_t Variant::to_unsigned_short () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	uint16_t ret;
	if (is_signed () ? SafeCast (v.val_.u.i, ret) : SafeCast (v.val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

int16_t Variant::to_short () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	int16_t ret;
	if (is_signed () ? SafeCast (v.val_.u.i, ret) : SafeCast (v.val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint32_t Variant::to_unsigned_long () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	uint32_t ret;
if (is_signed () ? SafeCast (v.val_.u.i, ret) : SafeCast (v.val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

int32_t Variant::to_long () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	int32_t ret;
if (is_signed () ? SafeCast (v.val_.u.i, ret) : SafeCast (v.val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

uint64_t Variant::to_unsigned_long_long () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	uint64_t ret;
	if (is_signed ()) {
		if (SafeCast (v.val_.u.i, ret))
			throw_out_of_range ();
	} else
		ret = v.val_.u.ui;
	return ret;
}

int64_t Variant::to_long_long () const
{
	assert (is_integral ());
	const Variant& v = dereference_const ();
	int64_t ret;
	if (!is_signed ()) {
		if (SafeCast (v.val_.u.ui, ret))
			throw_out_of_range ();
	} else
		ret = v.val_.u.i;
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
	assert (is_floating_point () || is_integral ());
	float ret;
	if (is_floating_point ()) {
		feclearexcept (FE_ALL_EXCEPT);
		ret = (float)val_.u.d;
		check_fp ();
	} else if (is_signed () ? SafeCast (val_.u.i, ret) : SafeCast (val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

double Variant::to_double () const
{
	assert (is_floating_point () || is_integral ());
	double ret;
	if (is_floating_point ()) {
		feclearexcept (FE_ALL_EXCEPT);
		ret = (double)val_.u.d;
		check_fp ();
	} else if (is_signed () ? SafeCast (val_.u.i, ret) : SafeCast (val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

long double Variant::to_long_double () const
{
	assert (is_floating_point () || is_integral ());
	long double ret;
	if (is_floating_point ())
		ret = val_.u.d;
	else if (is_signed () ? SafeCast (val_.u.i, ret) : SafeCast (val_.u.ui, ret))
		throw_out_of_range ();
	return ret;
}

string Variant::to_string () const
{
	string s;

	switch (type_) {
		case VT::CHAR: {
			s.reserve (3);
			s += '\'';
			append (s, as_char ());
			s += '\'';
		} break;
		case VT::WCHAR: {
			s.reserve (3);
			s += '\'';
			append (s, as_wchar ());
			s += '\'';
		} break;
		case VT::STRING: {
			s.reserve (val_.u.s.size () + 2);
			s += '\"';
			for (auto pc = val_.u.s.begin (); pc != val_.u.s.end (); ++pc)
				append (s, *pc);
			s += '\"';
		} break;
		case VT::WSTRING: {
			s.reserve (val_.u.ws.size () + 2);
			s += '\"';
			for (auto pc = val_.u.ws.begin (); pc != val_.u.ws.end (); ++pc)
				append (s, *pc);
			s += '\"';
		} break;
		case VT::FIXED: {
			char buf [31 + 14];
			assert (val_.u.fixed.exponent > -31);
			decNumberToString (&as_decNumber (), buf);
			s = buf;
		} break;
		case VT::ENUM_ITEM:
			s = val_.u.enum_item->qualified_name ();
			break;
		case VT::CONSTANT:
			s = val_.u.constant->qualified_name ();
			break;
		default:
			if (is_integral ())
				s = is_signed () ? std::to_string (val_.u.i) : std::to_string (val_.u.ui);
			else {
				assert (is_floating_point ());
				s = std::to_string (val_.u.d);
			}
	}

	return s;
}

void Variant::append (string& s, unsigned c)
{
	assert (c <= 0xFFFF);

	if (isprint (c)) {
		s += (char)c;
		return;
	}

	static const struct Escape {
		char c;
		char esc;
	} escapes [] = {
		{ '\'', '\'' },
		{ '\"', '\"' },
		{ '\?', '?' },
		{ '\\', '\\' },
		{ '\a', 'a' },
		{ '\a', 'a' },
		{ '\b', 'b' },
		{ '\f', 'f' },
		{ '\n', 'n' },
		{ '\r', 'r' },
		{ '\t', 't' },
		{ '\v', 'v' }
	};
		
	for (auto p = escapes; p != end (escapes); ++p) {
		if (p->c == c) {
			s += '\\';
			s += p->esc;
			return;
		}
	}

	char buf [5];
	_ultoa (c, buf, 16);
	size_t cc = strlen (buf);
	s.reserve (s.length () + (cc + 1) / 2 * 2);
	s += "\\x";
	switch (cc) {
		case 1:
			s += '0';
		case 2:
			s += buf;
			break;
		case 3:
			s += '0';
		case 4:
			s += buf;
			break;
		default:
			assert (false);
	}
}

}
