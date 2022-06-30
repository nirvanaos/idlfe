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
#include "../include/AST/Fixed.h"
#include "Builder/decNumber.h"
#include <assert.h>

using namespace std;

namespace AST {

string Fixed::to_string () const
{
	char buf [31 + 14];
	assert (exponent_ > -31);
	decNumberToString ((const decNumber*)this, buf);
	return buf;
}

vector <uint8_t> Fixed::to_BCD (int32_t& scale) const
{
	size_t len = (digits_ + 2) / 2;
	vector <uint8_t> bcd (len);
	decPackedFromNumber (bcd.data (), (int32_t)bcd.size (), &scale, (const decNumber*)this);
	return bcd;
}

Fixed Fixed::normalize (const Fixed& f) noexcept
{
	if (f.digits_ > 31) {
		decContext ctx;
		decContextDefault (&ctx, DEC_INIT_BASE);
		ctx.digits = 31;
		Fixed ret;
		decNumberReduce ((decNumber*)&ret, (const decNumber*)&f, &ctx);
		return ret;
	} else
		return f;
}

}
