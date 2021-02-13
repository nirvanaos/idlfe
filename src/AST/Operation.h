#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"

namespace AST {

class Operation :
	public NamedItem
{
public:
	Operation (const Location& loc, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, Item::Kind::OPERATION, parent, name)
	{}

};

}

#endif
