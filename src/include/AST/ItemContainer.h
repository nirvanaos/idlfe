/// \file ItemContainer.h
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
#ifndef NIDL_AST_ITEMCONTAINER_H_
#define NIDL_AST_ITEMCONTAINER_H_
#pragma once

#include "ItemScope.h"
#include "Container.h"
#include "ForwardDeclarable.h"

namespace AST {

/// Base for Interface and ValueType.
class ItemContainer :
	public ItemScope,
	public Container,
	public ForwardDeclarable
{
protected:
	ItemContainer (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemScope (kind, builder, name)
	{}

private:
	friend class Build::Builder;

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		if (ItemWithId::prefix (builder, pref, loc))
			return ItemScope::prefix (builder, pref, loc);
		else
			return false;
	}
};

typedef std::vector <const ItemContainer*> Containers;

}

#endif
