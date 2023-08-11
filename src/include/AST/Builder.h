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
#ifndef IDLFE_AST_BUILDER_H_
#define IDLFE_AST_BUILDER_H_
#pragma once

#include "../IDL_FrontEnd.h"
#include "Root.h"
#include "ValueType.h"
#include "Parameter.h"
#include "ScopedName.h"
#include "Exception.h"
#include "Declarators.h"
#include "Variant.h"
#include "../BE/MessageOut.h"
#include <ostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wswitch"
#endif

namespace AST {

namespace Build {
class Eval;
}

class Root;
class OperationBase;
class Attribute;
class StructBase;
class Union;

class Builder : public BE::MessageOut
{
public:
	Builder () = delete;
	Builder (const Builder&) = delete;
	Builder& operator = (const Builder&) = delete;

	Builder (IDL_FrontEnd& compiler, const std::string& file);
	~Builder ();

	static const int FILE_FLAG_START = 0x1;
	static const int FILE_FLAG_SYSTEM = 0x2;
	void linemarker (const std::string& name, const Location& loc, int flags);
	void line (const std::string& filename);

	void pragma (const char*, const Location& loc);

	const std::filesystem::path& file () const noexcept
	{
		return *cur_file_;
	}

	bool is_main_file () const noexcept
	{
		return file_stack_.size () <= 1;
	}

	const std::string& prefix () const;

	ItemScope* cur_parent () const;
	Symbols* cur_scope () const;

	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name);
	Type lookup_type (const ScopedName& scoped_name);

	void native (const SimpleDeclarator& name);

	void module_begin (const SimpleDeclarator& name);

	void module_end ()
	{
		scope_end ();
	}

	void interface_decl (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind ());
	void interface_begin (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind ());
	void interface_bases (const ScopedNames& bases);

	void operation_begin (bool oneway, Type& type, const SimpleDeclarator& name);

	void parameter (Parameter::Attribute att, Type& type, const SimpleDeclarator& name);
	
	void raises (const ScopedNames& names);
	void operation_context (const Variants& strings);

	void operation_end ()
	{
		operation_.clear ();
	}

	void attribute (bool readonly, Type& type, const SimpleDeclarators& declarators);

	void attribute_begin (bool readonly, Type& type, const SimpleDeclarator& name);
	
	void getraises (const ScopedNames& names);
	void setraises (const ScopedNames& names);

	void attribute_end ()
	{
		attribute_.clear ();
	}

	void interface_end ();

	void type_def (Type& type, const Declarators& declarators);

	void struct_decl (const SimpleDeclarator& name);
	void struct_begin (const SimpleDeclarator& name);

	const Ptr <NamedItem>* struct_end ()
	{
		return constr_type_end ();
	}

	void exception_begin (const SimpleDeclarator& name);

	void exception_end ()
	{
		constr_type_end ();
	}

	void member (Type& type, const Declarators& names);

	void union_decl (const SimpleDeclarator& name);
	void union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc);
	void union_label (const Variant& label, const Location& loc);
	void union_default (const Location& loc);
	void union_element (Type& type, const Declarator& decl);
	const Ptr <NamedItem>* union_end ();

	const Ptr <NamedItem>* enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items);

	void valuetype_decl (const SimpleDeclarator& name, bool is_abstract = false);
	void valuetype_begin (const SimpleDeclarator& name, ValueType::Modifier mod = ValueType::Modifier::NONE);
	void valuetype_bases (bool truncatable, const ScopedNames& bases);
	void valuetype_supports (const ScopedNames& interfaces);
	void state_member (bool is_public, Type& type, const Declarators& names);

	void valuetype_factory_begin (const SimpleDeclarator& name);

	void valuetype_factory_end ()
	{
		operation_end ();
	}

	void valuetype_end ()
	{
		interface_end ();
	}

	void valuetype_box (const SimpleDeclarator& name, const Type& type);

	void eval_push (const Type& t, const Location& loc);
	void eval_pop ();
	Build::Eval& eval () const;

	void constant (const Type& t, const SimpleDeclarator& name, Variant&& val, const Location& loc);

	unsigned positive_int (const Variant& v, const Location& loc);

	Type fixed_pt_type (unsigned digits, unsigned scale, const Location& loc)
	{
		if (digits > 31 || scale > digits) {
			message (loc, MessageType::ERROR, std::string ("fixed <") + std::to_string (digits) + ", " + std::to_string (scale) + "> type specification is invalid.");
			return Type ();
		} else
			return Type::make_fixed (digits, scale);
	}

	void type_id (const ScopedName& name, const Variant& id, const Location& id_loc)
	{
		if (!id.empty ())
			type_id (name, id.as_string (), id_loc);
	}

	void type_prefix (const ScopedName& name, const Variant& s, const Location& id_loc);

	void see_prev_declaration (const Location& loc);
	void see_declaration_of (const Location& loc, const std::string& name);

	Ptr <Root> finalize ();

	void check_anonymous (const Type& type, const SimpleDeclarator& name);

	void validate_id (const Identifier& name, const Location& loc);

