/// \file EvalLongLong.h Long integer expression evaluiator.
#ifndef NIDL_AST_EVALLONGLONG_H_
#define NIDL_AST_EVALLONGLONG_H_

#include "EvalLong.h"

namespace AST {

/// Long integer evaluator
class EvalLongLong : public EvalLong
{
public:
	EvalLongLong (Builder& builder) :
		EvalLong (builder)
	{}

	virtual Variant expr (const Variant& l, char op, const Variant& r, unsigned line);
	virtual Variant expr (char op, const Variant& v, unsigned line);
};

}

#endif
