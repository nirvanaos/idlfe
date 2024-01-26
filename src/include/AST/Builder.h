// \file
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef IDLFE_AST_BUILDER_H_
#define IDLFE_AST_BUILDER_H_
#pragma once

#include "../IDL_FrontEnd.h"
#include "Root.h"
#include "ValueType.h"
#include "Parameter.h"
#include "ScopedName.h"
#include "Exception.h"
#include "Declarators.h"
#include "Variant.h"
#include "../BE/MessageOut.h"
#include <ostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace AST {

namespace Build {
class Eval;
class EvalEnum;
}

class OperationBase;
class Attribute;
class StructBase;
class Union;
class Module;

/// \brief The %AST builder.
class Builder : public BE::MessageOut
{
public:
	/// \brief Currently parsed IDL file path.
	const std::filesystem::path* file () const noexcept
	{
		return cur_file_;
	}

	/// \returns Current location.
	Location location () const;

	/// \brief Test if currently parsed file is main.
	///
	/// \returns `true` if currently parsed file is compiled file.
	///          `false` if currently parsed file is included file.
	bool is_main_file () const noexcept
	{
		return cur_file_ == &tree_->file ();
	}

	/// \returns Repository id prefix. See the `typeprefix` IDL keyword.
	const std::string& prefix () const;

	/// \brief Set repository id prefix for item. See the `typeprefix` IDL keyword.
	/// 
	/// \param name Item name.
	/// \param s Prefix string.
	/// \param id_loc `typeprefix` location.
	void type_prefix (const ScopedName& name, const Variant& s, const Location& id_loc);

	/// \brief Set repository id for item. See the `typeid` IDL keyword.
	/// 
	/// \param name Item name.
	/// \param id Repository id.
	/// \param id_loc `typeid` location.
	void type_id (const ScopedName& name, const Variant& id, const Location& id_loc)
	{
		if (!id.empty ())
			type_id (name, id.as_string (), id_loc);
	}

	/// \brief Set repository id version for item. See `#pragma version`.
	/// \param name Item name.
	/// \param ver Item version.
	/// \param loc location.
	void pragma_version (const ScopedName& name, const Version& ver, const Location& loc)
	{
		ItemWithId* rep_id = lookup_rep_id (name);
		if (rep_id)
			rep_id->pragma_version (*this, ver, loc);
	}

	/// \returns Current parent item or `nullptr` if parent item is invalid.
	const Item* cur_parent () const;

	/// \brief Create native type.
	/// 
	/// \param name Native type name.
	void native (const SimpleDeclarator& name);

	/// \brief Begin module.
	/// 
	/// \param name The module name.
	/// \returns Module definition.
	void module_begin (const SimpleDeclarator& name);

	/// \brief End module.
	void module_end ()
	{
		assert (!scope_stack_.empty ());
		assert (scope_stack_.back ()->kind () == Item::Kind::MODULE);
		scope_end ();
	}

	/// \brief Create interface forward declaration.
	/// 
	/// \param name The interface name.
	/// \param ik The interface kind.
	void interface_decl (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind ());

	/// \brief Begin interface definition.
	/// 
	/// \param name The interface name.
	/// \param ik The interface kind.
	void interface_begin (const SimpleDeclarator& name, InterfaceKind ik = InterfaceKind ());

	/// \brief Set current interface bases.
	/// 
	/// \param bases Base names.
	void interface_bases (const ScopedNames& bases);

	/// \brief Begin operation definition for the current interface or valuetype.
	/// 
	/// \param oneway `true` for `oneway` operations.
	/// \param type Return type.
	/// \param name The operation name.
	void operation_begin (bool oneway, Type&& type, const SimpleDeclarator& name);

	/// \brief Add parameter to the current operation.
	/// 
	/// \param att The parameter attribute.
	/// \param type The parameter type.
	/// \param name The parameter name.
	void parameter (Parameter::Attribute att, Type&& type, const SimpleDeclarator& name);
	
