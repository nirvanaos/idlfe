#ifndef NIDL_AST_ITEMCONTAINER_H_
#define NIDL_AST_ITEMCONTAINER_H_

#include "ItemScope.h"
#include "Container.h"

namespace AST {

class ItemContainer :
	public ItemScope,
	public Container
{
public:
	ItemContainer (const Location& loc, Item::Kind kind, const ItemScope* parent, const std::string& name) :
		ItemScope (loc, kind, parent, name)
	{}
};

}

#endif
