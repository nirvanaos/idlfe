#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder.h"
#include <iostream>

class Driver :
	public yyFlexLexer
{
public:
	Driver (const std::string& file, std::istream& yyin);

	int parse ()
	{
		return parser_.parse ();
	}

	void parser_error (const yy::parser::location_type& l, const std::string& msg)
	{
		builder_.parser_error (l.begin.line, msg);
	}

	void preprocessor_directive (const char*, size_t);

	void module_begin (const std::string& id, const yy::parser::location_type& l)
	{
		builder_.module_begin (id, l.begin.line);
	}

	void module_end ()
	{
		builder_.module_end ();
	}

private:
	AST::Builder builder_;
	yy::parser parser_;
};

inline yy::parser::symbol_type yylex (Driver& drv)
{
	return drv.yylex ();
}

/*

inline void yyerror (const char* msg)
{
	std::cerr << msg << std::endl;
}
*/
#endif
