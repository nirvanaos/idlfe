#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Interface.h"
#include "Parameter.h"
#include "ScopedName.h"
#include "Eval.h"
#include "Declarators.h"
#include <ostream>
#include <stack>

namespace AST {

class Operation;

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
	}

	Ptr <AST> tree () const
	{
		return tree_;
	}

	unsigned err_cnt () const
	{
		return err_cnt_;
	}

	void parser_error (unsigned line, const std::string& msg)
	{
		message (Location (file (), line), MessageType::ERROR, msg);
	}

	void file (const std::string& name, unsigned line);

	void pragma (const char*, unsigned line);

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

	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name);
	const Ptr <NamedItem>* lookup_type (const ScopedName& scoped_name);

	void native (const SimpleDeclarator& name);

	void module_begin (const SimpleDeclarator& name);

	void module_end ()
	{
		scope_end ();
	}

	void interface_decl (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_begin (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_bases (const ScopedNames& bases);

	void operation_begin (bool oneway, const Type& type, const SimpleDeclarator& name);
	void operation_parameter (Parameter::Attribute att, const Type& type, const SimpleDeclarator& name);
	void operation_raises (const ScopedNames& raises);

	void operation_end ()
	{
		interface_data_.cur_op = nullptr;
		interface_data_.cur_op_params.clear ();
	}

	void interface_end ()
	{
		scope_end ();
		interface_data_.clear ();
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
	void union_begin (const SimpleDeclarator& name, const Type& switch_type);
	
	const Ptr <NamedItem>* union_end ()
	{
		return constr_type_end ();
	}

	void enum_begin (const SimpleDeclarator& name);
	
	void enum_item (const SimpleDeclarator& name);

	const Ptr <NamedItem>* enum_end ()
	{
		return constr_type_end ();
	}

	void eval_push (const Type& t, unsigned line);

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

	void constant (const Type& t, const SimpleDeclarator& name, Variant&& val, unsigned line);

	unsigned positive_int (const Variant& v, unsigned line);

	Type fixed_pt_type (unsigned digits, unsigned scale, unsigned line)
	{
		if (digits > 31 || scale > digits) {
			message (Location (file (), line), MessageType::ERROR, std::string ("fixed <") + std::to_string (digits) + ", " + std::to_string (scale) + "> type specification is invalid.");
			return Type ();
		} else
			return Type::make_fixed (digits, scale);
	}

private:
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
	void error_symbol_not_found (const ScopedName& sn);

	const ItemScope* cur_scope () const;

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

	struct InterfaceData
	{
		Symbols all_operations;
		Operation* cur_op;
		Symbols cur_op_params;

		void clear ()
		{
			all_operations.clear ();
		}
	} interface_data_;
};

}

#endif
