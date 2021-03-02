#ifndef NIDL_AST_STRUCT_H_
#define NIDL_AST_STRUCT_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class StructDecl :
	public RepositoryId,
	public NamedItem
{
public:
	StructDecl (const ItemScope* parent, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::STRUCT_DECL, parent, name)
	{}
};

class Struct :
	public RepositoryId,
	public ItemContainer
{
public:
	Struct (const ItemScope* parent, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::STRUCT, parent, name)
	{}
};

}

#endif
