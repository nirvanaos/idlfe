#ifndef NIDL_AST_DECLARATORS_H_
#define NIDL_AST_DECLARATORS_H_

#include <string>
#include <vector>
#include "Location.h"
#include <forward_list>

namespace AST {

typedef std::forward_list <unsigned> FixedArraySizes;

class Declarator : 
	public std::string,
	public Location
{
public:
	Declarator ()
	{}

	Declarator (const std::string& name, const Location& loc, const FixedArraySizes& array = FixedArraySizes ()) :
		std::string (name),
		Location (loc),
		array_ (array)
	{}

	Declarator (const Declarator&) = default;
	Declarator (Declarator&&) = default;

	Declarator& operator = (const Declarator&) = default;
	Declarator& operator = (Declarator&&) = default;

private:
	FixedArraySizes array_;
};

typedef std::forward_list <Declarator> Declarators;

}

#endif
