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
#include "EvalFloatingPoint.h"
#include "../../include/AST/Builder.h"
#include "../../include/AST/Constant.h"
#include <stdexcept>
#include <cfenv>

namespace AST {
namespace Build {

Variant EvalFloatingPoint::literal_float (const std::string& s, const Location& loc)
{
	try {
		size_t idx;
		long double d = std::stold (s, &idx);
		if (idx != s.size ())
			throw std::runtime_error ("Invalid floating-point constant.");
		return Variant (d);
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalFloatingPoint::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference_type ();
		if (t.tkind () == Type::Kind::BASIC_TYPE && (is_integral (t.basic_type ()) || is_floating_point (t.basic_type ())))
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

void EvalFloatingPoint::check_inexact (const Location& loc) const
{
	if (fetestexcept (FE_INEXACT))
		builder_.message (loc, Builder::MessageType::WARNING, "precision lost in conversion");
}

Variant EvalFloatingPoint::expr (const Variant& cl, char op, const Variant& cr, const Location& loc)
{
	if (!cl.empty () && !cr.empty ()) {
		const Variant& l = cl.dereference_const ();
		const Variant& r = cr.dereference_const ();
		if (l.is_integral () && r.is_integral ())
			return EvalIntegral::expr (l, op, r, loc);
		else {
			assert (l.is_floating_point () && r.is_floating_point ());
			try {
				long double ret;
				long double lv = l.to_long_double ();
				long double rv = r.to_long_double ();
				feclearexcept (FE_ALL_EXCEPT);
				switch (op) {
					case '+':
						ret = lv + rv;
						break;
					case '-':
						ret = lv - rv;
						break;
					case '*':
						ret = lv * rv;
						break;
					case '/':
						ret = lv / rv;
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
				int ex = fetestexcept (FE_ALL_EXCEPT);
				if (ex & FE_OVERFLOW)
					overflow (op);
				else if (ex & FE_UNDERFLOW)
					underflow (op);
				else if (ex & FE_DIVBYZERO)
					zero_divide (op);
				return ret;
			} catch (const std::exception& ex) {
				error (loc, ex);
			}
		}
	}
	return Variant ();
}

Variant EvalFloatingPoint::expr (char op, const Variant& cv, const Location& loc)
{
	if (!cv.empty ()) {
		const Variant& v = cv.dereference_const ();
		if (v.is_integral ())
			return EvalIntegral::expr (op, v, loc);
		else {
			assert (v.is_floating_point ());
			try {
				long double d = v.to_long_double ();
				switch (op) {
					case '-':
						d = -d;
						break;
					case '+':
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
				return d;
			} catch (const std::exception& ex) {
				error (loc, ex);
			}
		}
	}
	return Variant ();
}

Variant EvalFloatingPoint::cast (const Type& t, Variant&& v, const Location& loc)
{
	Variant ret;
	if (!v.empty ()) {
		const Type& type = t.dereference_type ();
		try {
			switch (type.basic_type ()) {
				case BasicType::FLOAT:
					ret = v.to_float ();
					break;
				case BasicType::DOUBLE:
					ret = v.to_double ();
					break;
				case BasicType::LONGDOUBLE:
					ret = v.to_double ();
					break;
				default:
					assert (false);
			}
			check_inexact (loc);

			if (v.vtype () == Variant::VT::CONSTANT)
				ret = v;

		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return ret;
}

}
}
