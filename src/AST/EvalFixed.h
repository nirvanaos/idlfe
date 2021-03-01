/// \file EvalFixed.h Fixed expression evaluiator.
#ifndef NIDL_AST_EVALFIXED_H_
#define NIDL_AST_EVALFIXED_H_

#include "Eval.h"

namespace AST {

/// /// Fixed evaluator
class EvalFixed : public Eval
{
	struct Context;
public:
	EvalFixed (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_fixed (const std::string& s, unsigned line);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, unsigned line);
	virtual Variant expr (char op, const Variant& v, unsigned line);

	virtual Variant cast (const Type& t, Variant&& v, unsigned line);
};

}

#endif
