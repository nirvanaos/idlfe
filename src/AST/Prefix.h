#ifndef NIDL_AST_PREFIX_H_
#define NIDL_AST_PREFIX_H_

#include "Item.h"
#include <string>

namespace AST {

class Prefix :
	public Item
{
public:
	Prefix (std::string&& s) :
		Item (Item::Kind::PREFIX),
		prefix_ (std::move (s))
	{}

	const std::string& prefix () const
	{
		return prefix_;
	}

private:
	const std::string prefix_;
};

}

#endif
