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
			if (isspace (*end)) {
				if (!l)
					l = 1;
				s = end + 1;
				while (isspace (*s))
					++s;
				if (*s == '\"') {
					const char* end = strchr (s + 1, '\"');
					if (end) {
						file (string (s + 1, end - s - 1), AST::Location (file (), lineno () - 1));
						yylineno = l;
						return;
					}
				}
			}
		} else {
			char* numend;
			unsigned long l = strtoul (s, &numend, 10);
			if (isspace (*numend)) {
				s = numend + 1;
				while (isspace (*s))
					++s;
				if (*s == '\"') {
					const char* name = s + 1;
					const char* nameend = strchr (name, '\"');
					if (nameend) {
						s = nameend + 1;
						int flags = 0;
						while (*s) {
							while (isspace (*s))
								++s;
							if (isdigit (s [0]) && isspace (s [1])) {
								switch (*s) {
									case '1':
										flags |= FILE_FLAG_START;
										break;
									case '3':
										flags |= FILE_FLAG_SYSTEM;
										break;
								}
								s += 2;
							} else
								break;
						}
						file (string (name, nameend - name), AST::Location (file (), lineno () - 1), flags);
						yylineno = l;
						return;
					}
				}
			}
		}
	}
	message (AST::Location (file (), lineno () - 1), MessageType::ERROR, string ("invalid preprocessor directive: ") + dir);
}

void Driver::parser_error (const yy::location& loc, const std::string& cmsg)
{
	static const struct Punct
	{
		const char* name;
		const char* value;
	} punctuators [] = {
		{ "T_LEFT_CURLY_BRACKET", "{" },
		{ "T_RIGHT_CURLY_BRACKET", "}" },
		{ "T_LEFT_SQUARE_BRACKET", "[" },
		{ "T_RIGHT_SQUARE_BRACKET", "]" },
		{ "T_LEFT_PARANTHESIS", "(" },
		{ "T_RIGHT_PARANTHESIS", ")" },
		{ "T_COLON", ":" },
		{ "T_COMMA", "," },
		{ "T_SEMICOLON", ";" },
		{ "T_EQUAL", "=" },
		{ "T_SHIFTRIGHT", ">>" },
		{ "T_SHIFTLEFT", "<<" },
		{ "T_PLUS_SIGN", "+" },
		{ "T_MINUS_SIGN", "-" },
		{ "T_ASTERISK", "*" },
		{ "T_SOLIDUS", "/" },
		{ "T_PERCENT_SIGN", "%" },
		{ "T_TILDE", "~" },
		{ "T_VERTICAL_LINE", "|" },
		{ "T_CIRCUMFLEX", "^" },
		{ "T_AMPERSAND", "&" },
		{ "T_LESS_THAN_SIGN", "<" },
		{ "T_GREATER_THAN_SIGN", ">" },
		{ "T_SCOPE", "::" }
	};

	string msg = cmsg;

	for (size_t pos = 0; (pos = msg.find ('_')) != msg.npos;) {
		if ((pos > 0) && (pos == 1 || (' ' == msg [pos - 2])) && ('T' == msg [pos - 1]) && isalpha (msg [pos + 1])) {
			// T_ prefix.
			if (isupper (msg [pos + 1])) {
				// Punctuator
				size_t nameend = pos + 2;
				for (;; ++nameend) {
					char c = msg [nameend];
					if (!isupper (c) && '_' != c)
						break;
				}
				string name = msg.substr (pos - 1, nameend - pos + 1);
				const Punct* punct = nullptr;
				for (const Punct* p = punctuators; p < end (punctuators); ++p) {
					if (name == p->name) {
						punct = p;
						break;
					}
				}
				if (punct) {
					string val = "'";
					val += punct->value;
					val += '\'';
					msg.replace (pos - 1, name.length (), val);
					pos = pos - 1 + val.length ();
					continue;
				}
			}

			// Erase prefix
			msg.erase (--pos, 2);
		} else
			msg [pos++] = ' ';
	}

	message (loc, MessageType::ERROR, msg);
}

}
