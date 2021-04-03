/// \file Native.h
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
#ifndef NIDL_AST_NATIVE_H_
#define NIDL_AST_NATIVE_H_

#include "NamedItem.h"
#include "RepositoryId.h"

namespace AST {

/// The 'native` type.
class Native :
	public NamedItem,
	public RepositoryId
{
private:
	template <class T> friend class Ptr;

	Native (const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (Kind::NATIVE, builder, name),
		RepositoryId (*this, builder)
	{}
};

}

#endif
