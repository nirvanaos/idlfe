/// \file
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
#ifndef IDLFE_AST_ITEMWITHID_H_
#define IDLFE_AST_ITEMWITHID_H_
#pragma once

#include "NamedItem.h"

namespace AST {

struct Version
{
	uint16_t major, minor;
};

/// \brief Items which have repository identifiers derive from this class.
class ItemWithId : public NamedItem
{
public:
	/// \returns The repository id.
	std::string repository_id () const;

	/// \returns ItemWithId const pointer if item derives from ItemWithId.
	///          Otherwise returns `nullptr`.
	static const ItemWithId* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

protected:
	ItemWithId (Kind kind, Builder& builder, const SimpleDeclarator& name);

	virtual bool prefix (Builder& builder, const std::string& pref, const Location& loc);

private:
	friend class Builder;

	static ItemWithId* cast (NamedItem* item) noexcept;

	bool check_prefix (Builder& builder, const Location& loc) const noexcept;

	void type_id (Builder& builder, const std::string& id, const Location& loc);

	void pragma_version (Builder& builder, const Version v, const Location& loc);

	void set_id (const ItemWithId& src)
	{
		data_ = src.data_;
	}

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
	}
	data_;
};

}

#endif
