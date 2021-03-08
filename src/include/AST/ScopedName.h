#ifndef NIDL_AST_SCOPEDNAME_H_
#define NIDL_AST_SCOPEDNAME_H_

#include <string>
#include <vector>
#include <forward_list>
#include "Location.h"

namespace AST {

struct ScopedName : 
	std::vector <std::string>,
	Location
{
	ScopedName () : 
		from_root (false)
	{}

	ScopedName (const Location& loc) :
		Location (loc),
		from_root (false)
	{}

	ScopedName (const Location& loc, bool root, const std::string& name);

	ScopedName (const ScopedName&) = default;
	ScopedName (ScopedName&&) = default;

	ScopedName& operator = (const ScopedName&) = default;
	ScopedName& operator = (ScopedName&&) = default;

	std::string stringize () const;

	bool from_root;
};

typedef std::forward_list <ScopedName> ScopedNames;

}

#endif
