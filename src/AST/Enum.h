#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "ItemContainer.h"

namespace AST {

class EnumItem :
	public NamedItem
{
public:
	EnumItem (const Location& loc, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, Item::Kind::ENUM_ITEM, parent, name)
	{}
};

class Enum :
	public ItemContainer
{
public:
	Enum (const Location& loc, const ItemScope* parent, const std::string& name) :
		ItemContainer (loc, Item::Kind::ENUM, parent, name)
	{}
};

}

#endif
