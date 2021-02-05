#ifndef NIDL_AST_SYMBOLS_H_
#define NIDL_AST_SYMBOLS_H_

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

	std::string stringize () const;

private:
	const std::string& file_;
	unsigned line_;
};

class Symbol
{
public:
	Symbol (const Ptr <NamedItem>& item, const Location& loc);

	bool operator < (const Symbol& rhs) const
	{
		return item_->name () < rhs.item_->name ();
	}

	const Ptr <NamedItem>& item () const
	{
		return item_;
	}

	const Location& location () const
	{
		return location_;
	}

private:
	Ptr <NamedItem> item_;
	Location location_;
};

typedef std::set <Symbol> Symbols;

}

#endif
