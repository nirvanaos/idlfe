#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "ItemContainer.h"

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

class Interface :
	public InterfaceKind,
	public ItemContainer
{
public:
	Interface (const Location& loc, const ItemScope* parent, const std::string& name, InterfaceKind kind = InterfaceKind::UNCONSTRAINED) :
		InterfaceKind (kind),
		ItemContainer (loc, Item::Kind::INTERFACE, parent, name)
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
	public NamedItem
{
public:
	InterfaceDecl (const Location& loc, const ItemScope* parent, const std::string& name, InterfaceKind kind = InterfaceKind::UNCONSTRAINED) :
		InterfaceKind (kind),
		NamedItem (loc, Item::Kind::INTERFACE_DECL, parent, name)
	{}
};

}

#endif
