#include "Eval.h"
#include "Builder.h"
extern "C" {
#include <decNumber/decNumber.h>
}

using namespace std;

namespace AST {

void Eval::invalid_literal_type (unsigned line) const
{
	builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, "Invalid literal type.");
}

void Eval::invalid_operation (unsigned line) const
{
	builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, "Invalid operation.");
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

Variant Eval::constant (const Ptr <NamedItem>* constant, unsigned line)
{
	if (constant) {
		const NamedItem* item = *constant;
		if (item->kind () != Item::Kind::CONSTANT)
			builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, item->name () + " is not a constant.");
		else
			builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, "Invalid constant type.");
		builder_.message (**constant, Builder::MessageType::MESSAGE, string ("See definition of ") + item->name () + ".");
	}
	return Variant ();
}

Variant Eval::expr_or (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_xor (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_and (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_shift_right (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_shift_left (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_add (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_sub (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_mul (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_div (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_rem (const Variant& l, const Variant& r, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_minus (const Variant& v, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_plus (const Variant& v, unsigned line)
{
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr_tilde (const Variant& v, unsigned line)
{
	invalid_operation (line);
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

// Integer evaluator

Variant EvalLong::literal_char (const string& s, unsigned line)
{
	try {
		assert (s.size () > 2);
		assert (s.front () == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 1;
		char v = unescape_char (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::literal_wchar (const string& s, unsigned line)
{
	try {
		assert (s.size () > 3);
		assert (s [0] == 'L');
		assert (s [1] == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 2;
		wchar_t v = unescape_wchar (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::literal_int (const string& s, unsigned line)
{
	try {
		size_t idx;
		unsigned long long ull = stoull (s, &idx, 0);
		if (idx != s.size ())
			throw runtime_error ("Invalid integer constant.");
		if (ull > numeric_limits <uint32_t>::max ())
			return Variant (ull);
		else
			return Variant ((unsigned long)ull);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

// Long integer evaluator

// Double evaluator

Variant EvalDouble::literal_float (const string& s, unsigned line)
{
	try {
		size_t idx;
		double d = stod (s, &idx);
		if (idx != s.size ())
			throw runtime_error ("Invalid floating-point constant.");
		return Variant (d);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

// String evaluator

Variant EvalString::literal_string (const string& s, unsigned line, const Variant* append)
{
	try {
		assert (s.size () >= 2);
		assert (s.front () == '\"');
		assert (s.back () == '\"');
		const char* p = &s.front () + 1;
		const char* end = &s.back ();
		string v;
		v.reserve (s.size ());
		while (p < end) {
			char c = unescape_char (p);
			if (!c)
				throw runtime_error ("A string literal shall not contain the character \'\\0\'.");
			v += c;
		}
		if (append)
			v += append->str ();
		return Variant (move (v));
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

// Wide string evaluator

Variant EvalWString::literal_wstring (const string& s, unsigned line, const Variant* append)
{
	try {
		assert (s.size () >= 3);
		assert (s [0] == 'L');
		assert (s [1] == '\"');
		assert (s.back () == '\"');
		const char* p = &s.front () + 2;
		const char* end = &s.back ();
		wstring v;
		v.reserve (s.size ());
		while (p < end) {
			wchar_t c = unescape_wchar (p);
			if (!c)
				throw runtime_error ("A wide string literal shall not contain the character \'\\0\'.");
		}
		if (append)
			v += append->wstr ();
		return Variant (move (v));
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

// Fixed evaluator

struct EvalFixed::Context : decContext
{
	Context ()
	{
		decContextDefault (this, DEC_INIT_BASE);
	}
};

Variant EvalFixed::literal_fixed (const string& s, unsigned line)
{
	assert (!s.empty ());
	assert (s.back () == 'd' || s.back () == 'D');

	decNumber v;
	Context ctx;
	decNumberFromString (&v, s.substr (0, s.length () - 1).c_str (), &ctx);
	if (DEC_CLASS_QNAN == decNumberClass (&v, &ctx)) {
		builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, string ("Fixed literal ") + s + " can not be converted.");
		return Variant ();
	} else
		return Variant (v);
}

}
