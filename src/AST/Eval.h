/// \file Eval.h Expression evaluiator.
#ifndef NIDL_AST_EVAL_H_
#define NIDL_AST_EVAL_H_

#include "Constant.h"

namespace AST {

class Builder;
struct ScopedName;

/// Abstract evaluator.
class Eval
{
public:
	Eval (Builder& builder) :
		builder_ (builder)
	{}

	virtual Variant literal_char (const std::string& s, unsigned line);
	virtual Variant literal_wchar (const std::string& s, unsigned line);
	virtual Variant literal_int (const std::string& s, unsigned line);
	virtual Variant literal_float (const std::string& s, unsigned line);
	virtual Variant literal_string (const std::string& s, unsigned line, const Variant* append = nullptr);
	virtual Variant literal_wstring (const std::string& s, unsigned line, const Variant* append = nullptr);
	virtual Variant literal_fixed (const std::string& s, unsigned line);
	virtual Variant literal_boolean (bool v, unsigned line);

	virtual Variant constant (const ScopedName& constant);

	virtual Variant expr (const Variant& l, char op, const Variant& r, unsigned line);
	virtual Variant expr (char op, const Variant& v, unsigned line);

protected:
	[[noreturn]] static void invalid_escape_seq ();
	[[noreturn]] static void invalid_char_const ();
	[[noreturn]] void overflow (char op);
	[[noreturn]] void underflow (char op);
	[[noreturn]] void zero_divide (char op);

	static wchar_t unescape_wchar (const char*& p);
	static char unescape_char (const char*& p);
	void invalid_literal_type (unsigned line) const;
	void invalid_operation (char op, unsigned line) const;
	void error (unsigned line, const std::exception& ex) const;
	static int from_hdigit (int c) noexcept;
	static unsigned from_hex (const char*& p, unsigned maxlen);
	const Constant* lookup_const (const ScopedName& constant) const;
	void see_definition (const NamedItem& item) const;
	void invalid_constant_type (const ScopedName& constant) const;

protected:
	Builder& builder_;
};

}

#endif
