#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_

#include "ItemContainer.h"
#include "Member.h"
#include "Variant.h"
#include "RepositoryId.h"

namespace AST {

class UnionDecl :
	public NamedItem,
	public RepositoryId
{
public:
	UnionDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::UNION_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

class Union :
	public ItemContainer,
	public RepositoryId
{
public:
	Union (const Build::Builder& builder, const Build::SimpleDeclarator& name, const Type& switch_type) :
		ItemContainer (Item::Kind::UNION, builder, name),
		RepositoryId (*this, builder),
		switch_type_ (switch_type)
	{}

private:
	Type switch_type_;
};

class Case :
	public Member,
	public Variant
{
public:
	Case (const Variant& label, const Build::Builder& builder, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::CASE),
		Variant (label)
	{}
};

class Default :
	public Member
{
public:
	Default (const Build::Builder& builder, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::DEFAULT)
	{}
};

}

#endif
