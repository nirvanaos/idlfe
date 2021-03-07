/// \file Include.h
#ifndef NIDL_AST_INCLUDE_H_
#define NIDL_AST_INCLUDE_H_

#include "Item.h"
#include <string>

namespace AST {

/// `#include`
class Include :
	public Item
{
public:
	/// Returns the name of included IDL file.
	const std::string& name () const
	{
		return name_;
	}

	/// \internals
	Include (const std::string& name) :
		Item (Kind::INCLUDE),
		name_ (name)
	{}

private:
	std::string name_;
	/// \endinternals
};

}

#endif
