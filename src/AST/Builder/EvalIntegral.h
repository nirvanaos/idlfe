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
#ifndef NIDL_AST_EVALINTEGRAL_H_
#define NIDL_AST_EVALINTEGRAL_H_

#include "Eval.h"

namespace AST {
namespace Build {

/// Integral evaluator.
class EvalIntegral : public Eval
{
public:
	EvalIntegral (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_boolean (bool v, const Location& loc);
	virtual Variant literal_char (const std::string& s, const Location& loc);
	virtual Variant literal_wchar (const std::string& s, const Location& loc);
	virtual Variant literal_int (const std::string& s, const Location& loc);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);
};

}
}

#endif
