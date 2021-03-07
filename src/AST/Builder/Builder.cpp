#include "Builder.h"
#include "EvalIntegral.h"
#include "EvalFloatingPoint.h"
#include "EvalString.h"
#include "EvalFixed.h"
#include "EvalEnum.h"
#include "../Include.h"
#include "../Module.h"
#include "../Native.h"
#include "../TypeDef.h"
#include "../Operation.h"
#include "../Attribute.h"
#include "../Struct.h"
#include "../Union.h"
#include "../Enum.h"
#include "../Exception.h"
#include "../Member.h"
#include "../Constant.h"
#include <assert.h>
#include <stdexcept>
#include <map>
#include <set>

using namespace std;

namespace AST {
namespace Build {

void Builder::message (const Location& l, MessageType mt, const string& err)
{
	static const char* const msg_types [4] = { "error", "error", "warning", "message" };

	err_out_ << l.file () << '(' << l.line () << "): " << msg_types [(size_t)mt] << " : " << err << endl;

	if (mt == MessageType::ERROR && (++err_cnt_ >= 20))
		throw runtime_error ("Too many errors, compilation aborted.");
}

void Builder::pragma (const char* s, const Location& loc)
{
	assert (*s == '#');
	++s;
	while (isspace (*s))
		++s;
	if (*s) {
		assert (!strncmp (s, "pragma ", 7));
		s += 7;
		while (isspace (*s))
			++s;
		const char* end = s + 1;
		while (!isspace (*end))
			++end;
		string pr (s, end - s);
		if (pr == "ID") {
			s = end + 1;
			ScopedName name (loc);
			string id;
			if (get_scoped_name (s, name) && get_quoted_string (s, id) && !id.empty ()) {
				RepositoryId* rep_id = lookup_rep_id (name);
				if (rep_id)
					rep_id->pragma_id (*this, id, loc);
				return;
			}
		} else if (pr == "prefix") {
			string prefix;
			if (get_quoted_string (s, prefix)) {
				prefix_stack_.top () = move (prefix);
				return;
			}
		} else if (pr == "version") {
			s = end + 1;
			ScopedName name (loc);
			if (get_scoped_name (s, name)) {
				while (isspace (*s))
					++s;
				Version ver;
				char* endptr;
				unsigned long u = strtoul (s, &endptr, 10);
				if (endptr > s && u <= numeric_limits <uint16_t>::max ()) {
					ver.major = (uint16_t)u;
					s = endptr;
					while (isspace (*s))
						++s;
					if ('.' == *s) {
						++s;
						while (isspace (*s))
							++s;
						u = strtoul (s, &endptr, 10);
						if (endptr > s && u <= numeric_limits <uint16_t>::max ()) {
							ver.minor = (uint16_t)u;
							RepositoryId* rep_id = lookup_rep_id (name);
							if (rep_id)
								rep_id->pragma_version (*this, ver, loc);
							return;
						}
					}
				}
			}
		} else {
			message (loc, MessageType::WARNING, string ("Unknown pragma \'") + pr + "\'.");
			return;
		}
	}

	message (loc, MessageType::ERROR, "Invalid pragma syntax.");
}

RepositoryId* Builder::lookup_rep_id (const ScopedName& sn)
{
	RepositoryId* rep_id = nullptr;
	const Ptr <NamedItem>* it = lookup (sn);
	if (it) {
		NamedItem* item = *it;
		rep_id = RepositoryId::cast (item);
		if (!rep_id) {
			message (sn, MessageType::ERROR, sn.stringize () + " has not repository id.");
			message (*item, MessageType::MESSAGE, string ("See declaration of ") + item->qualified_name () + '.');
		}
	}
	return rep_id;
}

bool Builder::get_quoted_string (const char*& s, std::string& qs)
{
	const char* begin = s;
	while (isspace (*begin))
		++begin;
	if ('\"' == *begin) {
		++begin;
		const char* end = strchr (begin, '\"');
		if (end) {
			qs.assign (begin, end - begin);
			s = end + 1;
			return true;
		}
	}
	return false;
}

bool Builder::get_scoped_name (const char*& s, ScopedName& sn)
{
	const char* begin = s;
	char c;
	while ((c = isspace (*begin)))
		++begin;

	if (':' == c) {
		++begin;
		if (':' == *begin) {
			do {
				c = *++begin;
			} while (isspace (c));
			if (!isalpha (c))
				return false;
			sn.from_root = true;
		} else
			return false;
	} else if (!isalpha (c))
		return false;

	assert (isalpha (c));
	assert (*begin == c);

	for (;;) {
		const char* end = begin + 1;
		for (;;) {
			c = *end;
			if (isalnum (c) || '_' == c)
				++end;
			else
				break;
		}
		sn.push_back (string (begin, end - begin));
		begin = end;
		while ((c = isspace (*begin)))
			++begin;
		if (':' == c) {
			++begin;
			if (':' == *begin)
				c = *++begin;
			else
				return false;
		} else if (!c)
			break;
	}
	return true;
}

void Builder::file (const std::string& name, const Location& loc)
{
	if (*cur_file_ != name) {
		if (tree_->file () == name) {
			cur_file_ = &tree_->file ();
			is_main_file_ = true;
			prefix_stack_.pop ();
		} else {
			auto ins = tree_->add_file (name);
			if (is_main_file ())
				tree_->append (Ptr <Item>::make <Include> (name));
			cur_file_ = &*ins.first;
			is_main_file_ = false;
			if (ins.second)
				prefix_stack_.emplace ();
			else
				prefix_stack_.pop ();
		}
	}
}

void Builder::error_name_collision (const SimpleDeclarator& name, const Location& prev_loc)
{
	message (name, MessageType::ERROR, name + " is already declared.");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

void Builder::error_symbol_not_found (const ScopedName& sn)
{
	message (sn, MessageType::ERROR, string ("Symbol not found: ") + sn.stringize ());
}

const Ptr <NamedItem>* Builder::lookup (const ScopedName& scoped_name)
{
	auto name = scoped_name.begin ();
	Symbols::const_iterator f;
	if (scoped_name.from_root) {
		const Symbols* scope = scope_stack_.front ();
		f = scope->find (*name);
		if (f == scope->end ()) {
			error_symbol_not_found (scoped_name);
			return nullptr;
		}
	} else {
		ScopeStack::const_iterator it = scope_stack_.end () - 1;
		for (;;) {
			const Symbols* scope = *it;
			f = scope->find (*name);
			if (f != scope->end ())
				break;
			if (it == scope_stack_.begin ()) {
				error_symbol_not_found (scoped_name);
				return nullptr;
			}
			--it;
		}
	}

	while (scoped_name.end () != ++name) {
		const ItemScope* scope = ItemScope::cast (*f);
		if (scope) {
			f = scope->find (*name);
			if (f == scope->end ()) {
				error_symbol_not_found (scoped_name);
				return nullptr;
			}
		} else {
			error_symbol_not_found (scoped_name);
			return nullptr;
		}
	}

	return &*f;
}

unsigned Builder::positive_int (const Variant& v, const Location& loc)
{
	assert (v.is_integral ());
	try {
		uint32_t i = v.to_unsigned_long ();
		if (i)
			return i;
		message (loc, Builder::MessageType::ERROR, "Expected positive integer.");
	} catch (const exception& ex) {
		message (loc, Builder::MessageType::ERROR, ex.what ());
	}
	return 1;
}

const Ptr <NamedItem>* Builder::lookup_type (const ScopedName& scoped_name)
{
	const Ptr <NamedItem>* item = lookup (scoped_name);
	if (item) {
		const NamedItem* p = *item;
		if (!p->is_type ()) {
			message (scoped_name, MessageType::ERROR, scoped_name.stringize () + " is not a type.");
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
			container_stack_.top ()->append (scope);
		container_stack_.push (scope);
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
		container_stack_.pop ();
	}
}
const Ptr <NamedItem>* Builder::constr_type_end ()
{
	ItemScope* type = static_cast <ItemScope*> (scope_stack_.back ());

	if (type) {
		// Delete members from scope
		for (auto it = type->begin (); it != type->end ();) {
			switch ((*it)->kind ()) {
				case Item::Kind::MEMBER:
				case Item::Kind::UNION_ELEMENT:
					it = type->erase (it);
					break;
				default:
					++it;
			}
		}
	}

	scope_end ();
	if (type) {
		const Symbols& scope = *scope_stack_.back ();
		auto f = scope.find (type->name ());
		assert (f != scope.end ());
		return &*f;
	} else
		return nullptr;
}

void Builder::module_begin (const SimpleDeclarator& name)
{
	if (scope_begin ()) {
		Ptr <Module> mod = Ptr <Module>::make <Module> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (mod);
		if (!ins.second && (*ins.first)->kind () != Item::Kind::MODULE) {
			error_name_collision (name, **ins.first);
			scope_push (nullptr);
		} else {
			Module* mod = scast <Module> (*ins.first);
			scope_stack_.push_back (mod);
			if (is_main_file ()) {
				Ptr <ModuleItems> cont = Ptr <ModuleItems>::make <ModuleItems> (std::ref (*mod));
				container_stack_.top ()->append (cont);
				container_stack_.push (cont);
			}
		}
	}
}

void Builder::native (const SimpleDeclarator& name)
{
	if (scope_stack_.back ()) {
		Ptr <NamedItem> def = Ptr <NamedItem>::make <Native> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (def);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (def);
	}
}

void Builder::type_def (const Type& type, const Declarators& declarators)
{
	Symbols* scope = scope_stack_.back ();
	if (scope) {
		for (auto decl = declarators.begin (); decl != declarators.end (); ++decl) {
			Ptr <NamedItem> def = Ptr <NamedItem>::make <TypeDef> (ref (*this), ref (*decl), ref (type));
			auto ins = scope->insert (def);
			if (!ins.second)
				error_name_collision (*decl, **ins.first);
			else if (is_main_file ())
				container_stack_.top ()->append (def);
		}
	}
}

void Builder::error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc)
{
	message (name, MessageType::ERROR, string (new_kind.interface_kind_name ()) + " interface " + name + " is already defined as " + prev_kind.interface_kind_name () + ".");
	message (prev_loc, MessageType::MESSAGE, "See previous declaration.");
}

void Builder::interface_decl (const SimpleDeclarator& name, InterfaceKind ik)
{
	if (scope_stack_.back ()) {
		Ptr <InterfaceDecl> decl = Ptr <InterfaceDecl>::make <InterfaceDecl> (ref (*this), name, ik);
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			InterfaceKind prev_ik;
			switch (item.kind ()) {
				case Item::Kind::INTERFACE_DECL: {
					const InterfaceDecl& itf = static_cast <const InterfaceDecl&> (item);
					rid = &itf;
					prev_ik = itf;
				} break;
				case Item::Kind::INTERFACE: {
					const Interface& itf = static_cast <const Interface&> (item);
					rid = &itf;
					prev_ik = itf;
				} break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			if (prev_ik.interface_kind () != ik.interface_kind ()) {
				error_interface_kind (name, ik, prev_ik, item);
				return;
			}

			if (!rid->check_prefix (*this, name))
				return;
		}

		if (is_main_file ())
			container_stack_.top ()->append (decl);
	}
}

void Builder::interface_begin (const SimpleDeclarator& name, InterfaceKind ik)
{
	if (scope_begin ()) {
		Ptr <Interface> itf = Ptr <Interface>::make <Interface> (ref (*this), ref (name), ik);
		auto ins = scope_stack_.back ()->insert (itf);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if ((*ins.first)->kind () != Item::Kind::INTERFACE_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			} else {
				const InterfaceDecl& decl = static_cast <const InterfaceDecl&> (item);
				if (decl.interface_kind () != ik.interface_kind ())
					error_interface_kind (name, ik, decl, decl);
				decl.check_prefix (*this, name);
				static_cast <RepositoryIdData&> (*itf) = decl;
				const_cast <Ptr <NamedItem>&> (*ins.first) = itf;
			}
		}

		scope_push (itf);
	}
}

void Builder::interface_bases (const ScopedNames& bases)
{
	Interface* itf = static_cast <Interface*> (scope_stack_.back ());
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE);

