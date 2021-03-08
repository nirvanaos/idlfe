/// \file EvalFloatingPoint.cpp Floating point expression evaluiator.
#include "EvalFloatingPoint.h"
#include "Builder.h"
#include "../../include/AST/Constant.h"
#include <stdexcept>
#include <cfenv>

using namespace std;

namespace AST {
namespace Build {

Variant EvalFloatingPoint::literal_float (const string& s, const Location& loc)
{
	try {
		size_t idx;
		long double d = stold (s, &idx);
		if (idx != s.size ())
			throw runtime_error ("Invalid floating-point constant.");
		return Variant (d);
	} catch (const exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalFloatingPoint::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference_type ();
		if (t.kind () == Type::Kind::BASIC_TYPE && (is_integral (t.basic_type ()) || is_floating_point (t.basic_type ())))
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
		builder_.message (loc, Builder::MessageType::WARNING, "Precision lost in conversion.");
}

Variant EvalFloatingPoint::expr (const Variant& l, char op, const Variant& r, const Location& loc)
{
	if (!l.empty () && !r.empty ()) {
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
			} catch (const exception& ex) {
				error (loc, ex);
			}
		}
	}
	return Variant ();
}

Variant EvalFloatingPoint::expr (char op, const Variant& v, const Location& loc)
{
	if (!v.empty ()) {
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
			} catch (const exception& ex) {
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

		} catch (const exception& ex) {
			error (loc, ex);
		}
	}
	return ret;
}

}
}
