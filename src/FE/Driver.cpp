#include "Driver.h"
#include <assert.h>

using namespace std;

namespace FE {

Driver::Driver (const std::string& file, std::istream& yyin) :
	yyFlexLexer (yyin, cout),
	AST::Builder (file, cout),
	parser_ (*this)
{}

void Driver::preprocessor_directive (const char* const dir, unsigned line)
{
	const char* s = dir;
	assert (*s == '#');
	++s;
	while (isspace (*s))
		++s;
	if (*s) {
		if (!strncmp (s, "line", 4)) {
			s += 4;
			char* end;
			unsigned long l = strtoul (s, &end, 10);
			if (l > 0) {
				s = end;
				while (isspace (*s))
					++s;
				if (*s == '\"') {
					const char* end = s + strlen (s);
					while (isspace (*(--end)))
						;
					if (*end == '\"')
						file (string (s + 1, end - s - 1), line);
					else
						parser_error (line, string ("Invalid file name: ") + string (s, end - s));
				}
				yylineno = l;
			}
			return;
		}
	}
	parser_error (line, string ("Invalid preprocessor directive: ") + dir);
}

}
