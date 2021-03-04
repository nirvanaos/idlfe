/// \file EvalEnum.h Enum expression evaluiator.
#ifndef NIDL_AST_EVALENUM_H_
#define NIDL_AST_EVALENUM_H_

#include "Eval.h"

namespace AST {

class Enum;

namespace Build {

/// Enum expression evaluiator.
class EvalEnum : public Eval
{
public:
	EvalEnum (Builder& builder, const Ptr <NamedItem>& type) :
		Eval (builder),
		type_ (type)
	{}

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, unsigned line);

private:
	const Ptr <NamedItem>& type_;
};

}
}

#endif
