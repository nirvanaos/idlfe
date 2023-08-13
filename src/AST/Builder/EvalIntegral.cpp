/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
*  popov.nirvana@gmail.com
*/
#include "EvalIntegral.h"
#include "../../include/AST/Builder.h"
#include "../../include/AST/Constant.h"
#include "SafeInt/SafeInt.hpp"
#include <stdexcept>

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wswitch"
#endif

namespace AST {
namespace Build {

// Integral evaluator

Variant EvalIntegral::literal_boolean (bool v, const Location& loc)
{
	return Variant (v);
}

Variant EvalIntegral::literal_char (const std::string& s, const Location& loc)
{
	try {
		assert (s.size () > 2);
		assert (s.front () == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 1;
		char v = unescape_char (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalIntegral::literal_wchar (const std::string& s, const Location& loc)
{
	try {
		assert (s.size () > 3);
		assert (s [0] == 'L');
		assert (s [1] == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 2;
		wchar_t v = unescape_wchar (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalIntegral::literal_int (const std::string& s, const Location& loc)
{
	try {
		size_t idx;
		uint64_t ull = std::stoull (s, &idx, 0);
		if (idx != s.size ())
			throw std::runtime_error ("Invalid integer constant.");
		if (ull > std::numeric_limits <uint32_t>::max ())
			return Variant (ull);
		else
			return Variant ((uint32_t)ull);
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalIntegral::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference_type ();
		if (t.tkind () == Type::Kind::BASIC_TYPE && is_integral (t.basic_type ()))
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalIntegral::expr (const Variant& cl, char op, const Variant& cr, const Location& loc)
{
	if (!cl.empty () && !cr.empty ()) {
		const Variant& l = cl.dereference_const ();
		const Variant& r = cr.dereference_const ();
		assert (l.is_integral () && r.is_integral ());
		try {
			if (l.is_signed ()) {
				int64_t lv = l.to_long_long (), ret = 0;
				if ('>' == op || '<' == op) {
					uint32_t shift = r.to_unsigned_long ();
					if (shift > 32)
						throw std::range_error ("Shift size is too large.");
					ret = op == '>' ? lv >> shift : lv << shift;
				} else {
					int64_t rv = r.to_long_long ();
					switch (op) {
						case '|':
							ret = lv | rv;
							break;
						case '^':
							ret = lv ^ rv;
							break;
						case '&':
							ret = lv & rv;
							break;
						case '+':
							if (!SafeAdd (lv, rv, ret))
								overflow (op);
							break;
						case '-':
							if (!SafeSubtract (lv, rv, ret))
								overflow (op);
							break;
						case '*':
							if (!SafeMultiply (lv, rv, ret))
								overflow (op);
							break;
						case '/':
							if (!SafeDivide (lv, rv, ret))
								zero_divide (op);
							break;
						case '%':
							if (!SafeModulus (lv, rv, ret))
								zero_divide (op);
							break;
						default:
							invalid_operation (op, loc);
							return Variant ();
					}
				}
				return ret;
			} else {
				uint64_t lv = l.to_unsigned_long_long (), ret = 0;
				if ('>' == op || '<' == op) {
					uint64_t shift = r.to_unsigned_long ();
					if (shift > 32)
						throw std::range_error ("Shift size is too large.");
					ret = op == '>' ? lv >> shift : lv << shift;
				} else {
					uint64_t rv = r.to_unsigned_long_long ();
					switch (op) {
						case '|':
							ret = lv | rv;
							break;
						case '^':
							ret = lv ^ rv;
							break;
						case '&':
							ret = lv & rv;
							break;
						case '+':
							if (!SafeAdd (lv, rv, ret))
								overflow (op);
							break;
						case '-':
							if (!SafeSubtract (lv, rv, ret))
								overflow (op);
							break;
						case '*':
							if (!SafeMultiply (lv, rv, ret))
								overflow (op);
							break;
						case '/':
							if (!SafeDivide (lv, rv, ret))
								zero_divide (op);
							break;
						case '%':
							if (!SafeModulus (lv, rv, ret))
								zero_divide (op);
							break;
						default:
							invalid_operation (op, loc);
							return Variant ();
					}
				}
				return ret;
			}
		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalIntegral::expr (char op, const Variant& cv, const Location& loc)
{
	if (!cv.empty ()) {
		const Variant& v = cv.dereference_const ();
		try {
			if (v.is_signed ()) {
				int64_t i = v.to_long_long ();
				switch (op) {
					case '-':
						i = -i;
						break;
					case '+':
						break;
					case '~':
						i = -(i + 1);
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
				return i;
			} else {
				uint64_t u = v.to_unsigned_long_long ();
				switch (op) {
					case '-': {
						int64_t i;
						if (!SafeCast (u, i))
							overflow (op);
						return -i;
					}
									break;
					case '+':
						return u;
						break;
					case '~':
						return std::numeric_limits <uint32_t>::max () - u;;
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
			}
		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalIntegral::cast (const Type& t, Variant&& v, const Location& loc)
{
	Variant ret;
	if (!v.empty ()) {
		const Type& type = t.dereference_type ();
		// Always dereference to basic type and convert to check limits.
		try {
			switch (type.basic_type ()) {
				case BasicType::BOOLEAN:
					ret = v.to_boolean ();
					break;
				case BasicType::OCTET:
					ret = v.to_octet ();
					break;
				case BasicType::CHAR:
					ret = v.to_char ();
					break;
				case BasicType::WCHAR:
					ret = v.to_wchar ();
					break;
				case BasicType::USHORT:
					ret = v.to_unsigned_short ();
					break;
				case BasicType::SHORT:
					ret = v.to_short ();
					break;
				case BasicType::ULONG:
					ret = v.to_unsigned_long ();
					break;
				case BasicType::LONG:
					ret = v.to_long ();
					break;
				case BasicType::ULONGLONG:
					ret = v.to_unsigned_long_long ();
					break;
				case BasicType::LONGLONG:
					ret = v.to_long_long ();
					break;
				default:
					assert (false);
			}

			switch (v.vtype ()) {
				case Variant::VT::CONSTANT:
				case Variant::VT::CHAR:
				case Variant::VT::WCHAR:
					ret = v;
			}
		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return ret;
}

}
}
