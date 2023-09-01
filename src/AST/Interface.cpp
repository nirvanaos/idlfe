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
#include "../include/AST/Interface.h"

namespace AST {

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

void Interface::get_all_interfaces (IV_Bases& all) const
{
	all.reserve (all.size () + bases_.size () + 1);
	all.push_back (this);
	all.insert (all.end (), bases_.begin (), bases_.end ());
	for (const Interface* base : bases_)
		base->get_all_interfaces (all);
}

Interfaces Interface::get_all_bases () const
{
	std::unordered_set <const Interface*> bset;
	Interfaces bvec;
	get_all_bases (bset, bvec);
	return bvec;
}

void Interface::get_all_bases (std::unordered_set <const Interface*>& bset, Interfaces& bvec) const
{
	for (const Interface* base : bases_) {
		if (bset.insert (base).second) {
			base->get_all_bases (bset, bvec);
			bvec.push_back (base);
		}
	}
}

}
