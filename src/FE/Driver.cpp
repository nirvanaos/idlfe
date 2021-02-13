#include "Driver.h"
#include <assert.h>

using namespace std;

Driver::Driver (const std::string& file, std::istream& yyin) :
	yyFlexLexer (yyin, cout),
	builder_ (file, cout),
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
						builder_.file (string (s + 1, end - s - 1));
					else
						builder_.parser_error (line, string ("Invalid file name: ") + string (s, end - s));
				}
				yylineno = l;
			}
			return;
		}
	}
	builder_.parser_error (line, string ("Invalid preprocessor directive: ") + dir);
}

void Driver::pragma (const char* s, unsigned line)
{
	assert (false); // TODO: Implement
}
