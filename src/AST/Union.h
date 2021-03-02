#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_

#include "ItemContainer.h"
#include "Type.h"
#include "RepositoryId.h"

namespace AST {

class UnionDecl :
	public NamedItem,
	public RepositoryId
{
public:
	UnionDecl (const Builder& builder, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::UNION_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

class Union :
	public ItemContainer,
	public RepositoryId
{
public:
	Union (const Builder& builder, const SimpleDeclarator& name, const Type& switch_type) :
		ItemContainer (Item::Kind::UNION, builder, name),
		RepositoryId (*this, builder),
		switch_type_ (switch_type)
	{}

private:
	Type switch_type_;
};

}

#endif
