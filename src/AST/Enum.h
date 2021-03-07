/// \file Enum.h
#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "NamedItem.h"
#include "RepositoryId.h"
#include <vector>

namespace AST {

class EnumItem;

typedef std::vector <Ptr <EnumItem>> EnumItems;

/// The `enum` declaration.
class Enum :
	public NamedItem,
	public RepositoryId,
	public EnumItems
{
public:
	/// \internal

	Enum (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM, builder, name),
		RepositoryId (*this, builder)
	{}
	/// \endinternal
};

/// The enumerator.
class EnumItem :
	public NamedItem
{
public:
	/// \returns The pointer to enumerated type.
	const Ptr <NamedItem>& enum_type () const
	{
		return enum_type_;
	}

	/// \internal

	EnumItem (const Build::Builder& builder, const Ptr <NamedItem>& enum_type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, builder, name),
		enum_type_ (enum_type)
	{}

private:
	const Ptr <NamedItem>& enum_type_;
	/// \endinternal
};

}

#endif
