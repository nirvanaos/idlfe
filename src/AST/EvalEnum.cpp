#include "EvalEnum.h"
#include "Enum.h"

namespace AST {

Variant EvalEnum::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference_type ();
		if (t.kind () == Type::Kind::NAMED_TYPE && t.named_type () == type_) {
			return *pc;
		}
		invalid_constant_type (constant);
		see_definition (*pc);
	}
	return Variant ();
}

}
