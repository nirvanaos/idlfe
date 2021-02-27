#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Interface.h"
#include "ScopedName.h"
#include "Eval.h"
#include <ostream>
#include <map>

namespace AST {

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

	void native (const std::string& name, unsigned line);

	void module_begin (const std::string& name, unsigned line);

	void module_end ()
	{
		scope_end ();
	}

	void interface_decl (const std::string& name, unsigned line, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_begin (const std::string& name, unsigned line, InterfaceKind ik = InterfaceKind::UNCONSTRAINED);
	void interface_base (const ScopedName& name, unsigned line);

	void interface_end ()
	{
		scope_end ();
		interface_data_.clear ();
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

	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name, unsigned line)
	{
		return lookup (scoped_name, Location (*cur_file_, line));
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

	Type fixed (unsigned digits, unsigned scale, unsigned line)
	{
		if (digits > 31 || scale > digits) {
			message (Location (file (), line), MessageType::ERROR, std::string ("fixed <") + std::to_string (digits) + ", " + std::to_string (scale) + "> type specification is invalid.");
			return Type ();
		} else
			return Type::make_fixed (digits, scale);
	}

	const Ptr <NamedItem>* struct_begin (const std::string& name, unsigned line);

private:
	bool scope_begin ();
	void scope_push (ItemContainer* scope);
	void scope_end ();

	static bool is_scope (Item::Kind ik)
	{
		return Item::Kind::MODULE == ik || Item::Kind::INTERFACE == ik;
	}

	void error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc);
	void error_interface_kind (const Location& loc, const std::string& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);

	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name) const;
	const Ptr <NamedItem>* lookup (const ScopedName& scoped_name, const Location& loc);

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

	typedef std::map <const Item*, Location> Bases;

	struct InterfaceData
	{
		Bases bases;
		std::set <const Item*> all_bases;
		Symbols all_operations;

		void clear ()
		{
			bases.clear ();
			all_bases.clear ();
			all_operations.clear ();
		}
	} interface_data_;
};

}

#endif
