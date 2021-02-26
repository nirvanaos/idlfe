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
	Constant (const Location& loc, const ItemScope* parent, const std::string& name, const Variant& val) :
		NamedItem (loc, Item::Kind::CONSTANT, parent, name),
		Variant (val)
	{}
};

}

#endif
