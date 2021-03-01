/// \file EvalDouble.h Double expression evaluiator.
#ifndef NIDL_AST_EVALDOUBLE_H_
#define NIDL_AST_EVALDOUBLE_H_

#include "Eval.h"

namespace AST {

/// Double evaluator
class EvalDouble : public Eval
{
public:
	EvalDouble (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_float (const std::string& s, unsigned line);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, unsigned line);
	virtual Variant expr (char op, const Variant& v, unsigned line);

	virtual Variant cast (const Type& t, Variant&& v, unsigned line);

private:
	void check_inexact (unsigned line) const;
};

/// Long double is not yet supported
typedef EvalDouble EvalLongDouble;

}

#endif
