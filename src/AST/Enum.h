#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class EnumItem :
	public NamedItem
{
public:
	EnumItem (const Builder& builder, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, builder, name)
	{}
};

class Enum :
	public ItemContainer,
	public RepositoryId
{
public:
	Enum (const Builder& builder, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::ENUM, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
