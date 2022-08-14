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
#ifndef IDLFE_AST_UNIONELEMENT_H_
#define IDLFE_AST_UNIONELEMENT_H_
#pragma once

#include "Member.h"
#include "Variant.h"
#include <vector>

namespace AST {

/// `union` element definition.
/// 
/// \invariant kind () == Item::Kind::UNION_ELEMENT
/// 
class UnionElement :
	public Member
{
public:
	/// The vector of `case` labels.
	typedef std::vector <Variant> Labels;

	/// \returns The vector of `case` labels for this element. Empty for the `default` case.
	const Labels& labels () const noexcept
	{
		return labels_;
	}

	/// \returns `true` if this is the `default` element.
	bool is_default () const noexcept
	{
		return labels_.empty ();
	}

private:
	template <class T> friend class Ptr;

	UnionElement (Build::Builder& builder, std::vector <Variant>&& labels, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::UNION_ELEMENT),
		labels_ (std::move (labels))
	{}

private:
	const std::vector <Variant> labels_;
};

}

#endif
