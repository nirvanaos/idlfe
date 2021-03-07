/// \file EvalString.cpp String expression evaluiator.
#include "EvalString.h"
#include "Builder.h"
#include "../Constant.h"
#include <stdexcept>

using namespace std;

namespace AST {
namespace Build {

void EvalStringBase::error_length (const Location& loc) const
{
	builder_.message (loc, Builder::MessageType::ERROR, "String literal length exceedes the limit.");
}

[[noreturn]] void EvalStringBase::throw_char0 ()
{
	throw runtime_error ("A string literal shall not contain the character \'\\0\'.");
}

// String evaluator

Variant EvalString::literal_string (const string& s, const Location& loc, const Variant* append)
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
				throw_char0 ();
			v += c;
		}
		if (append)
			v += append->as_string ();
		return Variant (move (v));
	} catch (const exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalString::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->dereference_type ().kind () == Type::Kind::STRING)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalString::cast (const Type& t, Variant&& v, const Location& loc)
{
	if (!v.empty ()) {
		assert (v.dereference_const ().vtype () == Variant::VT::STRING);
		const Type& dt = t.dereference_type ();
		assert (dt.kind () == Type::Kind::STRING);
		if (dt.string_size () && v.dereference_const ().as_string ().length () > dt.string_size ()) {
			error_length (loc);
			return Variant ();
		}
	}
	return move (v);
}

// Wide string evaluator

Variant EvalWString::literal_wstring (const string& s, const Location& loc, const Variant* append)
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
				throw_char0 ();
		}
		if (append)
			v += append->as_wstring ();
		return Variant (move (v));
	} catch (const exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalWString::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->dereference_type ().kind () == Type::Kind::WSTRING)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalWString::cast (const Type& t, Variant&& v, const Location& loc)
{
	if (!v.empty ()) {
		assert (v.dereference_const ().vtype () == Variant::VT::WSTRING);
		const Type& dt = t.dereference_type ();
		assert (dt.kind () == Type::Kind::WSTRING);
		if (dt.string_size () && v.dereference_const ().as_wstring ().length () > dt.string_size ()) {
			error_length (loc);
			return Variant ();
		}
	}
	return move (v);
}

}
}
