/// \file
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
#ifndef IDLFE_AST_STRUCT_H_
#define IDLFE_AST_STRUCT_H_
#pragma once

#include "StructBase.h"
#include "ForwardDeclarable.h"

namespace AST {

class Struct;

/// \brief The `struct` forward declaration.
/// 
/// \invariant kind () == Item::Kind::STRUCT_DECL
/// 
class StructDecl :
	public ItemWithId
{
public:
	/// \returns `struct` definition reference.
	const Struct& definition () const
	{
		assert (definition_);
		return *definition_;
	}

private:
	template <class T> friend class Ptr;
	friend class Build::Builder;

	StructDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemWithId (Item::Kind::STRUCT_DECL, builder, name),
		definition_ (nullptr)
	{}

private:
	const Struct* definition_;
};

/// \brief The `struct` definition.
/// 
/// \invariant kind () == Item::Kind::STRUCT
/// 
class Struct :
	public StructBase,
	public ForwardDeclarable
{
private:
	template <class T> friend class Ptr;
	friend class Build::Builder;

	Struct (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		StructBase (Item::Kind::STRUCT, builder, name)
	{}
};

}

#endif
