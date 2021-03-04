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
		if (!parser_.parse () && !err_cnt ())
			return tree ();
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
