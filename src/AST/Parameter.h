#ifndef NIDL_AST_PARAMETER_H_
#define NIDL_AST_PARAMETER_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class Parameter :
	public NamedItem,
	public Type
{
public:
	enum class Attribute
	{
		IN,
		OUT,
		INOUT
	};

	Parameter (const ItemScope* parent, Attribute att, const Type& type, const SimpleDeclarator& name) :
		NamedItem (Item::Kind::PARAMETER, parent, name),
		Type (type),
		attribute_ (att)
	{}

	Attribute attribute () const
	{
		return attribute_;
	}

private:
	Attribute attribute_;
};

}

#endif
