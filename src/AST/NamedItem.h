#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Declarators.h"
#include "RepositoryId.h"

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

	const std::string& pragma_id () const
	{
		return pragma_id_;
	}

	void pragma_id (std::string&& id)
	{
		pragma_id_ = std::move (id);
	}

	const Version& version () const
	{
		return version_;
	}

	void version (const Version& ver)
	{
		version_ = ver;
	}

private:
	const std::string name_;
	const ItemScope* parent_;
	std::string pragma_id_;
	Version version_;
};

}

#endif
