/// \file Location.h
/// Defines the Location class.
#ifndef NIDL_AST_LOCATION_H_
#define NIDL_AST_LOCATION_H_

#include <string>
#include <assert.h>

/// Support for the Bison location.
namespace yy {
class location;
}

namespace AST {

class Location
{
public:
	Location () :
		file_ (nullptr),
		line_ (0)
	{}

	Location (const std::string& file, unsigned line) :
		file_ (&file),
		line_ (line)
	{}

	Location (const yy::location&);

	const std::string& file () const
	{
		assert (file_);
		return *file_;
	}

	unsigned line () const
	{
		return line_;
	}

private:
	const std::string* file_;
	unsigned line_;
};

}

#endif
