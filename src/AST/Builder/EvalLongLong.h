/// \file EvalLongLong.h Long integer expression evaluiator.
#ifndef NIDL_AST_EVALLONGLONG_H_
#define NIDL_AST_EVALLONGLONG_H_

#include "EvalLong.h"

namespace AST {
namespace Build {

/// Long integer evaluator
class EvalLongLong : public EvalLong
{
public:
	EvalLongLong (Builder& builder) :
		EvalLong (builder)
	{}

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);
};

}
}

#endif
