/// \file EvalFixed.cpp Fixed expression evaluiator.
#include "EvalFixed.h"
#include "Builder.h"

extern "C" {
#include <decNumber/decNumber.h>
}

#include <stdexcept>

using namespace std;

namespace AST {

// Fixed evaluator

struct EvalFixed::Context : decContext
{
	Context ()
	{
		decContextDefault (this, DEC_INIT_BASE);
		this->digits = 62;
	}

	void check () const;
};

void EvalFixed::Context::check () const
{
	if (status)
		throw runtime_error (decContextStatusToString (this));
}

Variant EvalFixed::literal_fixed (const string& s, unsigned line)
{
	assert (!s.empty ());
	assert (s.back () == 'd' || s.back () == 'D');

	decNumber v;
	Context ctx;
	decNumberFromString (&v, s.substr (0, s.length () - 1).c_str (), &ctx);
	try {
		ctx.check ();
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
	return v;
}

Variant EvalFixed::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (static_cast <const Type*> (pc)->kind () == Type::Kind::FIXED)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalFixed::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.kind () == Type::Kind::FIXED && r.kind () == Type::Kind::FIXED);
		try {
			Context ctx;
			decNumber lv, rv, ret;
			l.as_decNumber (lv);
			r.as_decNumber (rv);
			switch (op) {
				case '+':
					decNumberAdd (&ret, &lv, &rv, &ctx);
					break;
				case '-':
					decNumberSubtract (&ret, &lv, &rv, &ctx);
					break;
				case '*':
					decNumberMultiply (&ret, &lv, &rv, &ctx);
					break;
				case '/':
					decNumberDivide (&ret, &lv, &rv, &ctx);
					break;
				default:
					invalid_operation (op, line);
					return Variant ();
			}
			ctx.check ();
			return ret;
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalFixed::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.kind () == Type::Kind::FIXED);
		try {
			Context ctx;
			decNumber dn;
			v.as_decNumber (dn);
			switch (op) {
				case '-':
					decNumberMinus (&dn, &dn, &ctx);
					break;
				case '+':
					break;
				default:
					invalid_operation (op, line);
					return Variant ();
			}
			return dn;
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalFixed::cast (const Type& t, Variant&& v, unsigned line)
{
	assert (t.kind () == Type::Kind::FIXED);
	assert (v.kind () == Type::Kind::VOID || v.kind () == Type::Kind::FIXED);
	return v;
}

}
