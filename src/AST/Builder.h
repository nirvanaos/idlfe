#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Interface.h"
#include "Parameter.h"
#include "ScopedName.h"
#include "Eval.h"
#include "Declarators.h"
#include <ostream>
#include <map>

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
		container_stack_.push_back (tree_);
	}

	void parser_error (unsigned line, const std::string& msg)
	{
		message (Location (file (), line), MessageType::ERROR, msg);
	}

	void file (const std::string& name);

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

	void native (const std::string& name, unsigned line);

	void module_begin (const std::string& name, unsigned line);

	void module_end ()
	{
		scope_end ();
	}

	void interface_decl (const std::string& name, unsigned line, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_begin (const std::string& name, unsigned line, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_bases (const ScopedNames& bases);

	void operation_begin (bool oneway, const Type& type, const std::string& name, unsigned line);
	void operation_parameter (Parameter::Attribute att, const Type& type, const std::string& name, unsigned line);
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

	void struct_decl (const std::string& name, unsigned line);
	void struct_begin (const std::string& name, unsigned line);
	
	const Ptr <NamedItem>* struct_end ()
	{
		return constr_type_end ();
	}

	void union_decl (const std::string& name, unsigned line);
	void union_begin (const std::string& name, const Type& switch_type, unsigned line);
	
	const Ptr <NamedItem>* union_end ()
	{
		return constr_type_end ();
	}

	void enum_begin (const std::string& name, unsigned line);
	
	void enum_item (const std::string& name, unsigned line);

	const Ptr <NamedItem>* enum_end ()
	{
		return constr_type_end ();
	}

	template <class Ev>
	void set_eval ()
	{
		eval_ = std::make_unique <Ev> (*this);
	}

	Eval& eval () const
	{
		assert (eval_);
		return *eval_.get ();
	}

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

	void error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc);
	void error_interface_kind (const Location& loc, const std::string& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);
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
	typedef std::vector <Container*> ContainerStack;
	ContainerStack container_stack_;
	std::unique_ptr <Eval> eval_;

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
