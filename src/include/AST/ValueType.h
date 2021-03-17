/// \file ValueType.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
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
#ifndef NIDL_AST_VALUETYPE_H_
#define NIDL_AST_VALUETYPE_H_

#include "Interface.h"
#include "Type.h"

namespace AST {

/// Value type definition.
class ValueType :
	public ItemContainer
{
public:
	enum class Modifier
	{
		NONE,
		CUSTOM, ///< `custom`
		ABSTRACT, ///< `abstract`
		TRUNCATABLE ///< `truncatable`
	};

	/// \returns Value type modifier.
	Modifier modifier () const
	{
		return modifier_;
	}

	ValueType (const Build::Builder& builder, const Build::SimpleDeclarator& name, Modifier modifier) :
		ItemContainer (Item::Kind::VALUE_TYPE, builder, name),
		modifier_ (modifier)
	{}

private:
	Modifier modifier_;
};

/// Value box definition.
class ValueBox :
	public NamedItem,
	public Type,
	public RepositoryId
{
public:
	ValueBox (const Build::Builder& builder, const Build::SimpleDeclarator& name, const Type& type) :
		NamedItem (Item::Kind::VALUE_BOX, builder, name),
		Type (type),
		RepositoryId (*this, builder)
	{}
};

/// Value type forward declaration.
class ValueTypeDecl :
	public NamedItem,
	public RepositoryId
{
public:
	/// \returns `true` if the value type declared `abstract`.
	bool is_abstract () const
	{
		return is_abstract_;
	}

	ValueTypeDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, bool abstr) :
		NamedItem (Item::Kind::VALUE_TYPE_DECL, builder, name),
		RepositoryId (*this, builder),
		is_abstract_ (abstr)
	{}

private:
	bool is_abstract_;
};

}

#endif
