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
#include "Builder.h"
#include "EvalIntegral.h"
#include "EvalFloatingPoint.h"
#include "EvalString.h"
#include "EvalFixed.h"
#include "EvalEnum.h"
#include "../../include/AST/Include.h"
#include "../../include/AST/Module.h"
#include "../../include/AST/Native.h"
#include "../../include/AST/TypeDef.h"
#include "../../include/AST/Operation.h"
#include "../../include/AST/Attribute.h"
#include "../../include/AST/Struct.h"
#include "../../include/AST/Union.h"
#include "../../include/AST/Enum.h"
#include "../../include/AST/Exception.h"
#include "../../include/AST/UnionElement.h"
#include "../../include/AST/Constant.h"
#include "../../include/AST/StateMember.h"
#include "../../include/AST/ValueFactory.h"
#include "../../include/AST/ValueBox.h"
#include <stdexcept>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

namespace AST {
namespace Build {

void Builder::message (const Location& l, MessageType mt, const string& err)
{
	static const char* const msg_types [] = { "error", "warning", "message" };

	err_out_ << l.file () << '(' << l.line () << "): " << msg_types [(size_t)mt] << ": " << err << endl;

	if (mt == MessageType::ERROR && (++err_cnt_ >= 20))
		throw runtime_error ("Too many errors, compilation aborted.");
}

void Builder::pragma (const char* s, const Location& loc)
{
	assert (*s == '#');
	++s;
	while (isspace (*s))
		++s;
	if (*s) {
		assert (!strncmp (s, "pragma ", 7));
		s += 7;
		while (isspace (*s))
			++s;
		const char* end = s + 1;
		while (!isspace (*end))
			++end;
		string pr (s, end - s);
		if (pr == "ID") {
			s = end + 1;
			ScopedName name (loc);
			string id;
			if (get_scoped_name (s, name) && get_quoted_string (s, id, loc) && !id.empty ()) {
				type_id (name, id, loc);
				return;
			}
		} else if (pr == "prefix") {
			string pref;
			if (get_quoted_string (s, pref, loc)) {
				prefix (pref, loc);
				return;
			}
		} else if (pr == "version") {
			s = end + 1;
			ScopedName name (loc);
			if (get_scoped_name (s, name)) {
				while (isspace (*s))
					++s;
				Version ver;
				char* endptr;
				unsigned long u = strtoul (s, &endptr, 10);
				if (endptr > s && u <= numeric_limits <uint16_t>::max ()) {
					ver.major = (uint16_t)u;
					s = endptr;
					while (isspace (*s))
						++s;
					if ('.' == *s) {
						++s;
						while (isspace (*s))
							++s;
						u = strtoul (s, &endptr, 10);
						if (endptr > s && u <= numeric_limits <uint16_t>::max ()) {
							ver.minor = (uint16_t)u;
							RepositoryId* rep_id = lookup_rep_id (name);
							if (rep_id)
								rep_id->pragma_version (*this, ver, loc);
							return;
						}
					}
				}
			}
		} else {
			message (loc, MessageType::WARNING, string ("unknown pragma ") + pr);
			return;
		}
	}

	message (loc, MessageType::ERROR, "invalid pragma syntax");
}

void Builder::type_id (const ScopedName& name, const std::string& id, const Location& id_loc)
{
	if (id.empty ())
		message (id_loc, MessageType::ERROR, "the repository id must not be empty");
	else {
		RepositoryId* rep_id = lookup_rep_id (name);
		if (rep_id)
			rep_id->type_id (*this, id, name);
	}
}

RepositoryId* Builder::lookup_rep_id (const ScopedName& name)
{
	RepositoryId* rep_id = nullptr;
	const Ptr <NamedItem>* it = lookup (name);
	if (it) {
		NamedItem* item = *it;
		rep_id = RepositoryId::cast (item);
		if (!rep_id) {
			message (name, MessageType::ERROR, name.stringize () + " has not repository id");
			see_declaration_of (*item, item->qualified_name ());
		}
	}
	return rep_id;
}

void Builder::type_prefix (const ScopedName& name, const Variant& s, const Location& id_loc)
{
	if (!s.empty ()) {
		const string& pref = s.as_string ();
		if (name.empty ())
			prefix (pref, name);
		else {
			const Ptr <NamedItem>* item = lookup (name);
			if (item) {
				ItemScope* scope = ItemScope::cast (*item);
				if (scope) {
					if (prefix_valid (pref, id_loc))
						scope->prefix (*this, pref, name);
				} else {
					message (name, MessageType::ERROR, name.stringize () + " can not have a prefix");
					see_declaration_of (**item, (*item)->qualified_name ());
				}
			}
		}
	}
}

bool Builder::get_quoted_string (const char*& ps, std::string& qs, const Location& loc)
{
	const char* p = ps;
	while (isspace (*p))
		++p;
	if ('"' == *p) {
		++p;
		string s;
		while ('"' != *p) {
			try {
				char c = Eval::unescape_char (p);
				if (!c)
					break;
				s += c;
			} catch (const exception& ex) {
				message (loc, MessageType::ERROR, ex.what ());
				return false;
			}
		}
		if ('"' == *p) {
			ps = p + 1;
			return true;
		} else
			message (loc, MessageType::ERROR, string ("invalid string: ") + ps);
	}
	return false;
}

bool Builder::get_scoped_name (const char*& s, ScopedName& sn)
{
	const char* begin = s;
	char c;
	while ((c = isspace (*begin)))
		++begin;

	if (':' == c) {
		++begin;
		if (':' == *begin) {
			do {
				c = *++begin;
			} while (isspace (c));
			if (!isalpha (c))
				return false;
			sn.from_root = true;
		} else
			return false;
	} else if (!isalpha (c))
		return false;

	assert (isalpha (c));
	assert (*begin == c);

	for (;;) {
		const char* end = begin + 1;
		for (;;) {
			c = *end;
			if (isalnum (c) || '_' == c)
				++end;
			else
				break;
		}
		sn.push_back (Identifier (begin, end - begin));
		begin = end;
		while ((c = isspace (*begin)))
			++begin;
		if (':' == c) {
			++begin;
			if (':' == *begin)
				c = *++begin;
			else
				return false;
		} else if (!c)
			break;
	}
	return true;
}

void Builder::file (const std::string& name, const Location& loc, int flags)
{
	auto ins = tree_->add_file (name);
	if (!ins.second && !(flags & FILE_FLAG_START)) {
		const string& file = *ins.first;
		auto it = file_stack_.end () - 1;
		for (; it != file_stack_.begin (); --it) {
			if (it->file == &file)
				break;
		}
		if (it->file == &file) {
			file_stack_.erase (it + 1, file_stack_.end ());
			if (file_stack_.size () == 1)
				is_main_file_ = true;
			return;
		}
	}
	// Use #include at global scope only.
	if ((flags & FILE_FLAG_START) && is_main_file () && container_stack_.size () == 1) {
		is_main_file_ = false;
		try {
			filesystem::path file;
			if (flags & FILE_FLAG_SYSTEM)
				file = name;
			else
				file = filesystem::relative (name, tree_->file ().parent_path ());
			tree_->append (*Ptr <Item>::make <Include> (move (file), flags & FILE_FLAG_SYSTEM));
		} catch (const filesystem::filesystem_error& ex) {
			message (loc, MessageType::ERROR, ex.what ());
		}
	}
	file_stack_.emplace_back (*ins.first);
}

void Builder::error_name_collision (const SimpleDeclarator& name, const Location& prev_loc)
{
	message (name, MessageType::ERROR, name + " is already declared");
	see_prev_declaration (prev_loc);
}

void Builder::see_prev_declaration (const Location& loc)
{
	message (loc, MessageType::MESSAGE, "see previous declaration");
}

void Builder::see_declaration_of (const Location& loc, const string& name)
{
	message (loc, MessageType::MESSAGE, string ("see declaration of ") + name);
}

const Ptr <NamedItem>* Builder::lookup (const ScopedName& scoped_name)
{
	auto name = scoped_name.begin ();
	pair <bool, const Ptr <NamedItem>*> f = { false, nullptr };
	if (scoped_name.from_root) {
		const Symbols& scope = *tree_;
		const Ptr <NamedItem>* p = scope.find (*name);
		f = make_pair (p, p);
	} else {
		for (ScopeStack::const_iterator it = scope_stack_.end (); it != scope_stack_.begin ();) {
			--it;
			const ItemScope* scope = *it;
			if (scope) {
				f = lookup (*scope, *name, scoped_name);
				if (f.first)
					break;
			}
		}

		if (!f.first) {
			const Ptr <NamedItem>* p = static_cast <const Symbols&> (*tree_).find (*name);
			f = make_pair (p, p);
		}
	}

	while (f.second && scoped_name.end () != ++name) {
		const ItemScope* scope = ItemScope::cast (*f.second);
		if (scope) {
			f = lookup (*scope, *name, scoped_name);
			if (!f.first)
				break;
		} else
			break;
	}

	if (!f.first)
		message (scoped_name, MessageType::ERROR, string ("symbol not found: ") + scoped_name.stringize ());

	return f.second;
}

pair <bool, const Ptr <NamedItem>*> Builder::lookup (const ItemScope& scope, const Identifier& name, const Location& loc)
{
	switch (scope.kind ()) {

		case Item::Kind::INTERFACE: {
			Containers containers;
			static_cast <const Interface&> (scope).get_all_containers (containers);
			return lookup (containers, name, loc);
		} break;

		case Item::Kind::VALUE_TYPE: {
			Containers containers;
			static_cast <const ValueType&> (scope).get_all_containers (containers);
			return lookup (containers, name, loc);
		} break;

		default: {
			const Ptr <NamedItem>* p = static_cast <const Symbols&> (scope).find (name);
			return make_pair (p, p);
		}
	}
}

std::pair <bool, const Ptr <NamedItem>*> Builder::lookup (const Containers& containers, const Identifier& name, const Location& loc)
{
	set <const ItemContainer*> unique;
	set <const Ptr <NamedItem>*> found;
	for (const ItemContainer* cont : containers) {
		if (unique.insert (cont).second) {
			auto p = static_cast <const Symbols&> (*cont).find (name);
			if (p)
				found.insert (p);
		}
	}
	if (found.size () > 1) {
		// Ambiguous
		message (loc, Builder::MessageType::ERROR, string ("ambiguous name ") + name);
		auto it = found.begin ();
		const NamedItem* p = **it;
		message (*p, Builder::MessageType::MESSAGE, string ("could be ") + p->qualified_name ());
		++it;
		for (;;) {
			p = **it;
			string msg = string ("or ") + p->qualified_name ();
			if (found.end () == ++it) {
				msg += '.';
				message (*p, Builder::MessageType::MESSAGE, msg);
				break;
			} else
				message (*p, Builder::MessageType::MESSAGE, msg);
		}
		return make_pair (true, nullptr);
	} else if (!found.empty ())
		return make_pair (true, *found.begin ());
	else
		return make_pair (false, nullptr);
}

unsigned Builder::positive_int (const Variant& v, const Location& loc)
{
	assert (v.is_integral ());
	try {
		uint32_t i = v.to_unsigned_long ();
		if (i)
			return i;
		message (loc, Builder::MessageType::ERROR, "expected positive integer");
	} catch (const exception& ex) {
		message (loc, Builder::MessageType::ERROR, ex.what ());
	}
	return 1;
}

Type Builder::lookup_type (const ScopedName& scoped_name)
{
	const Ptr <NamedItem>* item = lookup (scoped_name);
	if (item) {
		const NamedItem* p = *item;
		if (!p->is_type ()) {
			message (scoped_name, MessageType::ERROR, scoped_name.stringize () + " is not a type");
			item = nullptr;
		}
	}
	return Type (item);
}

ItemScope* Builder::cur_parent () const
{
	if (!scope_stack_.empty ())
		return scope_stack_.back ();
	else
		return nullptr;
}

Symbols* Builder::cur_scope () const
{
	if (!scope_stack_.empty ()) {
		ItemScope* scope = scope_stack_.back ();
		if (scope)
			return &static_cast <Symbols&> (*scope);
		else
			return nullptr;
	} else
		return &static_cast <Symbols&> (*tree_);
}

void Builder::scope_push (ItemContainer* scope)
{
	scope_stack_.push_back (scope);
	if (is_main_file ()) {
		if (scope)
			container_stack_.top ()->append (*scope);
		container_stack_.push (scope);
	}
}

Symbols* Builder::scope_begin ()
{
	Symbols* scope = cur_scope ();
	if (!scope)
		scope_push (nullptr);
	return scope;
}

void Builder::scope_end ()
{
	assert (!scope_stack_.empty ());
	scope_stack_.pop_back ();
	if (is_main_file ()) {
		assert (container_stack_.size () > 1);
		container_stack_.pop ();
	}
}

const string& Builder::prefix () const
{
	const ItemScope* scope = cur_parent ();
	if (scope)
		return scope->prefix ();
	else
		return file_stack_.back ().prefix;
}

void Builder::prefix (const std::string& pref, const Location& loc)
{
	if (prefix_valid (pref, loc)) {
		if (!scope_stack_.empty ()) {
			ItemScope* scope = scope_stack_.back ();
			if (scope)
				scope->prefix (*this, pref, loc);
		} else
			file_stack_.back ().prefix = pref;
	}
}

bool Builder::prefix_valid (const std::string& pref, const Location& loc)
{
	bool valid = true;
	if (!pref.empty ()) {
		switch (pref.front ()) {
			case '_':
			case '-':
			case '.':
				valid = false;
				message (loc, MessageType::ERROR, "the prefix shall not begin with the characters underscore (_), hyphen (-) or period (.)");
		}
		if (valid) {
			if (pref.back () == '/') {
				valid = false;
				message (loc, MessageType::ERROR, "the prefix shall not contain a trailing slash (/)");
			} else {
				for (const char* p = pref.c_str (); true; ++p) {
					char c = *p;
					if (!c)
						break;
					switch (c) {
						case '_':
						case '-':
						case '.':
						case '/':
							break;
						default:
							if (!isalnum (c)) {
								valid = false;
								string msg = "invalid character '";
								Variant::append (msg, c);
								msg += "' in the prefix";
								message (loc, MessageType::ERROR, msg);
							}
					}
					if (!valid)
						break;
				}
			}
		}
	}
	return valid;
}

const Ptr <NamedItem>* Builder::constr_type_end ()
{
	ItemScope* type = scope_stack_.back ();

	if (type) {
		// Delete members from scope
		Symbols& scope = *type;
		for (auto it = scope.begin (); it != scope.end ();) {
			switch ((*it)->kind ()) {
				case Item::Kind::MEMBER:
				case Item::Kind::UNION_ELEMENT:
					it = scope.erase (it);
					break;
				default:
					++it;
			}
		}
	}

	scope_end ();

	if (type) {
		const Symbols* scope = cur_scope ();
		if (scope) {
			auto f = scope->find (type->name ());
			assert (f);
			return f;
		}
	}

	return nullptr;
}

void Builder::module_begin (const SimpleDeclarator& name)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		Ptr <Module> mod = Ptr <Module>::make <Module> (ref (*this), ref (name));
		auto ins = scope->insert (*mod);
		if (!ins.second && (*ins.first)->kind () != Item::Kind::MODULE) {
			error_name_collision (name, **ins.first);
			scope_push (nullptr);
		} else {
			Module* mod = scast <Module> (*ins.first);
			scope_stack_.push_back (mod);
			if (is_main_file ()) {
				Ptr <ModuleItems> cont = Ptr <ModuleItems>::make <ModuleItems> (std::ref (*mod));
				container_stack_.top ()->append (*cont);
				container_stack_.push (cont);
			}
		}
	}
}