		// Process bases
		map <const Item*, Location> direct_bases;
		set <const Item*> all_bases;
		for (auto base_name = bases.begin (); base_name != bases.end (); ++base_name) {
			const Ptr <NamedItem>* pbase = lookup (*base_name);
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
					if (itf == base_itf) {
						message (*base_name, MessageType::ERROR, "May not derive from itself.");
						continue;
					}
					switch (itf->interface_kind ()) {
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
						auto ins = direct_bases.emplace (base, *base_name);
						if (!ins.second) {
							message (*base_name, MessageType::ERROR, base_name->stringize () + " is already base of " + itf->name () + ".");
							message (ins.first->second, MessageType::MESSAGE, "See the previous declaration.");
							continue;
						}
						{
							vector <const Interface*> bases;
							base_itf->get_all_interfaces (bases);
							for (const Interface* itf : bases) {
								if (all_bases.insert (itf).second) {
									// Check operation names
									const Container& members = *itf;
									for (auto it = members.begin (); it != members.end (); ++it) {
										const Item* member = *it;
										switch (member->kind ()) {
											case Item::Kind::OPERATION:
											case Item::Kind::ATTRIBUTE:
												NamedItem* op = static_cast <NamedItem*> (const_cast <Item*> (member));
												auto ins = interface_.all_operations.insert (op);
												if (!ins.second) {
													string opatt = member->kind () == Item::Kind::OPERATION ? "Operation name " : "Attribute name ";
													message (*base_name, MessageType::ERROR, opatt + op->name () + " collision.");
													message (**ins.first, MessageType::MESSAGE, (*ins.first)->qualified_name ());
													message (*op, MessageType::MESSAGE, op->qualified_name ());
													continue;
												}
										}
									}
								}
							}
						}

						// OK
						itf->add_base (base_itf);
					}
				}
				if (err) {
					message (*base_name, MessageType::ERROR, err);
					message (*base, MessageType::MESSAGE, "See declaration of " + base_name->stringize ());
				}
			}
		}
	}
}

