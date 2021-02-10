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

	Item* back () const
	{
		return items_.back ().get ();
	}

private:
	std::vector <Ptr <Item> > items_;
};

}

#endif