void Builder::native (const SimpleDeclarator& name)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <NamedItem> def = Ptr <NamedItem>::make <Native> (ref (*this), ref (name));
		auto ins = scope->insert (*def);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (*def);
	}
}

void Builder::type_def (const Type& type, const Declarators& declarators)
{
	check_complete_or_ref (type, declarators.front ());
	Symbols* scope = cur_scope ();
	if (scope) {
		for (auto decl = declarators.begin (); decl != declarators.end (); ++decl) {
			Ptr <NamedItem> def;
			if (decl->array_sizes ().empty ())
				def = Ptr <NamedItem>::make <TypeDef> (ref (*this), ref (*decl), ref (type));
			else {
				Type arr (type, decl->array_sizes ());
				def = Ptr <NamedItem>::make <TypeDef> (ref (*this), ref (*decl), ref (arr));
			}
			auto ins = scope->insert (*def);
			if (!ins.second)
				error_name_collision (*decl, **ins.first);
			else if (is_main_file ())
				container_stack_.top ()->append (*def);
		}
	}
}

void Builder::error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc)
{
	string msg;
	if (new_kind.interface_kind () != InterfaceKind::Kind::UNCONSTRAINED) {
		msg += new_kind.interface_kind_name ();
		msg += ' ';
	}
	msg += "interface ";
	msg += name;
	msg += " is already declared as ";
	if (prev_kind.interface_kind () != InterfaceKind::Kind::UNCONSTRAINED)
		msg += prev_kind.interface_kind_name ();
	else
		msg += "unconstrained";
	message (name, MessageType::ERROR, msg);
	see_prev_declaration (prev_loc);
}

