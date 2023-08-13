/// \file
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

#include "Identifier.h"
#include "Location.h"
#include <forward_list>

namespace AST {

/// \brief The IDL simple declarator.
class SimpleDeclarator :
	public Identifier,
	public Location
{
public:
	SimpleDeclarator ()
	{}

	/// Constructor.
	/// 
	/// \param name The name.
	/// \param loc The location.
	SimpleDeclarator (const Identifier& name, const Location& loc) :
		Identifier (name),
		Location (loc)
	{}

	/// Constructor.
	/// 
	/// \param name The name.
	/// \param loc The location.
	SimpleDeclarator (Identifier&& name, const Location& loc) :
		Identifier (std::move (name)),
		Location (loc)
	{}

	SimpleDeclarator (const SimpleDeclarator&) = default;
	SimpleDeclarator (SimpleDeclarator&&) = default;

	SimpleDeclarator& operator = (const SimpleDeclarator&) = default;
	SimpleDeclarator& operator = (SimpleDeclarator&&) = default;
};

/// \brief The IDL simple declarators.
typedef std::forward_list <SimpleDeclarator> SimpleDeclarators;

/// \brief Array dimensions.
typedef std::forward_list <unsigned> FixedArraySizes;

/// \brief The IDL declarator.
class Declarator :
	public SimpleDeclarator
{
public:
	Declarator ()
	{}

	/// Array declarator.
	/// 
	/// \param name The name.
	/// \param loc The location.
	/// \param array Array sizes.
	Declarator (const Identifier& name, const Location& loc, const FixedArraySizes& array) :
		SimpleDeclarator (name, loc),
		array_ (array)
	{}

	/// Array declarator.
	/// 
	/// \param name The name.
	/// \param loc The location.
	/// \param array Array sizes.
	Declarator (Identifier&& name, const Location& loc, const FixedArraySizes& array) :
		SimpleDeclarator (std::move (name), loc),
		array_ (array)
	{}

	/// Simple declarator.
	/// 
	/// \param name The name.
	/// \param loc The location.
	Declarator (const Identifier& name, const Location& loc) :
		SimpleDeclarator (name, loc)
	{}

	/// Simple declarator.
	/// 
	/// \param name The name.
	/// \param loc The location.
	Declarator (Identifier&& name, const Location& loc) :
		SimpleDeclarator (std::move (name), loc)
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

	/// \returns Array dimensions.
	const FixedArraySizes& array_sizes () const
	{
		return array_;
	}

private:
	FixedArraySizes array_;
};

/// \brief The IDL declarators.
typedef std::forward_list <Declarator> Declarators;

}

#endif
