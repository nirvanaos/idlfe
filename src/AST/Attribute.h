#ifndef NIDL_AST_ATTRIBUTE_H_
#define NIDL_AST_ATTRIBUTE_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class Attribute :
	public NamedItem,
	public Type
{
public:
	Attribute (const Build::Builder& builder, bool readonly, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ATTRIBUTE, builder, name),
		Type (type),
		readonly_ (readonly)
	{}

	bool readonly () const
	{
		return readonly_;
	}

private:
	bool readonly_;
};

}

#endif
