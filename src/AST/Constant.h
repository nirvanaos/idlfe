/// \file Constant.h
#ifndef NIDL_AST_CONSTANT_H_
#define NIDL_AST_CONSTANT_H_

#include "NamedItem.h"
#include "Type.h"
#include "Variant.h"

namespace AST {

/// `const` definition.
class Constant :
	public NamedItem,
	public Type,
	public Variant
{
public:
	/// \internal

	Constant (const Build::Builder& builder, const Type& type, const Build::SimpleDeclarator& name, Variant&& val) :
		NamedItem (Item::Kind::CONSTANT, builder, name),
		Type (type),
		Variant (std::move (val))
	{}
	/// \endinternal
};

}

#endif
