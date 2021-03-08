/// \file ItemScope.h
/*
* Nirvana IDL Front End Library.
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
#ifndef NIDL_AST_ITEMSCOPE_H_
#define NIDL_AST_ITEMSCOPE_H_

#include "Symbols.h"
#include "ItemScope.h"

namespace AST {

/// The set of NamedItem elements.
class ItemScope :
	public NamedItem,
	public Symbols
{
public:
	/// \internal
	ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

	static ItemScope* cast (NamedItem* item) noexcept;

	static const ItemScope* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	const std::string& prefix () const noexcept
	{
		return prefix_;
	}

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		prefix_ = pref;
		return true;
	}

private:
	std::string prefix_;
	/// \endinternal
};

}

#endif
