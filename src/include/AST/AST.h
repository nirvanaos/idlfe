/// \file AST.h
/*
* Nirvana IDL Front End Library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  silver.popov@google.com
*/
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
	/// \returns The name of compiled IDL file.
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
