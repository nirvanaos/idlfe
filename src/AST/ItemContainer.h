#ifndef NIDL_AST_ITEMCONTAINER_H_
#define NIDL_AST_ITEMCONTAINER_H_

#include "ItemScope.h"
#include "Container.h"
#include "RepositoryId.h"

namespace AST {

class ItemContainer :
	public ItemScope,
	public RepositoryId,
	public Container
{
public:
	ItemContainer (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (kind, builder, name),
		RepositoryId (*this, builder)
	{}

	std::pair <ItemScope::iterator, bool> insert (NamedItem* item)
	{
		return ItemScope::insert (item);
	}

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		if (RepositoryId::prefix (builder, pref, loc))
			return ItemScope::prefix (builder, pref, loc);
		else
			return false;
	}
};

}

#endif
