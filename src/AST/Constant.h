#ifndef NIDL_AST_CONSTANT_H_
#define NIDL_AST_CONSTANT_H_

#include "NamedItem.h"
#include "Variant.h"

namespace AST {

class Constant :
	public NamedItem,
	public Variant
{
public:
	Constant (const ItemScope* parent, const SimpleDeclarator& name, const Variant& val) :
		NamedItem (Item::Kind::CONSTANT, parent, name),
		Variant (val)
	{}
};

}

#endif
