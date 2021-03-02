#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class EnumItem :
	public NamedItem
{
public:
	EnumItem (const ItemScope* parent, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, parent, name)
	{}
};

class Enum :
	public RepositoryId,
	public ItemContainer
{
public:
	Enum (const ItemScope* parent, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::ENUM, parent, name)
	{}
};

}

#endif
