#ifndef NIDL_AST_AST_H_
#define NIDL_AST_AST_H_

#include "Container.h"
#include <string>

namespace AST {

class AST :
	public Container
{
public:
	AST (const AST&) = default;
	AST (AST&&) = default;

	AST& operator = (const AST&) = default;
	AST& operator = (AST&&) = default;

	const std::string& file () const
	{
		return file_;
	}

protected:
	AST (const char* file) :
		file_ (file)
	{}

private:
	const std::string file_;
};

}

#endif
