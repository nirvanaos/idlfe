#ifndef NIDL_AST_INTERFACE_H_
#define NIDL_AST_INTERFACE_H_

#include "NamedItem.h"
#include "Container.h"

namespace AST {

class InterfaceDecl :
	public NamedItem
{
public:
	enum class Kind
	{
		UNCONSTRAINED,
		ABSTRACT,
		LOCAL
	};

	InterfaceDecl (const std::string& name, Kind kind = Kind::UNCONSTRAINED) :
		NamedItem (Item::Kind::INTERFACE_DECL, name),
		kind_ (kind)
	{}

	Kind interface_kind () const
	{
		return kind_;
	}

	static const char* kind_name (Kind ic);

protected:
	InterfaceDecl (Item::Kind ic, const std::string& name, Kind kind = Kind::UNCONSTRAINED) :
		NamedItem (ic, name),
		kind_ (kind)
	{}

private:
	Kind kind_;
};

class Interface :
	public InterfaceDecl,
	public Container
{
public:
	Interface (const std::string& name, Kind kind = Kind::UNCONSTRAINED) :
		InterfaceDecl (Item::Kind::INTERFACE, name, kind)
	{}
};

}

#endif
