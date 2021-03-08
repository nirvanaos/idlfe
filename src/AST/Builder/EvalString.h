/*
* Nirvana IDL Front End Library.
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
#ifndef NIDL_AST_EVALSTRING_H_
#define NIDL_AST_EVALSTRING_H_

#include "Eval.h"

namespace AST {
namespace Build {

class EvalStringBase : public Eval
{
public:
	EvalStringBase (Builder& builder) :
		Eval (builder)
	{}

protected:
	[[noreturn]] static void throw_char0 ();
	void error_length (const Location& loc) const;
};

/// String evaluator

class EvalString : public EvalStringBase
{
public:
	EvalString (Builder& builder) :
		EvalStringBase (builder)
	{}

	virtual Variant literal_string (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

/// Wide string evaluator

class EvalWString : public EvalStringBase
{
public:
	EvalWString (Builder& builder) :
		EvalStringBase (builder)
	{}

	virtual Variant literal_wstring (const std::string& s, const Location& loc, const Variant* append);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}
}

#endif
