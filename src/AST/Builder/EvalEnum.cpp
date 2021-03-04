#include "EvalEnum.h"
#include "Enum.h"
#include "Builder.h"
#include "Constant.h"

namespace AST {

Variant EvalEnum::constant (const ScopedName& constant)
{
	Variant ret;
	const Ptr <NamedItem>* pitem = builder_.lookup (constant);
	if (pitem) {
		const NamedItem& item = **pitem;
		switch (item.kind ()) {
			case Item::Kind::ENUM_ITEM: {
				const EnumItem& enum_item = static_cast <const EnumItem&> (item);
				if (&enum_item.enum_type () == &type_)
					return enum_item;
			} break;
			case Item::Kind::CONSTANT: {
				const Constant& c = static_cast <const Constant&> (item);
				const Type& t = c.dereference_type ();
				if (t.kind () == Type::Kind::NAMED_TYPE && t.named_type () == &type_)
					return c;
			} break;
		}
		builder_.message (constant, Builder::MessageType::ERROR, constant.stringize () + " is not a valid enumerator for " + type_->qualified_name () + '.');
		see_definition (item);
	}
	return Variant ();
}

}
