#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"

namespace AST {

class Operation :
	public NamedItem
{
public:
	Operation (const std::string& name) :
		NamedItem (Item::Kind::OPERATION, name)
	{}

};

}

#endif
