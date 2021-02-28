#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Declarators.h"

namespace AST {

class ItemScope;

class NamedItem :
	public Item,
	public Location
{
public:
	NamedItem (Kind kind, const ItemScope* parent, const SimpleDeclarator& name) :
		Item (kind),
		Location (name),
		name_ (name),
		parent_ (parent)
	{}

	const std::string& name () const
	{
		return name_;
	}

	const ItemScope* parent () const
	{
		return parent_;
	}

	std::string qualified_name () const;

private:
	const std::string name_;
	const ItemScope* parent_;
};

}

#endif
