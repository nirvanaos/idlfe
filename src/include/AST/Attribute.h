/// \file Attribute.h
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
#ifndef NIDL_AST_ATTRIBUTE_H_
#define NIDL_AST_ATTRIBUTE_H_

#include "Exception.h"
#include "Type.h"

namespace AST {

/// \brief The `attribute` specification.
class Attribute :
	public NamedItem,
	public Type
{
public:
	/// \returns `true` if the attribute is marked as `readonly`.
	bool readonly () const noexcept
	{
		return readonly_;
	}

	/// \returns Potential user exceptions may be raised when the attribute is read.
	const Raises& getraises () const noexcept
	{
		return getraises_;
	}

	/// \returns Potential user exceptions may be raised when the attribute is written.
	const Raises& setraises () const noexcept
	{
		return setraises_;
	}

private:
	friend class Build::Builder;
	template <class T> friend class Ptr;

	Attribute (const Build::Builder& builder, bool readonly, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::ATTRIBUTE, builder, name),
		Type (type),
		readonly_ (readonly)
	{}

	void getraises (Raises&& raises) noexcept
	{
		getraises_ = std::move (raises);
	}

	void setraises (Raises&& raises) noexcept
	{
		setraises_ = std::move (raises);
	}

private:
	bool readonly_;
	Raises getraises_;
	Raises setraises_;
};

}

#endif
