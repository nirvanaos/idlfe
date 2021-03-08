#include "../include/AST/Symbols.h"
#include "../include/AST/RepositoryId.h"
#include "../include/AST/ItemScope.h"
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

pair <bool, const Ptr <NamedItem>*> Symbols::find (Build::Builder&, const std::string& name, const Location&) const
{
	auto p = find (name);
	if (p)
		return make_pair (true, p);
	else
		return make_pair (false, nullptr);
}

const Ptr <NamedItem>* Symbols::find (const std::string& name) const
{
	auto f = Base::find (name);
	if (f != end ())
		return &*f;
	else
		return nullptr;
}

void Symbols::check_rep_ids_unique (Build::Builder& builder, map <string, const NamedItem*>& ids) const
{
	for (auto it = begin (); it != end (); ++it) {
		NamedItem* item = *it;
		const RepositoryId* rid = RepositoryId::cast (item);
		if (rid)
			rid->check_unique (builder, ids);
		const ItemScope* child = ItemScope::cast (item);
		if (child)
			child->check_rep_ids_unique (builder, ids);
	}
}

}
