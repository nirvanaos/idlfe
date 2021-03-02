#ifndef NIDL_AST_TYPEDEF_H_
#define NIDL_AST_TYPEDEF_H_

#include "NamedItem.h"
#include "Type.h"
#include "RepositoryId.h"

namespace AST {

class TypeDef :
	public NamedItem,
	public RepositoryId,
	public Type
{
public:
	TypeDef (const Builder& builder, const SimpleDeclarator& name, const Type& t) :
		NamedItem (Item::Kind::TYPEDEF, builder, name),
		RepositoryId (*this, builder),
		Type (t)
	{}
};

}

#endif
