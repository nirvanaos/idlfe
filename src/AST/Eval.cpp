/// \file Eval.cpp Expression evaluiator.
#include "Eval.h"
#include "Builder.h"
#include "Constant.h"

using namespace std;

namespace AST {

void Eval::invalid_literal_type (unsigned line) const
{
	builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, "Invalid literal type.");
}

void Eval::invalid_operation (char op, unsigned line) const
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
	builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, string ("Operation ") + sop + " is invalid in this context.");
}

void Eval::error (unsigned line, const std::exception& ex) const
{
	builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, ex.what ());
}

Variant Eval::literal_char (const string& s, unsigned line)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_wchar (const string& s, unsigned line)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_int (const string& s, unsigned line)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_float (const string& s, unsigned line)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_string (const string& s, unsigned line, const Variant* append)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_wstring (const string& s, unsigned line, const Variant* append)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_fixed (const string& s, unsigned line)
{
	invalid_literal_type (line);
	return Variant ();
}

Variant Eval::literal_boolean (bool v, unsigned line)
{
	invalid_literal_type (line);
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
			builder_.message (constant, Builder::MessageType::ERROR, constant.stringize () + " is not a constant.");
			see_definition (*item);
		} else
			return static_cast <const Constant*> (item);
	}
	return nullptr;
}

void Eval::see_definition (const NamedItem& item) const
{
	builder_.message (item, Builder::MessageType::MESSAGE, string ("See definition of ") + item.qualified_name () + ".");
}

void Eval::invalid_constant_type (const ScopedName& constant) const
{
	builder_.message (constant, Builder::MessageType::ERROR, string ("Constant ") + constant.stringize () + " type is invalid.");
}

Variant Eval::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	invalid_operation (op, line);
	return Variant ();
}

Variant Eval::expr (char op, const Variant& v, unsigned line)
{
	invalid_operation (op, line);
	return Variant ();
}

Variant Eval::cast (const Type& t, Variant&& v, unsigned line)
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
