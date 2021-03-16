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
#include "../include/AST/ItemScope.h"
#include "../include/AST/Module.h"
#include "../include/AST/Interface.h"
#include "../include/AST/Struct.h"
#include "../include/AST/Union.h"
#include "../include/AST/Exception.h"
#include "../include/AST/ValueType.h"
#include "Builder/Builder.h"

namespace AST {

ItemScope::ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
	NamedItem (kind, builder, name),
	prefix_ (builder.prefix ())
{}

ItemScope* ItemScope::cast (NamedItem* item) noexcept
{
	if (item) {
		switch (item->kind ()) {
			case Kind::MODULE:
				return static_cast <Module*> (item);
			case Kind::INTERFACE:
				return static_cast <Interface*> (item);
			case Kind::STRUCT:
				return static_cast <Struct*> (item);
			case Kind::UNION:
				return static_cast <Union*> (item);
			case Kind::EXCEPTION:
				return static_cast <Exception*> (item);
			case Kind::VALUETYPE:
				return static_cast <ValueType*> (item);
		}
	}
	return nullptr;
}

}
