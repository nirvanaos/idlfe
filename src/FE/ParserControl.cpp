#include "ParserControl.h"

using namespace std;

ParserControl::ParserControl (const char* file, std::istream& yyin) :
	yyFlexLexer (yyin, cout),
	builder_ (file)
{}
