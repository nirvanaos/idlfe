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
#include "Driver.h"
#include <assert.h>

using namespace std;

namespace FE {

Driver::Driver (const std::string& file, std::istream& yyin) :
	yyFlexLexer (yyin, cout),
	AST::Build::Builder (file, cout),
	parser_ (*this)
{}

void Driver::preprocessor_directive (const char* const dir)
{
	const char* s = dir;
	assert (*s == '#');
	++s;
	while (isspace (*s))
		++s;
	if (*s) {
		if (!strncmp (s, "line", 4)) {
			s += 4;
			char* end;
			unsigned long l = strtoul (s, &end, 10);
			if (l > 0) {
				s = end;
				while (isspace (*s))
					++s;
				if (*s == '\"') {
					const char* end = s + strlen (s);
					while (isspace (*(--end)))
						;
					if (*end == '\"')
						file (string (s + 1, end - s - 1), location ());
					else
						parser_error (location (), string ("Invalid file name: ") + string (s, end - s));
				}
				yylineno = l;
			}
			return;
		}
	}
	parser_error (location (), string ("Invalid preprocessor directive: ") + dir);
}

}
