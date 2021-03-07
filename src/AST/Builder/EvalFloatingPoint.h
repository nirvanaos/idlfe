/// \file EvalFloatingPoint.h Floating point expression evaluiator.
#ifndef NIDL_AST_EVALFLOATINGPOINT_H_
#define NIDL_AST_EVALFLOATINGPOINT_H_

#include "EvalIntegral.h"

namespace AST {
namespace Build {

/// Floating point evaluator.
class EvalFloatingPoint : public EvalIntegral
{
public:
	EvalFloatingPoint (Builder& builder) :
		EvalIntegral (builder)
	{}

	virtual Variant literal_float (const std::string& s, const Location& loc);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);

private:
	void check_inexact (const Location& loc) const;
};

}
}

#endif
