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

namespace AST {

namespace Build {
class Builder;
}

class NamedItem;

struct Version
{
	uint16_t major, minor;
};

/// Items which have repository ids derives from this class.
class RepositoryId
{
public:
	/// \returns The repository id.
	std::string repository_id () const;

	/// \returns The NamedItem.
	const NamedItem& item () const
	{
		return item_;
	}

	/// \returns RepositoryId pointer if item derives from RepositoryId.
	static const RepositoryId* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	/// \internal

	static RepositoryId* cast (NamedItem* item) noexcept;

	bool check_prefix (Build::Builder& builder, const Location& loc) const;

	void type_id (Build::Builder& builder, const std::string& id, const Location& loc);

	void pragma_version (Build::Builder& builder, const Version v, const Location& loc);

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc);

	RepositoryId& operator = (const RepositoryId& src)
	{
		data_ = src.data_;
		return *this;
	}

protected:
	RepositoryId (const NamedItem& item, const Build::Builder& builder);
	RepositoryId (const RepositoryId&) = delete;

private:
	enum
	{
		EXPLICIT_ID,
		EXPLICIT_PREFIX,
		EXPLICIT_VERSION,

		EXPLICIT_SPECIFICATIONS
	};

	struct Data
	{
		Data (const std::string& prefix) :
			prefix_or_id (prefix)
		{
			version.major = 1;
			version.minor = 0;
		}

		std::string prefix_or_id;
		Version version;

		Location explicit_ [EXPLICIT_SPECIFICATIONS];
	};

	const NamedItem& item_;
	Data data_;
	/// \endinternal
};

}

#endif
