/// \file Struct.h
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
	StructDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::STRUCT_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

class Struct :
	public ItemContainer
{
public:
	/// \internals
	Struct (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemContainer (Item::Kind::STRUCT, builder, name)
	{}
	/// \endinternals
};

}

#endif
