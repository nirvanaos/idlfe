#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include "Native.h"
#include <assert.h>
#include <stdexcept>

using namespace std;

namespace AST {

Builder::Symbol::Symbol (const Ptr <NamedItem>& item, const Location& loc) :
	item_ (item),
	location_ (loc)
{}

void Builder::message (const Location& l, MessageType mt, const string& err)
{
	static const char* const msg_types [3] = { "error", "warning", "message" };

	err_out_ << l.file () << '(' << l.line () << "): " << msg_types [(size_t)mt] << " : " << err << endl;
}

void Builder::file (const std::string& name)
{
	if (AST::file () == name)
		cur_file_ = &AST::file ();
	else {
		if (is_main_file ())
			append (make_item <Include> (name));
		cur_file_ = &*included_files_.insert (name).first;
	}
}

void Builder::error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc)
{
	message (loc, MessageType::ERROR, name + " is already declared.");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

void Builder::module_begin (const string& name, unsigned line)
{
	Ptr <ModuleNS> ns = make_item <ModuleNS> (ref (name));
	Location l (*cur_file_, line);
	auto ins = namespace_stack_.back ()->emplace (Symbol (ns, l));
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::MODULE) {
		error_name_collision (l, name, ins.first->location ());
	} else {
		namespace_stack_.push_back (&ns->symbols);
		if (is_main_file ()) {
			Ptr <Module> mod = make_item <Module> (ref (name));
			module_stack_.back ()->append (mod);
			module_stack_.push_back (mod.get ());
		}
	}
}

void Builder::module_end ()
{
	namespace_stack_.pop_back ();
}

void Builder::native (const std::string& name, unsigned line)
{
	Location l (*cur_file_, line);
	auto ins = namespace_stack_.back ()->emplace (Symbol (make_item <Native> (name), l));
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::NATIVE) {
		error_name_collision (l, name, ins.first->location ());
	}
}

void Builder::interface_decl (const std::string& name, unsigned line, InterfaceDecl::Kind ik)
{
	Location l (*cur_file_, line);
	auto ins = namespace_stack_.back ()->emplace (Symbol (make_item <InterfaceDecl> (name, ik), l));
	if (!ins.second) {
		Item::Kind item_kind = ins.first->item ()->kind ();
		if (item_kind != Item::Kind::INTERFACE && item_kind != Item::Kind::INTERFACE_DECL) {
			error_name_collision (l, name, ins.first->location ());
		} else {
			InterfaceDecl* existent_decl = static_cast <InterfaceDecl*> (ins.first->item ().get ());
			if (existent_decl->interface_kind () != ik) {
				message (l, MessageType::ERROR, string (InterfaceDecl::kind_name (ik)) + "interface " + name + " is already defined as " + InterfaceDecl::kind_name (existent_decl->interface_kind ()));
				message (ins.first->location (), MessageType::MESSAGE, "See previous declaration.");
			}
		}
	}
}

void Builder::interface_begin (const std::string& name, unsigned line, InterfaceDecl::Kind ik)
{
	Location l (*cur_file_, line);
	Symbols& ns = *namespace_stack_.back ();
	Symbol itf (make_item <Interface> (name, ik), l);
	auto ins = ns.emplace (itf);
	if (!ins.second) {
		if (ins.first->item ()->kind () != Item::Kind::INTERFACE_DECL) {
			error_name_collision (l, name, ins.first->location ());
		} else {
			InterfaceDecl* existent_decl = static_cast <InterfaceDecl*> (ins.first->item ().get ());
			if (existent_decl->interface_kind () != ik) {
				message (l, MessageType::ERROR, string (InterfaceDecl::kind_name (ik)) + "interface " + name + " is already defined as " + InterfaceDecl::kind_name (existent_decl->interface_kind ()));
				message (ins.first->location (), MessageType::MESSAGE, "See previous declaration.");
				return;
			} else {
				ns.erase (ins.first);
				ns.insert (itf);
			}
		}
	}
	if (is_main_file ())
		module_stack_.back ()->append (itf.item ());
	cur_type_ = itf.item ().get ();
}

void Builder::interface_base (const ScopedName& name, unsigned line)
{
	// TODO: Implement
}

void Builder::interface_end ()
{
	assert (cur_type_);
	assert (cur_type_->kind () == Item::Kind::INTERFACE);
	if (is_main_file ()) {
		assert (module_stack_.back ()->back () == cur_type_);
		module_stack_.pop_back ();
	}
	cur_type_ = nullptr;
}

}
