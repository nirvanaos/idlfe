/// \file Eval.h Expression evaluiator.
#ifndef NIDL_AST_EVAL_H_
#define NIDL_AST_EVAL_H_

#include "../Variant.h"

namespace AST {

struct ScopedName;
class Constant;

namespace Build {

class Builder;
/// Evaluator base.
class Eval
{
public:
	Eval (Builder& builder) :
		builder_ (builder)
	{}

	virtual Variant literal_char (const std::string& s, const Location& loc);
	virtual Variant literal_wchar (const std::string& s, const Location& loc);
	virtual Variant literal_int (const std::string& s, const Location& loc);
	virtual Variant literal_float (const std::string& s, const Location& loc);
	virtual Variant literal_string (const std::string& s, const Location& loc, const Variant* append = nullptr);
	virtual Variant literal_wstring (const std::string& s, const Location& loc, const Variant* append = nullptr);
	virtual Variant literal_fixed (const std::string& s, const Location& loc);
	virtual Variant literal_boolean (bool v, const Location& loc);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, const Location& loc);
	virtual Variant expr (char op, const Variant& v, const Location& loc);

	virtual Variant cast (const Type& t, Variant&& v, const Location& loc);

	static char unescape_char (const char*& p);

protected:
	[[noreturn]] static void invalid_escape_seq ();
	[[noreturn]] static void invalid_char_const ();
	[[noreturn]] void overflow (char op);
	[[noreturn]] void underflow (char op);
	[[noreturn]] void zero_divide (char op);

	static wchar_t unescape_wchar (const char*& p);
	void invalid_literal_type (const Location& loc) const;
	void invalid_operation (char op, const Location& loc) const;
	void error (const Location& loc, const std::exception& ex) const;
	static int from_hdigit (int c) noexcept;
	static unsigned from_hex (const char*& p, unsigned maxlen);
	const Constant* lookup_const (const ScopedName& constant) const;
	void see_definition (const NamedItem& item) const;
	void invalid_constant_type (const ScopedName& constant) const;

protected:
	Builder& builder_;
};

}
}

#endif
