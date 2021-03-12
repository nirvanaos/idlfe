/// \file Enum.h
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
#ifndef NIDL_AST_ENUM_H_
#define NIDL_AST_ENUM_H_

#include "NamedItem.h"
#include "RepositoryId.h"
#include <vector>

namespace AST {

class EnumItem;

/// The enumerator.
class EnumItem :
	public NamedItem
{
public:
	/// \returns The pointer to enumerated type.
	const Ptr <NamedItem>& enum_type () const
	{
		return enum_type_;
	}

	/// \internal

	EnumItem (const Build::Builder& builder, const Ptr <NamedItem>& enum_type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM_ITEM, builder, name),
		enum_type_ (enum_type)
	{}

private:
	const Ptr <NamedItem>& enum_type_;
	/// \endinternal
};

/// The container of the EnumItem elements.
typedef std::vector <Ptr <EnumItem>> EnumItems;

/// The `enum` declaration.
class Enum :
	public NamedItem,
	public RepositoryId,
	public EnumItems
{
public:
	/// \internal

	Enum (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ENUM, builder, name),
		RepositoryId (*this, builder)
	{}
	/// \endinternal
};

}

#endif
