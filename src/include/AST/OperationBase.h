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
#ifndef IDLFE_AST_OPERATIONBASE_H_
#define IDLFE_AST_OPERATIONBASE_H_
#pragma once

#include "Parameter.h"
#include "Exception.h"

namespace AST{

/// \brief An operation base.
class OperationBase :
	public NamedItem,
	public ContainerT <Parameter>
{
public:
	/// \returns The possible user exceptions for the operation.
	const Raises& raises () const noexcept
	{
		return raises_;
	}

protected:
	OperationBase (Item::Kind kind, Builder& builder, const SimpleDeclarator& name) :
		NamedItem (kind, builder, name)
	{}

private:
	friend class Builder;

	void raises (Raises&& exceptions)
	{
		raises_ = std::move (exceptions);
	}

private:
	Raises raises_;
};

}

#endif

