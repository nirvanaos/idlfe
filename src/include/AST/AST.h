/// \file AST.h Abstract Synax Tree.
#ifndef NIDL_AST_AST_H_
#define NIDL_AST_AST_H_

#include "Item.h"
#include "Symbols.h"
#include "Container.h"

namespace AST {

/// Abstract Syntax Tree.
class AST :
	public Item,
	public Symbols,
	public Container
{
public:
	/// \returns The name of IDL file.
	const std::string& file () const
	{
		return *main_file_;
	}

	/// \internal

	AST (const std::string& file) :
		Item (Item::Kind::AST)
	{
		main_file_ = &*files_.insert (file).first;
		
	}

	std::pair <std::set <std::string>::iterator, bool> add_file (const std::string& name)
	{
		return files_.insert (name);
	}

private:
	std::set <std::string> files_;
	const std::string* main_file_;
	/// \endinternal
};

}

#endif
