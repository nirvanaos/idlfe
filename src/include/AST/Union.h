/// \file Union.h
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
#ifndef NIDL_AST_UNION_H_
#define NIDL_AST_UNION_H_
#pragma once

#include "StructBase.h"
#include "ForwardDeclarable.h"
#include "UnionElement.h"

namespace AST {

class Union;

/// `union` forward declaration.
/// 
/// \invariant kind () == Item::Kind::UNION_DECL
/// 
class UnionDecl :
	public ItemWithId
{
public:
	/// \returns `union` definition reference.
	const Union& definition () const
	{
		assert (definition_);
		return *definition_;
	}

private:
	template <class T> friend class Ptr;
	friend class Build::Builder;

	UnionDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemWithId (Item::Kind::UNION_DECL, builder, name),
		definition_ (nullptr)
	{}

private:
	const Union* definition_;
};

/// `union` definition.
/// 
/// \invariant kind () == Item::Kind::UNION
/// 
class Union :
	public ItemWithId,
	public ContainerT <UnionElement>,
	public ForwardDeclarable
{
public:
	/// \returns The discriminator type.
	const Type& discriminator_type () const noexcept
	{
		return discriminator_type_;
	}

	/// \returns Default discriminator value.
	/// 
	/// Default label is different from the all declared labels.
	/// If no such value exists, the returned variant will be empty.
	/// If union has the `default` label, default_label is always non empty.
	const Variant& default_label () const noexcept
	{
		return default_label_;
	}

	/// \returns UnionElement for `default` label.
	///          If union does not have `default` label, returns `nullptr`.
	const UnionElement* default_element () const noexcept
	{
		return default_element_;
	}

	/// For convenience, the Union may be casted to const StructBase.
	/// 
	/// \returns const StructBase&
	operator const StructBase& () const noexcept
	{
		return reinterpret_cast <const StructBase&> (*this);
	}

	void default_label (const Variant& val)
	{
		default_label_ = val;
	}

	void default_element (const UnionElement& def) noexcept
	{
		default_element_ = &def;
	}

private:
	template <class T> friend class Ptr;

	Union (const Build::Builder& builder, const Build::SimpleDeclarator& name,
		const Type& discriminator_type) :
		ItemWithId (Item::Kind::UNION, builder, name),
		discriminator_type_ (discriminator_type),
		default_element_ (nullptr)
	{}

private:
	Type discriminator_type_;
	const UnionElement* default_element_;
	Variant default_label_;
};

}

#endif
