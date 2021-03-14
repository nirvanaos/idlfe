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
#include "../include/AST/Symbols.h"
#include "../include/AST/RepositoryId.h"
#include "../include/AST/ItemScope.h"

using namespace std;

namespace AST {

std::pair <Symbols::iterator, bool> Symbols::emplace (const NamedItem& item)
{
	return Base::emplace (&const_cast <NamedItem&> (item));
}

pair <bool, const Ptr <NamedItem>*> Symbols::find (Build::Builder&, const Identifier& name, const Location&) const
{
	auto p = find (name);
	if (p)
		return make_pair (true, p);
	else
		return make_pair (false, nullptr);
}

const Ptr <NamedItem>* Symbols::find (const Identifier& name) const
{
	auto f = Base::find (name);
	if (f != end ())
		return &*f;
	else
		return nullptr;
}

}
