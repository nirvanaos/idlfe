#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include <string>

namespace AST {

class NamedItem :
	public Item
{
public:
	NamedItem (Kind kind, const std::string& name) :
		Item (kind),
		name_ (name)
	{}

	const std::string& name () const
	{
		return name_;
	}

private:
	const std::string name_;
};

}

#endif
