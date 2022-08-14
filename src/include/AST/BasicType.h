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
#ifndef IDLFE_AST_BASICTYPE_H_
#define IDLFE_AST_BASICTYPE_H_
#pragma once

namespace AST {

/// \brief CORBA basic type enumeration.
enum class BasicType
{
	BOOLEAN, ///< `boolean`
	OCTET, ///< `octet`
	CHAR, ///< `char`
	WCHAR, ///< `wchar`
	USHORT, ///< `unsigned short`
	ULONG, ///< `unsigned long`
	ULONGLONG, ///< `unsigned long long`
	SHORT, ///< `short`
	LONG, ///< `long`
	LONGLONG, ///< `long long`
	FLOAT, ///< `float`
	DOUBLE, ///< `double`
	LONGDOUBLE, ///< `long double`
	OBJECT, ///< `Object`
	VALUE_BASE, ///< `ValueBase`
	ANY ///< `any`
};

/// \returns `true` if a type is integral type.
inline
bool is_integral (BasicType bt) noexcept
{
	return BasicType::BOOLEAN <= bt && bt <= BasicType::LONGLONG;
}

/// \returns `true` if a type is signed type.
inline
bool is_signed (BasicType bt) noexcept
{
	return BasicType::SHORT <= bt && bt <= BasicType::LONGDOUBLE;
}

inline
bool is_floating_point (BasicType bt) noexcept
{
	return BasicType::FLOAT <= bt && bt <= BasicType::LONGDOUBLE;
}

}

#endif