	/// \brief Set raises for the current operation.
	/// 
	/// \param names Exception names.
	void raises (const ScopedNames& names);

	/// \brief Set context for the current operation.
	/// 
	/// \param strings Context strings.
	void operation_context (const Variants& strings);

	/// \brief End of the operation definition.
	void operation_end ()
	{
		operation_.clear ();
	}

	/// \brief Add attributes to the current interface or valuetype.
	/// 
	/// \param readonly `true` if attributes are read-only.
	/// \param type The attributes type.
	/// \param declarators The attribute names.
	void attribute (bool readonly, Type&& type, const SimpleDeclarators& declarators);

	/// \brief Begin attribute definition for the current interface or valuetype.
	///
	/// \param readonly `true` if attribute is read-only.
	/// \param type The attribute type.
	/// \param name The attribute name.
	void attribute_begin (bool readonly, Type&& type, const SimpleDeclarator& name);
	
	/// \brief Set getraises for the current attribute.
	/// 
	/// \param names Exception names.
	void getraises (const ScopedNames& names);

	/// \brief Set setraises for the current attribute.
	/// 
	/// \param names Exception names.
	void setraises (const ScopedNames& names);

	/// \brief End of the attribute definition.
	void attribute_end ()
	{
		attribute_.clear ();
	}

	/// \brief End of the interface definition.
	void interface_end ()
	{
		iv_end (Item::Kind::INTERFACE);
	}

	/// \brief Create type alias.
	/// 
	/// \param type The type.
	/// \param declarators Type aliases.
	void type_def (Type&& type, const Declarators& declarators);

	/// \brief Create structure forward declaration.
	/// 
	/// \param name The structure name.
	void struct_decl (const SimpleDeclarator& name);

	/// \brief Begin of the structure definition.
	/// 
	/// \param name The structure name.
	void struct_begin (const SimpleDeclarator& name);

	/// \brief End of the structure definition.
	/// \returns The pointer to definition.
	const NamedItem* struct_end ()
	{
		return constr_type_end ();
	}

	/// \brief Begin of the exception definition.
	/// 
	/// \param name The exception name.
	void exception_begin (const SimpleDeclarator& name);

	/// \brief End of the exception definition.
	void exception_end ()
	{
		constr_type_end ();
	}

	/// \brief Add members to the current structure or exception.
	/// 
	/// \param type The members type.
	/// \param names The member names.
	void member (Type&& type, const Declarators& names);

	/// \brief Create union forward declaration.
	/// 
	/// \param name The union name.
	void union_decl (const SimpleDeclarator& name);

	/// \brief Begin of the union definition.
	/// 
	/// \param name The union name.
	/// \param switch_type The union discriminator type.
	/// \param type_loc Discriminator type location.
	void union_begin (const SimpleDeclarator& name, const Type& switch_type, const Location& type_loc);

	/// \brief Create label for the current union.
	/// 
	/// \param label The constant.
	/// \param loc The label location.
	void union_label (const Variant& label, const Location& loc);

	/// \brief Create default label for the current union.
	/// 
	/// \param loc The label location.
	void union_default (const Location& loc);

	/// \brief Create union element for the current label.
	/// 
	/// \param type The element type.
	/// \param decl The element name.
	void union_element (Type&& type, const Declarator& decl);
	
	/// \brief End of the union definition.
	/// \returns The pointer to definition.
	const NamedItem* union_end ();

	/// \brief Create enum type.
	/// 
	/// \param name The enum type name.
	/// \param items The enum items.
	/// \returns The pointer to definition.
	const NamedItem* enum_type (const SimpleDeclarator& name, const SimpleDeclarators& items);

	/// \brief Create valuetype forward declaration.
	/// 
	/// \param name The valuetype name.
	/// \param is_abstract `true` for abstract valuetype.
	void valuetype_decl (const SimpleDeclarator& name, bool is_abstract = false);

	/// \brief Begin valuetype definition.
	/// 
	/// \param name The valuetype name.
	/// \param mod The valuetype modifier.
	void valuetype_begin (const SimpleDeclarator& name, ValueType::Modifier mod = ValueType::Modifier::NONE);

