#ifndef NIDL_AST_NATIVE_H_
#define NIDL_AST_NATIVE_H_

#include "NamedItem.h"

namespace AST {

class Native :
	public NamedItem
{
public:
	Native (const Location& loc, const ItemScope* parent, const std::string& name) :
		NamedItem (loc, Kind::NATIVE, parent, name)
	{}
};

}

#endif
