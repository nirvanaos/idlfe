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
#include "Builder/Builder.h"
#include <algorithm>

using namespace std;
using namespace AST::Build;

namespace AST {

inline bool ci_compare (char l, char r)
{
	return std::tolower (l) < std::tolower (r);
}

bool ci_less (const string& l, const string& r)
{
	return lexicographical_compare (l.begin (), l.end (), r.begin (), r.end (), ci_compare);
}

NamedItem::NamedItem (Kind kind, const Builder& builder, const SimpleDeclarator& name) :
	Item (kind),
	Location (name),
	parent_ (builder.cur_scope ()),
	name_ (name)
{}

std::string NamedItem::qualified_name () const
{
	string qn;
	if (parent_)
		qn = parent_->qualified_name ();
	qn += "::";
	qn += name_;
	return qn;
}

ScopedName NamedItem::scoped_name () const
{
	ScopedName sn;
	if (parent_) {
		sn = parent_->scoped_name ();
		sn.push_back (name_);
	} else
		sn = ScopedName (*this, true, name_);
	return sn;
}

}
