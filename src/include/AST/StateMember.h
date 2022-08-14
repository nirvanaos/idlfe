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
#ifndef IDLFE_AST_STATEMEMBER_H_
#define IDLFE_AST_STATEMEMBER_H_
#pragma once

#include "Member.h"

namespace AST {

/// \brief State member.
/// 
/// \invariant kind () == Item::Kind::STATE_MEMBER
/// 
class StateMember : public Member
{
public:
	/// \returns `true` for public member.
	bool is_public () const noexcept
	{
		return is_public_;
	}

private:
	template <class T> friend class Ptr;

	StateMember (Build::Builder& builder, bool is_public, const Type& t, const Build::SimpleDeclarator& name) :
		Member (builder, t, name, Item::Kind::STATE_MEMBER),
		is_public_ (is_public)
	{}

private:
	bool is_public_;
};

}

#endif
