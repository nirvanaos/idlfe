#include "../include/AST/Container.h"

namespace AST {

void Container::append (Item* item)
{
	Base::emplace_back (item);
}

}
