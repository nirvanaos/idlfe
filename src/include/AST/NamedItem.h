/// \file NamedItem.h
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
#ifndef NIDL_AST_NAMEDITEM_H_
#define NIDL_AST_NAMEDITEM_H_

#include "Item.h"
#include "Identifier.h"
#include "Location.h"

namespace AST {

namespace Build {
class Builder;
class SimpleDeclarator;
}

class ItemScope;
struct ScopedName;

/// The named AST item.
class NamedItem :
	public Item,
	public Location
{
public:
	/// \returns The name of item. TODO: Remove
	const Identifier& name () const
	{
		return name_;
	}

	/// \returns The parent scope or `nullptr`.
	const ItemScope* parent () const
	{
		return parent_;
	}

	/// \returns The fully qualified name of the item.
	std::string qualified_name () const;

	/// \returns The fully qualified ScopedName of the item.
	ScopedName scoped_name () const;

	/// \internal

	NamedItem (Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

private:
	const ItemScope* parent_;
	const Identifier name_;
	/// \endinternal
};

/// Comparator
inline
bool operator < (const Ptr <NamedItem>& l, const Identifier& r)
{
	return l->name () < r;
}

/// Comparator
inline
bool operator < (const Identifier& l, const Ptr <NamedItem>& r)
{
	return l < r->name ();
}

/// Comparator
inline
bool operator < (const Ptr <NamedItem>& l, const Ptr <NamedItem>& r)
{
	return l->name () < r->name ();
}

}

#endif
