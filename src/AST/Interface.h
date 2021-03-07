/// \file Interface.h
#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class Interface;

/// The kind of interface.
class InterfaceKind
{
public:
	/// The kind of interface.
	enum Kind
	{
		UNCONSTRAINED,
		ABSTRACT,
		LOCAL
	};

	/// \returns The kind of interface.
	Kind interface_kind () const
	{
		return kind_;
	}

	/// \internals

	InterfaceKind (Kind kind = UNCONSTRAINED) :
		kind_ (kind)
	{}

	const char* interface_kind_name () const;

private:
	Kind kind_;
	/// \endinternals
};

/// The interface definition.
class Interface :
	public InterfaceKind,
	public ItemContainer,
	public RepositoryId
{
public:
	/// \returns The 
	const std::vector <const Interface*>& bases () const
	{
		return bases_;
	}

	/// \summary Get all direct and indirect bases.
	/// \param bases All bases.
	void get_all_bases (std::set <const Interface*>& bases) const;

	/// \internals

	void get_all_interfaces (std::vector <const Interface*>& all) const;

	Interface (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		ItemContainer (Item::Kind::INTERFACE, builder, name),
		RepositoryId (*this, builder)
	{}

	void add_base (const Interface* base)
	{
		bases_.push_back (base);
	}

private:
	std::vector <const Interface*> bases_;
	/// \endinternals
};

/// Interface forward declaration.
class InterfaceDecl :
	public InterfaceKind,
	public NamedItem,
	public RepositoryId
{
public:
	/// \internals
	InterfaceDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		NamedItem (Item::Kind::INTERFACE_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
	/// \endinternals
};

}

#endif
