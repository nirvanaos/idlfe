#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "NamedItem.h"
#include "Container.h"
#include "RepositoryId.h"

namespace AST {

class Enum :
	public NamedItem,
	public RepositoryId,
	public Container
{
public:
	Enum (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM, builder, name),
		RepositoryId (*this, builder)
	{}
};

class EnumItem :
	public NamedItem
{
public:
	EnumItem (const Build::Builder& builder, const Ptr <NamedItem>& enum_type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, builder, name),
		enum_type_ (enum_type)
	{}

	const Ptr <NamedItem>& enum_type () const
	{
		return enum_type_;
	}

private:
	const Ptr <NamedItem>& enum_type_;
};

}

#endif
