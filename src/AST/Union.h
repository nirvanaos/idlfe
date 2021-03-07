/// \file Union.h
#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_

#include "ItemContainer.h"
#include "Member.h"
#include "Variant.h"
#include "RepositoryId.h"

namespace AST {

/// `union` forward declaration.
class UnionDecl :
	public NamedItem,
	public RepositoryId
{
public:
	/// \internal
	UnionDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::UNION_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
	/// \endinternal
};

/// `union` definition.
class Union :
	public ItemContainer,
	public RepositoryId
{
public:
	const Type& discriminator_type () const
	{
		return discriminator_type_;
	}

	Union (const Build::Builder& builder, const Build::SimpleDeclarator& name, const Type& discriminator_type) :
		ItemContainer (Item::Kind::UNION, builder, name),
		RepositoryId (*this, builder),
		discriminator_type_ (discriminator_type)
	{}

private:
	Type discriminator_type_;
};

/// Union element definition.
class UnionElement :
	public Member
{
public:
	/// \returns The vector of `case` labels for this element. Empty for the `default` case,
	const std::vector <Variant>& labels () const
	{
		return labels_;
	}

	/// \returns `true` if this is the `default` element.
	bool is_default () const
	{
		return labels_.empty ();
	}

	/// \internal

	UnionElement (const Build::Builder& builder, std::vector <Variant>&& labels, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::UNION_ELEMENT),
		labels_ (std::move (labels))
	{}

private:
	const std::vector <Variant> labels_;
	/// \endinternal
};

}

#endif
