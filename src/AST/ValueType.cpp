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
#include "../include/AST/ValueType.h"
#include "Builder/Builder.h"

using namespace std;

namespace AST {

using namespace Build;

const char* ValueType::modifier_name () const
{
	static const char* const names [] = {
		"",
		"custom",
		"abstract",
		"truncatable"
	};

	return names [(size_t)modifier_];
}

void ValueType::get_all_containers (Containers& all) const
{
	all.reserve (all.size () + bases_.size () + supports_.size () + 1);
	all.push_back (this);
	all.insert (all.end (), bases_.begin (), bases_.end ());
	all.insert (all.end (), supports_.begin (), supports_.end ());
	for (const ValueType* base : bases_) {
		base->get_all_containers (all);
	}
	for (const Interface* base : supports_) {
		base->get_all_containers (all);
	}
}

}

