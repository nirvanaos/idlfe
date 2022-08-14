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
#ifndef IDLFE_AST_SYMBOLS_H_
#define IDLFE_AST_SYMBOLS_H_
#pragma once

#include "NamedItem.h"
#include <set>

namespace AST {

/// Set of named items.
class Symbols :
	public std::set <Ptr <NamedItem>, std::less <> >
{
	typedef std::set <Ptr <NamedItem>, std::less <> > Base;
public:
	/// Find a symbol.
	/// 
	/// \param name Item name.
	/// \returns NamedItem pointer if found, otherwise `nullptr`.
	const Ptr <NamedItem>* find (const Identifier& name) const noexcept;

	/// Add named item to symbols.
	/// 
	/// \param item A NamedItem.
	/// \returns std::pair <iterator, bool>.
	std::pair <iterator, bool> emplace (const NamedItem& item);

	/// Add named item to symbols.
	/// 
	/// \param item A NamedItem.
	/// \returns std::pair <iterator, bool>.
	std::pair <iterator, bool> insert (const NamedItem& item)
	{
		return emplace (item);
	}
};

}

#endif
