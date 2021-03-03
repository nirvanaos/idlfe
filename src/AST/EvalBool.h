/// \file EvalBool.h Boolean expression evaluiator.
#ifndef NIDL_AST_EVALBOOL_H_
#define NIDL_AST_EVALBOOL_H_

#include "Eval.h"

namespace AST {

/// Boolean evaluator.
class EvalBool : public Eval
{
public:
	EvalBool (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_boolean (bool v, const Location& loc);
	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}

#endif
