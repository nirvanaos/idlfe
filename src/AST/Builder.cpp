#include "Builder.h"
#include "Include.h"
#include "Module.h"
#include "Native.h"
#include "TypeDef.h"
#include "Operation.h"
#include "Struct.h"
#include "Union.h"
#include "Enum.h"
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

unsigned Builder::positive_int (const Variant& v, unsigned line)
{
	// TODO: Implement
	message (Location (file (), line), Builder::MessageType::ERROR, "Expected positive integer.");
	return 1;
}

const Ptr <NamedItem>* Builder::lookup (const ScopedName& scoped_name, const Location& loc)
{
	const Ptr <NamedItem>* item = lookup (scoped_name);
	if (!item)
		message (loc, MessageType::ERROR, string ("Symbol not found: ") + scoped_name.stringize ());
	return item;
}

const Ptr <NamedItem>* Builder::lookup_type (const ScopedName& scoped_name, unsigned line)
{
	Location loc (file (), line);
	const Ptr <NamedItem>* item = lookup (scoped_name, loc);
	if (item) {
		const NamedItem* p = *item;
		if (!p->is_type ()) {
			message (loc, MessageType::ERROR, scoped_name.stringize () + " is not a type.");
			item = nullptr;
		}
	}
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

void Builder::scope_end ()
{
	assert (scope_stack_.size () > 1);
	scope_stack_.pop_back ();
	if (is_main_file ()) {
		assert (container_stack_.size () > 1);
		container_stack_.pop_back ();
	}
}
const Ptr <NamedItem>* Builder::constr_type_end ()
{
	const ItemScope* type = static_cast <const ItemScope*> (scope_stack_.back ());
	scope_end ();
	if (type) {
		const Symbols& scope = *scope_stack_.back ();
		auto f = scope.find (type->name ());
		assert (f != scope.end ());
		return &*f;
	} else
		return nullptr;
}

void Builder::module_begin (const string& name, unsigned line)
{
	if (scope_begin ()) {
		Location loc (file (), line);
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

void Builder::native (const std::string& name, unsigned line)
{
	if (scope_stack_.back ()) {
		Location loc (file (), line);
		auto ins = scope_stack_.back ()->insert (Ptr <NamedItem>::make <Native> (ref (loc), cur_scope (), ref (name)));
		if (!ins.second && (*ins.first)->kind () != Item::Kind::NATIVE) {
			error_name_collision (loc, name, **ins.first);
		}
	}
}

void Builder::type_def (const Type& type, const Declarators& declarators)
{
	if (scope_stack_.back ()) {
		for (auto decl = declarators.begin (); decl != declarators.end (); ++decl) {
			auto ins = scope_stack_.back ()->insert (Ptr <NamedItem>::make <TypeDef> (ref (*decl), cur_scope (), ref (*decl), ref (type)));
			if (!ins.second)
				error_name_collision (*decl, *decl, **ins.first);
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
		Location loc (file (), line);
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
				else if (is_main_file ())
					container_stack_.back ()->append (decl);
			}
		}
	}
}

void Builder::interface_begin (const std::string& name, unsigned line, InterfaceKind ik)
{
	if (scope_begin ()) {
		Location loc (file (), line);
		Ptr <Interface> itf = Ptr <Interface>::make <Interface> (ref (loc), cur_scope (), ref (name), ik);
		Symbols& scope = *scope_stack_.back ();
		auto ins = scope.insert (itf);
		if (!ins.second) {
			if ((*ins.first)->kind () != Item::Kind::INTERFACE_DECL) {
				error_name_collision (loc, name, **ins.first);
				scope_push (nullptr);
				return;
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
	Interface* this_itf = static_cast <Interface*> (scope_stack_.back ());
	if (this_itf) {
		assert (this_itf->kind () == Item::Kind::INTERFACE);

		Location loc (file (), line);
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

void Builder::operation_begin (bool oneway, const Type& type, const std::string& name, unsigned line)
{
	Interface* itf = static_cast <Interface*> (scope_stack_.back ());
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE);
		Location loc (file (), line);
		if (oneway && type.kind () != Type::Kind::VOID) {
			message (loc, MessageType::WARNING, "oneway operation must be void. oneway attribute will be ignored.");
			oneway = false;
		}
		Ptr <Operation> op = Ptr <Operation>::make <Operation> (ref (loc), cur_scope (), oneway, ref (type), ref (name));
		auto ins = interface_data_.all_operations.insert (op);
		if (!ins.second) {
			message (loc, MessageType::ERROR, string ("Operation name ") + op->name () + " collision.");
			message (**ins.first, MessageType::MESSAGE, string ("See ") + (*ins.first)->qualified_name () + ".");
		} else {
			Symbols& scope = static_cast <Symbols&> (*itf);
			ins = scope.insert (op);
			if (!ins.second)
				error_name_collision (loc, name, **ins.first);
			else {
				interface_data_.cur_op = op;
				if (is_main_file ())
					container_stack_.back ()->append (op);
			}
		}
	}
}

void Builder::operation_parameter (Parameter::Attribute att, const Type& type, const std::string& name, unsigned line)
{
	Operation* op = interface_data_.cur_op;
	if (op) {
		Location loc (file (), line);
		if (att != Parameter::Attribute::IN && op->oneway ()) {
			message (loc, MessageType::WARNING, "oneway operation can have only in parameters. oneway attribute will be ignored.");
			op->oneway_clear ();
		}
		Ptr <Parameter> par = Ptr <Parameter>::make <Parameter> (ref (loc), cur_scope (), att, ref (type), ref (name));
		auto ins = interface_data_.cur_op_params.insert (par);
		if (!ins.second)
			message (loc, MessageType::ERROR, string ("Duplicated parameter ") + name + ".");
		else if (is_main_file ())
			op->append (op);
	}
}

void Builder::struct_decl (const std::string& name, unsigned line)
{
	if (scope_stack_.back ()) {
		Location loc (file (), line);
		Ptr <StructDecl> decl = Ptr <StructDecl>::make <StructDecl> (ref (loc), cur_scope (), ref (name));
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			Item::Kind item_kind = (*ins.first)->kind ();
			if (item_kind != Item::Kind::STRUCT && item_kind != Item::Kind::STRUCT_DECL)
				error_name_collision (loc, name, **ins.first);
			else if (is_main_file ())
				container_stack_.back ()->append (decl);
		}
	}
}

void Builder::struct_begin (const std::string& name, unsigned line)
{
	if (scope_begin ()) {
		Location loc (file (), line);
		Ptr <Struct> def = Ptr <Struct>::make <Struct> (ref (loc), cur_scope (), ref (name));
		Symbols& scope = *scope_stack_.back ();
		auto ins = scope.insert (def);
		if (!ins.second) {
			if ((*ins.first)->kind () != Item::Kind::STRUCT_DECL) {
				error_name_collision (loc, name, **ins.first);
				scope_push (nullptr);
				return;
			} else {
				const_cast <Ptr <NamedItem>&> (*ins.first) = def;
			}
		}
		scope_push (def);
	}
}

void Builder::union_decl (const std::string& name, unsigned line)
{
	if (scope_stack_.back ()) {
		Location loc (file (), line);
		Ptr <UnionDecl> decl = Ptr <UnionDecl>::make <UnionDecl> (ref (loc), cur_scope (), ref (name));
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			Item::Kind item_kind = (*ins.first)->kind ();
			if (item_kind != Item::Kind::UNION && item_kind != Item::Kind::UNION_DECL)
				error_name_collision (loc, name, **ins.first);
			else if (is_main_file ())
				container_stack_.back ()->append (decl);
		}
	}
}

void Builder::union_begin (const std::string& name, const Type& switch_type, unsigned line)
{
	if (scope_begin ()) {
		Location loc (file (), line);
		Ptr <Union> def = Ptr <Union>::make <Union> (ref (loc), cur_scope (), ref (name), ref (switch_type));
		Symbols& scope = *scope_stack_.back ();
		auto ins = scope.insert (def);
		if (!ins.second) {
			if ((*ins.first)->kind () != Item::Kind::UNION_DECL) {
				error_name_collision (loc, name, **ins.first);
				scope_push (nullptr);
				return;
			} else {
				const_cast <Ptr <NamedItem>&> (*ins.first) = def;
			}
		}
		scope_push (def);
	}
}

void Builder::enum_begin (const std::string& name, unsigned line)
{
	if (scope_begin ()) {
		Location loc (file (), line);
		Ptr <Enum> def = Ptr <Enum>::make <Enum> (ref (loc), cur_scope (), ref (name));
		Symbols& scope = *scope_stack_.back ();
		auto ins = scope.insert (def);
		if (!ins.second) {
			error_name_collision (loc, name, **ins.first);
			scope_push (nullptr);
		} else
			scope_push (def);
	}
}

void Builder::enum_item (const std::string& name, unsigned line)
{
	assert (scope_stack_.size () > 1);
	Symbols* en = scope_stack_.back ();
	if (en) {
		Location loc (file (), line);
		Ptr <EnumItem> item = Ptr <EnumItem>::make <EnumItem> (ref (loc), cur_scope (), ref (name));
		auto ins = en->insert (item);
		if (!ins.second)
			error_name_collision (loc, name, **ins.first);
		else if (is_main_file ())
			container_stack_.back ()->append (item);
	}
}

}
