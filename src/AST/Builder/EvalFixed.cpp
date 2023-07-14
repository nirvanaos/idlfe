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
#include "EvalFixed.h"
#include "Builder.h"
#include "../../include/AST/Constant.h"
#include <stdexcept>

namespace AST {
namespace Build {

// Fixed evaluator

EvalFixed::Context::Context (int numdigits)
{
	decContextDefault (this, DEC_INIT_BASE);
	digits = numdigits;
	traps = 0;
	emin = -numdigits;
	emax = numdigits;
}

void EvalFixed::Context::check () const
{
	if (status)
		throw std::runtime_error (decContextStatusToString (this));
}

Variant EvalFixed::literal_fixed (const std::string& s, const Location& loc)
{
	assert (!s.empty ());
	assert (s.back () == 'd' || s.back () == 'D');

	Fixed v;
	try {
		Context ctx (31);
		decNumberFromString ((decNumber*)&v, s.substr (0, s.length () - 1).c_str (), &ctx);
		ctx.check ();
		decNumberReduce ((decNumber*)&v, (const decNumber*)&v, &ctx);
		ctx.check ();
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
	return v;
}

Variant EvalFixed::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->dereference_type ().tkind () == Type::Kind::FIXED)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalFixed::expr (const Variant& l, char op, const Variant& r, const Location& loc)
{
	if (!l.empty () && !r.empty ()) {
		try {
			Context ctx;
			Fixed ret;
			const Fixed& lv = l.dereference_const ().as_Fixed ();
			const Fixed& rv = r.dereference_const ().as_Fixed ();
			switch (op) {
				case '+':
					decNumberAdd ((decNumber*)&ret, (const decNumber*)&lv, (const decNumber*)&rv, &ctx);
					break;
				case '-':
					decNumberSubtract ((decNumber*)&ret, (const decNumber*)&lv, (const decNumber*)&rv, &ctx);
					break;
				case '*':
					decNumberMultiply ((decNumber*)&ret, (const decNumber*)&lv, (const decNumber*)&rv, &ctx);
					break;
				case '/':
					decNumberDivide ((decNumber*)&ret, (const decNumber*)&lv, (const decNumber*)&rv, &ctx);
					break;
				default:
					invalid_operation (op, loc);
					return Variant ();
			}
			ctx.check ();
			return ret;
		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalFixed::expr (char op, const Variant& v, const Location& loc)
{
	if (!v.empty ()) {
		try {
			Context ctx;
			Fixed ret;
			switch (op) {
				case '-':
					decNumberMinus ((decNumber*)&ret, (const decNumber*)&v.dereference_const ().as_Fixed (), &ctx);
					break;
				case '+':
					break;
				default:
					invalid_operation (op, loc);
					return Variant ();
			}
			return ret;
		} catch (const std::exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalFixed::cast (const Type& t, Variant&& v, const Location& loc)
{
	assert (t.dereference_type ().tkind () == Type::Kind::FIXED);
	assert (v.empty () || v.dereference_const ().vtype () == Variant::VT::FIXED);
	return std::move (v);
}

}
}
