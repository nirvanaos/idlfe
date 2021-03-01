#include "Eval.h"
#include "Builder.h"
#include "Constant.h"
#include "SafeInt/SafeInt.hpp"
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
			invalid_constant_type (constant);
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
	invalid_operation (line);
	return Variant ();
}

Variant Eval::expr (char op, const Variant& v, unsigned line)
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

[[noreturn]] void Eval::overflow ()
{
	throw overflow_error ("Overflow.");
}

[[noreturn]] void Eval::zero_divide ()
{
	throw runtime_error ("Divide by zero.");
}

// Boolean evaluator.

Variant EvalBool::literal_boolean (bool v, unsigned line)
{
	return Variant (v);
}

Variant EvalBool::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference ();
		if (t.kind () == Type::Kind::BASIC_TYPE && t.basic_type () == BasicType::BOOLEAN)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalBool::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		bool lv = l.as_bool (), rv = r.as_bool (), ret;
		switch (op) {
			case '|':
				ret = lv || rv;
				break;
			case '^':
				ret = lv != rv;
				break;
			case '&':
				ret = lv && rv;
				break;
			default:
				invalid_operation (line);
				return Variant ();
		}
		return ret;
	}
	return Variant ();
}

Variant EvalBool::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		if ('~' == op)
			return !v.as_bool ();
		else
			invalid_operation (line);
	}
	return Variant ();
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
		uint64_t ull = stoull (s, &idx, 0);
		if (idx != s.size ())
			throw runtime_error ("Invalid integer constant.");
		if (ull > numeric_limits <uint32_t>::max ())
			return Variant (ull);
		else
			return Variant ((uint32_t)ull);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->is_integer ())
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalLong::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.is_integer () && r.is_integer ());
		try {
			if (
				l.basic_type () == BasicType::LONGLONG || r.basic_type () == BasicType::LONGLONG
				||
				l.basic_type () == BasicType::ULONGLONG || r.basic_type () == BasicType::ULONGLONG
			) {
				Variant ll = EvalLongLong (builder_).expr (l, op, r, line);
				if (l.is_signed ())
					return ll.to_long ();
				else
					return ll.to_unsigned_long ();
			} else {
				if (l.is_signed ()) {
					int32_t lv = l.to_long (), ret = 0;
					if ('>' == op || '<' == op) {
						uint32_t shift = r.to_unsigned_long ();
						if (shift > 32)
							throw range_error ("Shift size is too large.");
						ret = op == '>' ? lv >> shift : lv << shift;
					} else {
						int32_t rv = r.to_long ();
						switch (op) {
							case '|':
								ret = lv | rv;
								break;
							case '^':
								ret = lv ^ rv;
								break;
							case '&':
								ret = lv & rv;
								break;
							case '+':
								if (SafeAdd (lv, rv, ret))
									overflow ();
								break;
							case '-':
								if (SafeSubtract (lv, rv, ret))
									overflow ();
								break;
							case '*':
								if (SafeMultiply (lv, rv, ret))
									overflow ();
								break;
							case '/':
								if (SafeDivide (lv, rv, ret))
									zero_divide ();
								break;
							case '%':
								if (SafeModulus (lv, rv, ret))
									zero_divide ();
								break;
							default:
								invalid_operation (line);
								return Variant ();
						}
					}
					return ret;
				} else {
					uint32_t lv = l.to_unsigned_long (), ret = 0;
					if ('>' == op || '<' == op) {
						uint32_t shift = r.to_unsigned_long ();
						if (shift > 32)
							throw range_error ("Shift size is too large.");
						ret = op == '>' ? lv >> shift : lv << shift;
					} else {
						uint32_t rv = r.to_unsigned_long ();
						switch (op) {
							case '|':
								ret = lv | rv;
								break;
							case '^':
								ret = lv ^ rv;
								break;
							case '&':
								ret = lv & rv;
								break;
							case '+':
								if (SafeAdd (lv, rv, ret))
									overflow ();
								break;
							case '-':
								if (SafeSubtract (lv, rv, ret))
									overflow ();
								break;
							case '*':
								if (SafeMultiply (lv, rv, ret))
									overflow ();
								break;
							case '/':
								if (SafeDivide (lv, rv, ret))
									zero_divide ();
								break;
							case '%':
								if (SafeModulus (lv, rv, ret))
									zero_divide ();
								break;
							default:
								invalid_operation (line);
								return Variant ();
						}
					}
					return ret;
				}
			}
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalLong::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_integer ());
		try {
			if (v.is_signed ()) {
				int32_t i = v.to_long (), ret = 0;
				switch (op) {
					case '-':
						ret = -i;
						break;
					case '+':
						ret = i;
						break;
					case '~':
						ret = -(i + 1);
						break;
					default:
						invalid_operation (line);
						return Variant ();
				}
			} else {
				uint32_t u = v.to_unsigned_long ();
				switch (op) {
					case '-': {
							int32_t i;
							if (SafeCast (u, i))
								overflow ();
							return -i;
						}
						break;
					case '+':
						return u;
						break;
					case '~':
						return numeric_limits <uint32_t>::max () - u;;
						break;
					default:
						invalid_operation (line);
						return Variant ();
				}
			}
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
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
			v += append->as_string ();
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
			v += append->as_wstring ();
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
