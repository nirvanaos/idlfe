/// \file EvalString.h String expression evaluiator.
#ifndef NIDL_AST_EVALSTRING_H_
#define NIDL_AST_EVALSTRING_H_

#include "Eval.h"

namespace AST {
namespace Build {

class EvalStringBase : public Eval
{
public:
	EvalStringBase (Builder& builder) :
		Eval (builder)
	{}

protected:
	[[noreturn]] static void throw_char0 ();
	void error_length (const Location& loc) const;
};

/// String evaluator

class EvalString : public EvalStringBase
{
public:
	EvalString (Builder& builder) :
		EvalStringBase (builder)
	{}

	virtual Variant literal_string (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

/// Wide string evaluator

class EvalWString : public EvalStringBase
{
public:
	EvalWString (Builder& builder) :
		EvalStringBase (builder)
	{}

	virtual Variant literal_wstring (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}
}

#endif
