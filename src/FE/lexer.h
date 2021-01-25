#ifndef NIDL_LEXER_H_
#define NIDL_LEXER_H_

#include <FlexLexer.h>
#include <iostream>

extern yyFlexLexer lexer;

inline int yylex ()
{
	return lexer.yylex ();
}

inline void yyerror (const char* msg)
{
	std::cerr << msg << std::endl;
}

#endif
