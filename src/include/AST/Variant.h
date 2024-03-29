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
#ifndef IDLFE_AST_VARIANT_H_
#define IDLFE_AST_VARIANT_H_
#pragma once

#include "Type.h"
#include "Fixed.h"
#include <forward_list>

namespace AST {

class EnumItem;
class Constant;

/// \brief Stores the constant value.
class Variant
{
public:
	/// Value vtype. 
	enum class VT
	{
		EMPTY, ///< No value

		BOOLEAN, ///< Variant::as_bool ()
		OCTET, ///< Variant::as_octet ()
		CHAR, ///< Variant::as_char ()
		WCHAR, ///< Variant::as_wchar ()
		USHORT, ///< Variant::as_unsigned_short ()
		ULONG, ///< Variant::as_unsigned_long ()
		ULONGLONG, ///< Variant::as_unsigned_long_long ()
		SHORT, ///< Variant::as_short ()
		LONG, ///< Variant::as_long ()
		LONGLONG, ///< Variant::as_long_long ()
		FLOAT, ///< Variant::as_float ()
		DOUBLE, ///< Variant::as_double ()
		LONGDOUBLE, ///< Variant::as_long_double ()

		STRING, ///< Variant::as_string ()
		WSTRING, ///< Variant::as_wstring ()
		FIXED, ///< Variant::as_Fixed ();

		ENUM_ITEM, ///< Variant::as_enum_item ()
		CONSTANT ///< Variant::as_constant ()
	};

	/// \returns The value type.
	VT vtype () const noexcept
	{
		return type_;
	}

	// Integral

	/// \returns `boolean` value.
	/// \invariant vtype () == VT::BOOLEAN
	bool as_bool () const noexcept
	{
		assert (vtype () == VT::BOOLEAN);
		return val_.u.i != 0;
	}

	/// \returns `octet` value.
	/// \invariant vtype () == VT::OCTET
	uint8_t as_octet () const noexcept
	{
		assert (vtype () == VT::OCTET);
		return (uint8_t)val_.u.ui;
	}

	/// \returns `char` value.
	/// \invariant vtype () == VT::CHAR
	char as_char () const noexcept
	{
		assert (vtype () == VT::CHAR);
		return (char)val_.u.ui;
	}

	/// \returns `wchar` value.
	/// \invariant vtype () == VT::WCHAR
	wchar_t as_wchar () const noexcept
	{
		assert (vtype () == VT::WCHAR);
		return (wchar_t)val_.u.ui;
	}

	/// \returns `unsigned short` value.
	/// \invariant vtype () == VT::USHORT
	uint16_t as_unsigned_short () const noexcept
	{
		assert (vtype () == VT::USHORT);
		return (uint16_t)val_.u.ui;
	}

	/// \returns `short` value.
	/// \invariant vtype () == VT::SHORT
	int16_t as_short () const noexcept
	{
		assert (vtype () == VT::SHORT);
		return (int16_t)val_.u.i;
	}

	/// \returns `unsigned long` value.
	/// \invariant vtype () == VT::ULONG
	uint32_t as_unsigned_long () const noexcept
	{
		assert (vtype () == VT::ULONG);
		return (uint32_t)val_.u.ui;
	}

	/// \returns `long` value.
	/// \invariant vtype () == VT::LONG
	int32_t as_long () const noexcept
	{
		assert (vtype () == VT::LONG);
		return (int32_t)val_.u.i;
	}

	/// \returns `unsigned long long` value.
	/// \invariant vtype () == VT::ULONGLONG
	uint64_t as_unsigned_long_long () const noexcept
	{
		assert (vtype () == VT::ULONGLONG);
		return val_.u.ui;
	}

	/// \returns `long long` value.
	/// \invariant vtype () == VT::LONGLONG
	int64_t as_long_long () const noexcept
	{
		assert (vtype () == VT::LONGLONG);
		return val_.u.i;
	}

	// Floating point

	/// \returns `float` value.
	/// \invariant vtype () == VT::FLOAT
	float as_float () const noexcept
	{
		assert (vtype () == VT::FLOAT);
		return (float)val_.u.d;
	}

