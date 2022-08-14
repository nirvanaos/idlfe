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
#ifndef IDLFE_AST_INTERFACE_H_
#define IDLFE_AST_INTERFACE_H_
#pragma once

#include "IV_Base.h"
#include <unordered_set>

namespace AST {

class Interface;

/// \brief The kind of interface.
class InterfaceKind
{
public:
	/// The kind of interface.
	enum Kind
	{
		UNCONSTRAINED, ///< `interface`
		ABSTRACT,      ///< `abstract interface`
		LOCAL,         ///< `local interface`

		/// `pseudo interface` is Nirvana IDL extension
		PSEUDO
	};

	/// \returns The kind of interface.
	Kind interface_kind () const noexcept
	{
		return kind_;
	}

	InterfaceKind (Kind kind = UNCONSTRAINED) noexcept :
		kind_ (kind)
	{}

private:
	friend class Build::Builder;

	const char* interface_kind_name () const noexcept;

private:
	Kind kind_;
};

/// \brief The sequence of interfaces.
typedef std::vector <const Interface*> Interfaces;

/// \brief %Interface definition.
/// 
/// \invariant kind () == Item::Kind::INTERFACE
/// 
/// Child item types:
/// - Attribute
/// - Constant
/// - Enum
/// - Exception
/// - Native
/// - Operation
/// - StructDecl
/// - Struct
/// - TypeDef
/// - UnionDecl
/// - Union
/// 
class Interface :
	public IV_Base,
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
	/// \returns All bases.
	Interfaces get_all_bases () const;

private:
	template <class T> friend class Ptr;
	friend class Build::Builder;
	friend class ValueType;

	void get_all_interfaces (Build::IV_Bases& all) const;

	Interface (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		IV_Base (Item::Kind::INTERFACE, builder, name),
		InterfaceKind (kind)
	{}

	void add_base (const Interface& base)
	{
		bases_.push_back (&base);
	}

	void get_all_bases (std::unordered_set <const Interface*>& bset, Interfaces& bvec) const;

private:
	Interfaces bases_;
};

/// \brief %Interface forward declaration.
/// 
/// \invariant kind () == Item::Kind::INTERFACE_DECL
/// 
class InterfaceDecl :
	public ItemWithId,
	public InterfaceKind
{
private:
	template <class T> friend class Ptr;

	InterfaceDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		ItemWithId (Item::Kind::INTERFACE_DECL, builder, name),
		InterfaceKind (kind)
	{}
};

}

#endif
