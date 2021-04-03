/// \file Container.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
*  popov.nirvana@gmail.com
*/
#ifndef NIDL_AST_CONTAINER_H_
#define NIDL_AST_CONTAINER_H_

#include "Item.h"
#include <vector>

namespace AST {

/// Sequential container of the AST items.
template <class T>
class ContainerT :
	public std::vector <Ptr <T>>
{
protected:
	typedef std::vector <Ptr <T>> Base;

protected:
	friend class Build::Builder;

	void append (T& item)
	{
		Base::emplace_back (&item);
	}
};

class CodeGen;

/// Sequential container of the AST items.
class Container :
	public ContainerT <Item>
{
	typedef ContainerT <Item> Base;
public:
	/// Visit all items for the code generation.
	/// \returns `true` if unsuppported building blocks were occurred.
	bool visit (CodeGen& cg) const;

private:
	friend class Build::Builder;

	void append (Item& item);
};

}

#endif
