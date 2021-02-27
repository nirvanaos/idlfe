#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include "Native.h"
#include "Operation.h"
#include <assert.h>
#include <stdexcept>

using namespace std;

namespace AST {

void Builder::message (const Location& l, MessageType mt, const string& err)
{
	static const char* const msg_types [4] = { "error", "error", "warning", "message" };

	err_out_ << l.file () << '(' << l.line () << "): " << msg_types [(size_t)mt] << " : " << err << endl;

	if (mt == MessageType::ERROR && (++err_cnt_ >= 20))
		throw runtime_error ("Too many errors, compilation aborted.");
}

void Builder::pragma (const char* s, unsigned line)
{
	assert (false); // TODO: Implement
}

void Builder::file (const std::string& name)
{
	if (tree_->file () == name) {
		cur_file_ = &tree_->file ();
		is_main_file_ = true;
	} else {
		cur_file_ = &tree_->add_file (name);
		is_main_file_ = false;
		if (is_main_file ())
			tree_->append (Ptr <Item>::make <Include> (name));
	}
}

void Builder::error_name_collision (const Location& loc, const std::string& name, const Location& prev_loc)
{
	message (loc, MessageType::ERROR, name + " is already declared.");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

inline
const Ptr <NamedItem>* Builder::lookup (const ScopedName& scoped_name) const
{
	auto name = scoped_name.begin ();
	Symbols::const_iterator f;
	if (scoped_name.from_root) {
		const Symbols* scope = scope_stack_.front ();
		f = scope->find (*name);
		if (f == scope->end ())
			return nullptr;
	} else {
		ScopeStack::const_iterator it = scope_stack_.end () - 1;
		for (;;) {
			const Symbols* scope = *it;
			f = scope->find (*name);
			if (f != scope->end ())
				break;
			if (it == scope_stack_.begin ())
				return nullptr;
			--it;
		}
	}

	while (scoped_name.end () != ++name) {
		const ItemScope* scope = ItemScope::cast (*f);
		if (scope) {
			f = scope->find (*name);
			if (f == scope->end ())
				return nullptr;
		} else
			return nullptr;
	}

	return &*f;
}

const Ptr <NamedItem>* Builder::lookup (const ScopedName& scoped_name, const Location& loc)
{
	const Ptr <NamedItem>* item = lookup (scoped_name);
	if (!item)
		message (loc, MessageType::ERROR, string ("Symbol not found: ") + scoped_name.stringize ());
	return item;
}

const ItemScope* Builder::cur_scope () const
{
	if (scope_stack_.size () > 1)
		return static_cast <const ItemScope*> (scope_stack_.back ());
	else
		return nullptr;
}

void Builder::scope_push (ItemContainer* scope)
{
	scope_stack_.push_back (scope);
	if (is_main_file ()) {
		if (scope)
			container_stack_.back ()->append (scope);
		container_stack_.push_back (scope);
	}
}

bool Builder::scope_begin ()
{
	if (scope_stack_.back ())
		return true;
	else {
		scope_push (nullptr);
		return false;
	}
}

unsigned Builder::positive_int (const Variant& v, unsigned line)
{
	message (Location (file (), line), Builder::MessageType::ERROR, "Expected positive integer.");
	return 1;
}

void Builder::module_begin (const string& name, unsigned line)
{
	if (scope_begin ()) {
		Location loc (*cur_file_, line);
		Ptr <Module> mod = Ptr <Module>::make <Module> (ref (loc), cur_scope (), ref (name));
		auto ins = scope_stack_.back ()->insert (mod);
		if (!ins.second && (*ins.first)->kind () != Item::Kind::MODULE) {
			error_name_collision (loc, name, **ins.first);
			scope_push (nullptr);
		} else {
			Module* mod = scast <Module> (*ins.first);
			scope_stack_.push_back (mod);
			if (is_main_file ()) {
				Ptr <ModuleItems> cont = Ptr <ModuleItems>::make <ModuleItems> (std::ref (*mod));
				container_stack_.back ()->append (cont);
				container_stack_.push_back (cont);
			}
		}
	}
}

void Builder::scope_end ()
{
	assert (scope_stack_.size () > 1);
	scope_stack_.pop_back ();
	if (is_main_file ()) {
		assert (container_stack_.size () > 1);
		container_stack_.pop_back ();
	}
}

void Builder::native (const std::string& name, unsigned line)
{
	if (scope_stack_.back ()) {
		Location loc (*cur_file_, line);
		auto ins = scope_stack_.back ()->insert (Ptr <NamedItem>::make <Native> (ref (loc), cur_scope (), name));
		if (!ins.second && (*ins.first)->kind () != Item::Kind::NATIVE) {
			error_name_collision (loc, name, **ins.first);
		}
	}
}

void Builder::error_interface_kind (const Location& loc, const std::string& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc)
{
	message (loc, MessageType::ERROR, string (new_kind.interface_kind_name ()) + " interface " + name + " is already defined as " + prev_kind.interface_kind_name () + ".");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

void Builder::interface_decl (const std::string& name, unsigned line, InterfaceKind ik)
{
	if (scope_stack_.back ()) {
		Location loc (*cur_file_, line);
		Ptr <InterfaceDecl> decl = Ptr <InterfaceDecl>::make <InterfaceDecl> (ref (loc), cur_scope (), name, ik);
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			Item::Kind item_kind = (*ins.first)->kind ();
			if (item_kind != Item::Kind::INTERFACE && item_kind != Item::Kind::INTERFACE_DECL) {
				error_name_collision (loc, name, **ins.first);
			} else {
				InterfaceDecl* existent_decl = scast <InterfaceDecl> (*ins.first);
				if (existent_decl->interface_kind () != ik.interface_kind ())
					error_interface_kind (loc, name, ik, existent_decl->interface_kind (), **ins.first);
				else 	if (is_main_file ())
					container_stack_.back ()->append (decl);
			}
		}
	}
}

void Builder::interface_begin (const std::string& name, unsigned line, InterfaceKind ik)
{
	if (scope_begin ()) {
		Location loc (*cur_file_, line);
		Ptr <Interface> itf = Ptr <Interface>::make <Interface> (ref (loc), cur_scope (), name, ik);
		Symbols& scope = *scope_stack_.back ();
		auto ins = scope.insert (itf);
		if (!ins.second) {
			if ((*ins.first)->kind () != Item::Kind::INTERFACE_DECL) {
				error_name_collision (loc, name, **ins.first);
				scope_push (nullptr);
			} else {
				InterfaceDecl* existent_decl = scast <InterfaceDecl> (*ins.first);
				if (existent_decl->interface_kind () != ik.interface_kind ())
					error_interface_kind (loc, name, ik, *existent_decl, **ins.first);
				const_cast <Ptr <NamedItem>&> (*ins.first) = itf;
			}
		}
		scope_push (itf);
	}
}

void Builder::interface_base (const ScopedName& name, unsigned line)
{
	assert (scope_stack_.size () > 1);
	if (scope_stack_.back ()) {
		Interface* this_itf = static_cast <Interface*> (scope_stack_.back ());
		assert (this_itf->kind () == Item::Kind::INTERFACE);

		Location loc (*cur_file_, line);
		const Ptr <NamedItem>* pbase = lookup (name, loc);
		if (pbase) {
			const NamedItem* base = *pbase;
			const char* err = nullptr;
			if (base->kind () != Item::Kind::INTERFACE)
				if (base->kind () == Item::Kind::INTERFACE_DECL)
					err = "Incomplete interface is not allowed.";
				else
					err = "Invalid base type.";
			else {
				const Interface* base_itf = static_cast <const Interface*> (base);
				if (this_itf == base_itf) {
					message (loc, MessageType::ERROR, "May not derive from itself.");
					return;
				}
				switch (this_itf->interface_kind ()) {
					case InterfaceKind::UNCONSTRAINED:
						if (InterfaceKind::LOCAL == base_itf->interface_kind ())
							err = "Unconstrained interface may not derive local interface.";
						break;
					case InterfaceKind::ABSTRACT:
						if (InterfaceKind::ABSTRACT != base_itf->interface_kind ())
							err = "An abstract interface may only inherit from abstract interfaces.";
						break;
				}
				if (!err) {
					auto ins = interface_data_.bases.emplace (base, loc);
					if (!ins.second) {
						message (loc, MessageType::ERROR, name.stringize () + " is already base of " + this_itf->name () + ".");
						message (ins.first->second, MessageType::MESSAGE, "See the previous declaration.");
						return;
					}
					{
						vector <const Interface*> bases;
						base_itf->get_all_interfaces (bases);
						for (const Interface* itf : bases) {
							if (interface_data_.all_bases.insert (itf).second) {
								// Check operation names
								const Symbols& members = *itf;
								for (Symbols::const_iterator it = members.begin (); it != members.end (); ++it) {
									const Item* member = *it;
									if (member->kind () == Item::Kind::OPERATION) {
										Operation* op = static_cast <Operation*> (const_cast <Item*> (member));
										auto ins = interface_data_.all_operations.insert (op);
										if (!ins.second) {
											message (loc, MessageType::ERROR, string ("Operation name ") + op->name () + " collision.");
											message (**ins.first, MessageType::MESSAGE, (*ins.first)->qualified_name ());
											message (*op, MessageType::MESSAGE, op->qualified_name ());
											return;
										}
									}
								}
							}
						}
					}

					// OK
					this_itf->add_base (base_itf);
				}
			}
			if (err) {
				message (loc, MessageType::ERROR, err);
				message (*base, MessageType::MESSAGE, "See declaration of " + name.stringize ());
			}
		}
	}
}

}
