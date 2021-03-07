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

class UnionElement :
	public Member
{
public:
	UnionElement (const Build::Builder& builder, std::vector <Variant>&& labels, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::UNION_ELEMENT),
		labels_ (std::move (labels))
	{}

	const std::vector <Variant>& labels () const
	{
		return labels_;
	}

	bool is_default () const
	{
		return labels_.empty ();
	}

private:
	const std::vector <Variant> labels_;
};

}

#endif
