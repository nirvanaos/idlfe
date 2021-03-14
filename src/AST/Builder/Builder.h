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
#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "../../include/AST/AST.h"
#include "../../include/AST/Interface.h"
#include "../../include/AST/Parameter.h"
#include "../../include/AST/ScopedName.h"
#include "../../include/AST/Exception.h"
#include "Eval.h"
#include "Declarators.h"
#include <ostream>
#include <stack>
#include <map>

namespace AST {

class Root;
class Operation;
class Attribute;

namespace Build {

/// Abstract Syntax Tree builder.
/// This class does not depend on any Flex/Bison or other parser/scanner declarations.
class Builder
{
public:
	Builder (const std::string& file, std::ostream& err_out) :
		err_cnt_ (0),
		err_out_ (err_out.rdbuf ()),
		tree_ (Ptr <Root>::make <Root> (file))
	{
		scope_stack_.push_back (tree_);
		container_stack_.push (tree_);
		file_stack_.emplace_back (*tree_->add_file (file).first);
	}

	unsigned err_cnt () const
	{
		return err_cnt_;
	}

	static const int FILE_FLAG_START = 0x1;
	static const int FILE_FLAG_SYSTEM = 0x2;
	void file (const std::string& name, const Location& loc, int flags = 0);

	void pragma (const char*, const Location& loc);

	const std::string& file () const
	{
		return *file_stack_.back ().file;
	}

	bool is_main_file () const
	{
		return file_stack_.size () == 1;
	}

	enum class MessageType
	{
		ERROR,
		WARNING,
		MESSAGE
	};

	void message (const Location& l, MessageType mt, const std::string& err);

	const std::string& prefix () const;

	ItemScope* cur_scope () const;

	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name);
	const Ptr <NamedItem>* lookup_type (const ScopedName& scoped_name);

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
	void operation_parameter (Parameter::Attribute att, const Type& type, const SimpleDeclarator& name);
	
	void operation_raises (const ScopedNames& names);
	void operation_context (const Variants& strings);

	void operation_end ()
	{
		interface_.operation.clear ();
	}

	void attribute (bool readonly, const Type& type, const SimpleDeclarators& declarators);

	void attribute_begin (bool readonly, const Type& type, const SimpleDeclarator& name);
	
	void getraises (const ScopedNames& names);
	void setraises (const ScopedNames& names);

	void attribute_end ()
	{
		interface_.attribute.clear ();
	}

	void interface_end ()
	{
		// Delete all operations and attributes from scope
		Symbols* scope = scope_stack_.back ();
		for (auto it = scope->begin (); it != scope->end ();) {
			switch ((*it)->kind ()) {
				case Item::Kind::OPERATION:
				case Item::Kind::ATTRIBUTE:
					it = scope->erase (it);
					break;
				default:
					++it;
			}
		}
		scope_end ();
		interface_.clear ();
	}

	void type_def (const Type& type, const Declarators& declarators);

	void struct_decl (const SimpleDeclarator& name);
	void struct_begin (const SimpleDeclarator& name);

	const Ptr <NamedItem>* struct_end ()
	{
		return constr_type_end ();
	}

	void exception_begin (const SimpleDeclarator& name);

	void exception_end ()
	{
		return scope_end ();
	}

	void member (const Type& type, const Declarators& declarators);

	void union_decl (const SimpleDeclarator& name);
	void union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc);
	void union_label (const Variant& label, const Location& loc);
	void union_default (const Location& loc);
	void union_element (const Type& type, const Build::Declarator& decl);
	const Ptr <NamedItem>* union_end ();

	const Ptr <NamedItem>* enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items);

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

private:
	bool prefix_valid (const std::string& pref, const Location& loc);
	void prefix (const std::string& pref, const Location& loc);
	bool get_quoted_string (const char*& s, std::string& qs, const Location& loc);
	static bool get_scoped_name (const char*& s, ScopedName& name);
	RepositoryId* lookup_rep_id (const ScopedName& name);
	void type_id (const ScopedName& name, const std::string& id, const Location& id_loc);

	bool scope_begin ();
	void scope_push (ItemContainer* scope);
	void scope_end ();

	const Ptr <NamedItem>* constr_type_end ();

	Raises raises (const ScopedNames& names);

	typedef std::map <std::string, const NamedItem&> RepIdMap;
	void check_rep_ids_unique (RepIdMap& ids, const Symbols& sym);
	void check_unique (RepIdMap& ids, const RepositoryId& rid);

	void check_complete (const Container& items);
	bool check_complete (const Type& type, const Location& loc);

	void error_name_collision (const SimpleDeclarator& name, const Location& prev_loc);
	void error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);

private:
	unsigned err_cnt_;
	std::ostream err_out_;
	Ptr <Root> tree_;
	typedef std::vector <Symbols*> ScopeStack;
	ScopeStack scope_stack_;
	std::stack <Container*> container_stack_;
	std::stack <std::unique_ptr <Eval>> eval_stack_;

	struct File
	{
		const std::string* file;
		std::string prefix;

		File (const std::string& f) :
			file (&f)
		{}
	};

	std::vector <File> file_stack_;

	struct InterfaceData
	{
		Symbols all_operations;

		void clear ()
		{
			all_operations.clear ();
		}

		struct OperationData
		{
			Operation* op;
			Symbols params;

			OperationData () :
				op (nullptr)
			{}

			void clear ()
			{
				op = nullptr;
				params.clear ();
			}
		} operation;

		struct AttributeData
		{
			Attribute* att;

			void clear ()
			{
				att = nullptr;
			}
		} attribute;

	} interface_;

	struct UnionData
	{
		std::map <Variant::Key, Location> all_labels;
		bool has_default;
		Location default_loc;

		UnionData () :
			has_default (false)
		{}

		void clear ()
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
