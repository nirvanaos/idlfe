/// \file Root.h
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
#ifndef NIDL_AST_ROOT_H_
#define NIDL_AST_ROOT_H_

#include "Item.h"
#include "Symbols.h"
#include "Container.h"
#include <filesystem>
#include <unordered_set>

namespace AST {

/// \brief Abstract Syntax Tree root.
class Root :
	public Item,
	public Container
{
public:
	/// \returns The name of compiled IDL file.
	const std::filesystem::path& file () const noexcept
	{
		return main_file_;
	}

	/// \brief Visit all items for the code generation.
	/// \returns `true` if unsuppported building blocks were occurred.
	bool visit (CodeGen& cg) const;

private:
	template <class T> friend class Ptr;

	Root (const std::string& file) :
		Item (Item::Kind::ROOT),
		main_file_ (add_file (file))
	{}

	friend class Build::Builder;

	const std::filesystem::path& add_file (const std::string& name)
	{
		return *files_.insert (std::filesystem::absolute (name)).first;
	}

	operator Symbols& () noexcept
	{
		return symbols_;
	}

	operator const Symbols& () const noexcept
	{
		return symbols_;
	}

private:

	struct fs_hash
	{
		size_t operator () (const std::filesystem::path& p) const
		{
			return std::filesystem::hash_value (p);
		}
	};

	std::unordered_set <std::filesystem::path, fs_hash> files_;
	const std::filesystem::path& main_file_;
	Symbols symbols_;
};

}

#endif
