#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Module.h"
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

	void file (const char* name);

	void module_begin (const std::string& name, unsigned line);
	void module_end ();

private:
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
