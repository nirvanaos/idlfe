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
#ifndef IDLFE_BE_MESSAGEOUT_H_
#define IDLFE_BE_MESSAGEOUT_H_
#pragma once

#include <string>
#include <iostream>

namespace AST {
class Location;
}

namespace BE {

class MessageOut
{
public:
	MessageOut (std::ostream& err_out = std::cerr, unsigned max_err_cnt = 20);

	enum class MessageType
	{
		ERROR,
		WARNING,
		MESSAGE
	};

	void message (const AST::Location& l, MessageType mt, const std::string& err);

	void message (const std::exception& ex);

	unsigned error_count () const
	{
		return err_cnt_;
	}

private:
	std::ostream err_out_;
	const unsigned max_err_cnt_;
	unsigned err_cnt_;
};

}

#endif
