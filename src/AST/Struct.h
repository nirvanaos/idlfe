#ifndef NIDL_AST_STRUCT_H_
#define NIDL_AST_STRUCT_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class StructDecl :
	public NamedItem,
	public RepositoryId
{
public:
	StructDecl (const Builder& builder, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::STRUCT_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

class Struct :
	public ItemContainer,
	public RepositoryId
{
public:
	Struct (const Builder& builder, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::STRUCT, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
