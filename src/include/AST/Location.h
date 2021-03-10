/// \file Location.h
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
*  silver.popov@google.com
*/
#ifndef NIDL_AST_LOCATION_H_
#define NIDL_AST_LOCATION_H_

#include <string>
#include <assert.h>

/// Support for the Bison location.
namespace yy {
class location;
}

namespace AST {

/// Stores the location information.
class Location
{
public:
	Location () :
		file_ (nullptr),
		line_ (0)
	{}

	Location (const std::string& file, unsigned line) :
		file_ (&file),
		line_ (line)
	{}

	Location (const yy::location&);

	const std::string& file () const
	{
		assert (file_);
		return *file_;
	}

	unsigned line () const
	{
		return line_;
	}

	operator bool () const
	{
		return file_ != nullptr;
	}

private:
	const std::string* file_;
	unsigned line_;
};

}

#endif
