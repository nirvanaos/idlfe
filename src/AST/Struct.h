#ifndef NIDL_AST_STRUCT_H_
#define NIDL_AST_STRUCT_H_

#include "ItemContainer.h"

namespace AST {

class StructDecl :
	public NamedItem
{
public:
	StructDecl (const Location& loc, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, Item::Kind::STRUCT_DECL, parent, name)
	{}
};

class Struct :
	public ItemContainer
{
public:
	Struct (const Location& loc, const ItemScope* parent, const std::string& name) :
		ItemContainer (loc, Item::Kind::STRUCT, parent, name)
	{}
};

}

#endif
