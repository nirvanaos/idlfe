#ifndef NIDL_AST_SYMBOLS_H_
#define NIDL_AST_SYMBOLS_H_

#include "NamedItem.h"
#include <set>

namespace AST {

bool ci_less (const std::string& l, const std::string& r);

inline
bool operator < (const Ptr <NamedItem>& l, const std::string& r)
{
	return ci_less (l->name (), r);
}

inline
bool operator < (const std::string& l, const Ptr <NamedItem>& r)
{
	return ci_less (l, r->name ());
}

inline
bool operator < (const Ptr <NamedItem>& l, const Ptr <NamedItem>& r)
{
	return ci_less (l->name (), r->name ());
}

class Symbols :
	public std::set <Ptr <NamedItem>, std::less <>>
{
	typedef std::set <Ptr <NamedItem>, std::less <>> Base;
public:
	// Methods made outline to reduce size.
	std::pair <iterator, bool> insert (NamedItem* item);
	const iterator find (const std::string& name) const;
};

}

#endif
