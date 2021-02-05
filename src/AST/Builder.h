#ifndef NIDL_AST_BUILDER_H_
#define NIDL_AST_BUILDER_H_

#include "AST.h"
#include "Symbols.h"
#include "Module.h"

namespace AST {

class Builder : public AST
{
public:
	Builder (const char* file) :
		AST (file),
		cur_file_ (&AST::file ()),
		cur_type_ (nullptr)
	{
		namespace_stack_.push_back (&global_namespace_);
		module_stack_.push_back (this);
	}

	void file (const char* name);

	bool is_main_file () const
	{
		return cur_file_ == &AST::file ();
	}

	void begin_module (const char* name, unsigned line);
	void end ();

	void throw_syntax_error (const std::string& file, unsigned line, const std::string& err) const;

private:
	struct ModuleNS :
		public NamedItem
	{
		ModuleNS (const char* name) :
			NamedItem (Kind::MODULE, name)
		{}

		Symbols symbols;
	};

	std::set <std::string> included_files_;
	const std::string* cur_file_;
	Symbols global_namespace_;
	std::vector <Symbols*> namespace_stack_;
	std::vector <Container*> module_stack_;
	NamedItem* cur_type_;
};

}

#endif
