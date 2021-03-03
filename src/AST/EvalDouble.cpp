/// \file EvalDouble.cpp Double expression evaluiator.
#include "EvalDouble.h"
#include "Builder.h"
#include <stdexcept>
#include <cfenv>

using namespace std;

namespace AST {

Variant EvalDouble::literal_float (const string& s, const Location& loc)
{
	try {
		size_t idx;
		double d = stod (s, &idx);
		if (idx != s.size ())
			throw runtime_error ("Invalid floating-point constant.");
		return Variant (d);
	} catch (const exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalDouble::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->is_floating_pt ())
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

void EvalDouble::check_inexact (const Location& loc) const
{
	if (fetestexcept (FE_INEXACT))
		builder_.message (loc, Builder::MessageType::WARNING, "Precision lost in conversion.");
}

Variant EvalDouble::expr (const Variant& l, char op, const Variant& r, const Location& loc)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.is_floating_pt () && r.is_floating_pt ());
		try {
			double ret;
			if (l.basic_type () == BasicType::LONGDOUBLE || r.basic_type () == BasicType::LONGDOUBLE) {
				ret = EvalLongDouble (builder_).expr (l, op, r, loc).to_double ();
				check_inexact (loc);
			} else {
				double lv = l.to_double ();
				check_inexact (loc);
				double rv = r.to_double ();
				check_inexact (loc);
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
			}
			return ret;
		} catch (const exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalDouble::expr (char op, const Variant& v, const Location& loc)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_floating_pt ());
		try {
			double d = v.to_double ();
			check_inexact (loc);
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
	return Variant ();
}

Variant EvalDouble::cast (const Type& t, Variant&& v, const Location& loc)
{
	Variant ret;
	assert (t.is_floating_pt ());
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_floating_pt ());
		try {
			switch (t.basic_type ()) {
				case BasicType::FLOAT:
					ret = v.to_float ();
					break;
				case BasicType::DOUBLE:
					ret = v.to_double ();
					break;
				case BasicType::LONGDOUBLE:
					ret = v.to_double ();
					break;
			}
			check_inexact (loc);
		} catch (const exception& ex) {
			error (loc, ex);
		}
	}
	return ret;
}

}
