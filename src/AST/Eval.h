/// \file Eval.h Expression evaluiator.
#ifndef NIDL_AST_EVAL_H_
#define NIDL_AST_EVAL_H_

#include "Variant.h"

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

	virtual Variant expr_or (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_xor (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_and (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_shift_right (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_shift_left (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_add (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_sub (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_mul (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_div (const Variant& l, const Variant& r, unsigned line);
	virtual Variant expr_rem (const Variant& l, const Variant& r, unsigned line);

	virtual Variant expr_minus (const Variant& v, unsigned line);
	virtual Variant expr_plus (const Variant& v, unsigned line);
	virtual Variant expr_tilde (const Variant& v, unsigned line);

protected:
	[[noreturn]] static void invalid_escape_seq ();
	[[noreturn]] static void invalid_char_const ();
	static wchar_t unescape_wchar (const char*& p);
	static char unescape_char (const char*& p);
	void invalid_literal_type (unsigned line) const;
	void invalid_operation (unsigned line) const;
	void error (unsigned line, const std::exception& ex) const;
	static int from_hdigit (int c) noexcept;
	static unsigned from_hex (const char*& p, unsigned maxlen);
	const Ptr <NamedItem>* lookup_const (const ScopedName& constant) const;
	void see_definition (const NamedItem& item) const;

protected:
	Builder& builder_;
};

/// Integer evaluator.
class EvalLong : public Eval
{
public:
	EvalLong (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_char (const std::string& s, unsigned line);
	virtual Variant literal_wchar (const std::string& s, unsigned line);
	virtual Variant literal_int (const std::string& s, unsigned line);
};

/// Long integer evaluator
class EvalLongLong : public EvalLong
{
public:
	EvalLongLong (Builder& builder) :
		EvalLong (builder)
	{}

};

/// Double evaluator
class EvalDouble : public Eval
{
public:
	EvalDouble (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_float (const std::string& s, unsigned line);
};

/// Long double is not yet supported
typedef EvalDouble EvalLongDouble;

/// String evaluator

class EvalString : public Eval
{
public:
	EvalString (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_string (const std::string& s, unsigned line, const Variant* append);
};

/// Wide string evaluator

class EvalWString : public Eval
{
public:
	EvalWString (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_wstring (const std::string& s, unsigned line, const Variant* append);
};

/// Fixed evaluator

class EvalFixed : public Eval
{
public:
	EvalFixed (Builder& builder) :
		Eval (builder)
	{}

	virtual Variant literal_fixed (const std::string& s, unsigned line);

private:
	struct Context;
};

}

#endif
