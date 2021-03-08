#ifndef NIDL_AST_FIXED_H_
#define NIDL_AST_FIXED_H_

#include <stdint.h>

extern "C" struct _decNumber;

namespace AST {

/// This structure is completely equivalent to decNumber.
struct Fixed
{
	int32_t digits;
	int32_t exponent;
	uint8_t lsu [31];

	Fixed (const _decNumber& dn)
	{
		*this = reinterpret_cast <const Fixed&> (dn);
	}

	operator const _decNumber& () const
	{
		return *(const _decNumber*)(const void*)(this);
	}
};

}

#endif