void Builder::operation_begin (bool oneway, const Type& type, const SimpleDeclarator& name)
{
	assert (scope_stack_.size () > 1);
	ItemScope* itf = static_cast <Interface*> (scope_stack_.back ());
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE);
		if (oneway && type.kind () != Type::Kind::VOID) {
			message (name, MessageType::WARNING, "oneway operation must be void. oneway attribute will be ignored.");
			oneway = false;
		}
		Ptr <Operation> op = Ptr <Operation>::make <Operation> (ref (*this), oneway, ref (type), ref (name));
		auto ins = interface_.all_operations.insert (op);
		if (!ins.second) {
			message (name, MessageType::ERROR, string ("Operation name ") + name + " collision.");
			message (**ins.first, MessageType::MESSAGE, string ("See ") + (*ins.first)->qualified_name () + ".");
		} else {
			ins = itf->insert (op);
			if (!ins.second)
				error_name_collision (name, **ins.first); // Op name collides with nested type.
			else {
				interface_.operation.op = op;
				// We always append operation to the container, whatever it is the main file or not.
				// We need it to build all_operations for derived interfaces.
				container_stack_.top ()->append (op);
			}
		}
	}
}

void Builder::attribute (bool readonly, const Type& type, const SimpleDeclarators& declarators)
{
	assert (scope_stack_.size () > 1);
	ItemScope* itf = static_cast <Interface*> (scope_stack_.back ());
	if (itf) {
		assert (itf->kind () == Item::Kind::INTERFACE);
		for (auto name = declarators.begin (); name != declarators.end (); ++name) {
			Ptr <NamedItem> item = Ptr <NamedItem>::make <Attribute> (ref (*this), readonly, ref (type), ref (*name));
			auto ins = interface_.all_operations.insert (item);
			if (!ins.second) {
				message (*name, MessageType::ERROR, string ("Attribute name ") + *name + " collision.");
				message (**ins.first, MessageType::MESSAGE, string ("See ") + (*ins.first)->qualified_name () + ".");
			} else {
				ins = itf->insert (item);
				if (!ins.second)
					error_name_collision (*name, **ins.first); // Op name collides with nested type.
				else {
					// We always append attribute to the container, whatever it is the main file or not.
					// We need it to build all_operations for derived interfaces.
					container_stack_.top ()->append (item);
				}
			}
		}
	}
}

