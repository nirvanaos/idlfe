#ifndef NIDL_AST_SYMBOLS_H_
#define NIDL_AST_SYMBOLS_H_

#include "NamedItem.h"
#include <set>
#include <map>

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
	virtual std::pair <bool, const Ptr <NamedItem>*> find (Build::Builder& builder, const std::string& name, const Location&) const;

	const Ptr <NamedItem>* find (const std::string& name) const;

	// Check for repository is uniquness
	void check_rep_ids_unique (Build::Builder& builder, std::map <std::string, const NamedItem*>& ids) const;
};

}

#endif
