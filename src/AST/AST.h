#ifndef NIDL_AST_AST_H_
#define NIDL_AST_AST_H_

#include "Item.h"
#include "Symbols.h"
#include "Container.h"

namespace AST {

class AST :
	public Item,
	public Symbols,
	public Container
{
public:
	AST (const std::string& file) :
		Item (Item::Kind::AST)
	{
		main_file_ = &*files_.insert (file).first;
		
	}

	const std::string& file () const
	{
		return *main_file_;
	}

	const std::string& add_file (const std::string& name);

private:
	std::set <std::string> files_;
	const std::string* main_file_;
};

}

#endif
