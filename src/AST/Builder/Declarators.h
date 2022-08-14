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
#ifndef IDLFE_AST_DECLARATORS_H_
#define IDLFE_AST_DECLARATORS_H_
#pragma once

#include "../../include/AST/Identifier.h"
#include "../../include/AST/Location.h"
#include <forward_list>

namespace AST {
namespace Build {

class SimpleDeclarator :
	public Identifier,
	public Location
{
public:
	SimpleDeclarator ()
	{}

	SimpleDeclarator (const Identifier& name, const Location& loc) :
		Identifier (name),
		Location (loc)
	{}

	SimpleDeclarator (const SimpleDeclarator&) = default;
	SimpleDeclarator (SimpleDeclarator&&) = default;

	SimpleDeclarator& operator = (const SimpleDeclarator&) = default;
	SimpleDeclarator& operator = (SimpleDeclarator&&) = default;
};

typedef std::forward_list <SimpleDeclarator> SimpleDeclarators;

typedef std::forward_list <unsigned> FixedArraySizes;

class Declarator :
	public SimpleDeclarator
{
public:
	Declarator ()
	{}

	Declarator (const Identifier& name, const Location& loc, const FixedArraySizes& array) :
		SimpleDeclarator (name, loc),
		array_ (array)
	{}

	Declarator (const Identifier& name, const Location& loc) :
		SimpleDeclarator (name, loc)
	{}

	Declarator (const SimpleDeclarator& decl) :
		SimpleDeclarator (decl)
	{}

	Declarator (SimpleDeclarator&& decl) :
		SimpleDeclarator (std::move (decl))
	{}

	Declarator (const Declarator&) = default;
	Declarator (Declarator&&) = default;

	Declarator& operator = (const Declarator&) = default;
	Declarator& operator = (Declarator&&) = default;

	const FixedArraySizes& array_sizes () const
	{
		return array_;
	}

private:
	FixedArraySizes array_;
};

typedef std::forward_list <Declarator> Declarators;

}
}

#endif
