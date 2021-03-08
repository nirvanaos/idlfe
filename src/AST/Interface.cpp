#include "Interface.h"
#include "Builder/Builder.h"

using namespace std;

namespace AST {

using namespace Build;

const char* InterfaceKind::interface_kind_name () const noexcept
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

void Interface::get_all_interfaces (vector <const Interface*>& all) const
{
	all.reserve (all.size () + bases_.size () + 1);
	all.push_back (this);
	all.insert (all.end (), bases_.begin (), bases_.end ());
	for (const Interface* base : bases_)
		base->get_all_interfaces (all);
}

void Interface::get_all_bases (set <const Interface*>& bases) const
{
	for (const Interface* base : bases_) {
		if (bases.insert (base).second)
			base->get_all_bases (bases);
	}
}

pair <bool, const Ptr <NamedItem>*> Interface::find (Builder& builder, const string& name, const Location& loc) const
{
	const Ptr <NamedItem>* p = ItemScope::find (name);
	if (p)
		return make_pair (true, p);

	set <const Ptr <NamedItem>*> found;
	for (auto base = bases_.begin (); base != bases_.end (); ++base) {
		(*base)->base_find (name, found);
	}

	if (found.empty ())
		return make_pair (false, nullptr);

	if (found.size () > 1) {
		// Ambiguous
		builder.message (loc, Builder::MessageType::ERROR, string ("Ambiguous name ") + name + '.');
		auto it = found.begin ();
		const Ptr <NamedItem>* p = *it;
		builder.message (**p, Builder::MessageType::MESSAGE, string ("Could be ") + (*p)->qualified_name ());
		++it;
		for (;;) {
			p = *it;
			string msg = string ("or ") + (*p)->qualified_name ();
			if (found.end () == ++it) {
				msg += '.';
				builder.message (**p, Builder::MessageType::MESSAGE, msg);
				break;
			} else
				builder.message (**p, Builder::MessageType::MESSAGE, msg);
		}
		return make_pair (true, nullptr);
	} else
		return make_pair (true, *found.begin ());
}

void Interface::base_find (const std::string& name, set <const Ptr <NamedItem>*>& found) const
{
	const Ptr <NamedItem>* p = ItemScope::find (name);
	if (p) {
		found.insert (p);
	} else {
		for (auto base = bases_.begin (); base != bases_.end (); ++base) {
			(*base)->base_find (name, found);
		}
	}
}

}
