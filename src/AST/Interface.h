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

	InterfaceKind (Kind kind) :
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

class InterfaceBase :
	public InterfaceKind,
	public RepositoryId
{
protected:
	InterfaceBase (InterfaceKind kind) :
		InterfaceKind (kind)
	{}
};

class Interface :
	public InterfaceBase,
	public ItemContainer
{
public:
	Interface (const ItemScope* parent, const SimpleDeclarator& name, InterfaceKind kind = InterfaceKind::UNCONSTRAINED) :
		InterfaceBase (kind),
		ItemContainer (Item::Kind::INTERFACE, parent, name)
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
	public InterfaceBase,
	public NamedItem
{
public:
	InterfaceDecl (const ItemScope* parent, const SimpleDeclarator& name, InterfaceKind kind = InterfaceKind::UNCONSTRAINED) :
		InterfaceBase (kind),
		NamedItem (Item::Kind::INTERFACE_DECL, parent, name)
	{}
};

}

#endif
