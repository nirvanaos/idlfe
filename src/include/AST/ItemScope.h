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
#ifndef IDLFE_AST_ITEMSCOPE_H_
#define IDLFE_AST_ITEMSCOPE_H_

#include "ItemWithId.h"
#include "Symbols.h"

namespace AST {

/// \brief The named item which defines a scope.
class ItemScope :
	public ItemWithId
{
public:
	/// \returns Symbols reference.
	operator Symbols& () noexcept
	{
		return symbols_;
	}

	/// \returns Symbols const reference.
	operator const Symbols& () const noexcept
	{
		return symbols_;
	}

	/// Cast NamedItem to ItemScope.
	/// If \p item points to object derived from ItemScope, returns ItemScope pointer.
	/// Otherwise returns `nullptr`.
	///
	/// \param item NamedItem pointer.
	/// \returns ItemScope pointer or `nullptr`.
	static const ItemScope* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	/// Cast NamedItem to ItemScope.
	/// If \p item points to object derived from ItemScope, returns ItemScope pointer.
	/// Otherwise returns `nullptr`.
	///
	/// \param item NamedItem pointer.
	/// \returns ItemScope pointer or `nullptr`.
	static ItemScope* cast (NamedItem* item) noexcept;

protected:
	friend class Build::Builder;

	ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		prefix_ = pref;
		return true;
	}

private:
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