void Builder::error_valuetype_mod (const SimpleDeclarator& name, bool is_abstract, const Location& prev_loc)
{
	string msg;
	if (is_abstract)
		msg += "abstract ";
	msg += "valuetype ";
	msg += name;
	msg += " is already declared as ";
	if (is_abstract)
		msg += "concrete";
	else
		msg += "abstract ";
	message (name, MessageType::ERROR, msg);
	see_prev_declaration (prev_loc);
}

void Builder::interface_decl (const SimpleDeclarator& name, InterfaceKind ik)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <InterfaceDecl> decl = Ptr <InterfaceDecl>::make <InterfaceDecl> (ref (*this), name, ik);
		auto ins = scope->insert (*decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			InterfaceKind prev_ik;
			switch (item.kind ()) {
				case Item::Kind::INTERFACE_DECL: {
					const InterfaceDecl& itf = static_cast <const InterfaceDecl&> (item);
					rid = &itf;
					prev_ik = itf;
				} break;
				case Item::Kind::INTERFACE: {
					const Interface& itf = static_cast <const Interface&> (item);
					rid = &itf;
					prev_ik = itf;
				} break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			if (prev_ik.interface_kind () != ik.interface_kind ())
				error_interface_kind (name, ik, prev_ik, item);

			rid->check_prefix (*this, name);
			static_cast <RepositoryId&> (*decl) = *rid;
			return; // Second declaration - skip it.
		}

		if (is_main_file ())
			container_stack_.top ()->append (*decl);
	}
}

void Builder::valuetype_decl (const SimpleDeclarator& name, bool is_abstract)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <ValueTypeDecl> decl = Ptr <ValueTypeDecl>::make <ValueTypeDecl> (ref (*this), name, is_abstract);
		auto ins = scope->insert (*decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			bool prev_abstract;
			switch (item.kind ()) {
				case Item::Kind::VALUE_TYPE_DECL: {
					const ValueTypeDecl& vt = static_cast <const ValueTypeDecl&> (item);
					rid = &vt;
					prev_abstract = vt.is_abstract ();
				} break;
				case Item::Kind::VALUE_TYPE: {
					const ValueType& vt = static_cast <const ValueType&> (item);
					rid = &vt;
					prev_abstract = vt.modifier () == ValueType::Modifier::ABSTRACT;
				} break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			if (prev_abstract != is_abstract)
				error_valuetype_mod (name, is_abstract, item);

			rid->check_prefix (*this, name);
			static_cast <RepositoryId&> (*decl) = *rid;
			return; // Ignore second declaration
		}

		if (is_main_file ())
			container_stack_.top ()->append (*decl);
	}
}

