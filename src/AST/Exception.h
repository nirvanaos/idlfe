#ifndef NIDL_AST_EXCEPTION_H_
#define NIDL_AST_EXCEPTION_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class Exception :
	public RepositoryId,
	public ItemContainer
{
public:
	Exception (const ItemScope* parent, const SimpleDeclarator& name) :
		ItemContainer (Item::Kind::EXCEPTION, parent, name)
	{}
};

}

#endif
