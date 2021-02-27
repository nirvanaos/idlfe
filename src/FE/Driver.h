#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder.h"
#include <iostream>

/// The proxy class between Flex/Bison and `AST::Builder`.
class Driver :
	public yyFlexLexer,
	public AST::Builder
{
public:
	Driver (const std::string& file, std::istream& yyin);

	int parse ()
	{
		return parser_.parse ();
	}

	void preprocessor_directive (const char*, unsigned line);

private:
	yy::parser parser_;
};

inline yy::parser::symbol_type yylex (Driver& drv)
{
	return drv.yylex ();
}

#endif
