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
#include "../include/AST/CodeGen.h"

namespace AST {

void Container::append (Item& item)
{
	Base::append (item);
}

bool Container::visit (CodeGen& cg) const
{
	bool unsupported = false;
	for (const auto& item : *this) {
		switch (item->kind ()) {
			case Item::Kind::INCLUDE:
				cg.leaf (static_cast <const Include&> (*item));
				break;
			case Item::Kind::NATIVE:
				cg.leaf (static_cast <const Native&> (*item));
				break;
			case Item::Kind::TYPE_DEF:
				cg.leaf (static_cast <const TypeDef&> (*item));
				break;
			case Item::Kind::CONSTANT:
				cg.leaf (static_cast <const Constant&> (*item));
				break;
			case Item::Kind::MODULE_ITEMS: {
				const ModuleItems& mod = static_cast <const ModuleItems&> (*item);
				cg.begin (mod);
				unsupported = mod.visit (cg) || unsupported;
				cg.end (mod);
			} break;
			case Item::Kind::INTERFACE_DECL:
				cg.leaf (static_cast <const InterfaceDecl&> (*item));
				break;
			case Item::Kind::INTERFACE: {
				const Interface& t = static_cast <const Interface&> (*item);
				cg.begin (t);
				unsupported = t.visit (cg) || unsupported;
				cg.end (t);
			} break;
			case Item::Kind::OPERATION:
				cg.leaf (static_cast <const Operation&> (*item));
				break;
			case Item::Kind::ATTRIBUTE:
				cg.leaf (static_cast <const Attribute&> (*item));
				break;
			case Item::Kind::EXCEPTION: {
				const Exception& t = static_cast <const Exception&> (*item);
				cg.begin (t);
				unsupported = t.visit (cg) || unsupported;
				cg.end (t);
			} break;
			case Item::Kind::STRUCT_DECL:
				cg.leaf (static_cast <const StructDecl&> (*item));
				break;
			case Item::Kind::STRUCT: {
				const Struct& t = static_cast <const Struct&> (*item);
				cg.begin (t);
				unsupported = t.visit (cg) || unsupported;
				cg.end (t);
			} break;
			case Item::Kind::MEMBER:
				cg.leaf (static_cast <const Member&> (*item));
				break;
			case Item::Kind::UNION_DECL:
				cg.leaf (static_cast <const UnionDecl&> (*item));
				break;
			case Item::Kind::UNION: {
				const Union& t = static_cast <const Union&> (*item);
				cg.begin (t);
				unsupported = t.visit (cg) || unsupported;
				cg.end (t);
			} break;
			case Item::Kind::UNION_ELEMENT:
				cg.leaf (static_cast <const UnionElement&> (*item));
				break;
			case Item::Kind::ENUM:
				cg.leaf (static_cast <const Enum&> (*item));
				break;
			case Item::Kind::VALUE_TYPE_DECL:
				cg.leaf (static_cast <const ValueTypeDecl&> (*item));
				break;
			case Item::Kind::VALUE_TYPE: {
				const ValueType& t = static_cast <const ValueType&> (*item);
				cg.begin (t);
				unsupported = t.visit (cg) || unsupported;
				cg.end (t);
			} break;
			case Item::Kind::VALUE_BOX:
				cg.leaf (static_cast <const ValueBox&> (*item));
				break;
			case Item::Kind::STATE_MEMBER:
				cg.leaf (static_cast <const StateMember&> (*item));
				break;
			case Item::Kind::VALUE_FACTORY:
				cg.leaf (static_cast <const ValueFactory&> (*item));
				break;
			default:
				unsupported = true;
		}
	}
	return unsupported;
}

}
