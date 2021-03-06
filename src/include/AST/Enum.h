/// \file Enum.h
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
#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "NamedItem.h"
#include "RepositoryId.h"
#include "Container.h"

namespace AST {

class EnumItem;

/// The enumerator.
class EnumItem :
	public NamedItem
{
public:
	/// \returns The pointer to enumerated type.
	const NamedItem& enum_type () const noexcept
	{
		return enum_type_;
	}

private:
	template <class T> friend class Ptr;

	EnumItem (const Build::Builder& builder, const NamedItem& enum_type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, builder, name),
		enum_type_ (enum_type)
	{}

private:
	const NamedItem& enum_type_;
};

/// The `enum` declaration.
class Enum :
	public NamedItem,
	public RepositoryId,
	public ContainerT <EnumItem>
{
private:
	template <class T> friend class Ptr;

	Enum (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
