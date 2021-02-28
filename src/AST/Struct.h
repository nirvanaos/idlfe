#ifndef NIDL_AST_STRUCT_H_
#define NIDL_AST_STRUCT_H_

#include "ItemContainer.h"

namespace AST {

class StructDecl :
	public NamedItem
{
public:
	StructDecl (const ItemScope* parent, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::STRUCT_DECL, parent, name)
	{}
};

class Struct :
	public ItemContainer
{
public:
	Struct (const ItemScope* parent, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::STRUCT, parent, name)
	{}
};

}

#endif
