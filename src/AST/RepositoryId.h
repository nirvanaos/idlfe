/// \file RepositoryId
#ifndef NIDL_AST_REPOSITORYID_H_
#define NIDL_AST_REPOSITORYID_H_

#include "Location.h"
#include <stdint.h>
#include <map>

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
	RepositoryIdData (const std::string& prefix) :
		prefix_or_id_ (prefix)
	{
		version_.major = 1;
		version_.minor = 0;
	}

	std::string prefix_or_id_;
	Version version_;

	enum
	{
		EXPLICIT_ID,
		EXPLICIT_PREFIX,
		EXPLICIT_VERSION,

		EXPLICIT_SPECIFICATIONS
	};

	Location explicit_ [EXPLICIT_SPECIFICATIONS];
};

/// Items which have repository ids derives from this class.
class RepositoryId :
	public RepositoryIdData
{
public:
	/// \returns The repository id.
	std::string repository_id () const;

	/// \returns The NamedItem.
	const NamedItem& item () const
	{
		return item_;
	}

	/// \internal
	/// 
	static RepositoryId* cast (NamedItem* item) noexcept;

	static const RepositoryId* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	bool check_prefix (Build::Builder& builder, const Location& loc) const;

	void type_id (Build::Builder& builder, const std::string& id, const Location& loc);

	void pragma_version (Build::Builder& builder, const Version v, const Location& loc);

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc);

	bool check_unique (Build::Builder& builder, std::map <std::string, const NamedItem*>& ids) const;

protected:
	RepositoryId (const NamedItem& item, const Build::Builder& builder);

private:
	const NamedItem& item_;
	/// \endinternal
};

}

#endif
