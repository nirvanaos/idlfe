/// \file Operation.h
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
#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"
#include "Parameter.h"
#include "Exception.h"
#include "Variant.h"

namespace AST {

/// The operation.
class Operation :
	public NamedItem,
	public Type,
	public ContainerT <Parameter>
{
public:
	/// \returns `true` if this is an `onevay` operation.
	bool oneway () const
	{
		return oneway_;
	}

	/// The user exceptions.
	typedef std::vector <const Exception*> Raises;

	/// \returns The possible user exceptions for the operation.
	const Raises& raises () const
	{
		return raises_;
	}

	/// The context.
	typedef std::vector <std::string> Context;

	/// \returns The list of context values for the operation.
	const Context context () const
	{
		return context_;
	}

	/// \internals

	Operation (const Build::Builder& builder, bool oneway, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::OPERATION, builder, name),
		Type (type),
		oneway_ (oneway)
	{}

	void oneway_clear ()
	{
		oneway_ = false;
	}

	void add_exception (const Exception* ex)
	{
		raises_.push_back (ex);
	}

	void context (const Build::Variants& strings)
	{
		for (auto it = strings.begin (); it != strings.end (); ++it) {
			if (!it->empty ()) {
				assert (it->vtype () == Variant::VT::STRING);
				context_.push_back (it->as_string ());
			}
		}
	}

private:
	bool oneway_;
	Raises raises_;
	Context context_;
	/// \endinternals
};

}

#endif
