#ifndef NIDL_AST_INCLUDE_H_
#define NIDL_AST_INCLUDE_H_

#include "Item.h"
#include <string>

namespace AST {

class Include :
	public Item
{
public:
	Include (const std::string& name) :
		Item (Kind::INCLUDE),
		name_ (name)
	{}

private:
	std::string name_;
};

}

#endif
