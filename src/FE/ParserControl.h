#ifndef NIDL_PARSERCONTROL_H_
#define NIDL_PARSERCONTROL_H_

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include "../AST/Builder.h"
#include <iostream>

class ParserControl :
	public yyFlexLexer
{
public:
	ParserControl (const char* file, std::istream& yyin);

//	int yylex (YYSTYPE* lvalp, YYLTYPE* llocp);
	void preprocessor_directive (const char*);

private:
	AST::Builder builder_;
};

#define yyFlexLexer yyFlexLexer

/*
inline int yylex ()
{
	return lexer->yylex ();
}

inline void yyerror (const char* msg)
{
	std::cerr << msg << std::endl;
}
*/
#endif
