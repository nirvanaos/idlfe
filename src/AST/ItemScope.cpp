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
#include "../include/AST/ItemScope.h"
#include "../include/AST/Builder.h"

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wswitch"
#endif

namespace AST {

ItemScope::ItemScope (Item::Kind kind, Builder& builder, const SimpleDeclarator& name) :
	ItemWithId (kind, builder, name),
	prefix_ (builder.prefix ())
{}

ItemScope* ItemScope::cast (NamedItem* item) noexcept
{
	if (item) {
		switch (item->kind ()) {
			case Kind::MODULE:
			case Kind::INTERFACE:
			case Kind::VALUE_TYPE:
				return static_cast <ItemScope*> (item);
		}
	}
	return nullptr;
}

}
