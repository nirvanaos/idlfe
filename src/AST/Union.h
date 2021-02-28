#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_

#include "ItemContainer.h"
#include "Type.h"

namespace AST {

class UnionDecl :
	public NamedItem
{
public:
	UnionDecl (const Location& loc, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, Item::Kind::UNION_DECL, parent, name)
	{}
};

class Union :
	public ItemContainer
{
public:
	Union (const Location& loc, const ItemScope* parent, const std::string& name, const Type& switch_type) :
		ItemContainer (loc, Item::Kind::UNION, parent, name),
		switch_type_ (switch_type)
	{}

private:
	Type switch_type_;
};

}

#endif
