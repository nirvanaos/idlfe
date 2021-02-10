#ifndef NIDL_AST_SCOPEDNAME_H_
#define NIDL_AST_SCOPEDNAME_H_

#include <string>
#include <vector>

namespace AST {

struct ScopedName : std::vector <std::string>
{
	ScopedName () {}
	ScopedName (bool root, const std::string& name);

	bool from_root;
};

}

#endif