void Builder::interface_begin (const SimpleDeclarator& name, InterfaceKind ik)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		Ptr <Interface> itf = Ptr <Interface>::make <Interface> (ref (*this), ref (name), ik);
		auto ins = scope->insert (*itf);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if ((*ins.first)->kind () != Item::Kind::INTERFACE_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			} else {
				const InterfaceDecl& decl = static_cast <const InterfaceDecl&> (item);
				if (decl.interface_kind () != ik.interface_kind ())
					error_interface_kind (name, ik, decl, decl);
				decl.check_prefix (*this, name);
				static_cast <RepositoryId&> (*itf) = decl;
				const_cast <Ptr <NamedItem>&> (*ins.first) = itf;
				itf->set_has_forward_dcl ();
			}
		}

		scope_push (itf);
	}
}

void Builder::valuetype_begin (const SimpleDeclarator& name, ValueType::Modifier mod)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		Ptr <ValueType> vt = Ptr <ValueType>::make <ValueType> (ref (*this), ref (name), mod);
		auto ins = scope->insert (*vt);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if ((*ins.first)->kind () != Item::Kind::VALUE_TYPE_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			} else {
				const ValueTypeDecl& decl = static_cast <const ValueTypeDecl&> (item);
				bool is_abstract = ValueType::Modifier::ABSTRACT == mod;
				if (is_abstract != decl.is_abstract ())
					error_valuetype_mod (name, is_abstract, decl);
				decl.check_prefix (*this, name);
				static_cast <RepositoryId&> (*vt) = decl;
				const_cast <Ptr <NamedItem>&> (*ins.first) = vt;
			}
			vt->set_has_forward_dcl ();
		}

		scope_push (vt);
	}
}

void Builder::interface_bases (const ScopedNames& bases)
{
	assert (!bases.empty ());
	assert (!scope_stack_.empty ());
	Interface* itf = static_cast <Interface*> (scope_stack_.back ());
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE);
		assert (itf->interface_kind () != InterfaceKind::PSEUDO);

		// Process bases
		map <const Item*, Location> direct_bases;
		for (auto base_name = bases.begin (); base_name != bases.end (); ++base_name) {
			const Ptr <NamedItem>* pbase = lookup (*base_name);
			if (pbase) {
				const NamedItem* base = *pbase;
				const char* err = nullptr;
				if (base->kind () != Item::Kind::INTERFACE) {
					if (base->kind () == Item::Kind::INTERFACE_DECL)
						err = "incomplete type is not allowed";
					else
						err = "invalid base type";
				} else {
					const Interface* base_itf = static_cast <const Interface*> (base);
					if (itf == base_itf) {
						message (*base_name, MessageType::ERROR, "may not derive from itself");
						continue;
					}
					switch (itf->interface_kind ()) {
						case InterfaceKind::UNCONSTRAINED:
							switch (base_itf->interface_kind ()) {
								case InterfaceKind::LOCAL:
									err = "the unconstrained interface may not inherit a local interface";
									break;
								case InterfaceKind::PSEUDO:
									err = "the unconstrained interface may not inherit a pseudo interface";
									break;
							}
							break;
						case InterfaceKind::ABSTRACT:
							if (InterfaceKind::ABSTRACT != base_itf->interface_kind ())
								err = "the abstract interface may only inherit abstract interfaces";
							break;
						case InterfaceKind::PSEUDO:
							if (InterfaceKind::PSEUDO != base_itf->interface_kind ())
								err = "the pseudo interface may only inherit pseudo interfaces";
							break;
					}
					if (!err) {
						auto ins = direct_bases.emplace (base, *base_name);
						if (!ins.second) {
							message (*base_name, MessageType::ERROR, base_name->stringize () + " is already base of " + itf->name ());
							see_prev_declaration (ins.first->second);
							continue;
						} else {
							Containers bases;
							base_itf->get_all_containers (bases);
							add_base_members (*base_name, bases);
						}

						// OK
						itf->add_base (*base_itf);
					}
				}
				if (err) {
					message (*base_name, MessageType::ERROR, err);
					see_declaration_of (*base, base_name->stringize ());
				}
			}
		}
	}
}

void Builder::valuetype_bases (bool truncatable, const ScopedNames& bases)
{
	assert (!bases.empty ());
	assert (!scope_stack_.empty ());
	ValueType* vt = static_cast <ValueType*> (scope_stack_.back ());
	if (vt) {
		assert (vt->kind () == Item::Kind::VALUE_TYPE);
		if (truncatable) {
			if (vt->modifier () != ValueType::Modifier::NONE)
				message (bases.front (), MessageType::ERROR, string (vt->modifier_name ()) + " valuetype may not be truncatable");
			else
				vt->set_truncatable ();
		}

		// Process bases
		map <const Item*, Location> direct_bases;
		bool first = true;
		for (auto base_name = bases.begin (); base_name != bases.end (); first = false, ++base_name) {
			const Ptr <NamedItem>* pbase = lookup (*base_name);
			if (pbase) {
				const NamedItem* base = *pbase;
				const char* err = nullptr;
				if (base->kind () != Item::Kind::VALUE_TYPE) {
					if (base->kind () == Item::Kind::VALUE_TYPE_DECL)
						err = "incomplete type is not allowed";
					else
						err = "invalid base type";
				} else {
					const ValueType* base_vt = static_cast <const ValueType*> (base);
					if (vt == base_vt) {
						message (*base_name, MessageType::ERROR, "may not derive from itself");
						continue;
					}

					if (base_vt->modifier () != ValueType::Modifier::ABSTRACT) {
						if (!first)
							err = "concrete base must be first";
						else if (vt->modifier () == ValueType::Modifier::ABSTRACT)
							err = "concrete type is not allowed";
					}
					if (!err) {
						auto ins = direct_bases.emplace (base, *base_name);
						if (!ins.second) {
							message (*base_name, MessageType::ERROR, base_name->stringize () + " is already base of " + vt->name ());
							see_prev_declaration (ins.first->second);
							continue;
						} else {
							Containers bases;
							base_vt->get_all_containers (bases);
							add_base_members (*base_name, bases);
						}

						// OK
						vt->add_base (*base_vt);
					}
				}
				if (err) {
					message (*base_name, MessageType::ERROR, err);
					see_declaration_of (*base, base_name->stringize ());
				}
			}
		}
	}
}

