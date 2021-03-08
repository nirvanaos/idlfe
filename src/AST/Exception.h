/// \file Exception.h
#ifndef NIDL_AST_EXCEPTION_H_
#define NIDL_AST_EXCEPTION_H_

#include "ItemContainer.h"
#include "RepositoryId.h"

namespace AST {

/// The `exception` declaration.
class Exception :
	public ItemContainer
{
public:
	/// \internal
	Exception (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		ItemContainer (Item::Kind::EXCEPTION, builder, name)
	{}
	/// \endinternal
};

}

#endif
