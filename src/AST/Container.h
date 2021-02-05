#ifndef NIDL_AST_CONTAINER_H_
#define NIDL_AST_CONTAINER_H_

#include "Item.h"
#include <vector>

namespace AST {

class Container
{
public:
	void append (Ptr <Item> item)
	{
		items_.push_back (item);
	}

private:
	std::vector <Ptr <Item> > items_;
};

}

#endif
