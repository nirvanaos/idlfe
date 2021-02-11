#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Module.h"
#include "Interface.h"
#include "ScopedName.h"
#include "Symbol.h"
#include <ostream>

namespace AST {

class Builder : public AST
{
public:
	Builder (const std::string& file, std::ostream& err_out) :
		err_out_ (err_out.rdbuf ()),
		AST (file),
		cur_file_ (&AST::file ())
	{
		scope_stack_.push_back (&global_namespace_);
		container_stack_.push_back (this);
	}

	void parser_error (unsigned line, const std::string& msg)
	{
		message (Location (*cur_file_, line), MessageType::ERROR, msg);
	}

	void file (const std::string& name);

	void native (const std::string& name, unsigned line);

	void scope_end ();

	void module_begin (const std::string& name, unsigned line);

	void interface_decl (const std::string& name, unsigned line, InterfaceDecl::Kind ik = InterfaceDecl::Kind::UNCONSTRAINED);
	void interface_begin (const std::string& name, unsigned line, InterfaceDecl::Kind ik = InterfaceDecl::Kind::UNCONSTRAINED);
	void interface_base (const ScopedName& name, unsigned line);

	bool is_main_file () const
	{
		return cur_file_ == &AST::file ();
	}

	enum class MessageType
	{
		ERROR,
		WARNING,
		MESSAGE
	};

	void message (const Location& l, MessageType mt, const std::string& err);

private:

	struct Scope :
		public NamedItem,
		public Symbols
	{
		Scope (Item::Kind kind, const std::string& name) :
			NamedItem (kind, name)
		{}
	};

	struct ModuleScope :
		public Scope
	{
		ModuleScope (const std::string& name) :
			Scope (Kind::MODULE, name)
		{}
	};

	struct InterfaceScope;

	struct InterfaceBase
	{
		const Symbol& itf;
		Location base_declaration_loc;

		InterfaceBase (const Symbol& _itf, const Location& loc) :
			itf (_itf),
			base_declaration_loc (loc)
		{}

		bool operator < (const InterfaceBase& rhs) const
		{
			return &itf < &rhs.itf;
		}
	};

	typedef std::set <InterfaceBase> InterfaceBases;

	struct InterfaceScope :
		public Scope
	{
		InterfaceScope (const std::string& name, Interface::Kind ik) :
			Scope (Kind::INTERFACE, name),
			interface_kind (ik)
		{}

		Interface::Kind interface_kind;
		InterfaceBases bases;
		Symbols all_operations;
	};

	static bool is_scope (Item::Kind ik)
	{
		return Item::Kind::MODULE == ik || Item::Kind::INTERFACE == ik;
	}

	void error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc);
	void error_interface_kind (const Location& loc, const std::string& name, Interface::Kind new_kind, Interface::Kind prev_kind, const Location& prev_loc);

	const Symbol* lookup (const ScopedName& scoped_name) const;
	const Symbol* lookup (const ScopedName& scoped_name, const Location& loc);

private:
	std::ostream err_out_;
	std::set <std::string> included_files_;
	const std::string* cur_file_;
	Symbols global_namespace_;
	typedef std::vector <Symbols*> ScopeStack;
	ScopeStack scope_stack_;
	typedef std::vector <Container*> ContainerStack;
	ContainerStack container_stack_;
};

}

#endif
