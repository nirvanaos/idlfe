#include "Interface.h"

using namespace std;

namespace AST {

const char* InterfaceKind::interface_kind_name () const
{
	switch (kind_) {
		case Kind::ABSTRACT:
			return "abstract";
		case Kind::LOCAL:
			return "local";
		default:
			return "";
	}
}

void Interface::get_all_interfaces (std::vector <const Interface*>& all) const
{
	all.reserve (all.size () + bases_.size () + 1);
	all.push_back (this);
	all.insert (all.end (), bases_.begin (), bases_.end ());
	for (const Interface* base : bases_)
		base->get_all_interfaces (all);
}

void Interface::get_all_bases (std::set <const Interface*>& bases) const
{
	for (const Interface* base : bases_) {
		if (bases.insert (base).second)
			base->get_all_bases (bases);
	}
}

}
