#ifndef NIDL_AST_SEQUENCE_H_
#define NIDL_AST_SEQUENCE_H_

#include "Type.h"

namespace AST {

class Sequence
{
public:
	Sequence (const Type& type, Dim size = 0) :
		type_ (type),
		size_ (size)
	{}

private:
	Type type_;
	Dim size_;
};

}

#endif
