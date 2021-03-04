#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

class Interface;

class InterfaceKind
{
public:
	enum Kind
	{
		UNCONSTRAINED,
		ABSTRACT,
		LOCAL
	};

	InterfaceKind (Kind kind = UNCONSTRAINED) :
		kind_ (kind)
	{}

	Kind interface_kind () const
	{
		return kind_;
	}

	const char* interface_kind_name () const;

private:
	Kind kind_;
};

class Interface :
	public InterfaceKind,
	public ItemContainer,
	public RepositoryId
{
public:
	Interface (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		ItemContainer (Item::Kind::INTERFACE, builder, name),
		RepositoryId (*this, builder)
	{}

	void add_base (const Interface* base)
	{
		bases_.push_back (base);
	}

	const std::vector <const Interface*>& bases () const
	{
		return bases_;
	}

	void get_all_interfaces (std::vector <const Interface*>& all) const;

	void get_all_bases (std::set <const Interface*>& bases) const;

private:
	std::vector <const Interface*> bases_;
};

class InterfaceDecl :
	public InterfaceKind,
	public NamedItem,
	public RepositoryId
{
public:
	InterfaceDecl (const Build::Builder& builder, const Build::SimpleDeclarator& name, InterfaceKind kind = InterfaceKind ()) :
		InterfaceKind (kind),
		NamedItem (Item::Kind::INTERFACE_DECL, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