void Builder::valuetype_supports (const ScopedNames& bases)
{
	assert (!bases.empty ());
	assert (!scope_stack_.empty ());
	ValueType* vt = static_cast <ValueType*> (scope_stack_.back ());
	if (vt) {
		assert (vt->kind () == Item::Kind::VALUE_TYPE);

		// Process bases
		map <const Item*, Location> direct_bases;
		bool first = true;
		for (auto base_name = bases.begin (); base_name != bases.end (); first = false, ++base_name) {
			const Ptr <NamedItem>* pbase = lookup (*base_name);
			if (pbase) {
				const NamedItem* base = *pbase;
				const char* err = nullptr;
				if (base->kind () != Item::Kind::INTERFACE) {
					if (base->kind () == Item::Kind::INTERFACE_DECL)
						err = "incomplete type is not allowed";
					else
						err = "invalid base type";
				} else {
					const Interface* base_itf = static_cast <const Interface*> (base);

					if (base_itf->interface_kind () != InterfaceKind::Kind::ABSTRACT && !first)
						err = "concrete base must be first";
					else {
						auto ins = direct_bases.emplace (base, *base_name);
						if (!ins.second) {
							message (*base_name, MessageType::ERROR, base_name->stringize () + " is already base of " + vt->name ());
							see_prev_declaration (ins.first->second);
							continue;
						} else {
							Containers bases;
							base_itf->get_all_containers (bases);
							add_base_members (*base_name, bases);
						}

						// OK
						vt->add_supports (*base_itf);
					}
				}
				if (err) {
					message (*base_name, MessageType::ERROR, err);
					see_declaration_of (*base, base_name->stringize ());
				}
			}
		}
	}
}

void Builder::add_base_members (const Location& loc, const Containers& bases)
{
	for (const ItemContainer* base : bases) {
		if (interface_.all_bases.insert (base).second) {
			// Check member names
			for (auto it = base->begin (); it != base->end (); ++it) {
				const Item* item = *it;
				switch (item->kind ()) {
					case Item::Kind::STATE_MEMBER: {
						const StateMember* member = static_cast <const StateMember*> (item);
						if (!member->is_public ())
							break; // Private members are not accessible via DynAny.
					}
					case Item::Kind::OPERATION:
					case Item::Kind::ATTRIBUTE: {
						NamedItem* member = static_cast <NamedItem*> (const_cast <Item*> (item));
						auto ins = interface_.all_members.insert (*member);
						if (!ins.second) {
							message (loc, MessageType::ERROR, string ("member name collision: ") + member->name ());
							message (**ins.first, MessageType::MESSAGE, string ("see ") + (*ins.first)->qualified_name ());
							message (*member, MessageType::MESSAGE, string ("see ") + member->qualified_name ());
						}
					} break;
				}
			}
		}
	}
}

bool Builder::check_member_name (const NamedItem& item)
{
#ifdef _DEBUG
	assert (!scope_stack_.empty ());
	ItemContainer* parent = static_cast <ItemContainer*> (scope_stack_.back ());
	assert (parent && parent->kind () == Item::Kind::INTERFACE || parent->kind () == Item::Kind::VALUE_TYPE);
#endif
	auto ins = interface_.all_members.insert (item);
	if (!ins.second) {
		message (item, MessageType::ERROR, string ("member name collision: ") + item.name ());
		message (**ins.first, MessageType::MESSAGE, string ("see ") + (*ins.first)->qualified_name ());
	}
	return ins.second;
}

void Builder::operation_begin (bool oneway, const Type& type, const SimpleDeclarator& name)
{
	assert (!scope_stack_.empty ());
	assert (!operation_.op); // operation_end () must be called

	check_anonymous (type, name);

	ItemContainer* parent = static_cast <ItemContainer*> (scope_stack_.back ());
	if (parent) {
		assert (parent->kind () == Item::Kind::INTERFACE || parent->kind () == Item::Kind::VALUE_TYPE);
		if (oneway && type.tkind () != Type::Kind::VOID) {
			message (name, MessageType::WARNING, "'oneway' operation must be 'void'. The 'oneway' attribute will be ignored");
			oneway = false;
		}
		Ptr <OperationBase> op = Ptr <OperationBase>::make <Operation> (ref (*this), oneway, ref (type), ref (name));
		if (check_member_name (*op)) {
			auto ins = static_cast <Symbols&> (*parent).insert (*op);
			if (!ins.second)
				error_name_collision (name, **ins.first); // Op name collides with nested type.
			else {
				operation_.op = op;
				// We always append operation to the container, whatever it is the main file or not.
				// We need it to build all_operations for derived interfaces.
				parent->append (*op);
			}
		}
	}
}

void Builder::valuetype_factory_begin (const SimpleDeclarator& name)
{
	assert (!scope_stack_.empty ());
	assert (!operation_.op); // operation_end () must be called

	ItemContainer* parent = static_cast <ItemContainer*> (scope_stack_.back ());
	if (parent) {
		assert (parent->kind () == Item::Kind::VALUE_TYPE);
		assert (static_cast <const ValueType*> (parent)->modifier () != ValueType::Modifier::ABSTRACT);
		Ptr <OperationBase> op = Ptr <OperationBase>::make <ValueFactory> (ref (*this), ref (name));
		if (check_member_name (*op)) {
			auto ins = static_cast <Symbols&> (*parent).insert (*op);
			if (!ins.second)
				error_name_collision (name, **ins.first); // Name collides with nested type.
			else {
				operation_.op = op;
				// We always append operation to the container, whatever it is the main file or not.
				// We need it to build all_operations for derived interfaces.
				parent->append (*op);
			}
		}
	}
}

void Builder::parameter (Parameter::Attribute att, const Type& type, const SimpleDeclarator& name)
{
	OperationBase* op = operation_.op;
	if (op) {
		if (att != Parameter::Attribute::IN) {
			assert (op->kind () == Item::Kind::OPERATION);
			Operation* itf_op = static_cast <Operation*> (op);
			if (itf_op->oneway ()) {
				message (name, MessageType::WARNING, "'oneway' operation can not return data. The 'oneway' attribute will be ignored");
				itf_op->oneway_clear ();
			}
		}
		Ptr <Parameter> par = Ptr <Parameter>::make <Parameter> (ref (*this), att, ref (type), ref (name));
		auto ins = operation_.params.insert (*par);
		if (!ins.second)
			message (name, MessageType::ERROR, string ("duplicated parameter ") + name);
		else if (is_main_file ())
			op->append (*par);
	}
}

