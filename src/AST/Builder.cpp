#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include "Native.h"
#include <assert.h>
#include <stdexcept>

using namespace std;

namespace AST {

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

inline
const Symbol* Builder::lookup (const ScopedName& scoped_name) const
{
	auto name = scoped_name.begin ();
	Symbols::const_iterator f;
	if (scoped_name.from_root) {
		const Symbols& symbols = *scope_stack_.front ();
		f = symbols.find (*name);
		if (f == symbols.end ())
			return nullptr;
	} else {
		ScopeStack::const_iterator scope = scope_stack_.end () - 1;
		for (;;) {
			const Symbols& symbols = **scope;
			f = symbols.find (*name);
			if (f != symbols.end ())
				break;
			if (scope == scope_stack_.begin ())
				return nullptr;
			--scope;
		}
	}

	while (scoped_name.end () != ++name) {
		if (!is_scope (f->item ()->kind ()))
			return nullptr;
		const Symbols& symbols = *static_cast <const Scope*> (f->item ().get ());
		f = symbols.find (*name);
		if (f == symbols.end ())
			return nullptr;
	}

	return &*f;
}

const Symbol* Builder::lookup (const ScopedName& scoped_name, const Location& loc)
{
	const Symbol* sym = lookup (scoped_name);
	if (!sym)
		message (loc, MessageType::ERROR, string ("Symbol not found: ") + scoped_name.stringize ());
	return sym;
}

void Builder::module_begin (const string& name, unsigned line)
{
	Ptr <ModuleScope> scope = make_item <ModuleScope> (ref (name));
	Location loc (*cur_file_, line);
	auto ins = scope_stack_.back ()->emplace (scope, loc);
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::MODULE) {
		error_name_collision (loc, name, ins.first->location ());
	} else {
		scope_stack_.push_back (static_cast <ModuleScope*> (ins.first->item ().get ()));
		if (is_main_file ()) {
			Ptr <Module> mod = make_item <Module> (ref (name));
			container_stack_.back ()->append (mod);
			container_stack_.push_back (mod.get ());
		}
	}
}

void Builder::scope_end ()
{
	scope_stack_.pop_back ();
	if (is_main_file ())
		container_stack_.pop_back ();
}

void Builder::native (const std::string& name, unsigned line)
{
	Location l (*cur_file_, line);
	auto ins = scope_stack_.back ()->emplace (Symbol (make_item <Native> (name), l));
	if (!ins.second && ins.first->item ()->kind () != Item::Kind::NATIVE) {
		error_name_collision (l, name, ins.first->location ());
	}
}

void Builder::error_interface_kind (const Location& loc, const std::string& name, Interface::Kind new_kind, Interface::Kind prev_kind, const Location& prev_loc)
{
	message (loc, MessageType::ERROR, string (InterfaceDecl::kind_name (new_kind)) + " interface " + name + " is already defined as " + InterfaceDecl::kind_name (prev_kind) + ".");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

void Builder::interface_decl (const std::string& name, unsigned line, InterfaceDecl::Kind ik)
{
	Location loc (*cur_file_, line);
	auto ins = scope_stack_.back ()->emplace (make_item <InterfaceDecl> (name, ik), loc);
	if (!ins.second) {
		Item::Kind item_kind = ins.first->item ()->kind ();
		if (item_kind != Item::Kind::INTERFACE && item_kind != Item::Kind::INTERFACE_DECL) {
			error_name_collision (loc, name, ins.first->location ());
		} else {
			InterfaceDecl* existent_decl = static_cast <InterfaceDecl*> (ins.first->item ().get ());
			if (existent_decl->interface_kind () != ik)
				error_interface_kind (loc, name, ik, existent_decl->interface_kind (), ins.first->location ());
		}
	}
}

void Builder::interface_begin (const std::string& name, unsigned line, InterfaceDecl::Kind ik)
{
	Location loc (*cur_file_, line);
	Ptr <InterfaceScope> module_scope = make_item <InterfaceScope> (name, ik);
	Symbol sym (module_scope, loc);
	Symbols& cur_scope = *scope_stack_.back ();
	auto ins = cur_scope.insert (sym);
	if (!ins.second) {
		if (ins.first->item ()->kind () != Item::Kind::INTERFACE_DECL) {
			error_name_collision (loc, name, ins.first->location ());
		} else {
			InterfaceDecl* existent_decl = static_cast <InterfaceDecl*> (ins.first->item ().get ());
			if (existent_decl->interface_kind () != ik) {
				error_interface_kind (loc, name, ik, existent_decl->interface_kind (), ins.first->location ());
				return;
			} else
				cur_scope.insert (cur_scope.erase (ins.first), sym);
		}
	}
	scope_stack_.push_back (module_scope.get ());
	if (is_main_file ()) {
		Ptr <Interface> itf = make_item <Interface> (name, ik);
		container_stack_.back ()->append (itf);
		container_stack_.push_back (itf.get ());
	}
}

void Builder::interface_base (const ScopedName& name, unsigned line)
{
	Location loc (*cur_file_, line);
	const Symbol* base = lookup (name, loc);
	if (base) {
		const Item* item = base->item ().get ();
		const char* err = nullptr;
		if (item->kind () != Item::Kind::INTERFACE)
			if (item->kind () == Item::Kind::INTERFACE_DECL)
				err = "Incomplete interface is not allowed.";
			else
				err = "Invalid base type.";
		else {
			assert (scope_stack_.size () > 1);
			InterfaceScope* this_itf = static_cast <InterfaceScope*> (scope_stack_.back ());
			assert (this_itf->kind () == Item::Kind::INTERFACE);
			const InterfaceScope* base_itf = static_cast <const InterfaceScope*> (item);
			if (this_itf == base_itf) {
				message (loc, MessageType::ERROR, "May not derive from itself.");
				return;
			}
			switch (this_itf->interface_kind) {
				case Interface::Kind::UNCONSTRAINED:
					if (Interface::Kind::LOCAL == base_itf->interface_kind)
						err = "Unconstrained interface may not derive local interface.";
					break;
				case Interface::Kind::ABSTRACT:
					if (Interface::Kind::ABSTRACT != base_itf->interface_kind)
						err = "An abstract interface may only inherit from abstract interfaces.";
					break;
			}
			if (!err) {
				auto ins = this_itf->bases.emplace (*base, loc);
				if (!ins.second) {
					message (loc, MessageType::ERROR, name.stringize () + " is already base of " + this_itf->name () + ".");
					message (ins.first->base_declaration_loc, MessageType::MESSAGE, "See the previous declaration.");
					return;
				}
			}
		}
		if (err) {
			message (loc, MessageType::ERROR, err);
			message (base->location (), MessageType::MESSAGE, "See declaration of " + name.stringize ());
		}
	}
}

}
