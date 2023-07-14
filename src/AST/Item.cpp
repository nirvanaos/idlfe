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
#include "../include/AST/Item.h"
#include <algorithm>

namespace AST {

bool Item::is_type () const noexcept
{
	static const Kind kinds [] = {
		Kind::NATIVE,
		Kind::TYPE_DEF,
		Kind::INTERFACE_DECL,
		Kind::INTERFACE,
		Kind::STRUCT_DECL,
		Kind::STRUCT,
		Kind::UNION_DECL,
		Kind::UNION,
		Kind::ENUM,
		Kind::VALUE_TYPE_DECL,
		Kind::VALUE_TYPE,
		Kind::VALUE_BOX
	};

	return std::find (kinds, std::end (kinds), kind_) != std::end (kinds);
}

bool Item::is_forward_decl () const noexcept
{
	static const Kind kinds [] = {
		Kind::INTERFACE_DECL,
		Kind::STRUCT_DECL,
		Kind::UNION_DECL,
		Kind::VALUE_TYPE_DECL
	};

	return std::find (kinds, std::end (kinds), kind_) != std::end (kinds);
}

}
