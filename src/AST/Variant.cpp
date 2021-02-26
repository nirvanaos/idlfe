#include "Variant.h"
#include <stdexcept>
#include <limits>
#include <assert.h>

using namespace std;

namespace AST {

Variant::Variant (Variant&& src) noexcept :
	Type (move (src))
{
	memcpy (&val_, &src.val_, sizeof (val_));
	src.reset ();
}

Variant& Variant::operator = (const Variant& src)
{
	clear ();
	copy (src);
	return *this;
}

Variant& Variant::operator = (Variant&& src) noexcept
{
	clear ();
	memcpy (&val_, &src.val_, sizeof (val_));
	src.reset ();
	return *this;
}

void Variant::clear () noexcept
{
	switch (kind ()) {
		case Kind::STRING:
			val_.s.~basic_string ();
			break;
		case Kind::WSTRING:
			val_.ws.~basic_string ();
			break;
	}
	reset ();
}

void Variant::copy (const Variant& src)
{
	Type::copy (src);
	switch (kind ()) {
		case Kind::STRING:
			new (&val_.s) string (src.val_.s);
			break;

		case Kind::WSTRING:
			new (&val_.ws) wstring (src.val_.ws);
			break;

		default:
			memcpy (&val_, &src.val_, sizeof (val_));
	}
}

wchar_t Variant::unescape_wchar (const char*& p)
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

char Variant::unescape_char (const char*& p)
{
	unsigned c = unescape_wchar (p);
	if (c > 0xFF)
		throw runtime_error ("Character out of range.");
	return (char)c;
}

int Variant::from_hdigit (int c) noexcept
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

unsigned Variant::from_hex (const char*& p, unsigned maxlen)
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

[[noreturn]] void Variant::invalid_escape_seq ()
{
	throw runtime_error ("Invalid escape sequence.");
}

[[noreturn]] void Variant::invalid_char_const ()
{
	throw runtime_error ("Invalid character constant.");
}

Variant Variant::make_char (const string& v)
{
	assert (v.size () > 2);
	assert (v.front () == '\'');
	assert (v.back () == '\'');
	Variant var (BasicType::CHAR);
	const char* p = &v.front () + 1;
	var.val_.ui = unescape_char (p);
	if (p != &v.back ())
		invalid_char_const ();
	return var;
}

Variant Variant::make_wchar (const string& v)
{
	assert (v.size () > 3);
	assert (v [0] == 'L');
	assert (v [1] == '\'');
	assert (v.back () == '\'');
	Variant var (BasicType::WCHAR);
	const char* p = &v.front () + 2;
	var.val_.ui = unescape_wchar (p);
	if (p != &v.back ())
		invalid_char_const ();
	return var;
}

Variant Variant::make_uint (const string& v)
{
	size_t idx;
	unsigned long long ull = stoull (v, &idx, 0);
	if (idx != v.size ())
		throw runtime_error ("Invalid integer constant.");
	Variant var;
	if (ull > numeric_limits <uint32_t>::max ())
		var = Variant (BasicType::ULONGLONG);
	else
		var = Variant (BasicType::ULONG);
	var.val_.ui = ull;
	return var;
}

Variant Variant::make_double (const string& v)
{
	size_t idx;
	double d = stod (v, &idx);
	if (idx != v.size ())
		throw runtime_error ("Invalid float constant.");
	Variant var (BasicType::DOUBLE);
	var.val_.d = d;
	return var;
}

Variant Variant::make_string (const std::string& v)
{
	assert (v.size () >= 2);
	assert (v.front () == '\"');
	assert (v.back () == '\"');
	const char* p = &v.front () + 1;
	string s;
	s.reserve (v.size ());
	while (p < &v.back ()) {
		char c = unescape_char (p);
		if (!c)
			throw runtime_error ("A string literal shall not contain the character \'\\0\'.");
		s += c;
	}
	Variant var (Type::make_string ());
	new (&var.val_.s) string (move (s));
	return var;
}

Variant Variant::make_wstring (const std::string& v)
{
	assert (v.size () >= 3);
	assert (v [0] == 'L');
	assert (v [1] == '\"');
	assert (v.back () == '\"');
	const char* p = &v.front () + 2;
	wstring s;
	s.reserve (v.size ());
	while (p < &v.back ()) {
		wchar_t c = unescape_wchar (p);
		if (!c)
			throw runtime_error ("A string literal shall not contain the character \'\\0\'.");
		s += c;
	}
	Variant var (Type::make_wstring ());
	new (&var.val_.ws) wstring (move (s));
	return var;
}

Variant Variant::make_fixed (const string& v)
{
	assert (!v.empty ());
	assert (v.back () == 'd' || v.back () == 'D');
	size_t idx = v.find ('.');
	uint16_t digits, scale;
	if (idx == string::npos) {
		digits = (uint16_t)(v.length () - 1);
		scale = 0;
	} else {
		digits = (uint16_t)(v.length () - 2);
		scale = digits - (uint16_t)idx;
	}

	double d = stod (v, &idx);
	if (idx != v.size () - 1)
		throw runtime_error ("Invalid fixed constant.");
	Variant var (Type::make_fixed (digits, scale));
	var.val_.fixed = d;
	return var;
}

Variant operator | (const Variant& l, const Variant& r)
{
	// If any of arguments have type VOID, we have a processed error.
	if (l.kind () == Type::Kind::VOID || r.kind () == Type::Kind::VOID)
		return Variant ();

	const Type& tl = l.dereference (), &tr = r.dereference ();
	if (tl.kind () != Type::Kind::BASIC_TYPE || tr.kind () != Type::Kind::BASIC_TYPE)
		throw runtime_error ("Invalid type for operation.");

}

}
