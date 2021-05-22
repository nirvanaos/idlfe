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

#include "ItemContainer.h"
#include "Type.h"
#include "ForwardDeclarable.h"

namespace AST {

/// `union` forward declaration.
class UnionDecl :
	public NamedItem,
	public RepositoryId
{
private:
	template <class T> friend class Ptr;

	UnionDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::UNION_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

/// `union` definition.
class Union :
	public ItemContainer,
	public ForwardDeclarable
{
public:
	/// \returns The discriminator type.
	const Type& discriminator_type () const noexcept
	{
		return discriminator_type_;
	}

private:
	template <class T> friend class Ptr;

	Union (const Build::Builder& builder, const Build::SimpleDeclarator& name, const Type& discriminator_type) :
		ItemContainer (Item::Kind::UNION, builder, name),
		discriminator_type_ (discriminator_type)
	{}

private:
	Type discriminator_type_;
};

}

#endif
