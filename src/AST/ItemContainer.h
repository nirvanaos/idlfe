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
	ItemContainer (Item::Kind kind, const Builder& builder, const SimpleDeclarator& name) :
		ItemScope (kind, builder, name)
	{}
};

}

#endif
