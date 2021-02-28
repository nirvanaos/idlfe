#ifndef NIDL_AST_EXCEPTION_H_
#define NIDL_AST_EXCEPTION_H_

#include "ItemContainer.h"

namespace AST {

class Exception :
	public ItemContainer
{
public:
	Exception (const Location& loc, const ItemScope* parent, const std::string& name) :
		ItemContainer (loc, Item::Kind::EXCEPTION, parent, name)
	{}
};

}

#endif
