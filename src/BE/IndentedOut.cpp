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
#include "../include/BE/IndentedOut.h"

using namespace std;

namespace BE {

IndentedOut::IndentedOut ()
{}

IndentedOut::IndentedOut (const filesystem::path& file)
{
	open (file);
}

IndentedOut::~IndentedOut ()
{
	if (is_open ())
		isbuf_.term (*this);
}

void IndentedOut::open (const std::filesystem::path& file)
{
	if (is_open ())
		throw runtime_error ("File is already open");
	create_directories (file.parent_path ());
	ofstream::open (file);
	if (!is_open ())
		throw runtime_error (string ("Can not open file: ") + file.string ());
	isbuf_.init (*this);
}

int IndentedOut::IndentedStreambuf::overflow (int c)
{
	if (c != '\n') {
		if (bol_ && indentation_) {
			for (unsigned cnt = indentation_; cnt; --cnt) {
				int ret = put_char ('\t');
				if (ret != '\t')
					return ret;

			}
		}
		empty_line_ = bol_ = false;
	} else if (bol_)
		empty_line_ = true;
	else
		bol_ = true;
	return put_char (c);
}

int IndentedOut::IndentedStreambuf::put_char (char c)
{
	int ret = out_->sputc (c);
	if (ret == c)
		last_char_ = c;
	return ret;
}

void IndentedOut::IndentedStreambuf::empty_line ()
{
	if (!bol_) {
		out_->sputc ('\n');
		bol_ = true;
	}
	if (!empty_line_) {
		out_->sputc ('\n');
		empty_line_ = true;
	}
}

}
