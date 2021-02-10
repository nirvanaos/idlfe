#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Module.h"
#include "Interface.h"
#include "ScopedName.h"
#include <set>
#include <ostream>

namespace AST {

class Builder : public AST
{
public:
	Builder (const std::string& file, std::ostream& err_out) :
		err_out_ (err_out.rdbuf ()),
		AST (file),
		cur_file_ (&AST::file ()),
		cur_type_ (nullptr)
	{
		namespace_stack_.push_back (&global_namespace_);
		module_stack_.push_back (this);
	}

	void parser_error (unsigned line, const std::string& msg)
	{
		message (Location (*cur_file_, line), MessageType::ERROR, msg);
	}

	void file (const std::string& name);

	void module_begin (const std::string& name, unsigned line);
	void module_end ();

	void native (const std::string& name, unsigned line);

	void interface_decl (const std::string& name, unsigned line, InterfaceDecl::Kind ik = InterfaceDecl::Kind::UNCONSTRAINED);
	void interface_begin (const std::string& name, unsigned line, InterfaceDecl::Kind ik = InterfaceDecl::Kind::UNCONSTRAINED);
	void interface_base (const ScopedName& name, unsigned line);
	void interface_end ();

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

	class Location
	{
	public:
		Location (const std::string& file, unsigned line) :
			file_ (file),
			line_ (line)
		{}

		const std::string& file () const
		{
			return file_;
		}

		unsigned line () const
		{
			return line_;
		}

	private:
		const std::string& file_;
		unsigned line_;
	};

	void message (const Location& l, MessageType mt, const std::string& err);

private:
	class Symbol
	{
	public:
		Symbol (const Ptr <NamedItem>& item, const Location& loc);

		bool operator < (const Symbol& rhs) const
		{
			return item_->name () < rhs.item_->name ();
		}

		const Ptr <NamedItem>& item () const
		{
			return item_;
		}

		const Location& location () const
		{
			return location_;
		}

	private:
		Ptr <NamedItem> item_;
		Location location_;
	};

	typedef std::set <Symbol> Symbols;

	struct ModuleNS :
		public NamedItem
	{
		ModuleNS (const std::string& name) :
			NamedItem (Kind::MODULE, name)
		{}

		Symbols symbols;
	};

	void error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc);

private:
	std::ostream err_out_;
	std::set <std::string> included_files_;
	const std::string* cur_file_;
	Symbols global_namespace_;
	std::vector <Symbols*> namespace_stack_;
	std::vector <Container*> module_stack_;
	NamedItem* cur_type_;
};

}

#endif
