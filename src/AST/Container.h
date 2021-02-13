#ifndef NIDL_AST_CONTAINER_H_
#define NIDL_AST_CONTAINER_H_

#include "Item.h"
#include <vector>

namespace AST {

class Container :
	public std::vector <Ptr <Item>>
{
	typedef std::vector <Ptr <Item>> Base;
public:
	void append (Item* item);
};


}

#endif
