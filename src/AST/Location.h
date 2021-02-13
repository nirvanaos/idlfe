#ifndef NIDL_AST_LOCATION_H_
#define NIDL_AST_LOCATION_H_

#include <string>

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

private:
	const std::string& file_;
	const unsigned line_;
};

}

#endif
