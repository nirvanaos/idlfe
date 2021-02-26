#ifndef NIDL_AST_SEQUENCE_H_
#define NIDL_AST_SEQUENCE_H_

#include "Type.h"

namespace AST {

class Sequence
{
public:
	Sequence (const Type& type) :
		type_ (type),
		size_ (0)
	{}

private:
	Type type_;
	Dim size_;
};

}

#endif