void Builder::operation_parameter (Parameter::Attribute att, const Type& type, const SimpleDeclarator& name)
{
	Operation* op = interface_.operation.op;
	if (op) {
		if (att != Parameter::Attribute::IN && op->oneway ()) {
			message (name, MessageType::WARNING, "oneway operation can not return data. oneway attribute will be ignored.");
			op->oneway_clear ();
		}
		Ptr <Parameter> par = Ptr <Parameter>::make <Parameter> (ref (*this), att, ref (type), ref (name));
		auto ins = interface_.operation.params.insert (par);
		if (!ins.second)
			message (name, MessageType::ERROR, string ("Duplicated parameter ") + name + ".");
		else if (is_main_file ())
			op->append (op);
	}
}

void Builder::operation_raises (const ScopedNames& raises)
{
	Operation* op = interface_.operation.op;
	if (op) {
		map <const Item*, Location> unique;
		for (auto name = raises.begin (); name != raises.end (); ++name) {
			const Ptr <NamedItem>* l = lookup (*name);
			if (l) {
				const NamedItem* item = *l;
				if (item->kind () != Item::Kind::EXCEPTION) {
					message (*name, MessageType::ERROR, name->stringize () + " is not an exception type.");
					message (*item, MessageType::MESSAGE, string ("See declaration of ") + item->qualified_name () + ".");
				} else {
					auto ins = unique.emplace (item, *name);
					if (!ins.second) {
						message (*name, MessageType::ERROR, string ("Duplicated exception specification ") + name->stringize () + ".");
						message (ins.first->second, MessageType::MESSAGE, string ("See previous specification of ") + item->qualified_name () + ".");
					} else
						op->add_exception (static_cast <const Exception*> (item));
				}
			}
		}
	}
}

