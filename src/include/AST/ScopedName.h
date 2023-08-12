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
#ifndef IDLFE_AST_SCOPEDNAME_H_
#define IDLFE_AST_SCOPEDNAME_H_
#pragma once

#include "Identifier.h"
#include "Location.h"
#include <vector>
#include <forward_list>

namespace AST {

/// \brief A scoped name: sequence of identifiers.
struct ScopedName : 
	std::vector <Identifier>,
	Location
{
	/// `true` if it is a root-scoped name, i.e. started from "::".
	bool from_root;

	/// \returns The name converted to string.
	std::string stringize () const;

	/// \brief Create empty name.
	ScopedName () :
		from_root (false)
	{}

	/// \brief Create empty name with location.
	/// 
	/// \param loc Location.
	ScopedName (const Location& loc) :
		Location (loc),
		from_root (false)
	{}

	/// \brief Create single-element name.
	/// 
	/// \param loc Location.
	/// \param root `true` if it is a root-scoped name, i.e. started from "::".
	/// \param name The name.
	ScopedName (const Location& loc, bool root, const Identifier& name);

	/// \brief Create single-element name.
	/// 
	/// \param loc Location.
	/// \param root `true` if it is a root-scoped name, i.e. started from "::".
	/// \param name The name.
	ScopedName (const Location& loc, bool root, Identifier&& name) noexcept;

	/// \brief Create multi-element name.
	/// 
	/// \param loc Location.
	/// \param root `true` if it is a root-scoped name, i.e. started from "::".
	/// \param names Name components.
	ScopedName (const Location& loc, bool root, std::initializer_list <const char*> names) :
		Location (loc),
		from_root (root)
	{
		reserve (names.size ());
		for (auto n : names) {
			emplace_back (n);
		}
	}

	/// \brief Copy constructor.
	ScopedName (const ScopedName&) = default;

	/// \brief Move constructor.
	ScopedName (ScopedName&&) = default;

	/// \brief Assignment.
	ScopedName& operator = (const ScopedName&) = default;

	/// \brief Assignment.
	ScopedName& operator = (ScopedName&&) = default;

	/// \brief Test for equality.
	/// 
	/// \param rhs Other ScopedName.
	/// \returns `true` if the names are equal.
	bool operator == (const ScopedName& rhs) const noexcept
	{
		return from_root == rhs.from_root &&
			static_cast <const std::vector <Identifier>&> (*this) == (rhs);
	}
};

/// Sequence of scoped names.
typedef std::forward_list <ScopedName> ScopedNames;

}

#endif
