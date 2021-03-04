#ifndef NIDL_AST_ITEMSCOPE_H_
#define NIDL_AST_ITEMSCOPE_H_

#include "Symbols.h"

namespace AST {

class ItemScope :
	public NamedItem,
	public Symbols
{
public:
	ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (kind, builder, name)
	{}

	static const ItemScope* cast (const NamedItem* item)
	{
		if (item) {
			switch (item->kind ()) {
				case Item::Kind::MODULE:
				case Item::Kind::INTERFACE:
				case Item::Kind::STRUCT:
				case Item::Kind::UNION:
				case Item::Kind::ENUM:
					return static_cast <const ItemScope*> (item);
			}
		}
		return nullptr;
	}
};

}

#endif
