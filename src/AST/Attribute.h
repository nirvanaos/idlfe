/// \file Attribute.h
#ifndef NIDL_AST_ATTRIBUTE_H_
#define NIDL_AST_ATTRIBUTE_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

/// `attribute` specification.
class Attribute :
	public NamedItem,
	public Type
{
public:
	/// \returns `true` if the attribute is marked as `readonly`.
	bool readonly () const
	{
		return readonly_;
	}

	/// \internal

	Attribute (const Build::Builder& builder, bool readonly, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ATTRIBUTE, builder, name),
		Type (type),
		readonly_ (readonly)
	{}

private:
	bool readonly_;
	/// \endinternal
};

}

#endif
