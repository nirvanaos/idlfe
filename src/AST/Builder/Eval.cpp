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
#include "Eval.h"
#include "Builder.h"
#include "../../include/AST/Constant.h"

using namespace std;

namespace AST {
namespace Build {

void Eval::invalid_literal_type (const Location& loc) const
{
	builder_.message (loc, Builder::MessageType::ERROR, "invalid literal type");
}

void Eval::invalid_operation (char op, const Location& loc) const
{
	string sop;
	switch (op) {
		case '>':
			sop = ">>";
			break;
		case '<':
			sop = "<<";
			break;
		default:
			sop = op;
	}
	builder_.message (loc, Builder::MessageType::ERROR, string ("operation ") + sop + " is invalid in this context");
}

void Eval::error (const Location& loc, const std::exception& ex) const
{
	builder_.message (loc, Builder::MessageType::ERROR, ex.what ());
}

Variant Eval::literal_char (const string& s, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_wchar (const string& s, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_int (const string& s, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_float (const string& s, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_string (const string& s, const Location& loc, const Variant* append)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_wstring (const string& s, const Location& loc, const Variant* append)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_fixed (const string& s, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::literal_boolean (bool v, const Location& loc)
{
	invalid_literal_type (loc);
	return Variant ();
}

Variant Eval::constant (const ScopedName& constant)
{
	return Variant ();
}

const Constant* Eval::lookup_const (const ScopedName& constant) const
{
	const Ptr <NamedItem>* pitem = builder_.lookup (constant);
	if (pitem) {
		const NamedItem* item = *pitem;
		if (item->kind () != Item::Kind::CONSTANT) {
			builder_.message (constant, Builder::MessageType::ERROR, constant.stringize () + " is not a constant");
			see_definition (*item);
		} else
			return static_cast <const Constant*> (item);
	}
	return nullptr;
}

void Eval::see_definition (const NamedItem& item) const
{
	builder_.message (item, Builder::MessageType::MESSAGE, string ("see definition of ") + item.qualified_name ());
}

void Eval::invalid_constant_type (const ScopedName& constant) const
{
	builder_.message (constant, Builder::MessageType::ERROR, string ("constant ") + constant.stringize () + " type is invalid");
}

Variant Eval::expr (const Variant& l, char op, const Variant& r, const Location& loc)
{
	invalid_operation (op, loc);
	return Variant ();
}

Variant Eval::expr (char op, const Variant& v, const Location& loc)
{
	invalid_operation (op, loc);
	return Variant ();
}

Variant Eval::cast (const Type& t, Variant&& v, const Location& loc)
{
	return Variant ();
}

wchar_t Eval::unescape_wchar (const char*& p)
{
	unsigned c;
	if ('\\' == (c = *(p++))) {
		c = *(p++);
		switch (c) {
			case 'a':
				c = '\a';
				break;
			case 'n':
				c = '\n';
				break;
			case 't':
				c = '\t';
				break;
			case 'v':
				c = '\v';
				break;
			case 'b':
				c = '\b';
				break;
			case 'r':
				c = '\r';
				break;
			case 'f':
				c = '\f';
				break;
			case '\?':
				c = '\?';
				break;
			case '\\':
				c = '\\';
				break;
			case '\'':
				c = '\'';
				break;
			case '\"':
				c = '\"';
				break;
			case 'x':
				c = from_hex (p, 2);
				break;
			case 'u':
				c = from_hex (p, 3);
				break;

			default:
				if ('0' <= c && c <= '7') {
					unsigned maxlen = 3;
					while (--maxlen) {
						int d = *p;
						if ('0' <= d && d <= '7') {
							c = c * 8 + (d - '0');
							++p;
						}
					}
				} else
					invalid_escape_seq ();
		}
	}
	return (wchar_t)c;
}

char Eval::unescape_char (const char*& p)
{
	unsigned c = unescape_wchar (p);
	if (c > 0xFF)
		throw runtime_error ("Character out of range.");
	return (char)c;
}

int Eval::from_hdigit (int c) noexcept
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else
		return -1;
}

unsigned Eval::from_hex (const char*& p, unsigned maxlen)
{
	unsigned c;
	int d = from_hdigit (*(p++));
	if (d >= 0)
		c = d;
	else
		invalid_escape_seq ();
	while (--maxlen) {
		d = from_hdigit (*p);
		if (d >= 0) {
			c = c * 16 + d;
			++p;
		}
	}
	return c;
}

[[noreturn]] void Eval::invalid_escape_seq ()
{
	throw runtime_error ("Invalid escape sequence.");
}

[[noreturn]] void Eval::invalid_char_const ()
{
	throw runtime_error ("Invalid character constant.");
}

[[noreturn]] void Eval::overflow (char op)
{
	throw overflow_error (string ("Overflow in ") + op + " operation.");
}

[[noreturn]] void Eval::underflow (char op)
{
	throw underflow_error (string ("Underflow in ") + op + " operation.");
}

[[noreturn]] void Eval::zero_divide (char op)
{
	throw runtime_error (string ("Divide by zero in ") + op + " operation.");
}

}
}
