/// \file Identifier.h
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
#ifndef NIDL_AST_IDENTIFIER_H_
#define NIDL_AST_IDENTIFIER_H_

#include <string>

namespace AST {

/// An identifier.
class Identifier : public std::string
{
public:
	/// CORBA IDL identifiers are case-insensitive.
	bool operator < (const Identifier& r) const noexcept;

	Identifier () {}

	// Unescape identifier.
	Identifier (const char* s, size_t len) :
		std::string ('_' == *s ? s + 1 : s, '_' == *s ? len - 1 : len)
	{}

	Identifier (const Identifier&) = default;
	Identifier (Identifier&&) = default;
	Identifier& operator = (const Identifier&) = default;
	Identifier& operator = (Identifier&&) = default;
};

}

#endif