void Builder::raises (const ScopedNames& names)
{
	if (operation_.op)
		operation_.op->raises (lookup_exceptions (names));
	else if (attribute_.att)
		attribute_.att->getraises (lookup_exceptions (names));
}

Raises Builder::lookup_exceptions (const ScopedNames& names)
{
	map <const Item*, Location> unique;
	Raises exceptions;
	for (auto name = names.begin (); name != names.end (); ++name) {
		const Ptr <NamedItem>* l = lookup (*name);
		if (l) {
			const NamedItem* item = *l;
			if (item->kind () != Item::Kind::EXCEPTION) {
				message (*name, MessageType::ERROR, name->stringize () + " is not an exception type");
				see_declaration_of (*item, item->qualified_name ());
			} else {
				auto ins = unique.emplace (item, *name);
				if (!ins.second) {
					message (*name, MessageType::ERROR, string ("duplicated exception specification ") + name->stringize ());
					see_prev_declaration (ins.first->second);
				} else
					exceptions.push_back (static_cast <const Exception*> (item));
			}
		}
	}
	return exceptions;
}

void Builder::operation_context (const Variants& strings)
{
	OperationBase* op = operation_.op;
	if (op) {
		assert (op->kind () == Item::Kind::OPERATION);
		Operation::Context ctx;
		for (auto it = strings.begin (); it != strings.end (); ++it) {
			if (!it->empty ()) {
				assert (it->vtype () == Variant::VT::STRING);
				ctx.push_back (it->as_string ());
			}
		}

		static_cast <Operation*> (op)->context (move (ctx));
	}
}

void Builder::attribute (bool readonly, const Type& type, const SimpleDeclarators& declarators)
{
	for (auto name = declarators.begin (); name != declarators.end (); ++name) {
		attribute_begin (readonly, type, *name);
		attribute_end ();
	}
}

void Builder::attribute_begin (bool readonly, const Type& type, const SimpleDeclarator& name)
{
	assert (!scope_stack_.empty ());
	assert (!attribute_.att); // attribute_end () must be called

	ItemContainer* parent = static_cast <ItemContainer*> (scope_stack_.back ());
	if (parent) {
		assert (parent->kind () == Item::Kind::INTERFACE || parent->kind () == Item::Kind::VALUE_TYPE);
		Ptr <Attribute> item = Ptr <Attribute>::make <Attribute> (ref (*this), readonly, ref (type), ref (name));
		if (check_member_name (*item)) {
			auto ins = static_cast <Symbols&> (*parent).insert (*item);
			if (!ins.second)
				error_name_collision (name, **ins.first); // Name collides with nested type.
			else {
				attribute_.att = item;
				// We always append attribute to the container, whatever it is the main file or not.
				// We need it to build all_operations for derived interfaces.
				parent->append (*item);
			}
		}
	}
}

void Builder::getraises (const ScopedNames& names)
{
	Attribute* att = attribute_.att;
	if (att)
		att->getraises (lookup_exceptions (names));
}

void Builder::setraises (const ScopedNames& names)
{
	Attribute* att = attribute_.att;
	if (att)
		att->setraises (lookup_exceptions (names));
}

void Builder::interface_end ()
{
	assert (!scope_stack_.empty ());
	
	ItemScope* itf = scope_stack_.back ();
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE || itf->kind () == Item::Kind::VALUE_TYPE);
		// Delete all operations and attributes from scope
		Symbols& scope = *itf;
		for (auto it = scope.begin (); it != scope.end ();) {
			switch ((*it)->kind ()) {
				case Item::Kind::OPERATION:
				case Item::Kind::ATTRIBUTE:
				case Item::Kind::STATE_MEMBER:
				case Item::Kind::VALUE_FACTORY:
					it = scope.erase (it);
					break;
				default:
					++it;
			}
		}
	}
	scope_end ();
	interface_.clear ();
}

void Builder::struct_decl (const SimpleDeclarator& name)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <StructDecl> decl = Ptr <StructDecl>::make <StructDecl> (ref (*this), ref (name));
		auto ins = scope->insert (*decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			switch (item.kind ()) {
				case Item::Kind::STRUCT_DECL:
					rid = &static_cast <const StructDecl&> (item);
					break;
				case Item::Kind::STRUCT:
					rid = &static_cast <const Struct&> (item);
					break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			rid->check_prefix (*this, name);
			return; // Ignore second declaration
		}

		if (is_main_file ())
			container_stack_.top ()->append (*decl);
	}
}

void Builder::struct_begin (const SimpleDeclarator& name)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		Ptr <Struct> def = Ptr <Struct>::make <Struct> (ref (*this), ref (name));
		auto ins = scope->insert (*def);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if (item.kind () != Item::Kind::STRUCT_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			}

			const StructDecl& decl = static_cast <const StructDecl&> (item);
			decl.check_prefix (*this, name);
			static_cast <RepositoryId&> (*def) = decl;
			const_cast <Ptr <NamedItem>&> (*ins.first) = def;
			def->set_has_forward_dcl ();
		}

		scope_push (def);
	}
}

void Builder::exception_begin (const SimpleDeclarator& name)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		Ptr <Exception> def = Ptr <Exception>::make <Exception> (ref (*this), ref (name));
		auto ins = scope->insert (*def);
		if (!ins.second) {
			error_name_collision (name, **ins.first);
			scope_push (nullptr);
			return;
		}

		scope_push (def);
	}
}

bool Builder::check_complete_or_ref (const Type& type, const Location& loc)
{
	if (!type.is_complete_or_ref ()) {
		message (loc, MessageType::ERROR, "incomplete type is not allowed");
		see_declaration_of (type.named_type (), type.named_type ().qualified_name ());
		return false;
	}
	return true;
}

void Builder::member (const Type& type, const Declarators& names)
{
	assert (!scope_stack_.empty ());
	ItemScope* parent = scope_stack_.back ();
	if (parent) {
		assert (parent->kind () == Item::Kind::STRUCT || parent->kind () == Item::Kind::EXCEPTION);

		if (check_complete_or_ref (type, names.front ())) {
			for (auto decl = names.begin (); decl != names.end (); ++decl) {
				Ptr <NamedItem> item;
				if (decl->array_sizes ().empty ()) {
					item = Ptr <NamedItem>::make <Member> (ref (*this), ref (type), ref (*decl));
				} else {
					Type arr (type, decl->array_sizes ());
					item = Ptr <NamedItem>::make <Member> (ref (*this), ref (arr), ref (*decl));
				}
				auto ins = static_cast <Symbols&> (*parent).insert (*item);
				if (!ins.second)
					error_name_collision (*decl, **ins.first);
				else if (is_main_file ())
					container_stack_.top ()->append (*item);
			}
		}
	}
}