void Builder::struct_decl (const SimpleDeclarator& name)
{
	if (scope_stack_.back ()) {
		Ptr <StructDecl> decl = Ptr <StructDecl>::make <StructDecl> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			switch (item.kind ()) {
				case Item::Kind::STRUCT_DECL:
					rid = &static_cast <const StructDecl&> (item);
					break;
				case Item::Kind::STRUCT:
					rid = &static_cast <const Struct&> (item);
					break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			if (!rid->check_prefix (*this, name))
				return;
		}

		if (is_main_file ())
			container_stack_.top ()->append (decl);
	}
}

void Builder::struct_begin (const SimpleDeclarator& name)
{
	if (scope_begin ()) {
		Ptr <Struct> def = Ptr <Struct>::make <Struct> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (def);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if (item.kind () != Item::Kind::STRUCT_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			}

			const StructDecl& decl = static_cast <const StructDecl&> (item);
			decl.check_prefix (*this, name);
			static_cast <RepositoryIdData&> (*def) = decl;
			const_cast <Ptr <NamedItem>&> (*ins.first) = def;
		}

		scope_push (def);
	}
}

void Builder::exception_begin (const SimpleDeclarator& name)
{
	if (scope_begin ()) {
		Ptr <Exception> def = Ptr <Exception>::make <Exception> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (def);
		if (!ins.second) {
			error_name_collision (name, **ins.first);
			scope_push (nullptr);
			return;
		}

		scope_push (def);
	}
}