	/// \returns `double` value.
	/// \invariant vtype () == VT::DOUBLE
	double as_double () const noexcept
	{
		assert (vtype () == VT::DOUBLE);
		return (double)val_.u.d;
	}

	/// \returns `long double` value.
	/// \invariant vtype () == VT::LONGDOUBLE
	long double as_long_double () const noexcept
	{
		assert (vtype () == VT::LONGDOUBLE);
		return val_.u.d;
	}

	// String

	/// \returns `string` value.
	/// \invariant vtype () == VT::STRING
	const std::string& as_string () const noexcept
	{
		assert (vtype () == VT::STRING);
		return val_.u.s;
	}

	/// \returns `wstring` value.
	/// \invariant vtype () == VT::WSTRING
	const std::wstring& as_wstring () const noexcept
	{
		assert (vtype () == VT::WSTRING);
		return val_.u.ws;
	}

	/// \returns Enumerator reference.
	/// \invariant vtype () == VT::ENUM_ITEM
	const EnumItem& as_enum_item () const noexcept
	{
		assert (vtype () == VT::ENUM_ITEM);
		return *val_.u.enum_item;
	}

	/// \returns Constant reference.
	/// \invariant vtype () == VT::CONSTANT
	const Constant& as_constant () const noexcept
	{
		assert (vtype () == VT::CONSTANT);
		return *val_.u.constant;
	}

	// Fixed

	/// \returns Fixed reference.
	/// \invariant vtype () == VT::FIXED
	const Fixed& as_Fixed () const noexcept
	{
		assert (VT::FIXED == type_);
		return val_.u.fixed;
	}

	/// Converts value to std::string.
	/// This method escapes character and string values accordingly to C constant escape rules.
	/// This method does not add 'L' prefix to wide strings and characters and does not add 'D'
	/// suffix to fixed numbers.
	std::string to_string () const;

	/// \returns `true` if Variant is empty.
	/// In the valid AST, Variant never be empty.
	bool empty () const noexcept
	{
		return vtype () == VT::EMPTY;
	}

	/// \returns The constant final value.
	const Variant& dereference_const () const noexcept;

	/// Destructor.
	~Variant ()
	{
		clear ();
	}

	/// VT::EMPTY
	Variant () :
		type_ (VT::EMPTY)
	{}

	/// Copy constructor.
	Variant (const Variant& src)
	{
		copy (src);
	}

	/// Move constructor.
	Variant (Variant&& src) noexcept
	{
		move (std::move (src));
	}

	/// VT::BOOLEAN
	Variant (bool v) noexcept :
		type_ (VT::BOOLEAN),
		val_ ((uint64_t)v)
	{}

	/// VT::OCTET
	Variant (uint8_t v) noexcept :
		type_ (VT::OCTET),
		val_ ((uint64_t)v)
	{}

	/// VT::CHAR
	Variant (char v) noexcept :
		type_ (VT::CHAR),
		val_ ((uint64_t)v)
	{}

	/// VT::WCHAR
	Variant (wchar_t v) noexcept :
		type_ (VT::WCHAR),
		val_ ((uint64_t)v)
	{}

	/// VT::SHORT
	Variant (int16_t v) noexcept :
		type_ (VT::SHORT),
		val_ ((int64_t)v)
	{}

	/// VT::USHORT
	Variant (uint16_t v) noexcept :
		type_ (VT::USHORT),
		val_ ((int64_t)v)
	{}

	/// VT::LONG
	Variant (int32_t v) noexcept :
		type_ (VT::LONG),
		val_ ((int64_t)v)
	{}

	/// VT::ULONG
	Variant (uint32_t v) noexcept :
		type_ (VT::ULONG),
		val_ ((int64_t)v)
	{}

	/// VT::LONGLONG
	Variant (int64_t v) noexcept :
		type_ (VT::LONGLONG),
		val_ (v)
	{}

	/// VT::ULONGLONG
	Variant (uint64_t v) noexcept :
		type_ (VT::ULONGLONG),
		val_ (v)
	{}

	/// VT::FLOAT
	Variant (float v) noexcept :
		type_ (VT::FLOAT),
		val_ ((long double)v)
	{}