	/// \brief Set current valuetype bases.
	/// 
	/// \param truncatable `true` if valuetype is truncatable.
	/// \param bases Base names.
	void valuetype_bases (bool truncatable, const ScopedNames& bases);

	/// \brief Set current valuetype supported interfaces.
	/// 
	/// \param interfaces Supported interface names.
	void valuetype_supports (const ScopedNames& interfaces);

	/// \brief Add members to the current valuetype.
	/// 
	/// \param is_public `true` for public members.
	/// \param type The members type.
	/// \param names The member names.
	void state_member (bool is_public, Type&& type, const Declarators& names);

	/// \brief Begin valuetype factory.
	/// 
	/// \param name The valuetype factory name.
	void valuetype_factory_begin (const SimpleDeclarator& name);

	/// \brief End valuetype factory.
	/// 
	/// \param name The valuetype factory name.
	void valuetype_factory_end ()
	{
		operation_end ();
	}

	/// \brief End of the valuetype definition.
	void valuetype_end ()
	{
		iv_end (Item::Kind::VALUE_TYPE);
	}

	/// \brief Create value box definition.
	/// 
	/// \param name The value box name.
	/// \param type Boxed type.
	void valuetype_box (const SimpleDeclarator& name, Type&& type);

	/// \brief Create constant definition.
	/// 
	/// \param t The constant type.
	/// \param name The constant name.
	/// \param val The constant value.
	/// \param loc The constant value location.
	void constant (Type&& t, const SimpleDeclarator& name, Variant&& val, const Location& loc);

	/// \brief Create interface constant definition (Nirvana extension).
	/// \param t The constant type.
	/// \param name The constant name.
	void constant (Type&& t, const SimpleDeclarator& name);

	/// \brief Find type by name.
	/// 
	/// \param scoped_name The scoped name.
	/// \returns Type.
	Type lookup_type (const ScopedName& scoped_name);

	void see_prev_declaration (const Location& loc);
	void see_declaration_of (const Location& loc, const std::string& name);

	void check_anonymous (const Type& type, const SimpleDeclarator& name);

	void validate_id (const Identifier& name, const Location& loc);

protected:
	Builder () = delete;
	Builder (const Builder&) = delete;
	Builder& operator = (const Builder&) = delete;

	Builder (IDL_FrontEnd& compiler, const std::string& file);
	~Builder ();

	static const int FILE_FLAG_START = 0x1;
	static const int FILE_FLAG_SYSTEM = 0x2;
	void linemarker (const std::string& name, const Location& loc, int flags);
	void line (const std::string& filename);
	void pragma (const char* s, const Location& loc);
	Ptr <Root> finalize ();

	void eval_push (const Type& t, const Location& loc);
	void eval_pop ();
	Build::Eval& eval () const;

	unsigned positive_int (const Variant& v, const Location& loc);

	Type fixed_pt_type (unsigned digits, unsigned scale, const Location& loc)
	{
		if (digits > 31 || scale > digits) {
			message (loc, MessageType::ERROR, std::string ("fixed <") + std::to_string (digits) + ", " + std::to_string (scale) + "> type specification is invalid.");
			return Type ();
		} else
			return Type::make_fixed (digits, scale);
	}

private:
	friend class NamedItem;
	friend class Build::Eval;
	friend class Build::EvalEnum;

	const NamedItem* lookup (const ScopedName& scoped_name);
	bool prefix_valid (const std::string& pref, const Location& loc);
	void prefix (const std::string& pref, const Location& loc);
	bool get_quoted_string (const char*& s, std::string& qs, const Location& loc);
	static bool get_scoped_name (const char*& s, ScopedName& name);
	ItemWithId* lookup_rep_id (const ScopedName& name);
	void type_id (const ScopedName& name, const std::string& id, const Location& id_loc);

	Symbols* cur_scope () const;
	Symbols* scope_begin ();
	void scope_push (IV_Base* scope);
	void scope_end ();
	void iv_end (Item::Kind kind);

