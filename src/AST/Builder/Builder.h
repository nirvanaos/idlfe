#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "../../include/AST/AST.h"
#include "../../include/AST/Interface.h"
#include "../../include/AST/Parameter.h"
#include "../../include/AST/ScopedName.h"
#include "Eval.h"
#include "Declarators.h"
#include <ostream>
#include <stack>
#include <map>

namespace AST {

class AST;
class Operation;

namespace Build {

/// Abstract Syntax Tree builder.
/// This class does not depend on any Flex/Bison or other parser/scanner declarations.
class Builder
{
public:
	Builder (const std::string& file, std::ostream& err_out) :
		err_cnt_ (0),
		err_out_ (err_out.rdbuf ()),
		tree_ (Ptr <AST>::make <AST> (file)),
		is_main_file_ (true)
	{
		cur_file_ = &tree_->file ();
		scope_stack_.push_back (tree_);
		container_stack_.push (tree_);
		prefix_stack_.emplace ();
	}

	unsigned err_cnt () const
	{
		return err_cnt_;
	}

	void parser_error (const Location& loc, const std::string& msg)
	{
		message (loc, MessageType::ERROR, msg);
	}

	void file (const std::string& name, const Location& loc);

	void pragma (const char*, const Location& loc);

	const std::string& file () const
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
	void operation_raises (const ScopedNames& raises);
	void operation_context (const Variants& context);

	void operation_end ()
	{
		interface_.operation.clear ();
	}

	void attribute (bool readonly, const Type& type, const SimpleDeclarators& declarators);

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

	Ptr <AST> finalize ()
	{
		if (!err_cnt_ && tree_) {
			try {
				std::map <std::string, const NamedItem*> ids;
				tree_->check_rep_ids_unique (*this, ids);
			} catch (const std::exception& ex) {
				err_out_ << ex.what () << std::endl;
			}
			if (err_cnt_)
				tree_ = nullptr;
		}
		return std::move (tree_);
	}

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

	static bool is_scope (Item::Kind ik)
	{
		return Item::Kind::MODULE == ik || Item::Kind::INTERFACE == ik;
	}

	void error_name_collision (const SimpleDeclarator& name, const Location& prev_loc);
	void error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);

private:
	unsigned err_cnt_;
	std::ostream err_out_;
	Ptr <AST> tree_;
	const std::string* cur_file_;
	bool is_main_file_;
	typedef std::vector <Symbols*> ScopeStack;
	ScopeStack scope_stack_;
	std::stack <Container*> container_stack_;
	std::stack <std::unique_ptr <Eval>> eval_stack_;
	std::stack <std::string> prefix_stack_;

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

			void clear ()
			{
				op = nullptr;
				params.clear ();
			}
		} operation;

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
