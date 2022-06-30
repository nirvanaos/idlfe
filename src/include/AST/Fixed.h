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
#ifndef NIDL_AST_FIXED_H_
#define NIDL_AST_FIXED_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace AST {

/// Fixed point constant.
class Fixed
{
public:
	/// \returns Number of digits for `fixed` type.
	uint16_t digits () const noexcept
	{
		return (uint16_t)digits_;
	}

	/// \returns Scale for `fixed` type.
	uint16_t scale () const noexcept
	{
		return (uint16_t)-exponent_;
	}

	/// \returns Fixed point as a character string.
	std::string to_string () const;

	/// Converts fixed to binary-coded decimal.
	/// Returned array is CORBA fixed point CDR compatible.
	/// 
	/// \param[out] scale The scale result.
	/// \returns The BCD bytes.
	std::vector <uint8_t> to_BCD (int32_t& scale) const;

	Fixed () noexcept
	{}

	static Fixed normalize (const Fixed& f) noexcept;

private:
	/// This structure is completely equivalent to struct decNumber 62 digits
	/// https://github.com/nirvanaos/decNumber.
	int32_t digits_;
	int32_t exponent_;
	uint8_t bits_;
	uint8_t lsu_ [31];
};

}

#endif