void Builder::state_member (bool is_public, const Type& type, const Declarators& names)
{
	assert (!scope_stack_.empty ());
	ValueType* vt = static_cast <ValueType*> (scope_stack_.back ());
	if (vt) {
		assert (vt->kind () == Item::Kind::VALUE_TYPE);
		assert (vt->modifier () != ValueType::Modifier::ABSTRACT);
		if (check_complete_or_ref (type, names.front ())) {
			for (auto name = names.begin (); name != names.end (); ++name) {
				Ptr <NamedItem> item = Ptr <NamedItem>::make <StateMember> (ref (*this), is_public, ref (type), ref (*name));
				if (!is_public || check_member_name (*item)) {
					auto ins = static_cast <Symbols&> (*vt).insert (*item);
					if (!ins.second)
						error_name_collision (*name, **ins.first); // Name collides with nested type.
					else {
						// We always append member to the container, whatever it is the main file or not.
						// We need it to build all_operations for derived interfaces.
						vt->append (*item);
					}
				}
			}
		}
	}
}

void Builder::union_decl (const SimpleDeclarator& name)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <UnionDecl> decl = Ptr <UnionDecl>::make <UnionDecl> (ref (*this), ref (name));
		auto ins = scope->insert (*decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			switch (item.kind ()) {
				case Item::Kind::UNION_DECL:
					rid = &static_cast <const UnionDecl&> (item);
					break;
				case Item::Kind::UNION:
					rid = &static_cast <const Union&> (item);
					break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			rid->check_prefix (*this, name);
			return; // Ignore second definition
		}

		if (is_main_file ())
			container_stack_.top ()->append (*decl);
	}
}

void Builder::union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc)
{
	Symbols* scope = scope_begin ();
	if (scope) {
		const Type& t = switch_type.dereference_type ();

		bool type_OK = false;
		const NamedItem* enum_type = nullptr;
		switch (t.tkind ()) {
			case Type::Kind::BASIC_TYPE: {
				BasicType bt = t.basic_type ();
				if (is_integral (bt))
					type_OK = true;
			} break;

			case Type::Kind::NAMED_TYPE: {
				if (t.named_type ().kind () == Item::Kind::ENUM) {
					enum_type = &switch_type.named_type ();
					type_OK = true;
				}
			}
		}

		if (!type_OK) {
			message (type_loc, MessageType::ERROR, "invalid switch type");
			eval_stack_.push (make_unique <Eval> (*this));
			scope_push (nullptr);
			return;
		}

		Ptr <Union> def = Ptr <Union>::make <Union> (ref (*this), ref (name), ref (switch_type));
		auto ins = scope->insert (*def);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if (item.kind () != Item::Kind::UNION_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			}

			const UnionDecl& decl = static_cast <const UnionDecl&> (item);
			decl.check_prefix (*this, name);
			static_cast <RepositoryId&> (*def) = decl;
			const_cast <Ptr <NamedItem>&> (*ins.first) = def;
			def->set_has_forward_dcl ();
		}

		scope_push (def);
		eval_push (switch_type, type_loc);

		// If the <switch_type_spec> is an enumeration, the identifier for the enumeration is
		// as well in the scope of the union; as a result, it must be distinct from the element declarators.
		if (enum_type)
			cur_scope ()->insert (*enum_type);
	}
}

void Builder::union_label (const Variant& label, const Location& loc)
{
	assert (!scope_stack_.empty ());
	if (scope_stack_.back ()) {
		if (!label.empty ()) {
			auto ins = union_.all_labels.emplace (label.to_key (), loc);
			if (!ins.second) {
				message (loc, MessageType::ERROR, label.dereference_const ().to_string () + " is already used");
				see_prev_declaration (ins.first->second);
			} else if (union_.element.is_default)
				message (loc, MessageType::WARNING, "default element, case is ignored");
			else
				union_.element.labels.push_back (label);
		}
	}
}

void Builder::union_default (const Location& loc)
{
	assert (!scope_stack_.empty ());
	if (scope_stack_.back ()) {
		if (union_.has_default)
			message (loc, MessageType::ERROR, "union already has the default element");
		else {
			union_.has_default = true;
			union_.default_loc = loc;
			union_.element.is_default = true;
			if (!union_.element.labels.empty ()) {
				message (loc, MessageType::WARNING, "default element, other cases are ignored");
				union_.element.labels.clear ();
			}
		}
	}
}

void Builder::union_element (const Type& type, const Build::Declarator& decl)
{
	assert (!scope_stack_.empty ());
	ItemScope* parent = scope_stack_.back ();
	if (parent && (union_.element.is_default || !union_.element.labels.empty ())) {
		assert (parent->kind () == Item::Kind::UNION);
		Ptr <NamedItem> item;
		if (decl.array_sizes ().empty ()) {
			item = Ptr <NamedItem>::make <UnionElement> (ref (*this), move (union_.element.labels), ref (type), ref (decl));
		} else {
			Type arr (type, decl.array_sizes ());
			item = Ptr <NamedItem>::make <UnionElement> (ref (*this), move (union_.element.labels), ref (arr), ref (decl));
		}
		auto ins = static_cast <Symbols&> (*parent).insert (*item);
		if (!ins.second)
			error_name_collision (decl, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (*item);
	}
	union_.element.clear ();
}

const Ptr <NamedItem>* Builder::union_end ()
{
	assert (!scope_stack_.empty ());
	Union* u = static_cast <Union*> (scope_stack_.back ());
	if (u) {
		// A union type can contain a default label only where the values given in the non-default labels
		// do not cover the entire range of the union's discriminant type.
		if (union_.has_default && union_.all_labels.size () > u->discriminator_type ().key_max ())
			message (union_.default_loc, MessageType::ERROR, "non-default labels cover the entire range of the union's discriminant type");
		eval_pop ();
	}
	union_.clear ();
	return constr_type_end ();
}

const Ptr <NamedItem>* Builder::enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items)
{
	assert (!scope_stack_.empty ());
	assert (!items.empty ());
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <Enum> def = Ptr <Enum>::make <Enum> (ref (*this), ref (name));
		auto ins = scope->insert (*def);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else {
			if (is_main_file ())
				container_stack_.top ()->append (*def);
			for (auto item = items.begin (); item != items.end (); ++item) {
				Ptr <EnumItem> enumerator = Ptr <EnumItem>::make <EnumItem> (ref (*this), ref (*def), ref (*item));
				auto ins = scope->insert (*enumerator);
				if (!ins.second)
					error_name_collision (*item, **ins.first);
				else {
					if (def->size () == numeric_limits <uint32_t>::max ()) {
						message (*item, MessageType::ERROR, "too many enumerators");
						break;
					}
					if (is_main_file ())
						def->append (*enumerator);
				}
			}
		}
		return &*ins.first;
	}
	return nullptr;
}

