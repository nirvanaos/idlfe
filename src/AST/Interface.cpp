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
#include "../include/AST/Interface.h"
#include "Builder/Builder.h"

using namespace std;

namespace AST {

using namespace Build;

const char* InterfaceKind::interface_kind_name () const noexcept
{
	static const char* const names [] = {
		"",
		"abstract",
		"local",
		"pseudo"
	};

	return names [(size_t)kind_];
}

void Interface::get_all_containers (Containers& all) const
{
	all.reserve (all.size () + bases_.size () + 1);
	all.push_back (this);
	all.insert (all.end (), bases_.begin (), bases_.end ());
	for (const Interface* base : bases_)
		base->get_all_containers (all);
}

void Interface::get_all_bases (set <const Interface*>& bases) const
{
	for (const Interface* base : bases_) {
		if (bases.insert (base).second)
			base->get_all_bases (bases);
	}
}

}
