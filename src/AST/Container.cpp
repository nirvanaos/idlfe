#include "Container.h"

namespace AST {

void Container::append (Item* item)
{
	Base::emplace_back (item);
}

}
