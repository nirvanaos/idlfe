/// \file Member.h
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
*  silver.popov@google.com
*/
#ifndef NIDL_AST_MEMBER_H_
#define NIDL_AST_MEMBER_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

/// The member of `struct` or `union`.
class Member :
	public NamedItem,
	public Type
{
public:
	Member (const Build::Builder& builder, const Type& t, const Build::SimpleDeclarator& name, Item::Kind kind = Item::Kind::MEMBER) :
		NamedItem (kind, builder, name),
		Type (t)
	{}
};

}

#endif
