#ifndef NIDL_AST_ITEMSCOPE_H_
#define NIDL_AST_ITEMSCOPE_H_

#include "Symbols.h"

namespace AST {

class ItemScope :
	public NamedItem,
	public Symbols
{
public:
	ItemScope (const Location& loc, Item::Kind kind, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, kind, parent, name)
	{}

	static const ItemScope* cast (const NamedItem* item)
	{
		if (item) {
			Item::Kind k = item->kind ();
			if (Item::Kind::MODULE == k || Item::Kind::INTERFACE == k)
				return static_cast <const ItemScope*> (item);
		}
		return nullptr;
	}
};

}

#endif
