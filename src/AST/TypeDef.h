#ifndef NIDL_AST_TYPEDEF_H_
#define NIDL_AST_TYPEDEF_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class TypeDef :
	public NamedItem,
	public Type
{
public:
	TypeDef (const Location& loc, const ItemScope* parent, const std::string& name, const Type& t) :
		NamedItem (loc, Item::Kind::TYPEDEF, parent, name),
		Type (t)
	{}
};

}

#endif
