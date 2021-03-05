/// \file Fixed.h
#ifndef NIDL_AST_FIXED_H_
#define NIDL_AST_FIXED_H_

#include <stdint.h>

namespace AST {

struct Fixed
{
	uint8_t bits;
	uint8_t lsu [31];
};

}

#endif
