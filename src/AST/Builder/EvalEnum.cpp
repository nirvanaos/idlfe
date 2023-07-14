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
#include "EvalEnum.h"
#include "Builder.h"
#include "../../include/AST/Enum.h"
#include "../../include/AST/Constant.h"

namespace AST {
namespace Build {

Variant EvalEnum::constant (const ScopedName& constant)
{
	Variant ret;
	const Ptr <NamedItem>* l = builder_.lookup (constant);
	if (l) {
		const NamedItem* item = *l;
		switch (item->kind ()) {
			case Item::Kind::ENUM_ITEM: {
				const EnumItem& enum_item = static_cast <const EnumItem&> (*item);
				if (&enum_item.enum_type () == &type_)
					return enum_item;
			} break;
			case Item::Kind::CONSTANT: {
				const Constant& c = static_cast <const Constant&> (*item);
				const Type& t = c.dereference_type ();
				if (t.tkind () == Type::Kind::NAMED_TYPE && &t.named_type () == &type_)
					return c;
			} break;
		}
		builder_.message (constant, Builder::MessageType::ERROR, constant.stringize () + " is not a valid enumerator for " + type_.qualified_name ());
		see_definition (*item);
	}
	return Variant ();
}

Variant EvalEnum::cast (const Type& t, Variant&& v, const Location& loc)
{
	return std::move (v);
}

}
}
