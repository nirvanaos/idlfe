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
	Modifier modifier () const noexcept
	{
		return modifier_;
	}

	/// \returns The base value types.
	const ValueTypes& bases () const noexcept
	{
		return bases_;
	}

	/// \returns The supported interfaces.
	const Interfaces& supports () const noexcept
	{
		return supports_;
	}

private:
	template <class T> friend class Ptr;

	ValueType (const Build::Builder& builder, const Build::SimpleDeclarator& name, Modifier modifier) :
		ItemContainer (Item::Kind::VALUE_TYPE, builder, name),
		modifier_ (modifier)
	{}

	friend class Build::Builder;

	const char* modifier_name () const noexcept;

	void set_truncatable ()
	{
		assert (modifier_ == Modifier::NONE);
		modifier_ = Modifier::TRUNCATABLE;
	}

	void add_base (const ValueType& vt)
	{
		bases_.push_back (&vt);
	}

	void add_supports (const Interface& itf)
	{
		supports_.push_back (&itf);
	}

	void get_all_containers (Containers& all) const;

private:
	Modifier modifier_;
	ValueTypes bases_;
	Interfaces supports_;
};

/// Value type forward declaration.
class ValueTypeDecl :
	public NamedItem,
	public RepositoryId
{
public:
	/// \returns `true` if the value type declared `abstract`.
	bool is_abstract () const noexcept
	{
		return is_abstract_;
	}

private:
	template <class T> friend class Ptr;

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