private:
	bool prefix_valid (const std::string& pref, const Location& loc);
	void prefix (const std::string& pref, const Location& loc);
	bool get_quoted_string (const char*& s, std::string& qs, const Location& loc);
	static bool get_scoped_name (const char*& s, ScopedName& name);
	ItemWithId* lookup_rep_id (const ScopedName& name);
	void type_id (const ScopedName& name, const std::string& id, const Location& id_loc);

	Symbols* scope_begin ();
	void scope_push (IV_Base* scope);
	void scope_end ();

	const Ptr <NamedItem>* constr_type_end ();

	Raises lookup_exceptions (const ScopedNames& names);

	std::pair <bool, const Ptr <NamedItem>*> lookup (const ItemScope& scope, const Identifier& name, const Location& loc);
	std::pair <bool, const Ptr <NamedItem>*> lookup (const IV_Bases& containers, const Identifier& name, const Location& loc);
	
	static const Ptr <NamedItem>* find (const Symbols& symbols, const Identifier& name)
	{
		return symbols.find (name);
	}

	void error_name_collision (const SimpleDeclarator& name, const Location& prev_loc);
	void error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);
	void error_valuetype_mod (const SimpleDeclarator& name, bool is_abstract, const Location& prev_loc);

	void add_base_members (const Location& loc, const IV_Bases& bases);
	bool check_member_name (const NamedItem& item);

	bool check_complete (const Type& type, const Location& loc);
	bool check_complete_or_ref (const Type& type, const Location& loc);
	bool check_complete_or_seq (const Type& type, const Location& loc);

	typedef std::unordered_map <std::string, const NamedItem&> RepIdMap;
	void check_rep_ids_unique (RepIdMap& ids, const Symbols& sym);
	void check_unique (RepIdMap& ids, const ItemWithId& rid);

	static bool is_base_of (const Interface& base, const Interface& derived);
	static void collect_concrete_interfaces (const ValueType& vt, std::unordered_map <const Interface*, const ValueType*>& interfaces);

	void check_complete (const Symbols& symbols);

	static Type make_type (const Type& t, const Declarator& decl);

private:
	IDL_FrontEnd& compiler_;
	Ptr <Root> tree_;
	typedef std::vector <ItemScope*> ScopeStack;
	ScopeStack scope_stack_;
	std::stack <Container*> container_stack_;
	std::stack <std::unique_ptr <Build::Eval> > eval_stack_;

	struct File
	{
		std::string file;
		std::string prefix;

		File (const std::string& f) :
			file (f)
		{}
	};

	std::vector <File> file_stack_;
	const std::filesystem::path* cur_file_;

	// Current interface data. Also used for value types.
	struct InterfaceData
	{
		Symbols all_members;
		std::unordered_set <const Item*> all_bases;

		void clear ()
		{
			all_members.clear ();
			all_bases.clear ();
		}

	}
	interface_;

	// Current operation data
	struct OperationData
	{
		OperationBase* op;
		Symbols params;

		OperationData () :
			op (nullptr)
		{}

		void clear ()
		{
			op = nullptr;
			params.clear ();
		}
	}
	operation_;

	// Current attribute data
	struct AttributeData
	{
		Attribute* att;

		AttributeData () noexcept :
			att (nullptr)
		{}

		void clear () noexcept
		{
			att = nullptr;
		}
	}
	attribute_;

	// Constructed type: struct, exception, union
	struct ConstrType
	{
		const Ptr <NamedItem>* symbol;
		Symbols members;

		ConstrType () noexcept :
			symbol (nullptr)
		{}

		void clear () noexcept
		{
			symbol = nullptr;
			members.clear ();
		}

		NamedItem* obj () noexcept
		{
			return symbol ? *symbol : nullptr;
		}
	}
	constr_type_;

	// Current union data
	struct UnionData
	{
		std::unordered_map <Variant::Key, Location> all_labels;
		bool has_default;
		Location default_loc;

		UnionData () :
			has_default (false)
		{}

		void clear () noexcept
		{
			all_labels.clear ();
			has_default = false;
		}

		struct ElementData
		{
			bool is_default;
			std::vector <Variant> labels;

			void clear ()
			{
				is_default = false;
				labels.clear ();
			}
		}
		element;
	}
	union_;
};

}

#endif
