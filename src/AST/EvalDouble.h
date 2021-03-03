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

	virtual Variant literal_float (const std::string& s, const Location& loc);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);

private:
	void check_inexact (const Location& loc) const;
};

/// Long double is not yet supported
typedef EvalDouble EvalLongDouble;

}

#endif
