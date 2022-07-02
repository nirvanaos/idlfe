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
#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "../../include/AST/Root.h"
#include "../../include/AST/ValueType.h"
#include "../../include/AST/Parameter.h"
#include "../../include/AST/ScopedName.h"
#include "../../include/AST/Exception.h"
#include "Eval.h"
#include "Declarators.h"
#include <ostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace AST {

class Root;
class OperationBase;
class Attribute;
class StructBase;
class Union;

namespace Build {

class Builder
{
public:
	Builder (const std::string& file, std::ostream& err_out, bool anonymous_deprecated) :
		err_cnt_ (0),
		err_out_ (err_out.rdbuf ()),
		tree_ (Ptr <Root>::make <Root> (file)),
		anonymous_deprecated_ (anonymous_deprecated),
		cur_file_ (&tree_->file ()),
		is_main_file_ (true)
	{
		container_stack_.push (tree_);
		file_stack_.emplace_back (file);
	}

	unsigned err_cnt () const
	{
		return err_cnt_;
	}

	static const int FILE_FLAG_START = 0x1;
	static const int FILE_FLAG_SYSTEM = 0x2;
	void file (const std::string& name, const Location& loc, int flags);
	void line (const std::string& filename);

	void pragma (const char*, const Location& loc);

	const std::filesystem::path& file () const
	{
		return *cur_file_;
	}

	bool is_main_file () const
	{
		return is_main_file_;
	}

	enum class MessageType
	{
		ERROR,
		WARNING,
		MESSAGE
	};

	void message (const Location& l, MessageType mt, const std::string& err);

	const std::string& prefix () const;

	ItemScope* cur_parent () const;
	Symbols* cur_scope () const;

	const NamedItem* lookup (const ScopedName& scoped_name);
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

	void operation_begin (bool oneway, const Type& type, const SimpleDeclarator& name);

	void parameter (Parameter::Attribute att, const Type& type, const SimpleDeclarator& name);
	
	void raises (const ScopedNames& names);
	void operation_context (const Variants& strings);

	void operation_end ()
	{
		operation_.clear ();
	}

	void attribute (bool readonly, const Type& type, const SimpleDeclarators& declarators);

	void attribute_begin (bool readonly, const Type& type, const SimpleDeclarator& name);
	
	void getraises (const ScopedNames& names);
	void setraises (const ScopedNames& names);

	void attribute_end ()
	{
		attribute_.clear ();
	}

	void interface_end ();

	void type_def (const Type& type, const Declarators& declarators);

	void struct_decl (const SimpleDeclarator& name);
	void struct_begin (const SimpleDeclarator& name);

	const NamedItem* struct_end ()
	{
		return constr_type_end ();
	}

	void exception_begin (const SimpleDeclarator& name);

	void exception_end ()
	{
		constr_type_end ();
	}

	void member (const Type& type, const Declarators& names);

	void union_decl (const SimpleDeclarator& name);
	void union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc);
	void union_label (const Variant& label, const Location& loc);
	void union_default (const Location& loc);
	void union_element (const Type& type, const Build::Declarator& decl);
	const NamedItem* union_end ();

	const NamedItem* enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items);

	void valuetype_decl (const SimpleDeclarator& name, bool is_abstract = false);
	void valuetype_begin (const SimpleDeclarator& name, ValueType::Modifier mod = ValueType::Modifier::NONE);
	void valuetype_bases (bool truncatable, const ScopedNames& bases);
	void valuetype_supports (const ScopedNames& interfaces);
	void state_member (bool is_public, const Type& type, const Declarators& names);

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

	void eval_pop ()
	{
		assert (!eval_stack_.empty ());
		eval_stack_.pop ();
	}

	Eval& eval () const
	{
		assert (!eval_stack_.empty ());
		return *eval_stack_.top ();
	}

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

	const NamedItem* constr_type_end ();

	Raises lookup_exceptions (const ScopedNames& names);

	std::pair <bool, const NamedItem*> lookup (const ItemScope& scope, const Identifier& name, const Location& loc);
	std::pair <bool, const NamedItem*> lookup (const IV_Bases& containers, const Identifier& name, const Location& loc);

	void error_name_collision (const SimpleDeclarator& name, const Location& prev_loc);
	void error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);
	void error_valuetype_mod (const SimpleDeclarator& name, bool is_abstract, const Location& prev_loc);

	void add_base_members (const Location& loc, const IV_Bases& bases);
	bool check_member_name (const NamedItem& item);
	bool check_complete_or_ref (const Type& type, const Location& loc);

	typedef std::unordered_map <std::string, const NamedItem&> RepIdMap;
	void check_rep_ids_unique (RepIdMap& ids, const Symbols& sym);
	void check_unique (RepIdMap& ids, const ItemWithId& rid);

	void check_complete (const Container& items);
	bool check_complete (const Type& type, const Location& loc);
	void check_complete (const OperationBase& op);
	void check_complete (const StructBase& s);
	void check_complete (const Union& u);

	static bool is_base_of (const Interface& base, const Interface& derived);
	static void collect_concrete_interfaces (const ValueType& vt, std::unordered_map <const Interface*, const ValueType*>& interfaces);

private:
	unsigned err_cnt_;
	std::ostream err_out_;
	Ptr <Root> tree_;
	typedef std::vector <ItemScope*> ScopeStack;
	ScopeStack scope_stack_;
	std::stack <Container*> container_stack_;
	std::stack <std::unique_ptr <Eval> > eval_stack_;
	bool anonymous_deprecated_;

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
	bool is_main_file_;

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

	} interface_;

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
	} operation_;

	struct AttributeData
	{
		Attribute* att;

		AttributeData () :
			att (nullptr)
		{}

		void clear ()
		{
			att = nullptr;
		}
	} attribute_;

	struct ConstrType
	{
		ItemWithId* obj;
		Symbols members;

		ConstrType () :
			obj (nullptr)
		{}

		void clear () noexcept
		{
			obj = nullptr;
			members.clear ();
		}

	} constr_type_;

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
		} element;
	} union_;
};

}
}

#endif
