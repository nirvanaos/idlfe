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

	Variant (char v);
	Variant (wchar_t v);
	Variant (unsigned long v);
	Variant (unsigned long long v);
	Variant (double v);
	Variant (long double v);

	Variant (std::string&& v) :
		Type (Type::Kind::STRING)
	{
		new (&val_.s) std::string (std::move (v));
	}

	Variant (std::wstring&& v) :
		Type (Type::Kind::WSTRING)
	{
		new (&val_.ws) std::wstring (std::move (v));
	}

	Variant (const _decNumber& v);

	const uint8_t* bcd () const
	{
		assert (kind () == Type::Kind::FIXED);
		return val_.fixed;
	}

	size_t bcd_length () const
	{
		assert (kind () == Type::Kind::FIXED);
		return bcd_length (fixed_digits ());
	}

private:
	Variant (const Type& t) :
		Type (t)
	{}

	void clear () noexcept;
	void copy (const Variant& src);

	static unsigned bcd_length (unsigned digits)
	{
		return (digits + 2) / 2;
	}

private:
	union Val
	{
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
