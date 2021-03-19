/// \file Interface.h
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
#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "ItemContainer.h"

namespace AST {

class Interface;

/// The kind of interface.
class InterfaceKind
{
public:
	/// The kind of interface.
	enum Kind
	{
		UNCONSTRAINED,
		ABSTRACT,
		LOCAL,
		PSEUDO
	};

	/// \returns The kind of interface.
	Kind interface_kind () const noexcept
	{
		return kind_;
	}

	/// \returns The interface kind keyword. 
	///          Returns the empty string for the UNCONSTRAINED interface.
	const char* interface_kind_name () const noexcept;

	InterfaceKind (Kind kind = UNCONSTRAINED) noexcept :
		kind_ (kind)
	{}

private:
	Kind kind_;
};

/// The sequence of interfaces.
typedef std::vector <const Interface*> Interfaces;

/// Interface definition.
class Interface :
	public ItemContainer,
	public InterfaceKind
{
public:
	/// \returns The base interfaces.
	const Interfaces& bases () const noexcept
	{
		return bases_;
	}

	/// Get all direct and indirect bases.
	/// 
	/// \param bases All bases.
	void get_all_bases (std::set <const Interface*>& bases) const;

private:
	template <class T> friend class Ptr;
	friend class Build::Builder;
	friend class ValueType;

	void get_all_containers (Containers& all) const;

	Interface (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		ItemContainer (Item::Kind::INTERFACE, builder, name)
	{}

	void add_base (const Interface& base)
	{
		bases_.push_back (&base);
	}

private:
	Interfaces bases_;
};

/// Interface forward declaration.
class InterfaceDecl :
	public NamedItem,
	public InterfaceKind,
	public RepositoryId
{
private:
	template <class T> friend class Ptr;

	InterfaceDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		NamedItem (Item::Kind::INTERFACE_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
