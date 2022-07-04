/// \file Module.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIDL_AST_MODULE_H_
#define NIDL_AST_MODULE_H_
#pragma once

#include "ItemScope.h"
#include "Container.h"

namespace AST {

/// %Module as a scope.
/// 
/// \invariant kind () == Item::Kind::MODULE
/// 
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
/// 
/// \invariant kind () == Item::Kind::MODULE_ITEMS
/// 
/// Child item types:
/// - Constant
/// - Enum
/// - Exception
/// - Interface
/// - ModuleItems
/// - Native
/// - StructDecl
/// - Struct
/// - TypeDef
/// - UnionDecl
/// - Union
/// - ValueType
/// - ValueBox
/// 
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