	/// VT::DOUBLE
	Variant (double v) noexcept :
		type_ (VT::DOUBLE),
		val_ ((long double)v)
	{}

	/// VT::LONGDOUBLE
	Variant (long double v) noexcept :
		type_ (VT::LONGDOUBLE),
		val_ (v)
	{}

	/// VT::STRING
	Variant (std::string&& v) noexcept :
		type_ (VT::STRING),
		val_ (std::move (v))
	{}

	/// VT::STRING
	Variant (const char* s) :
		Variant (std::string (s))
	{}

	/// VT::WSTRING
	Variant (std::wstring&& v) noexcept :
		type_ (VT::WSTRING),
		val_ (std::move (v))
	{}

	/// VT::WSTRING
	Variant (const wchar_t* s) :
		Variant (std::wstring (s))
	{}

	/// VT::FIXED
	Variant (const Fixed& v) noexcept :
		type_ (VT::FIXED),
		val_ (v)
	{}

	/// VT::ENUM_ITEM
	Variant (const EnumItem& item) noexcept :
		type_ (VT::ENUM_ITEM),
		val_ (item)
	{}

	/// VT::CONSTANT
	Variant (const Constant& constant) noexcept :
		type_ (VT::CONSTANT),
		val_ (constant)
	{}

	/// Copy assignment.
	Variant& operator = (const Variant& src);

	/// Move assignment.
	Variant& operator = (Variant&& src) noexcept;

	bool to_boolean () const
	{
		assert (is_integral ());
		return val_.u.i != 0;
	}

	uint8_t to_octet () const;

	char to_char () const
	{
		return (char)to_octet ();
	}

	wchar_t to_wchar () const
	{
		return (wchar_t)to_unsigned_short ();
	}

	uint16_t to_unsigned_short () const;
	int16_t to_short () const;
	uint32_t to_unsigned_long () const;
	int32_t to_long () const;
	uint64_t to_unsigned_long_long () const;
	int64_t to_long_long () const;
	float to_float () const;
	double to_double () const;
	long double to_long_double () const;

	typedef uint64_t Key;

	Key to_key () const noexcept
	{
		if (is_integral ())
			return val_.u.ui;
		else {
			assert (VT::ENUM_ITEM == type_);
			return (Key)val_.u.enum_item;
		}
	}

	bool is_integral () const noexcept
	{
		return VT::BOOLEAN <= type_ && type_ <= VT::LONGLONG;
	}

	bool is_signed () const noexcept
	{
		return VT::SHORT <= type_ && type_ <= VT::LONGDOUBLE;
	}

	bool is_floating_point () const noexcept
	{
		return VT::FLOAT <= type_ && type_ <= VT::LONGDOUBLE;
	}

private:
	friend class Builder;

	void clear () noexcept;
	void copy (const Variant& src);
	void move (Variant&& src);

	static void check_fp ();

	static void append (std::string& s, unsigned c);

	[[noreturn]] static void throw_out_of_range ();

private:
	VT type_;

	union U
	{
		int64_t i;
		uint64_t ui;
		long double d;
		Fixed fixed;
		std::string s;
		std::wstring ws;
		const EnumItem* enum_item;
		const Constant* constant;

		~U ()
		{}

		U ()
		{}

		U (const int64_t& src) :
			i (src)
		{}

		U (const uint64_t& src) :
			ui (src)
		{}

		U (const long double& src) :
			d (src)
		{}

		U (std::string&& src) :
			s (std::move (src))
		{}

		U (std::wstring&& src) :
			ws (std::move (src))
		{}

		U (const Fixed& src) :
			fixed (src)
		{}

		U (const EnumItem& src) :
			enum_item (&src)
		{}

		U (const Constant& src) :
			constant (&src)
		{}
	};

	struct Plain
	{
		int data [sizeof (U) / sizeof (int)];
	};

	union Val
	{
		U u;
		Plain plain;

		~Val ()
		{}

		Val ()
		{}

		template <typename T>
		Val (const T& src) :
			u (src)
		{}

		template <typename T>
		Val (T&& src) :
			u (std::move (src))
		{}

	} val_;
};

typedef std::forward_list <Variant> Variants;

}

#endif
