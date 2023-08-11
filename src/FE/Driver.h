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
#ifndef IDLFE_FE_DRIVER_H_
#define IDLFE_FE_DRIVER_H_
#pragma once

#ifdef _MSC_BUILD
#pragma warning (push)
#pragma warning (disable:4065)
#endif

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#ifdef _MSC_BUILD
#pragma warning (pop)
#endif

#include "../include/AST/Builder.h"

#include "../include/IDL_FrontEnd.h"
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
	public AST::Builder
{
public:
	static AST::Ptr <AST::Root> parse (IDL_FrontEnd& compiler, const std::string& file, std::istream& yyin)
	{
		Driver drv (compiler, file, yyin);
		return drv.parse ();
	}

	yy::location location () const
	{
		return yy::location (&file (), lineno ());
	}

	void preprocessor_directive (const char*);

	void pragma (const char* s, const AST::Location& loc)
	{
		AST::Builder::pragma (s, loc);
	}

	void parser_error (const yy::location& loc, const std::string& msg);

	AST::Type lookup_type (const AST::ScopedName& scoped_name)
	{
		return AST::Builder::lookup_type (scoped_name);
	}

	void eval_push (const AST::Type& t, const AST::Location& loc)
	{
		AST::Builder::eval_push (t, loc);
	}

	void eval_pop ()
	{
		AST::Builder::eval_pop ();
	}

	AST::Build::Eval& eval () const
	{
		return AST::Builder::eval ();
	}

	unsigned positive_int (const AST::Variant& v, const AST::Location& loc)
	{
		return AST::Builder::positive_int (v, loc);
	}

	AST::Type fixed_pt_type (unsigned digits, unsigned scale, const AST::Location& loc)
	{
		return AST::Builder::fixed_pt_type (digits, scale, loc);
	}

	void interface_end ()
	{
		AST::ItemScope* itf = cur_parent ();
		AST::Builder::interface_end ();
		if (itf) {
			assert (itf->kind () == AST::Item::Kind::INTERFACE);
			compiler_.interface_end (static_cast <AST::Interface&> (*itf), *this);
		}
	}

private:
	/// Returns reference to FlexLexer.
	virtual Driver& driver ()
	{
		return *this;
	}

	Driver (IDL_FrontEnd& compiler, const std::string& file, std::istream& yyin);

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
