#include "Driver.h"

using namespace std;

Driver::Driver (const std::string& file, std::istream& yyin) :
	yyFlexLexer (yyin, cout),
	builder_ (file, cout),
	parser_ (*this)
{}

void Driver::preprocessor_directive (const char* s, size_t cc)
{

}
