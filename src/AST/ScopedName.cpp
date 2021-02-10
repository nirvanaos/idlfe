#include "ScopedName.h"

namespace AST {

ScopedName::ScopedName (bool root, const std::string& name) :
	from_root (root)
{
	push_back (name);
}

}
