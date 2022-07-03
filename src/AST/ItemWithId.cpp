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
#include "Builder/Builder.h"
#include "../include/AST/Struct.h"
#include "../include/AST/Union.h"
#include "../include/AST/Enum.h"
#include "../include/AST/TypeDef.h"
#include "../include/AST/Native.h"
#include "../include/AST/Exception.h"
#include "../include/AST/ValueType.h"
#include "../include/AST/ValueBox.h"
#include "../include/AST/Module.h"

using namespace std;
using namespace AST::Build;

namespace AST {

ItemWithId* ItemWithId::cast (NamedItem* item) noexcept
{
	ItemWithId* p = nullptr;
	if (item) {
		switch (item->kind ()) {
			case Item::Kind::MODULE:
			case Item::Kind::INTERFACE:
			case Item::Kind::INTERFACE_DECL:
			case Item::Kind::STRUCT:
			case Item::Kind::STRUCT_DECL:
			case Item::Kind::UNION:
			case Item::Kind::UNION_DECL:
			case Item::Kind::ENUM:
			case Item::Kind::TYPE_DEF:
			case Item::Kind::NATIVE:
			case Item::Kind::EXCEPTION:
			case Item::Kind::VALUE_TYPE_DECL:
			case Item::Kind::VALUE_TYPE:
			case Item::Kind::VALUE_BOX:
				p = static_cast <ItemWithId*> (item);
				break;
		}
	}
	return p;
}

ItemWithId::ItemWithId (Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
	NamedItem (kind, builder, name),
	data_ (builder.prefix ())
{}

bool ItemWithId::check_prefix (Builder& builder, const Location& loc) const noexcept
{
	if (!data_.explicit_ [EXPLICIT_ID] && !data_.explicit_ [EXPLICIT_PREFIX]) {
		const string& pref = builder.prefix ();
		if (data_.prefix_or_id != pref) {
			builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with different prefix \"" + data_.prefix_or_id
				+ "\". Current prefix is \"" + pref + "\".");
			builder.see_prev_declaration (*this);
			return false;
		}
	}
	return true;
}

bool ItemWithId::prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
{
	if (!data_.explicit_ [EXPLICIT_ID]) {
		if (data_.explicit_ [EXPLICIT_PREFIX]) {
			if (data_.prefix_or_id != pref) {
				builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with different prefix \"" + data_.prefix_or_id
					+ "\". Current prefix is \"" + pref + "\".");
				builder.see_prev_declaration (data_.explicit_ [EXPLICIT_PREFIX]);
				return false;
			}
		} else {
			data_.prefix_or_id = pref;
			data_.explicit_ [EXPLICIT_PREFIX] = loc;
		}
	}
	return true;
}

void ItemWithId::type_id (Builder& builder, const std::string& id, const Location& loc)
{
	if (data_.explicit_ [EXPLICIT_ID]) {
		if (data_.prefix_or_id != id) {
			builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with different repository ID \"" + data_.prefix_or_id + '\"');
			builder.see_prev_declaration (data_.explicit_ [EXPLICIT_ID]);
		}
	} else if (data_.explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with #pragma version");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_VERSION]);
	} else {
		data_.prefix_or_id = id;
		data_.explicit_ [EXPLICIT_ID] = loc;
	}
}

void ItemWithId::pragma_version (Builder& builder, const Version v, const Location& loc)
{
	if (data_.explicit_ [EXPLICIT_ID]) {
		builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with repository ID");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_ID]);
	} else if (data_.explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, qualified_name () + " is already declared with #pragma version");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_VERSION]);
	} else {
		data_.version = v;
		data_.explicit_ [EXPLICIT_VERSION] = loc;
	}
}

string ItemWithId::repository_id () const
{
	if (data_.explicit_ [EXPLICIT_ID])
		return data_.prefix_or_id;
	string id;
	id.reserve (5 + data_.prefix_or_id.length ());
	id = "IDL:";
	if (!data_.prefix_or_id.empty ()) {
		id += data_.prefix_or_id;
		id += '/';
	}
	ScopedName sn = scoped_name ();
	auto it = sn.begin ();
	id += *(it++);
	for (; it != sn.end (); ++it) {
		id += '/';
		id += *it;
	}
	id += ':';
	id += to_string (data_.version.major);
	id += '.';
	id += to_string (data_.version.minor);
	return id;
}

}
