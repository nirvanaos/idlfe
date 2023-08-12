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
#include "../include/AST/NamedItem.h"
#include "../include/AST/Declarators.h"
#include "../include/AST/Builder.h"

namespace AST {

Identifier NamedItem::unescape (Builder& builder, const SimpleDeclarator& name)
{
	Identifier id (name);
	if ('_' == id.front ())
		id.erase (0, 1);
	else
		builder.validate_id (name, name);
	return id;
}

NamedItem::NamedItem (Kind kind, Builder& builder, const SimpleDeclarator& name) :
	Item (kind),
	Location (name),
	parent_ (builder.cur_parent ()),
	name_ (unescape (builder, name))
{}

const ItemScope* NamedItem::parent () const noexcept
{
	if (parent_ && parent_->kind () != Item::Kind::ROOT)
		return &static_cast <const ItemScope&> (*parent_);
	else
		return nullptr;
}

const Symbols* NamedItem::parent_scope () const noexcept
{
	if (parent_) {
		if (parent_->kind () != Item::Kind::ROOT)
			return &static_cast <const Symbols&> (static_cast <const ItemScope&> (*parent_));
		else
			return &static_cast <const Symbols&> (static_cast <const Root&> (*parent_));
	} else
		return nullptr;
}

std::string NamedItem::qualified_name () const
{
	std::string qn;
	const ItemScope* par = parent ();
	if (par)
		qn = par->qualified_name ();
	qn += "::";
	qn += name_;
	return qn;
}

ScopedName NamedItem::scoped_name () const
{
	ScopedName sn;
	const ItemScope* par = parent ();
	if (par) {
		sn = par->scoped_name ();
		sn.push_back (name_);
	} else
		sn = ScopedName (*this, true, name_);
	return sn;
}

}
