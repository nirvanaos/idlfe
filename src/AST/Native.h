#ifndef NIDL_AST_NATIVE_H_
#define NIDL_AST_NATIVE_H_

#include "NamedItem.h"
#include "RepositoryId.h"

namespace AST {

class Native :
	public NamedItem,
	public RepositoryId
{
public:
	Native (const Builder& builder, const SimpleDeclarator& name) :
		NamedItem (Kind::NATIVE, builder, name),
		RepositoryId (*this)
	{}
};

}

#endif