void Builder::valuetype_box (const SimpleDeclarator& name, const Type& type)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		{
			const Type& t = type.dereference_type ();
			if (t.tkind () == Type::Kind::NAMED_TYPE) {
				switch (t.named_type ().kind ()) {
					case Item::Kind::VALUE_TYPE:
					case Item::Kind::VALUE_BOX:
						message (name, MessageType::ERROR, "value types may not be boxed");
						return;
				}
			}
		}
		Ptr <NamedItem> item = Ptr <NamedItem>::make <ValueBox> (ref (*this), ref (name), ref (type));
		auto ins = scope->insert (*item);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (*item);
	}
}

void Builder::constant (const Type& t, const SimpleDeclarator& name, Variant&& val, const Location& loc)
{
	Symbols* scope = cur_scope ();
	if (scope) {
		Ptr <NamedItem> item = Ptr <NamedItem>::make <Constant> (ref (*this), ref (t), ref (name), eval ().cast (t, move (val), loc));
		auto ins = scope->insert (*item);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (*item);
	}
	eval_pop ();
}

void Builder::eval_push (const Type& t, const Location& loc)
{
	const Type& type = t.dereference_type ();
	Eval* eval = nullptr;
	switch (type.tkind ()) {
		case Type::Kind::BASIC_TYPE:
			if (is_integral (type.basic_type ()))
				eval = new EvalIntegral (*this);
			else if (is_floating_point (type.basic_type ()))
				eval = new EvalFloatingPoint (*this);
			break;
		case Type::Kind::STRING:
			eval = new EvalString (*this);
			break;
		case Type::Kind::WSTRING:
			eval = new EvalWString (*this);
			break;
		case Type::Kind::FIXED:
			eval = new EvalFixed (*this);
			break;
		case Type::Kind::NAMED_TYPE: {
			const NamedItem& nt = type.named_type ();
			if (nt.kind () == Item::Kind::ENUM)
				eval = new EvalEnum (*this, nt);
		} break;
	}

	if (!eval) {
		message (loc, MessageType::ERROR, "invalid constant type");
		eval = new Eval (*this);
	}
	eval_stack_.push (unique_ptr <Eval> (eval));
}

Ptr <Root> Builder::finalize ()
{
	if (!err_cnt_ && tree_) {
		try {
			check_complete (*tree_);
			RepIdMap ids;
			check_rep_ids_unique (ids, *tree_);
		} catch (const runtime_error& err) {
			err_out_ << err.what () << endl;
		}
	}
	if (err_cnt_)
		tree_ = nullptr;
	return std::move (tree_);
}

void Builder::check_complete (const Container& items)
{
	for (auto it = items.begin (); it != items.end (); ++it) {
		const Item& item = **it;
		switch (item.kind ()) {
			case Item::Kind::TYPE_DEF: {
				const TypeDef& t = static_cast <const TypeDef&> (item);
				check_complete (t, t);
			} break;
			case Item::Kind::INTERFACE:
				check_complete (static_cast <const Interface&> (item));
				break;
			case Item::Kind::OPERATION: {
				const Operation& op = static_cast <const Operation&> (item);
				check_complete (op, op);
				check_complete (op);
			} break;
			case Item::Kind::ATTRIBUTE: {
				const Attribute& att = static_cast <const Attribute&> (item);
				check_complete (att, att);
			} break;
			case Item::Kind::EXCEPTION:
				check_complete (static_cast <const Exception&> (item));
				break;
			case Item::Kind::STRUCT:
				check_complete (static_cast <const Struct&> (item));
				break;
			case Item::Kind::MEMBER:
			case Item::Kind::UNION_ELEMENT:
			case Item::Kind::STATE_MEMBER: {
				const Member& m = static_cast <const Member&> (item);
				check_complete (m, m);
			} break;
			case Item::Kind::UNION:
				check_complete (static_cast <const Union&> (item));
				break;
			case Item::Kind::VALUE_TYPE:
				check_complete (static_cast <const ValueType&> (item));
				break;
			case Item::Kind::VALUE_BOX: {
				const ValueBox& vb = static_cast <const ValueBox&> (item);
				check_complete (vb, vb);
			} break;
			case Item::Kind::VALUE_FACTORY:
				check_complete (static_cast <const OperationBase&> (item));
				break;
		}
	}
}

bool Builder::check_complete (const Type& type, const Location& loc)
{
	if (!type.is_complete ()) {
		message (loc, MessageType::ERROR, "incomplete type is not allowed");
		see_declaration_of (type.named_type (),  type.named_type ().qualified_name ());
		return false;
	}
	return true;
}

void Builder::check_complete (const OperationBase& op)
{
	for (auto par = op.begin (); par != op.end (); ++par) {
		const Parameter& param = **par;
		check_complete (param, param);
	}
}

void Builder::check_rep_ids_unique (RepIdMap& ids, const Symbols& sym)
{
	for (auto it = sym.begin (); it != sym.end (); ++it) {
		NamedItem* item = *it;
		const RepositoryId* rid = RepositoryId::cast (item);
		if (rid)
			check_unique (ids, *rid);
		const ItemScope* child = ItemScope::cast (item);
		if (child)
			check_rep_ids_unique (ids, *child);
	}
}

void Builder::check_unique (RepIdMap& ids, const RepositoryId& rid)
{
	auto ins = ids.emplace (rid.repository_id (), rid.item ());
	if (!ins.second) {
		message (rid.item (), Builder::MessageType::ERROR, string ("repository ID ") + ins.first->first + " is duplicated");
		see_declaration_of (ins.first->second, ins.first->second.qualified_name ());
	}
}

void Builder::check_anonymous (const Type& type, const SimpleDeclarator& name)
{
	if (anonymous_deprecated_) {
		bool anonymous = false;
		switch (type.tkind ()) {
			case Type::Kind::STRING:
			case Type::Kind::WSTRING:
				anonymous = type.string_bound () != 0;
				break;
			case Type::Kind::FIXED:
			case Type::Kind::SEQUENCE:
			case Type::Kind::ARRAY:
				anonymous = true;
		}

		if (anonymous)
			message (name, MessageType::ERROR, name + ": anonymous type does not allowed");
	}
}

}
}
