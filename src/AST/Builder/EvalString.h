/// \file EvalString.h String expression evaluiator.
#ifndef NIDL_AST_EVALSTRING_H_
#define NIDL_AST_EVALSTRING_H_

#include "Eval.h"

namespace AST {
namespace Build {

/// String evaluator

class EvalString : public Eval
{
public:
	EvalString (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_string (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

/// Wide string evaluator

class EvalWString : public Eval
{
public:
	EvalWString (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_wstring (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}
}

#endif
