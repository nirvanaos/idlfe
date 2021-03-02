#ifndef NIDL_AST_NATIVE_H_
#define NIDL_AST_NATIVE_H_

#include "NamedItem.h"
#include "RepositoryId.h"

namespace AST {

class Native :
	public RepositoryId,
	public NamedItem
{
public:
	Native (const ItemScope* parent, const SimpleDeclarator& name) :
		NamedItem (Kind::NATIVE, parent, name)
	{}
};

}

#endif
