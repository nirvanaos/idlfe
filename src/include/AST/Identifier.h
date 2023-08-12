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
#ifndef IDLFE_AST_IDENTIFIER_H_
#define IDLFE_AST_IDENTIFIER_H_
#pragma once

#include <string>

namespace AST {

/// \brief An identifier.
class Identifier : public std::string
{
public:
	/// CORBA IDL identifiers are case-insensitive.
	bool operator < (const Identifier& r) const noexcept;

	/// Default constructor.
	Identifier () {}

	/// CORBA identifiers may be "escaped" by the underscore.
	/// 
	/// See 7.2.3.2 of https://www.omg.org/spec/IDL/4.2/
	Identifier (const char* s, size_t len) :
		std::string (s, len)
	{}

	/// CORBA identifiers may be "escaped" by the underscore.
	/// 
	/// See 7.2.3.2 of https://www.omg.org/spec/IDL/4.2/
	Identifier (const char* s) :
		std::string (s)
	{}

	/// CORBA identifiers may be "escaped" by the underscore.
	/// 
	/// See 7.2.3.2 of https://www.omg.org/spec/IDL/4.2/
	Identifier (std::string&& s) noexcept :
		std::string (std::move (s))
	{}

	/// Check for reserved word collision.
	/// 
	/// \returns `true` if identifier is valid.
	bool valid () const noexcept;

	/// Case-insensitive equivalency.
	bool operator == (const char* s) const noexcept;

	Identifier (const Identifier&) = default;
	Identifier (Identifier&&) = default;
	Identifier& operator = (const Identifier&) = default;
	Identifier& operator = (Identifier&&) = default;

	// Simplified tolower. We don't need to consider the locale settings.
	static char tolower (char c)
	{
		return ('A' <= c && c <= 'Z') ? c + 32 : c;
	}

private:
	static const char* const reserved_words_ [];
};

/// Case-insensitive compare.
bool operator < (const Identifier& l, const char* r) noexcept;

/// Case-insensitive compare.
bool operator < (const char* l, const Identifier& r) noexcept;

}

#endif
