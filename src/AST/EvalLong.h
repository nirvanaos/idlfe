/// \file EvalLong.h Integer expression evaluiator.
#ifndef NIDL_AST_EVALLONG_H_
#define NIDL_AST_EVALLONG_H_

#include "Eval.h"

namespace AST {

/// Integer evaluator.
class EvalLong : public Eval
{
public:
	EvalLong (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_char (const std::string& s, unsigned line);
	virtual Variant literal_wchar (const std::string& s, unsigned line);
	virtual Variant literal_int (const std::string& s, unsigned line);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, unsigned line);
	virtual Variant expr (char op, const Variant& v, unsigned line);

	virtual Variant cast (const Type& t, Variant&& v, unsigned line);
};

}

#endif
