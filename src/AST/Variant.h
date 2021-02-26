#ifndef NIDL_AST_VARIANT_H_
#define NIDL_AST_VARIANT_H_

#include "Type.h"

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

	Variant (const Ptr <NamedItem>* constant);
	static Variant make_char (const std::string& v);
	static Variant make_wchar (const std::string& v);
	static Variant make_uint (const std::string& v);
	static Variant make_double (const std::string& v);
	static Variant make_string (const std::string& v);
	static Variant make_wstring (const std::string& v);
	static Variant make_fixed (const std::string& v);

private:
	Variant (const Type& t) :
		Type (t)
	{}

	void clear () noexcept;
	void copy (const Variant& src);
	static wchar_t unescape_wchar (const char*& p);
	static char unescape_char (const char*& p);
	static int from_hdigit (int c) noexcept;
	static unsigned from_hex (const char*& p, unsigned maxlen);
	[[noreturn]] static void invalid_escape_seq ();
	[[noreturn]] static void invalid_char_const ();

private:
	union Val
	{
		int64_t i;
		uint64_t ui;
		double d;
		long double ld;
		long double fixed; // Temporary solution
		std::string s;
		std::wstring ws;

		Val () {}
		~Val () {}
	} val_;
};

Variant operator | (const Variant& l, const Variant& r);
Variant operator ^ (const Variant& l, const Variant& r);

}

#endif
