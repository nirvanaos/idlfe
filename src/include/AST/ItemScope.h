/// \file ItemScope.h
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
#ifndef NIDL_AST_ITEMSCOPE_H_
#define NIDL_AST_ITEMSCOPE_H_

#include "Symbols.h"
#include "ItemScope.h"

namespace AST {

/// The named item which defines a scope.
class ItemScope :
	public NamedItem
{
protected:
	ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		prefix_ = pref;
		return true;
	}

private:
	friend class Build::Builder;

	operator Symbols& () noexcept
	{
		return symbols_;
	}

	operator const Symbols& () const noexcept
	{
		return symbols_;
	}

	static const ItemScope* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	static ItemScope* cast (NamedItem* item) noexcept;

	const std::string& prefix () const noexcept
	{
		return prefix_;
	}

private:
	Symbols symbols_;
	std::string prefix_;
};

}

#endif
