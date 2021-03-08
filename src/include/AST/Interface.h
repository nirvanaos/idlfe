/// \file Interface.h
/*
* Nirvana IDL Front End Library.
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
*  silver.popov@google.com
*/
#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

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
		LOCAL
	};

	/// \returns The kind of interface.
	Kind interface_kind () const noexcept
	{
		return kind_;
	}

	/// \internals

	InterfaceKind (Kind kind = UNCONSTRAINED) noexcept :
		kind_ (kind)
	{}

	const char* interface_kind_name () const noexcept;

private:
	Kind kind_;
	/// \endinternals
};

/// The interface definition.
class Interface :
	public ItemContainer,
	public InterfaceKind
{
public:
	/// \returns The 
	const std::vector <const Interface*>& bases () const
	{
		return bases_;
	}

	/// \summary Get all direct and indirect bases.
	/// \param bases All bases.
	void get_all_bases (std::set <const Interface*>& bases) const;

	/// \internals

	void get_all_interfaces (std::vector <const Interface*>& all) const;

	Interface (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		ItemContainer (Item::Kind::INTERFACE, builder, name)
	{}

	void add_base (const Interface* base)
	{
		bases_.push_back (base);
	}

	virtual std::pair <bool, const Ptr <NamedItem>*> find (Build::Builder& builder, const std::string& name, const Location&) const;

private:
	void base_find (const std::string& name, std::set <const Ptr <NamedItem>*>& found) const;

private:
	std::vector <const Interface*> bases_;
	/// \endinternals
};

/// Interface forward declaration.
class InterfaceDecl :
	public NamedItem,
	public InterfaceKind,
	public RepositoryId
{
public:
	/// \internals
	InterfaceDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		NamedItem (Item::Kind::INTERFACE_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
	/// \endinternals
};

}

#endif
