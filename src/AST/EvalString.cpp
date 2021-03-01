/// \file EvalString.cpp String expression evaluiator.
#include "EvalString.h"
#include <stdexcept>

using namespace std;

namespace AST {

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

Variant EvalString::cast (const Type& t, Variant&& v, unsigned line)
{
	assert (t.kind () == Type::Kind::STRING);
	assert (v.kind () == Type::Kind::VOID || v.kind () == Type::Kind::STRING);
	return move (v);
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

Variant EvalWString::cast (const Type& t, Variant&& v, unsigned line)
{
	assert (t.kind () == Type::Kind::WSTRING);
	assert (v.kind () == Type::Kind::VOID || v.kind () == Type::Kind::WSTRING);
	return move (v);
}

}
