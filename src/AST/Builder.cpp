#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include <assert.h>
#include <stdexcept>

using namespace std;

namespace AST {

void Builder::throw_syntax_error (const std::string& file, unsigned line, const std::string& err) const
{
	throw runtime_error (err);
}

void Builder::file (const char* name)
{
	if (AST::file () == name)
		cur_file_ = &AST::file ();
	else {
		if (is_main_file ())
			append (make_item <Include> (name));
		cur_file_ = &*included_files_.insert (name).first;
	}
}

void Builder::begin_module (const char* name, unsigned line)
{
	Ptr <ModuleNS> ns = make_item <ModuleNS> (name);
	auto ins = namespace_stack_.back ()->emplace (Symbol (ns, Location (*cur_file_, line)));
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::MODULE)
		throw_syntax_error (*cur_file_, line, ns->name () + " is already defined at " + ins.first->location ().stringize () + ".");
	namespace_stack_.push_back (&ns->symbols);
	if (is_main_file ()) {
		Ptr <Module> mod = make_item <Module> (name);
		module_stack_.back ()->append (mod);
		module_stack_.push_back (mod);
	}
}

void Builder::end ()
{
}

}