	const NamedItem* constr_type_end ();

	Raises lookup_exceptions (const ScopedNames& names);

	std::pair <bool, const NamedItem*> lookup (const ItemScope& scope, const Identifier& name, const Location& loc);
	std::pair <bool, const NamedItem*> lookup (const IV_Bases& containers, const Identifier& name, const Location& loc);
	
	static const NamedItem* find (const Symbols& symbols, const Identifier& name)
	{
		return symbols.find (name);
	}

	void error_name_collision (const SimpleDeclarator& name, const Location& prev_loc);
	void error_interface_kind (const SimpleDeclarator& name, InterfaceKind new_kind, InterfaceKind prev_kind, const Location& prev_loc);
	void error_valuetype_mod (const SimpleDeclarator& name, bool is_abstract, const Location& prev_loc);

	void add_base_members (const Location& loc, const IV_Bases& bases);
	bool check_member_name (const NamedItem& item);

	bool check_complete (const Type& type, const Location& loc);
	bool check_complete_or_ref (const Type& type, const Location& loc);
	bool check_complete_or_seq (const Type& type, const Location& loc);

	typedef std::unordered_map <std::string, const NamedItem&> RepIdMap;
	void check_rep_ids_unique (RepIdMap& ids, const Symbols& sym);
	void check_unique (RepIdMap& ids, const ItemWithId& rid);

	static bool is_base_of (const Interface& base, const Interface& derived);
	static void collect_concrete_interfaces (const ValueType& vt, std::unordered_map <const Interface*, const ValueType*>& interfaces);

	void check_complete (const Symbols& symbols);

	static Type make_type (Type&& t, const Declarator& decl);

	void check_pseudo (const SimpleDeclarator& name, InterfaceKind::Kind ik);

protected:
	IDL_FrontEnd& compiler_;
	Ptr <Root> tree_;

private:
	typedef std::vector <ItemScope*> ScopeStack;
	ScopeStack scope_stack_;
	std::stack <Container*> container_stack_;
	std::stack <std::unique_ptr <Build::Eval> > eval_stack_;

	struct File
	{
		std::string file;
		std::string prefix;

		File (const std::string& f) :
			file (f)
		{}
	};

	std::vector <File> file_stack_;
	const std::filesystem::path* cur_file_;

	// Current interface data. Also used for value types.
	struct InterfaceData
	{
		Symbols all_members;
		std::unordered_set <const Item*> all_bases;

		void clear ()
		{
			all_members.clear ();
			all_bases.clear ();
		}

	}
	interface_;

	// Current operation data
	struct OperationData
	{
		OperationBase* op;
		Symbols params;

		OperationData () :
			op (nullptr)
		{}

		void clear ()
		{
			op = nullptr;
			params.clear ();
		}
	}
	operation_;

	// Current attribute data
	struct AttributeData
	{
		Attribute* att;

		AttributeData () noexcept :
			att (nullptr)
		{}

		void clear () noexcept
		{
			att = nullptr;
		}
	}
	attribute_;

	// Constructed type: struct, exception, union
	struct ConstrType
	{
		const Ptr <NamedItem>* symbol;
		Symbols members;

		ConstrType () noexcept :
			symbol (nullptr)
		{}

		void clear () noexcept
		{
			symbol = nullptr;
			members.clear ();
		}

		NamedItem* obj () noexcept
		{
			return symbol ? *symbol : nullptr;
		}
	}
	constr_type_;

	// Current union data
	struct UnionData
	{
		std::unordered_map <Variant::Key, Location> all_labels;
		bool has_default;
		Location default_loc;

		UnionData () :
			has_default (false)
		{}

		void clear () noexcept
		{
			all_labels.clear ();
			has_default = false;
		}

		struct ElementData
		{
			bool is_default;
			std::vector <Variant> labels;

			ElementData () :
				is_default (false)
			{}

			void clear ()
			{
				is_default = false;
				labels.clear ();
			}
		}
		element;
	}
	union_;
};

}

#endif
