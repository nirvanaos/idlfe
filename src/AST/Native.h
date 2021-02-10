#ifndef NIDL_AST_NATIVE_H_
#define NIDL_AST_NATIVE_H_

#include "NamedItem.h"

namespace AST {

class Native :
	public NamedItem
{
public:
	Native (const std::string& name) :
		NamedItem (Kind::NATIVE, name)
	{}
};

}

#endif
