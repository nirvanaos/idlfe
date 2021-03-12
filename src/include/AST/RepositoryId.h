/// \file RepositoryId
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
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
