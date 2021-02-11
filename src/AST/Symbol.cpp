#include "Symbol.h"
#include <algorithm>

using namespace std;

namespace AST {

inline bool ci_compare (char l, char r)
{
	return std::tolower (l) < std::tolower (r);
}

bool ci_less (const string& l, const string& r)
{
	return lexicographical_compare (l.begin (), l.end (), r.begin (), r.end (), ci_compare);
}

Symbol::Symbol (const Ptr <NamedItem>& item, const Location& loc) :
	item_ (item),
	location_ (loc)
{}

}
