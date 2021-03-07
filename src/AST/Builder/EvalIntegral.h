/// \file EvalIntegral.h Integral expression evaluiator.
#ifndef NIDL_AST_EVALINTEGRAL_H_
#define NIDL_AST_EVALINTEGRAL_H_

#include "Eval.h"

namespace AST {
namespace Build {

/// Integral evaluator.
class EvalIntegral : public Eval
{
public:
	EvalIntegral (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_boolean (bool v, const Location& loc);
	virtual Variant literal_char (const std::string& s, const Location& loc);
	virtual Variant literal_wchar (const std::string& s, const Location& loc);
	virtual Variant literal_int (const std::string& s, const Location& loc);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}
}

#endif
