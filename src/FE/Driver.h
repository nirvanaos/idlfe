/*
* Nirvana IDL front-end library.
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
#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder/Builder.h"
#include <iostream>

namespace FE {

/// The proxy class between Flex, Bison and `AST::Builder`.
class Driver :
	public yyFlexLexer,
	public AST::Build::Builder
{
public:
	static AST::Ptr <AST::AST> parse (const std::string& file, std::istream& yyin)
	{
		Driver drv (file, yyin);
		return drv.parse ();
	}

	yy::location location () const
	{
		return yy::location (&file (), lineno ());
	}

	void preprocessor_directive (const char*);

private:
	/// Returns reference to FlexLexer.
	virtual Driver& driver ()
	{
		return *this;
	}

	Driver (const std::string& file, std::istream& yyin);

	AST::Ptr <AST::AST> parse ()
	{
		if (!parser_.parse ())
			return finalize ();
		else
			return AST::Ptr <AST::AST> ();
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
