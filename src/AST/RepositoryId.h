#ifndef NIDL_AST_REPOSITORYID_H_
#define NIDL_AST_REPOSITORYID_H_

#include "Location.h"
#include <stdint.h>

namespace AST {

namespace Build {
class Builder;
}

class NamedItem;

struct Version
{
	uint16_t major, minor;
};

class RepositoryIdData
{
protected:
	enum class Definition
	{
		PREFIX,
		ID,
		VERSION
	};

	RepositoryIdData (const std::string& prefix) :
		definition_ (Definition::PREFIX),
		prefix_or_id_ (prefix)
	{
		version_.major = 1;
		version_.minor = 0;
	}

	Definition definition_;
	std::string prefix_or_id_;
	Version version_;
	Location pragma_loc_;
};

class RepositoryId :
	public RepositoryIdData
{
public:
	static RepositoryId* cast (NamedItem* item) noexcept;

	const NamedItem& item () const
	{
		return item_;
	}

	bool check_prefix (Build::Builder& builder, const Location& loc) const;

	void pragma_id (Build::Builder& builder, const std::string& id, const Location& loc);

	void pragma_version (Build::Builder& builder, const Version v, const Location& loc);

	std::string repository_id () const;

protected:
	RepositoryId (const NamedItem& item, const Build::Builder& builder);

private:
	static void see_prev_declaration (Build::Builder& builder, const Location& loc);

private:
	const NamedItem& item_;
};

}

#endif
