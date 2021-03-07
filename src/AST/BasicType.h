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
