#ifndef NIDL_AST_DECLARATORS_H_
#define NIDL_AST_DECLARATORS_H_

#include <string>
#include "Location.h"
#include <forward_list>

namespace AST {

class SimpleDeclarator :
	public std::string,
	public Location
{
public:
	SimpleDeclarator ()
	{}

	SimpleDeclarator (const std::string& name, const Location& loc) :
		std::string (name),
		Location (loc)
	{}

	SimpleDeclarator (const SimpleDeclarator&) = default;
	SimpleDeclarator (SimpleDeclarator&&) = default;

	SimpleDeclarator& operator = (const SimpleDeclarator&) = default;
	SimpleDeclarator& operator = (SimpleDeclarator&&) = default;
};

typedef std::forward_list <SimpleDeclarator> SimpleDeclarators;

typedef std::forward_list <unsigned> FixedArraySizes;

class Declarator : 
	public SimpleDeclarator
{
public:
	Declarator ()
	{}

	Declarator (const std::string& name, const Location& loc, const FixedArraySizes& array) :
		SimpleDeclarator (name, loc),
		array_ (array)
	{}

	Declarator (const std::string& name, const Location& loc) :
		SimpleDeclarator (name, loc)
	{}

	Declarator (const SimpleDeclarator& decl) :
		SimpleDeclarator (decl)
	{}

	Declarator (SimpleDeclarator&& decl) :
		SimpleDeclarator (std::move (decl))
	{}

	Declarator (const Declarator&) = default;
	Declarator (Declarator&&) = default;

	Declarator& operator = (const Declarator&) = default;
	Declarator& operator = (Declarator&&) = default;

	const FixedArraySizes& array_sizes () const
	{
		return array_;
	}

private:
	FixedArraySizes array_;
};

typedef std::forward_list <Declarator> Declarators;

}

#endif
