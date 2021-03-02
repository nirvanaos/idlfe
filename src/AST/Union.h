#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_

#include "ItemContainer.h"
#include "Type.h"
#include "RepositoryId.h"

namespace AST {

class UnionDecl :
	public RepositoryId,
	public NamedItem
{
public:
	UnionDecl (const ItemScope* parent, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::UNION_DECL, parent, name)
	{}
};

class Union :
	public RepositoryId,
	public ItemContainer
{
public:
	Union (const ItemScope* parent, const SimpleDeclarator& name, const Type& switch_type) :
		ItemContainer (Item::Kind::UNION, parent, name),
		switch_type_ (switch_type)
	{}

private:
	Type switch_type_;
};

}

#endif
