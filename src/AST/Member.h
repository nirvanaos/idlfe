#pragma once

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class Member :
	public NamedItem,
	public Type
{
public:
	Member (const ItemScope* parent, const Type& t, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::MEMBER, parent, name),
		Type (t)
	{}
};

}
