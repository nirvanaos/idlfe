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

/// Compiler messages output.
class MessageOut
{
public:
	/// Constructor.
	/// 
	/// \param out Output stream. Default is std::cerr.
	/// \param max_err_cnt Maximal error count.
	///   When count of MessageType::ERROR messages reaches this limit,
	///   the std::runtime_error exception is thrown.
	MessageOut (std::ostream& out = std::cerr, unsigned max_err_cnt = 20);

	/// Message types.
	enum class MessageType
	{
		ERROR,   ///< Error message.
		WARNING, ///< Warning message.
		MESSAGE  ///< Informational message.
	};

	/// Print message to the output stream.
	/// 
	/// \param l AST::Location info.
	/// \param mt The message type.
	/// \param msg The message.
	void message (const AST::Location& l, MessageType mt, const std::string& msg);

	/// Print exception information to the output stream.
	///
	/// \param ex An exception.
	void message (const std::exception& ex);

	/// \returns Count of messages of the MessageType::ERROR type.
	unsigned error_count () const
	{
		return err_cnt_;
	}

private:
	std::ostream out_;
	const unsigned max_err_cnt_;
	unsigned err_cnt_;
};

}

#endif
