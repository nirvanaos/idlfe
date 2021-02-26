#ifndef NIDL_AST_ARRAY_H_
#define NIDL_AST_ARRAY_H_

#include "Type.h"
#include <vector>

namespace AST {

class Array
{
public:
	Array (const Type& type) :
		type_ (type)
	{}

private:
	Type type_;
	std::vector <Dim> dimensions_;
};

}

#endif
