/// \file EvalLong.cpp Integer expression evaluiator.
#include "EvalLongLong.h"
#include "Builder.h"
#include "SafeInt/SafeInt.hpp"
#include <stdexcept>

using namespace std;

namespace AST {

// Integer evaluator

Variant EvalLong::literal_char (const string& s, unsigned line)
{
	try {
		assert (s.size () > 2);
		assert (s.front () == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 1;
		char v = unescape_char (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::literal_wchar (const string& s, unsigned line)
{
	try {
		assert (s.size () > 3);
		assert (s [0] == 'L');
		assert (s [1] == '\'');
		assert (s.back () == '\'');
		const char* p = &s.front () + 2;
		wchar_t v = unescape_wchar (p);
		if (p != &s.back ())
			invalid_char_const ();
		return Variant (v);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::literal_int (const string& s, unsigned line)
{
	try {
		size_t idx;
		uint64_t ull = stoull (s, &idx, 0);
		if (idx != s.size ())
			throw runtime_error ("Invalid integer constant.");
		if (ull > numeric_limits <uint32_t>::max ())
			return Variant (ull);
		else
			return Variant ((uint32_t)ull);
	} catch (const exception& ex) {
		error (line, ex);
		return Variant ();
	}
}

Variant EvalLong::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		if (pc->is_integer ())
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalLong::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		assert (l.is_integer () && r.is_integer ());
		try {
			if (
				l.basic_type () == BasicType::LONGLONG || r.basic_type () == BasicType::LONGLONG
				||
				l.basic_type () == BasicType::ULONGLONG || r.basic_type () == BasicType::ULONGLONG
				) {
				Variant ll = EvalLongLong (builder_).expr (l, op, r, line);
				if (l.is_signed ())
					return ll.to_long ();
				else
					return ll.to_unsigned_long ();
			} else {
				if (l.is_signed ()) {
					int32_t lv = l.to_long (), ret = 0;
					if ('>' == op || '<' == op) {
						uint32_t shift = r.to_unsigned_long ();
						if (shift > 32)
							throw range_error ("Shift size is too large.");
						ret = op == '>' ? lv >> shift : lv << shift;
					} else {
						int32_t rv = r.to_long ();
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
								invalid_operation (op, line);
								return Variant ();
						}
					}
					return ret;
				} else {
					uint32_t lv = l.to_unsigned_long (), ret = 0;
					if ('>' == op || '<' == op) {
						uint32_t shift = r.to_unsigned_long ();
						if (shift > 32)
							throw range_error ("Shift size is too large.");
						ret = op == '>' ? lv >> shift : lv << shift;
					} else {
						uint32_t rv = r.to_unsigned_long ();
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
								invalid_operation (op, line);
								return Variant ();
						}
					}
					return ret;
				}
			}
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalLong::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_integer ());
		try {
			if (v.is_signed ()) {
				int32_t i = v.to_long ();
				switch (op) {
					case '-':
						i = -i;
						break;
					case '+':
						break;
					case '~':
						i = -(i + 1);
						break;
					default:
						invalid_operation (op, line);
						return Variant ();
				}
				return i;
			} else {
				uint32_t u = v.to_unsigned_long ();
				switch (op) {
					case '-': {
						int32_t i;
						if (SafeCast (u, i))
							overflow (op);
						return -i;
					}
									break;
					case '+':
						return u;
						break;
					case '~':
						return numeric_limits <uint32_t>::max () - u;;
						break;
					default:
						invalid_operation (op, line);
						return Variant ();
				}
			}
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return Variant ();
}

Variant EvalLong::cast (const Type& t, Variant&& v, unsigned line)
{
	Variant ret;
	assert (t.is_integer ());
	if (v.kind () != Type::Kind::VOID) {
		assert (v.is_integer ());
		try {
			switch (t.basic_type ()) {
				case BasicType::OCTET:
					ret = v.to_octet ();
					break;
				case BasicType::CHAR:
					ret = v.to_char ();
					break;
				case BasicType::WCHAR:
					ret = v.to_wchar ();
					break;
				case BasicType::USHORT:
					ret = v.to_unsigned_short ();
					break;
				case BasicType::SHORT:
					ret = v.to_short ();
					break;
				case BasicType::ULONG:
					ret = v.to_unsigned_long ();
					break;
				case BasicType::LONG:
					ret = v.to_long ();
					break;
				case BasicType::ULONGLONG:
					ret = v.to_unsigned_long_long ();
					break;
				case BasicType::LONGLONG:
					ret = v.to_long_long ();
					break;
				default:
					builder_.message (Location (builder_.file (), line), Builder::MessageType::ERROR, "Invalid constant type.");
			}
		} catch (const exception& ex) {
			error (line, ex);
		}
	}
	return ret;
}

}
