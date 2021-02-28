#ifndef NIDL_AST_ARRAY_H_
#define NIDL_AST_ARRAY_H_

#include "Type.h"
#include <vector>

namespace AST {

class Array
{
public:
	Array (const Type& type, std::vector <Dim>&& dimensions) :
		type_ (type),
		dimensions_ (std::move (dimensions))
	{}

	const std::vector <Dim>& dimensions () const
	{
		return dimensions_;
	}

private:
	Type type_;
	std::vector <Dim> dimensions_;
};

}

#endif
