#ifndef NIDL_AST_SYMBOL_H_
#define NIDL_AST_SYMBOL_H_

#include "NamedItem.h"
#include <set>

namespace AST {

class Location
{
public:
	Location (const std::string& file, unsigned line) :
		file_ (file),
		line_ (line)
	{}

	const std::string& file () const
	{
		return file_;
	}

	unsigned line () const
	{
		return line_;
	}

private:
	const std::string& file_;
	unsigned line_;
};

class Symbol
{
public:
	Symbol (const Ptr <NamedItem>& item, const Location& loc);

	const Ptr <NamedItem>& item () const
	{
		return item_;
	}

	const Location& location () const
	{
		return location_;
	}

private:
	const Ptr <NamedItem> item_;
	const Location location_;
};

bool ci_less (const std::string& l, const std::string& r);

inline
bool operator < (const Symbol& l, const std::string& r)
{
	return ci_less (l.item ()->name (), r);
}

inline
bool operator < (const std::string& l, const Symbol& r)
{
	return ci_less (l, r.item ()->name ());
}

inline
bool operator < (const Symbol& l, const Symbol& r)
{
	return ci_less (l.item ()->name (), r.item ()->name ());
}

typedef std::set <Symbol, std::less <>> Symbols;

}

#endif
