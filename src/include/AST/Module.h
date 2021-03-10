/// \file Module.h
#ifndef NIDL_AST_MODULE_H_
#define NIDL_AST_MODULE_H_

#include "ItemScope.h"
#include "Container.h"

namespace AST {

/// Module as a scope.
class Module :
	public ItemScope
{
public:
	/// \internal
	Module (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (Kind::MODULE, builder, name)
	{}
	/// \endinternal
};

/// The seqence of items in a module.
class ModuleItems :
	public Item,
	public Container
{
public:
	/// \returns The module name.
	const std::string& name () const
	{
		return module_.name ();
	}

	/// Returns module as a scope.
	const Module& module () const
	{
		return module_;
	}

	/// \internal

	ModuleItems (const Module& mod) :
		Item (Item::Kind::MODULE_ITEMS),
		module_ (mod)
	{}

private:
	const Module& module_;
	/// \endinternal
};

}

#endif
