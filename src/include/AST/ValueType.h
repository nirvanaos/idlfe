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

class ValueType;

/// The sequence of value types.
typedef std::vector <const ValueType*> ValueTypes;

/// Value type definition.
class ValueType :
	public ItemContainer
{
public:
	/// Value type modifier.
	enum class Modifier
	{
		NONE,
		CUSTOM, ///< `custom`
		ABSTRACT, ///< `abstract`
		TRUNCATABLE ///< `truncatable`
	};

	/// \returns The value type modifier.
	Modifier modifier () const
	{
		return modifier_;
	}

	/// \returns The value type modifier name.
	const char* modifier_name () const;

	/// \returns The base value types.
	const ValueTypes& bases () const
	{
		return bases_;
	}

	/// \returns The supported interfaces.
	const Interfaces& supports () const
	{
		return supports_;
	}

	ValueType (const Build::Builder& builder, const Build::SimpleDeclarator& name, Modifier modifier) :
		ItemContainer (Item::Kind::VALUE_TYPE, builder, name),
		modifier_ (modifier)
	{}

	void set_truncatable ()
	{
		assert (modifier_ == Modifier::NONE);
		modifier_ = Modifier::TRUNCATABLE;
	}

	void add_base (const ValueType& vt)
	{
		bases_.push_back (&vt);
	}

	void add_support (const Interface& itf)
	{
		supports_.push_back (&itf);
	}

	void get_all_containers (Containers& all) const;

	virtual std::pair <bool, const Ptr <NamedItem>*> find (Build::Builder& builder, const Identifier& name, const Location&) const;

private:
	Modifier modifier_;
	ValueTypes bases_;
	Interfaces supports_;
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
