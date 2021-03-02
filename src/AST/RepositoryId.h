#ifndef NIDL_AST_REPOSITORYID_H_
#define NIDL_AST_REPOSITORYID_H_

#include "Location.h"
#include <stdint.h>

namespace AST {

class Builder;
class NamedItem;

struct Version
{
	uint16_t major, minor;
};

class RepositoryId
{
	enum class Definition
	{
		PREFIX,
		ID,
		VERSION
	};

public:
	static RepositoryId* cast (NamedItem* item) noexcept;

	RepositoryId () :
		definition_ (Definition::PREFIX)
	{
		version_.major = 1;
		version_.minor = 0;
	}

	bool check_prefix (Builder& builder, const NamedItem& item, const Location& loc) const;

	void pragma_id (Builder& builder, const std::string& id, const Location& loc);

	void pragma_version (Builder& builder, const Version v, const Location& loc);

	std::string repository_id (const NamedItem& item);

private:
	Definition definition_;
	std::string prefix_or_id_;
	Version version_;
	Location pragma_loc_;
};

}

#endif
