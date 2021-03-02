#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Declarators.h"

namespace AST {

class Builder;
class ItemScope;

class NamedItem :
	public Item,
	public Location
{
public:
	NamedItem (Kind kind, const Builder& builder, const SimpleDeclarator& name);

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
	const ItemScope* parent_;
	const std::string name_;
};

}

#endif
