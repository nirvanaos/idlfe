#ifndef NIDL_AST_MODULE_H_
#define NIDL_AST_MODULE_H_

#include "NamedItem.h"
#include "Container.h"

namespace AST {

class Module :
	public NamedItem,
	public Container
{
public:
	Module (const std::string& name) :
		NamedItem (Kind::MODULE, name)
	{}
};

}

#endif
