#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Location.h"

namespace AST {

namespace Build {
class Builder;
class SimpleDeclarator;
}

class ItemScope;
struct ScopedName;

class NamedItem :
	public Item,
	public Location
{
public:
	NamedItem (Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

	const std::string& name () const
	{
		return name_;
	}

	const ItemScope* parent () const
	{
		return parent_;
	}

	std::string qualified_name () const;
	ScopedName scoped_name () const;

private:
	const ItemScope* parent_;
	const std::string name_;
};

}

#endif
