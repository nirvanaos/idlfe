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
#include "../include/AST/ScopedName.h"

using namespace std;

namespace AST {

ScopedName::ScopedName (const Location& loc, bool root, const Identifier& name) :
	Location (loc),
	from_root (root)
{
	push_back (name);
}

string ScopedName::stringize () const
{
	const_iterator name = begin ();
	string ret;
	if (!from_root) {
		ret = *name;
		++name;
	}
	for (; name != end (); ++name) {
		ret += "::";
		ret += *name;
	}

	return ret;
}

}
