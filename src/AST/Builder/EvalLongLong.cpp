/// \file EvalLongLong.cpp Long integer expression evaluiator.
#include "EvalLongLong.h"
#include "Constant.h"
#include "SafeInt/SafeInt.hpp"
#include <stdexcept>

using namespace std;

namespace AST {

// Long integer evaluator

Variant EvalLongLong::expr (const Variant& l, char op, const Variant& r, const Location& loc)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.is_integer () && r.is_integer ());
		try {
			if (l.is_signed ()) {
				int64_t lv = l.to_long_long (), ret = 0;
				if ('>' == op || '<' == op) {
					uint32_t shift = r.to_unsigned_long ();
					if (shift > 64)
						throw range_error ("Shift size is too large.");
					ret = op == '>' ? lv >> shift : lv << shift;
				} else {
					int64_t rv = r.to_long_long ();
					switch (op) {
						case '|':
							ret = lv | rv;
							break;
						case '^':
							ret = lv ^ rv;
							break;
						case '&':
							ret = lv & rv;
							break;
						case '+':
							if (SafeAdd (lv, rv, ret))
								overflow (op);
							break;
						case '-':
							if (SafeSubtract (lv, rv, ret))
								overflow (op);
							break;
						case '*':
							if (SafeMultiply (lv, rv, ret))
								overflow (op);
							break;
						case '/':
							if (SafeDivide (lv, rv, ret))
								zero_divide (op);
							break;
						case '%':
							if (SafeModulus (lv, rv, ret))
								zero_divide (op);
							break;
						default:
							invalid_operation (op, loc);
							return Variant ();
					}
				}
				return ret;
			} else {
				uint64_t lv = l.to_unsigned_long_long (), ret = 0;
				if ('>' == op || '<' == op) {
					uint32_t shift = r.to_unsigned_long ();
					if (shift > 32)
						throw range_error ("Shift size is too large.");
					ret = op == '>' ? lv >> shift : lv << shift;
				} else {
					uint64_t rv = r.to_unsigned_long_long ();
					switch (op) {
						case '|':
							ret = lv | rv;
							break;
						case '^':
							ret = lv ^ rv;
							break;
						case '&':
							ret = lv & rv;
							break;
						case '+':
							if (SafeAdd (lv, rv, ret))
								overflow (op);
							break;
						case '-':
							if (SafeSubtract (lv, rv, ret))
								overflow (op);
							break;
						case '*':
							if (SafeMultiply (lv, rv, ret))
								overflow (op);
							break;
						case '/':
							if (SafeDivide (lv, rv, ret))
								zero_divide (op);
							break;
						case '%':
							if (SafeModulus (lv, rv, ret))
								zero_divide (op);
							break;
						default:
							invalid_operation (op, loc);
							return Variant ();
					}
				}
				return ret;
			}
		} catch (const exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

Variant EvalLongLong::expr (char op, const Variant& v, const Location& loc)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_integer ());
		try {
			if (v.is_signed ()) {
				int64_t i = v.to_long_long (), ret = 0;
				switch (op) {
					case '-':
						ret = -i;
						break;
					case '+':
						ret = i;
						break;
					case '~':
						ret = -(i + 1);
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
			} else {
				uint64_t u = v.to_unsigned_long_long ();
				switch (op) {
					case '-': {
						int64_t i;
						if (SafeCast (u, i))
							overflow (op);
						return -i;
					}
									break;
					case '+':
						return u;
						break;
					case '~':
						return numeric_limits <uint64_t>::max () - u;;
						break;
					default:
						invalid_operation (op, loc);
						return Variant ();
				}
			}
		} catch (const exception& ex) {
			error (loc, ex);
		}
	}
	return Variant ();
}

}