void Builder::member (const Type& type, const Declarators& declarators)
{
	assert (scope_stack_.size () > 1);
	ItemScope* parent = static_cast <ItemScope*> (scope_stack_.back ());
	if (parent) {
		assert (parent->kind () == Item::Kind::STRUCT || parent->kind () == Item::Kind::EXCEPTION);
		for (auto decl = declarators.begin (); decl != declarators.end (); ++decl) {
			Ptr <NamedItem> item;
			if (decl->array_sizes ().empty ()) {
				item = Ptr <NamedItem>::make <Member> (ref (*this), ref (type), ref (*decl));
			} else {
				Type arr = Type::make_array (type, decl->array_sizes ());
				item = Ptr <NamedItem>::make <Member> (ref (*this), ref (arr), ref (*decl));
			}
			auto ins = static_cast <Symbols*> (parent)->insert (item);
			if (!ins.second)
				error_name_collision (*decl, **ins.first);
			else if (is_main_file ())
				container_stack_.top ()->append (item);
		}
	}
}

void Builder::union_decl (const SimpleDeclarator& name)
{
	if (scope_stack_.back ()) {
		Ptr <UnionDecl> decl = Ptr <UnionDecl>::make <UnionDecl> (ref (*this), ref (name));
		auto ins = scope_stack_.back ()->insert (decl);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			const RepositoryId* rid = nullptr;
			switch (item.kind ()) {
				case Item::Kind::UNION_DECL:
					rid = &static_cast <const UnionDecl&> (item);
					break;
				case Item::Kind::UNION:
					rid = &static_cast <const Union&> (item);
					break;
			}

			if (!rid) {
				error_name_collision (name, **ins.first);
				return;
			}

			if (!rid->check_prefix (*this, name))
				return;
		}

		if (is_main_file ())
			container_stack_.top ()->append (decl);
	}
}

void Builder::union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc)
{
	if (scope_begin ()) {
		const Type& t = switch_type.dereference_type ();

		bool type_OK = false;
		const Ptr <NamedItem>* enum_type = nullptr;
		switch (t.kind ()) {
			case Type::Kind::BASIC_TYPE: {
				BasicType bt = t.basic_type ();
				if (is_integral (bt))
					type_OK = true;
			} break;

			case Type::Kind::NAMED_TYPE: {
				const Ptr <NamedItem>* pt = t.named_type ();
				if (pt && (*pt)->kind () == Item::Kind::ENUM) {
					enum_type = switch_type.named_type ();
					type_OK = true;
				}
			}
		}

		if (!type_OK) {
			message (type_loc, MessageType::ERROR, "Invalid switch type.");
			eval_stack_.push (move (make_unique <Eval> (*this)));
			scope_push (nullptr);
			return;
		} else
			eval_push (switch_type, type_loc);

		Ptr <Union> def = Ptr <Union>::make <Union> (ref (*this), ref (name), ref (switch_type));
		auto ins = scope_stack_.back ()->insert (def);
		if (!ins.second) {
			const NamedItem& item = **ins.first;
			if (item.kind () != Item::Kind::UNION_DECL) {
				error_name_collision (name, item);
				scope_push (nullptr);
				return;
			}

			const UnionDecl& decl = static_cast <const UnionDecl&> (item);
			decl.check_prefix (*this, name);
			static_cast <RepositoryIdData&> (*def) = decl;
			const_cast <Ptr <NamedItem>&> (*ins.first) = def;
		}

		scope_push (def);

		// If the <switch_type_spec> is an enumeration, the identifier for the enumeration is
		// as well in the scope of the union; as a result, it must be distinct from the element declarators.
		if (enum_type)
			scope_stack_.back ()->insert (*enum_type);
	}
}

void Builder::union_label (const Variant& label, const Location& loc)
{
	if (!label.empty ()) {
		auto ins = union_.all_labels.emplace (label.to_key (), loc);
		if (!ins.second) {
			message (loc, MessageType::ERROR, label.dereference_const ().to_string () + " is already used.");
			message (ins.first->second, MessageType::MESSAGE, "See previous declaration.");
		} else if (union_.element.is_default)
			message (loc, MessageType::WARNING, "Default element, case is ignored.");
		else
			union_.element.labels.push_back (label);
	}
}

