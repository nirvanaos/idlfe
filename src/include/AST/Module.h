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
private:
	template <class T> friend class Ptr;

	Module (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (Kind::MODULE, builder, name)
	{}
};

/// The seqence of items in a module.
class ModuleItems :
	public Item,
	public Container
{
public:
	/// \returns The module name.
	const Identifier& name () const noexcept
	{
		return module_.name ();
	}

	/// \returns The module as a scope.
	const Module& module () const noexcept
	{
		return module_;
	}

private:
	template <class T> friend class Ptr;

	ModuleItems (const Module& mod) :
		Item (Item::Kind::MODULE_ITEMS),
		module_ (mod)
	{}

private:
	const Module& module_;
};

}

#endif
