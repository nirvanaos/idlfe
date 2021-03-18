/// \file ItemContainer.h
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
#ifndef NIDL_AST_ITEMCONTAINER_H_
#define NIDL_AST_ITEMCONTAINER_H_

#include "ItemScope.h"
#include "Container.h"
#include "RepositoryId.h"

namespace AST {

/// The container of AST items.
class ItemContainer :
	public ItemScope,
	public RepositoryId,
	public Container
{
public:
	/// \internal
	ItemContainer (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (kind, builder, name),
		RepositoryId (*this, builder)
	{}

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		if (RepositoryId::prefix (builder, pref, loc))
			return ItemScope::prefix (builder, pref, loc);
		else
			return false;
	}

	using ItemScope::insert;
	using Container::begin;
	using Container::end;
	using Container::cbegin;
	using Container::cend;

	/// \endinternal
};

typedef std::vector <const ItemContainer*> Containers;

}

#endif
