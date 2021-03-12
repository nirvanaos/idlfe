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
#ifndef NIDL_AST_EVALENUM_H_
#define NIDL_AST_EVALENUM_H_

#include "Eval.h"

namespace AST {

class Enum;

namespace Build {

/// Enum expression evaluiator.
class EvalEnum : public Eval
{
public:
	EvalEnum (Builder& builder, const Ptr <NamedItem>& type) :
		Eval (builder),
		type_ (type)
	{}

	virtual Variant constant (const ScopedName& constant);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);

private:
	const Ptr <NamedItem>& type_;
};

}
}

#endif
