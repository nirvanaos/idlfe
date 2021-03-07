/// \file Container.h
#ifndef NIDL_AST_CONTAINER_H_
#define NIDL_AST_CONTAINER_H_

#include "Item.h"
#include <vector>

namespace AST {

/// Sequential container of the AST items.
class Container :
	public std::vector <Ptr <Item>>
{
	/// \internal
	typedef std::vector <Ptr <Item>> Base;
public:
	void append (Item* item);
	/// \endinternal
};


}

#endif
