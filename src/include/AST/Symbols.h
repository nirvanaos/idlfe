/// \file Symbols.h
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
#ifndef NIDL_AST_SYMBOLS_H_
#define NIDL_AST_SYMBOLS_H_

#include "NamedItem.h"
#include <set>
#include <map>

namespace AST {

/// The ordered container of pointers to NamedItem.
class Symbols :
	public std::set <Ptr <NamedItem>, std::less <>>
{
	typedef std::set <Ptr <NamedItem>, std::less <>> Base;
public:
	// Methods made outline to reduce size.
	std::pair <iterator, bool> emplace (const NamedItem&);

	std::pair <iterator, bool> insert (const NamedItem& item)
	{
		return emplace (item);
	}

	const Ptr <NamedItem>* find (const Identifier& name) const;

	virtual std::pair <bool, const Ptr <NamedItem>*> find (Build::Builder& builder, const Identifier& name, const Location&) const;

	// Check for repository is uniquness
	void check_rep_ids_unique (Build::Builder& builder, std::map <std::string, const NamedItem*>& ids) const;
};

}

#endif
