/// \file EvalDouble.cpp Double expression evaluiator.
#include "EvalDouble.h"
#include "Builder.h"
#include <stdexcept>
#include <cfenv>

using namespace std;

namespace AST {

Variant EvalDouble::literal_float (const string& s, unsigned line)
{
	try {
		size_t idx;
		double d = stod (s, &idx);
		if (idx != s.size ())
			throw runtime_error ("Invalid floating-point constant.");
		return Variant (d);
	} catch (const exception& ex) {
		error (line, ex);
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

void EvalDouble::check_inexact (unsigned line) const
{
	if (fetestexcept (FE_INEXACT))
		builder_.message (Location (builder_.file (), line), Builder::MessageType::WARNING, "Precision lost in conversion.");
}

Variant EvalDouble::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.is_floating_pt () && r.is_floating_pt ());
		try {
			double ret;
			if (l.basic_type () == BasicType::LONGDOUBLE || r.basic_type () == BasicType::LONGDOUBLE) {
				ret = EvalLongDouble (builder_).expr (l, op, r, line).to_double ();
				check_inexact (line);
			} else {
				double lv = l.to_double ();
				check_inexact (line);
				double rv = r.to_double ();
				check_inexact (line);
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
						invalid_operation (op, line);
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
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalDouble::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_floating_pt ());
		try {
			double d = v.to_double ();
			check_inexact (line);
			switch (op) {
				case '-':
					d = -d;
					break;
				case '+':
					break;
				default:
					invalid_operation (op, line);
					return Variant ();
			}
			return d;
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalDouble::cast (const Type& t, Variant&& v, unsigned line)
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
			check_inexact (line);
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return ret;
}

}
