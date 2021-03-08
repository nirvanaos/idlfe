#ifndef NIDL_AST_SEQUENCE_H_
#define NIDL_AST_SEQUENCE_H_

#include "Type.h"

namespace AST {

/// Sequence type.
class Sequence :
	public Type
{
public:
	/// Returns the size limit or 0.
	Dim size () const
	{
		return size_;
	}

	// Internals

	Sequence (const Type& type, Dim size = 0) :
		Type (type),
		size_ (size)
	{}

private:
	Dim size_;
};

}

#endif
