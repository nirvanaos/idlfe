#pragma once

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class Member :
	public NamedItem,
	public Type
{
public:
	Member (const Builder& builder, const Type& t, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::MEMBER, builder, name),
		Type (t)
	{}
};

}
