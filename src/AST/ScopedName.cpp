#include "ScopedName.h"

using namespace std;

namespace AST {

ScopedName::ScopedName (bool root, const std::string& name) :
	from_root (root)
{
	push_back (name);
}

string ScopedName::stringize () const
{
	const_iterator name = begin ();
	string ret;
	if (!from_root) {
		ret = *name;
		++name;
	}
	for (; name != end (); ++name) {
		ret += "::";
		ret += *name;
	}

	return ret;
}

}
