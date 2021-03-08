/// \file Variant.h
/*
* Nirvana IDL Front End Library.
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
#ifndef NIDL_AST_VARIANT_H_
#define NIDL_AST_VARIANT_H_

#include "Type.h"
#include "Fixed.h"

namespace AST {

class EnumItem;
class Constant;

/// Stores the constant value.
class Variant
{
public:
	/// Value vtype. 
	enum class VT
	{
		EMPTY, ///< In the valid AST, Variant never has this type.

		BOOLEAN, ///< as_bool ()
		OCTET, ///< as_octet ()
		CHAR, ///< as_char ()
		WCHAR, ///< as_wchar ()
		USHORT, ///< as_unsigned_short ()
		ULONG, ///< as_unsigned_long ()
		ULONGLONG, ///< as_unsigned_long_long ()
		SHORT, ///< as_short ()
		LONG, ///< as_long ()
		LONGLONG, ///< as_long_long ()
		FLOAT, ///< as_float ()
		DOUBLE, ///< as_double ()
		LONGDOUBLE, ///< as_long_double ()

		STRING, ///< as_string ()
		WSTRING, ///< as_wstring ()
		FIXED, ///< fixed_digits (); fixed_scale ();

		ENUM_ITEM, ///< as_enum_item ()
		CONSTANT ///< as_constant ()
	};

	/// \returns The value type.
	VT vtype () const noexcept
	{
		return type_;
	}

	// Integral

	bool as_bool () const noexcept
	{
		assert (vtype () == VT::BOOLEAN);
		return val_.u.i != 0;
	}

	uint8_t as_octet () const noexcept
	{
		assert (vtype () == VT::OCTET);
		return (uint8_t)val_.u.ui;
	}

	char as_char () const noexcept
	{
		assert (vtype () == VT::CHAR);
		return (char)val_.u.ui;
	}

	wchar_t as_wchar () const noexcept
	{
		assert (vtype () == VT::WCHAR);
		return (wchar_t)val_.u.ui;
	}

	uint16_t as_unsigned_short () const noexcept
	{
		assert (vtype () == VT::USHORT);
		return (uint16_t)val_.u.ui;
	}

	int16_t as_short () const noexcept
	{
		assert (vtype () == VT::SHORT);
		return (int16_t)val_.u.i;
	}

	uint32_t as_unsigned_long () const noexcept
	{
		assert (vtype () == VT::ULONG);
		return (uint32_t)val_.u.ui;
	}

	int32_t as_long () const noexcept
	{
		assert (vtype () == VT::LONG);
		return (int32_t)val_.u.i;
	}

	uint64_t as_unsigned_long_long () const noexcept
	{
		assert (vtype () == VT::ULONGLONG);
		return val_.u.ui;
	}

	int64_t as_long_long () const noexcept
	{
		assert (vtype () == VT::LONGLONG);
		return val_.u.i;
	}

	// Floating point

	float as_float () const noexcept
	{
		assert (vtype () == VT::FLOAT);
		return (float)val_.u.d;
	}

	double as_double () const noexcept
	{
		assert (vtype () == VT::DOUBLE);
		return (double)val_.u.d;
	}

	long double as_long_double () const noexcept
	{
		assert (vtype () == VT::LONGDOUBLE);
		return val_.u.d;
	}

	// String

	const std::string& as_string () const noexcept
	{
		assert (vtype () == VT::STRING);
		return val_.u.s;
	}

	const std::wstring& as_wstring () const noexcept
	{
		assert (vtype () == VT::WSTRING);
		return val_.u.ws;
	}

	/// Returns enumerator reference.
	const EnumItem& as_enum_item () const noexcept
	{
		assert (vtype () == VT::ENUM_ITEM);
		return *val_.u.enum_item;
	}

	/// Returns constant reference.
	const Constant& as_constant () const noexcept
	{
		assert (vtype () == VT::CONSTANT);
		return *val_.u.constant;
	}

	// Fixed

	uint16_t fixed_digits () const noexcept
	{
		assert (vtype () == VT::FIXED);
		return (uint16_t)val_.u.fixed.digits;
	}

	uint16_t fixed_scale () const noexcept
	{
		assert (vtype () == VT::FIXED);
		return (uint16_t)-val_.u.fixed.exponent;
	}

	/// Converts value to std::string.
	/// This method escapes character and string values accordingly to C constant escape rules.
	std::string to_string () const;

	/// \internal

	bool empty () const noexcept
	{
		return vtype () == VT::EMPTY;
	}

	const Variant& dereference_const () const noexcept;

	~Variant ();

	Variant () :
		type_ (VT::EMPTY)
	{}

	Variant (const Variant& src)
	{
		copy (src);
	}

	Variant (Variant&& src) noexcept;

	Variant& operator = (const Variant& src);
	Variant& operator = (Variant&& src) noexcept;

	Variant (bool v) noexcept :
		type_ (VT::BOOLEAN),
		val_ ((uint64_t)v)
	{}

	Variant (char v) noexcept :
		type_ (VT::CHAR),
		val_ ((uint64_t)v)
	{}

	Variant (wchar_t v) noexcept :
		type_ (VT::WCHAR),
		val_ ((uint64_t)v)
	{}

	Variant (int32_t v) noexcept :
		type_ (VT::LONG),
		val_ ((int64_t)v)
	{}

	Variant (uint32_t v) noexcept :
		type_ (VT::ULONG),
		val_ ((int64_t)v)
	{}

	Variant (int64_t v) noexcept :
		type_ (VT::LONGLONG),
		val_ (v)
	{}

	Variant (uint64_t v) noexcept :
		type_ (VT::ULONGLONG),
		val_ (v)
	{}

	Variant (float v) noexcept :
		type_ (VT::FLOAT),
		val_ ((long double)v)
	{}

	Variant (double v) noexcept :
		type_ (VT::DOUBLE),
		val_ ((long double)v)
	{}

	Variant (long double v) noexcept :
		type_ (VT::LONGDOUBLE),
		val_ (v)
	{}

	Variant (std::string&& v) noexcept :
		type_ (VT::STRING),
		val_ (std::move (v))
	{}

	Variant (std::wstring&& v) noexcept :
		type_ (VT::WSTRING),
		val_ (std::move (v))
	{}

	Variant (const _decNumber& v) noexcept :
		type_ (VT::FIXED),
		val_ (v)
	{}

	Variant (const EnumItem& item) noexcept :
		type_ (VT::ENUM_ITEM),
		val_ (item)
	{}

	Variant (const Constant& constant) noexcept :
		type_ (VT::CONSTANT),
		val_ (constant)
	{}

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
		const Variant& v = dereference_const ();
		if (v.is_integral ())
			return val_.u.ui;
		else {
			assert (VT::ENUM_ITEM == type_);
			return (Key)val_.u.enum_item;
		}
	}

	const _decNumber& as_decNumber () const noexcept
	{
		assert (VT::FIXED == type_);
		return val_.u.fixed;
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

	static void append (std::string& s, unsigned c);

private:
	void reset () noexcept
	{
		type_ = VT::EMPTY;
	}

	void clear () noexcept;
	void copy (const Variant& src);

	static void check_fp ();

private:
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

		U (const _decNumber& dn) :
			fixed (dn)
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
	/// \endinternal
};

namespace Build {
typedef std::forward_list <Variant> Variants;
}

}

#endif
