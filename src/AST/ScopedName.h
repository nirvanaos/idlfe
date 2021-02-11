#ifndef NIDL_AST_SCOPEDNAME_H_
#define NIDL_AST_SCOPEDNAME_H_

#include <string>
#include <vector>

namespace AST {

struct ScopedName : std::vector <std::string>
{
	ScopedName () {}
	ScopedName (bool root, const std::string& name);

	ScopedName (const ScopedName&) = default;
	ScopedName (ScopedName&&) = default;

	ScopedName& operator = (const ScopedName&) = default;
	ScopedName& operator = (ScopedName&&) = default;

	std::string stringize () const;

	bool from_root;
};

}

#endif
