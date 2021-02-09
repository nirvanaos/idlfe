#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include <assert.h>
#include <stdexcept>

using namespace std;

namespace AST {

Builder::Symbol::Symbol (const Ptr <NamedItem>& item, const Location& loc) :
	item_ (item),
	location_ (loc)
{}

void Builder::message (const Location& l, MessageType mt, const std::string& err)
{
	static const char* const msg_types [3] = { "error", "warning", "message" };

	err_out_ << l.file () << '(' << l.line () << "): " << msg_types [(size_t)mt] << " : " << err << endl;
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

void Builder::module_begin (const string& name, unsigned line)
{
	Ptr <ModuleNS> ns = make_item <ModuleNS> (ref (name));
	Location l (*cur_file_, line);
	auto ins = namespace_stack_.back ()->emplace (Symbol (ns, l));
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::MODULE) {
		message (l, MessageType::ERROR, ns->name () + " is already defined.");
		message (ins.first->location (), MessageType::MESSAGE, "See previous definition.");
	} else {
		namespace_stack_.push_back (&ns->symbols);
		if (is_main_file ()) {
			Ptr <Module> mod = make_item <Module> (ref (name));
			module_stack_.back ()->append (mod);
			module_stack_.push_back (mod);
		}
	}
}

void Builder::module_end ()
{
	namespace_stack_.pop_back ();
}

}
