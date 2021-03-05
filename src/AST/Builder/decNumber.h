/// \file decNumber.h Includes decNumber library header.
#ifndef NIDL_AST_DECNUMBER_H_
#define NIDL_AST_DECNUMBER_H_

extern "C" {
#include <decNumber/decNumber.h>
}
#include "../Fixed.h"

#if DECNUMDIGITS != 62
#error decNumber DECNUMDIGITS must be 62.
#endif

static_assert (sizeof (_decNumber::lsu) == sizeof (AST::Fixed::lsu), "Check decNumber library definitions.");

#endif
