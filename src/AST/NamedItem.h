#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Location.h"
#include <string>

namespace AST {

class ItemScope;

class NamedItem :
	public Item,
	public Location
{
public:
	NamedItem (const Location& loc, Kind kind, const ItemScope* parent, const std::string& name) :
		Item (kind),
		Location (loc),
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
