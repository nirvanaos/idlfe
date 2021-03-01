#ifndef NIDL_AST_VARIANT_H_
#define NIDL_AST_VARIANT_H_

#include "Type.h"

extern "C" struct _decNumber;

namespace AST {

class Builder;

class Variant :
	public Type
{
public:
	~Variant ()
	{
		clear ();
	}

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
	Variant (double v) noexcept;
	Variant (long double v) noexcept;

	Variant (std::string&& v) noexcept :
		Type (Type::Kind::STRING)
	{
		new (&val_.s) std::string (std::move (v));
	}

	Variant (std::wstring&& v) noexcept :
		Type (Type::Kind::WSTRING)
	{
		new (&val_.ws) std::wstring (std::move (v));
	}

	Variant (const _decNumber& v) noexcept;

	// Boolean

	bool as_bool () const noexcept
	{
		assert (dereference ().kind () == Kind::BASIC_TYPE);
		assert (dereference ().basic_type () == BasicType::BOOLEAN);
		return val_.b;
	}

	// Integer

	uint32_t as_unsigned_long () const;
	int32_t as_long () const;

	// String

	const std::string& as_string () const noexcept
	{
		assert (dereference ().kind () == Type::Kind::STRING);
		return val_.s;
	}

	const std::wstring& as_wstring () const noexcept
	{
		assert (dereference ().kind () == Type::Kind::WSTRING);
		return val_.ws;
	}

	// Fixed
	void as_decNumber (_decNumber& dn) const noexcept;

	const uint8_t* as_bcd () const noexcept
	{
		assert (dereference ().kind () == Type::Kind::FIXED);
		return val_.fixed;
	}

	size_t bcd_length () const noexcept
	{
		assert (dereference ().kind () == Type::Kind::FIXED);
		return bcd_length (fixed_digits ());
	}

private:
	void clear () noexcept;
	void copy (const Variant& src);

	static unsigned bcd_length (unsigned digits) noexcept
	{
		return (digits + 2) / 2;
	}

private:
	[[noreturn]] static void throw_out_of_range ();

private:
	union Val
	{
		bool b;
		int64_t i;
		uint64_t ui;
		long double d;
		uint8_t fixed [32];
		std::string s;
		std::wstring ws;

		Val () {}
		~Val () {}
	} val_;
};

}

#endif
