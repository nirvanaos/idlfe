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
#ifndef IDLFE_AST_ARRAY_H_
#define IDLFE_AST_ARRAY_H_
#pragma once

#include "Type.h"
#include <vector>

namespace AST {

/// \brief The array type descriptor.
class Array :
	public Type
{
public:
	/// \returns The vector of the array dimensions.
	const std::vector <Dim>& dimensions () const noexcept
	{
		return dimensions_;
	}

	/// Check types for equality.
	bool operator == (const Array& rhs) const noexcept
	{
		return Type::operator == (rhs) && dimensions () == rhs.dimensions ();
	}

private:
	friend class Type;

	Array (const Type& type, std::vector <Dim>&& dimensions) :
		Type (type),
		dimensions_ (std::move (dimensions))
	{}

private:
	std::vector <Dim> dimensions_;
};

}

#endif
