/// \file OperationBase.h
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
#ifndef NIDL_AST_OPERATIONBASE_H_
#define NIDL_AST_OPERATIONBASE_H_

#include "Parameter.h"
#include "Exception.h"

namespace AST{

/// The operation base.
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
	OperationBase (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name) :
		NamedItem (kind, builder, name)
	{}

private:
	friend class Build::Builder;

	void raises (Raises&& exceptions)
	{
		raises_ = std::move (exceptions);
	}

private:
	Raises raises_;
};

}

#endif

