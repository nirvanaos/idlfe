/// \file Include.h
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
*  silver.popov@google.com
*/
#ifndef NIDL_AST_INCLUDE_H_
#define NIDL_AST_INCLUDE_H_

#include "Item.h"
#include <filesystem>

namespace AST {

/// `#include` file.
class Include :
	public Item
{
public:
	/// Returns the name of included IDL file.
	const std::filesystem::path& file () const
	{
		return file_;
	}

	/// \internals
	Include (std::filesystem::path&& file) :
		Item (Kind::INCLUDE),
		file_ (std::move (file))
	{}

private:
	std::filesystem::path file_;
	/// \endinternals
};

}

#endif