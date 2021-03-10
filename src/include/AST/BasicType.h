/// \file BasicType.h
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
*  silver.popov@google.com
*/
#ifndef NIDL_AST_BASICTYPE_H_
#define NIDL_AST_BASICTYPE_H_

namespace AST {

/// Basic type enumeration.
enum class BasicType
{
	BOOLEAN,
	OCTET,
	CHAR,
	WCHAR,
	USHORT,
	ULONG,
	ULONGLONG,
	SHORT,
	LONG,
	LONGLONG,
	FLOAT,
	DOUBLE,
	LONGDOUBLE,
	OBJECT,
	VALUE_BASE,
	ANY
};

inline
bool is_integral (BasicType bt) noexcept
{
	return BasicType::BOOLEAN <= bt && bt <= BasicType::LONGLONG;
}

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