void Builder::union_default (const Location& loc)
{
	if (union_.has_default)
		message (loc, MessageType::ERROR, "Union already has the default element.");
	else {
		union_.has_default = true;
		union_.element.is_default = true;
		if (!union_.element.labels.empty ()) {
			message (loc, MessageType::WARNING, "Default element, other cases are ignored.");
			union_.element.labels.clear ();
		}
	}
}

void Builder::union_element (const Type& type, const Build::Declarator& decl)
{
	assert (scope_stack_.size () > 1);
	ItemScope* parent = static_cast <ItemScope*> (scope_stack_.back ());
	if (parent) {
		assert (parent->kind () == Item::Kind::UNION);
		Ptr <NamedItem> item;
		if (decl.array_sizes ().empty ()) {
			item = Ptr <NamedItem>::make <UnionElement> (ref (*this), move (union_.element.labels), ref (type), ref (decl));
		} else {
			Type arr = Type::make_array (type, decl.array_sizes ());
			item = Ptr <NamedItem>::make <UnionElement> (ref (*this), move (union_.element.labels), ref (arr), ref (decl));
		}
		auto ins = static_cast <Symbols*> (parent)->insert (item);
		if (!ins.second)
			error_name_collision (decl, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (item);
	}
}

const Ptr <NamedItem>* Builder::enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items)
{
	assert (scope_stack_.size () > 1);
	Symbols* scope = scope_stack_.back ();
	if (scope) {
		Ptr <Enum> def = Ptr <Enum>::make <Enum> (ref (*this), ref (name));
		auto ins = scope->insert (def);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else {
			if (is_main_file ())
				container_stack_.top ()->append (def);
			for (auto item = items.begin (); item != items.end (); ++item) {
				Ptr <EnumItem> enumerator = Ptr <EnumItem>::make <EnumItem> (ref (*this), ref (*ins.first), ref (name));
				ins = scope->insert (enumerator);
				if (!ins.second)
					error_name_collision (*item, **ins.first);
				else {
					if (def->size () == numeric_limits <uint32_t>::max ()) {
						message (*item, MessageType::ERROR, "Too many enumerators.");
						break;
					}
					def->push_back (enumerator);
				}
			}
		}
		return &*ins.first;
	}
	return nullptr;
}

void Builder::constant (const Type& t, const SimpleDeclarator& name, Variant&& val, const Location& loc)
{
	if (scope_stack_.back ()) {
		Ptr <NamedItem> item = Ptr <NamedItem>::make <Constant> (ref (*this), ref (t), ref (name), move (eval ().cast (t, move (val), loc)));
		auto ins = scope_stack_.back ()->insert (item);
		if (!ins.second)
			error_name_collision (name, **ins.first);
		else if (is_main_file ())
			container_stack_.top ()->append (item);
	}
	eval_pop ();
}

void Builder::eval_push (const Type& t, const Location& loc)
{
	const Type& type = t.dereference_type ();
	Eval* eval = nullptr;
	switch (type.kind ()) {
		case Type::Kind::BASIC_TYPE:
			if (is_integral (type.basic_type ()))
				eval = new EvalIntegral (*this);
			else if (is_floating_point (type.basic_type ()))
				eval = new EvalFloatingPoint (*this);
			break;
		case Type::Kind::STRING:
			eval = new EvalString (*this);
			break;
		case Type::Kind::WSTRING:
			eval = new EvalWString (*this);
			break;
		case Type::Kind::FIXED:
			eval = new EvalFixed (*this);
			break;
		case Type::Kind::NAMED_TYPE: {
			const Ptr <NamedItem>* ptype = type.named_type ();
			if (ptype) {
				if ((*ptype)->kind () == Item::Kind::ENUM)
					eval = new EvalEnum (*this, *ptype);
			} else
				eval = new Eval (*this);
		} break;
	}

	if (!eval) {
		message (loc, MessageType::ERROR, "Invalid constant type.");
		eval = new Eval (*this);
	}
	eval_stack_.push (unique_ptr <Eval> (eval));
}

}
}
