#ifndef NIDL_AST_MODULE_H_
#define NIDL_AST_MODULE_H_

#include "ItemScope.h"
#include "Container.h"

namespace AST {

class Module :
	public ItemScope
{
public:
	Module (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (Kind::MODULE, builder, name)
	{}
};

class ModuleItems :
	public Item,
	public Container
{
public:
	ModuleItems (const Module& mod) :
		Item (Item::Kind::MODULE_ITEMS),
		module_ (mod)
	{}

	const std::string& name () const
	{
		return module_.name ();
	}

private:
	const Module& module_;
};

}

#endif
