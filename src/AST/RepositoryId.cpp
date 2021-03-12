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
			case Item::Kind::TYPEDEF:
				p = static_cast <TypeDef*> (item);
				break;
			case Item::Kind::NATIVE:
				p = static_cast <Native*> (item);
				break;
			case Item::Kind::EXCEPTION:
				p = static_cast <Exception*> (item);
				break;
		}
	}
	return p;
}

RepositoryId::RepositoryId (const NamedItem& item, const Builder& builder) :
	item_ (item),
	RepositoryIdData (builder.prefix ())
{}

bool RepositoryId::check_prefix (Builder& builder, const Location& loc) const
{
	if (!explicit_ [EXPLICIT_ID] && !explicit_ [EXPLICIT_PREFIX]) {
		const string& pref = builder.prefix ();
		if (prefix_or_id_ != pref) {
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different prefix \"" + prefix_or_id_
				+ "\". Current prefix is \"" + pref + "\".");
			builder.see_prev_declaration (item ());
			return false;
		}
	}
	return true;
}

bool RepositoryId::prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
{
	if (!explicit_ [EXPLICIT_ID]) {
		if (explicit_ [EXPLICIT_PREFIX]) {
			if (prefix_or_id_ != pref) {
				builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different prefix \"" + prefix_or_id_
					+ "\". Current prefix is \"" + pref + "\".");
				builder.see_prev_declaration (explicit_ [EXPLICIT_PREFIX]);
				return false;
			}
		} else {
			prefix_or_id_ = pref;
			explicit_ [EXPLICIT_PREFIX] = loc;
		}
	}
	return true;
}

void RepositoryId::type_id (Builder& builder, const std::string& id, const Location& loc)
{
	if (explicit_ [EXPLICIT_ID]) {
		if (prefix_or_id_ != id) {
			builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with different repository ID \"" + prefix_or_id_ + "\".");
			builder.see_prev_declaration (explicit_ [EXPLICIT_ID]);
		}
	} else if (explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version.");
		builder.see_prev_declaration (explicit_ [EXPLICIT_VERSION]);
	} else {
		prefix_or_id_ = id;
		explicit_ [EXPLICIT_ID] = loc;
	}
}

void RepositoryId::pragma_version (Builder& builder, const Version v, const Location& loc)
{
	if (explicit_ [EXPLICIT_ID]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with repository ID.");
		builder.see_prev_declaration (explicit_ [EXPLICIT_ID]);
	} else if (explicit_ [EXPLICIT_VERSION]) {
		builder.message (loc, Builder::MessageType::ERROR, item ().qualified_name () + " is already declared with #pragma version.");
		builder.see_prev_declaration (explicit_ [EXPLICIT_VERSION]);
	} else {
		version_ = v;
		explicit_ [EXPLICIT_VERSION] = loc;
	}
}

string RepositoryId::repository_id () const
{
	if (explicit_ [EXPLICIT_ID])
		return prefix_or_id_;
	string id;
	id.reserve (5 + prefix_or_id_.length ());
	id = "IDL:";
	if (!prefix_or_id_.empty ()) {
		id += prefix_or_id_;
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
	id += to_string (version_.major);
	id += '.';
	id += to_string (version_.minor);
	return id;
}

bool RepositoryId::check_unique (Build::Builder& builder, map <std::string, const NamedItem*>& ids) const
{
	auto ins = ids.emplace (repository_id (), &item ());
	if (!ins.second) {
		builder.message (item (), Builder::MessageType::ERROR, string ("Repository ID ") + ins.first->first + " is duplicated.");
		builder.see_declaration_of (*ins.first->second, ins.first->second->qualified_name ());
	}
	return ins.second;
}

}
