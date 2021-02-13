#include "Symbols.h"
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

std::pair <Symbols::iterator, bool> Symbols::insert (NamedItem* item)
{
	return Base::insert (item);
}

const Symbols::iterator Symbols::find (const std::string& name) const
{
	return Base::find (name);
}

}
