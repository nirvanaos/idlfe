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
#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder/Builder.h"
#include <iostream>
#include <filesystem>

inline AST::Location::Location (const yy::location& loc) noexcept :
	file_ (loc.begin.filename),
	line_ (loc.begin.line)
{}

namespace FE {

/// The proxy class between Flex, Bison and `AST::Builder`.
class Driver :
	public yyFlexLexer,
	public AST::Build::Builder
{
public:
	static AST::Ptr <AST::Root> parse (const std::string& file, std::istream& yyin, bool anonymous_deprecated)
	{
		Driver drv (file, yyin, anonymous_deprecated);
		return drv.parse ();
	}

	yy::location location () const
	{
		return yy::location (&file (), lineno ());
	}

	void preprocessor_directive (const char*);

	void parser_error (const yy::location& loc, const std::string& msg);

private:
	/// Returns reference to FlexLexer.
	virtual Driver& driver ()
	{
		return *this;
	}

	Driver (const std::string& file, std::istream& yyin, bool anonymous_deprecated);

	AST::Ptr <AST::Root> parse ()
	{
		if (!parser_.parse ())
			return finalize ();
		else
			return AST::Ptr <AST::Root> ();
	}

private:
	yy::parser parser_;
};

}

inline yy::parser::symbol_type yylex (FE::Driver& drv)
{
	return drv.yylex ();
}

#endif
