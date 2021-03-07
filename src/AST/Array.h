#ifndef NIDL_AST_ARRAY_H_
#define NIDL_AST_ARRAY_H_

#include "Type.h"
#include <vector>

namespace AST {

/// The array type.
class Array :
	public Type
{
public:
	/// Returns the vector of array dimensions.
	const std::vector <Dim>& dimensions () const
	{
		return dimensions_;
	}

	/// \internal

	Array (const Type& type, std::vector <Dim>&& dimensions) :
		Type (type),
		dimensions_ (std::move (dimensions))
	{}

private:
	std::vector <Dim> dimensions_;
	/// \endinternal
};

}

#endif
