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
#include "Builder/Builder.h"
#include "../include/AST/Struct.h"
#include "../include/AST/Union.h"
#include "../include/AST/Enum.h"
#include "../include/AST/TypeDef.h"
#include "../include/AST/Native.h"
#include "../include/AST/Exception.h"
#include "../include/AST/ValueType.h"

using namespace std;
using namespace AST::Build;

namespace AST {

RepositoryId* RepositoryId::cast (NamedItem* item) noexcept
{
	RepositoryId* p = nullptr;
	if (item) {
		switch (item->kind ()) {
			case Item::Kind::INTERFACE:
				p = static_cast <Interface*> (item);
				break;
			case Item::Kind::INTERFACE_DECL:
				p = static_cast <InterfaceDecl*> (item);
				break;
			case Item::Kind::STRUCT:
				p = static_cast <Struct*> (item);
				break;
			case Item::Kind::STRUCT_DECL:
				p = static_cast <StructDecl*> (item);
				break;
			case Item::Kind::UNION:
				p = static_cast <Union*> (item);
				break;
			case Item::Kind::UNION_DECL:
				p = static_cast <UnionDecl*> (item);
				break;
			case Item::Kind::ENUM:
				p = static_cast <Enum*> (item);
				break;
			case Item::Kind::TYPE_DEF:
				p = static_cast <TypeDef*> (item);
				break;
			case Item::Kind::NATIVE:
				p = static_cast <Native*> (item);
				break;
			case Item::Kind::EXCEPTION:
				p = static_cast <Exception*> (item);
				break;
			case Item::Kind::VALUE_TYPE_DECL:
				p = static_cast <ValueTypeDecl*> (item);
				break;
			case Item::Kind::VALUE_TYPE:
				p = static_cast <ValueType*> (item);
				break;
			case Item::Kind::VALUE_BOX:
				p = static_cast <ValueBox*> (item);
				break;
		}
	}
	return p;
}

RepositoryId::RepositoryId (const NamedItem& item, const Builder& builder) :
	item_ (item),
	data_ (builder.prefix ())
{}

bool RepositoryId::check_prefix (Builder& builder, const Location& loc) const
{
	if (!data_.explicit_ [EXPLICIT_ID] && !data_.explicit_ [EXPLICIT_PREFIX]) {
		const string& pref = builder.prefix ();
		if (data_.prefix_or_id != pref) {
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different prefix \"" + data_.prefix_or_id
				+ "\". Current prefix is \"" + pref + "\".");
			builder.see_prev_declaration (item ());
			return false;
		}
	}
	return true;
}

bool RepositoryId::prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
{
	if (!data_.explicit_ [EXPLICIT_ID]) {
		if (data_.explicit_ [EXPLICIT_PREFIX]) {
			if (data_.prefix_or_id != pref) {
				builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different prefix \"" + data_.prefix_or_id
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

void RepositoryId::type_id (Builder& builder, const std::string& id, const Location& loc)
{
	if (data_.explicit_ [EXPLICIT_ID]) {
		if (data_.prefix_or_id != id) {
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different repository ID \"" + data_.prefix_or_id + '\"');
			builder.see_prev_declaration (data_.explicit_ [EXPLICIT_ID]);
		}
	} else if (data_.explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_VERSION]);
	} else {
		data_.prefix_or_id = id;
		data_.explicit_ [EXPLICIT_ID] = loc;
	}
}

void RepositoryId::pragma_version (Builder& builder, const Version v, const Location& loc)
{
	if (data_.explicit_ [EXPLICIT_ID]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with repository ID");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_ID]);
	} else if (data_.explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version");
		builder.see_prev_declaration (data_.explicit_ [EXPLICIT_VERSION]);
	} else {
		data_.version = v;
		data_.explicit_ [EXPLICIT_VERSION] = loc;
	}
}

string RepositoryId::repository_id () const
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
	ScopedName sn = item ().scoped_name ();
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
