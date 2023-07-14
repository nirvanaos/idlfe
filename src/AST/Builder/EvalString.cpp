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
#include "EvalString.h"
#include "Builder.h"
#include "../../include/AST/Constant.h"
#include <stdexcept>

namespace AST {
namespace Build {

void EvalStringBase::error_length (const Location& loc) const
{
	builder_.message (loc, Builder::MessageType::ERROR, "string literal length exceedes the limit");
}

[[noreturn]] void EvalStringBase::throw_char0 ()
{
	throw std::runtime_error ("A string literal shall not contain the character \'\\0\'.");
}

// String evaluator

Variant EvalString::literal_string (const std::string& s, const Location& loc, const Variant* append)
{
	try {
		assert (s.size () >= 2);
		assert (s.front () == '\"');
		assert (s.back () == '\"');
		const char* p = &s.front () + 1;
		const char* end = &s.back ();
		std::string v;
		v.reserve (s.size ());
		while (p < end) {
			char c = unescape_char (p);
			if (!c)
				throw_char0 ();
			v += c;
		}
		if (append)
			v += append->as_string ();
		return Variant (std::move (v));
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalString::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->dereference_type ().tkind () == Type::Kind::STRING)
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
		assert (dt.tkind () == Type::Kind::STRING);
		if (dt.string_bound () && v.dereference_const ().as_string ().length () > dt.string_bound ()) {
			error_length (loc);
			return Variant ();
		}
	}
	return std::move (v);
}

// Wide string evaluator

Variant EvalWString::literal_wstring (const std::string& s, const Location& loc, const Variant* append)
{
	try {
		assert (s.size () >= 3);
		assert (s [0] == 'L');
		assert (s [1] == '\"');
		assert (s.back () == '\"');
		const char* p = &s.front () + 2;
		const char* end = &s.back ();
		std::wstring v;
		v.reserve (s.size ());
		while (p < end) {
			wchar_t c = unescape_wchar (p);
			if (!c)
				throw_char0 ();
			v += c;
		}
		if (append)
			v += append->as_wstring ();
		return Variant (std::move (v));
	} catch (const std::exception& ex) {
		error (loc, ex);
		return Variant ();
	}
}

Variant EvalWString::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->dereference_type ().tkind () == Type::Kind::WSTRING)
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
		assert (dt.tkind () == Type::Kind::WSTRING);
		if (dt.string_bound () && v.dereference_const ().as_wstring ().length () > dt.string_bound ()) {
			error_length (loc);
			return Variant ();
		}
	}
	return std::move (v);
}

}
}
