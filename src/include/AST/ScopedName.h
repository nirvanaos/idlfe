/// \file ScopedName.h
/*
* Nirvana IDL front-end library.
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
*  popov.nirvana@gmail.com
*/
#ifndef NIDL_AST_SCOPEDNAME_H_
#define NIDL_AST_SCOPEDNAME_H_

#include "Identifier.h"
#include "Location.h"
#include <vector>
#include <forward_list>

namespace AST {

/// The scoped name.
struct ScopedName : 
	std::vector <Identifier>,
	Location
{
	/// `true` if it is a root-scoped name, i.e. started from "::".
	bool from_root;

	/// \returns The name converted to string.
	std::string stringize () const;

	ScopedName () :
		from_root (false)
	{}

	ScopedName (const Location& loc) :
		Location (loc),
		from_root (false)
	{}

	ScopedName (const Location& loc, bool root, const Identifier& name);

	ScopedName (const ScopedName&) = default;
	ScopedName (ScopedName&&) = default;

	ScopedName& operator = (const ScopedName&) = default;
	ScopedName& operator = (ScopedName&&) = default;
};

typedef std::forward_list <ScopedName> ScopedNames;

}

#endif
