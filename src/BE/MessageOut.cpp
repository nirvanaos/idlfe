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
#include "../include/BE/MessageOut.h"
#include "../include/AST/Location.h"

using namespace std;

namespace BE {

MessageOut::MessageOut (ostream& err_out, unsigned max_err_cnt) :
	out_ (err_out.rdbuf ()),
	max_err_cnt_ (max_err_cnt),
	err_cnt_ (0)
{}

void MessageOut::message (const AST::Location& l, MessageType mt, const string& msg)
{
	static const char* const msg_types [] = { "error", "warning", "message" };

	if (l)
		out_ << l.file ().string () << '(' << l.line () << "): ";
	out_ << msg_types [(size_t)mt] << ": " << msg << endl;

	if (mt == MessageType::ERROR && (++err_cnt_ >= max_err_cnt_))
		throw runtime_error ("too many errors, compilation aborted");
}

void MessageOut::message (const std::exception& ex)
{
	out_ << ex.what () << endl;
}

}
