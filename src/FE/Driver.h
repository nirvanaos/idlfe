#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder.h"
#include <iostream>

namespace FE {

/// The proxy class between Flex, Bison and `AST::Builder`.
class Driver :
	public yyFlexLexer,
	public AST::Builder
{
public:
	static AST::Ptr <AST::AST> parse (const std::string& file, std::istream& yyin)
	{
		Driver driver (file, yyin);
		return driver.parse ();
	}

	void preprocessor_directive (const char*, unsigned line);

private:
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
