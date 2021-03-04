#ifndef NIDL_AST_VARIANT_H_
#define NIDL_AST_VARIANT_H_

#include "Type.h"

extern "C" struct _decNumber;

namespace AST {

class EnumItem;
class Constant;

class Variant :
	public Type
{
public:
	/// \returns If variant references to constant, `const Constant*`. Otherwise `nullptr`.
	const Constant* constant () const noexcept
	{
		return val_.v.constant;
	}

	const Variant& dereference_const () const noexcept;

	// Boolean

	bool as_bool () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::BOOLEAN);
		return dereference_const ().val_.v.u.b;
	}

	// Integer

	uint8_t as_octet () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::OCTET);
		return (uint8_t)dereference_const ().val_.v.u.ui;
	}

	uint8_t to_octet () const;

	char as_char () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::CHAR);
		return (char)dereference_const ().val_.v.u.ui;
	}

	char to_char () const;

	wchar_t as_wchar () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::WCHAR);
		return (wchar_t)dereference_const ().val_.v.u.ui;
	}

	wchar_t to_wchar () const;

	uint16_t as_unsigned_short () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::USHORT);
		return (uint16_t)dereference_const ().val_.v.u.ui;
	}

	uint16_t to_unsigned_short () const;

	int16_t as_short () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::SHORT);
		return (int16_t)dereference_const ().val_.v.u.i;
	}

	int16_t to_short () const;

	uint32_t as_unsigned_long () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::ULONG);
		return (uint32_t)dereference_const ().val_.v.u.ui;
	}

	uint32_t to_unsigned_long () const;

	int32_t as_long () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::LONG);
		return (int32_t)dereference_const ().val_.v.u.i;
	}

	int32_t to_long () const;

	uint64_t as_unsigned_long_long () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::ULONGLONG);
		return dereference_const ().val_.v.u.ui;
	}

	uint64_t to_unsigned_long_long () const;

	int64_t as_long_long () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::LONGLONG);
		return dereference_const ().val_.v.u.i;
	}

	int64_t to_long_long () const;

	// Floating point

	float as_float () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::FLOAT);
		return (float)dereference_const ().val_.v.u.d;
	}

	float to_float () const;

	double as_double () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::DOUBLE);
		return (double)dereference_const ().val_.v.u.d;
	}

	double to_double () const;

	long double as_long_double () const noexcept
	{
		assert (dereference_type ().kind () == Kind::BASIC_TYPE);
		assert (dereference_type ().basic_type () == BasicType::LONGDOUBLE);
		return dereference_const ().val_.v.u.d;
	}

	long double to_long_double () const;

	// String

	const std::string& as_string () const noexcept
	{
		assert (dereference_type ().kind () == Type::Kind::STRING);
		return dereference_const ().val_.v.u.s;
	}

	const std::wstring& as_wstring () const noexcept
	{
		assert (dereference_type ().kind () == Type::Kind::WSTRING);
		return dereference_const ().val_.v.u.ws;
	}

	// Fixed

	const uint8_t* as_bcd () const noexcept
	{
		assert (dereference_type ().kind () == Type::Kind::FIXED);
		return val_.v.u.fixed;
	}

	size_t bcd_length () const noexcept
	{
		assert (dereference_type ().kind () == Type::Kind::FIXED);
		return bcd_length (fixed_digits ());
	}

	// Internals

	bool empty () const noexcept
	{
		return kind () == Type::Kind::VOID;
	}

	void as_decNumber (_decNumber& dn) const noexcept;

	~Variant ();

	Variant () {}

	Variant (const Variant& src)
	{
		copy (src);
	}

	Variant (Variant&& src) noexcept;

	Variant& operator = (const Variant& src);
	Variant& operator = (Variant&& src) noexcept;

	Variant (bool v) noexcept;
	Variant (char v) noexcept;
	Variant (wchar_t v) noexcept;
	Variant (int32_t v) noexcept;
	Variant (uint32_t v) noexcept;
	Variant (int64_t v) noexcept;
	Variant (uint64_t v) noexcept;
	Variant (float v) noexcept;
	Variant (double v) noexcept;
	Variant (long double v) noexcept;

	Variant (std::string&& v) noexcept :
		Type (Type::Kind::STRING),
		val_ (std::move (v))
	{}

	Variant (std::wstring&& v) noexcept :
		Type (Type::Kind::WSTRING),
		val_ (std::move (v))
	{}

	Variant (const _decNumber& v) noexcept;

	Variant (const Constant& constant) noexcept;
	Variant (const EnumItem& item) noexcept;

private:
	void clear () noexcept;
	void copy (const Variant& src);

	static unsigned bcd_length (unsigned digits) noexcept
	{
		return (digits + 2) / 2;
	}

	static void check_fp ();

private:
	[[noreturn]] static void throw_out_of_range ();

private:
	union U
	{
		bool b;
		int64_t i;
		uint64_t ui;
		long double d;
		uint8_t fixed [32];
		std::string s;
		std::wstring ws;
		const EnumItem* enum_item;

		~U ()
		{}

		U ()
		{}

		U (const bool src) :
			b (src)
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

		U (const EnumItem& src) :
			enum_item (&src)
		{}
	};

	struct V
	{
		~V ()
		{}

		V () :
			constant (nullptr)
		{}

		template <typename T>
		V (const T& src) :
			u (src),
			constant (nullptr)
		{}

		template <typename T>
		V (T&& src) :
			u (std::move (src)),
			constant (nullptr)
		{}

		V (const Constant& src) :
			constant (&src)
		{}

		U u;
		const Constant* constant;
	};

	struct Plain
	{
		int data [sizeof (V)];
	};

	union Val
	{
		V v;
		Plain plain;

		~Val ()
		{}

		Val ()
		{}

		template <typename T>
		Val (const T& src) :
			v (src)
		{}

		template <typename T>
		Val (T&& src) :
			v (std::move (src))
		{}

	} val_;
};

}

#endif
