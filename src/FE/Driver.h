#ifndef NIDL_FE_DRIVER_H_
#define NIDL_FE_DRIVER_H_

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
#include "../AST/Builder.h"
#include <iostream>

class Driver :
	public yyFlexLexer
{
public:
	Driver (const std::string& file, std::istream& yyin);

	int parse ()
	{
		return parser_.parse ();
	}

	void parser_error (const yy::parser::location_type& l, const std::string& msg)
	{
		builder_.parser_error (l.begin.line, msg);
	}

	void preprocessor_directive (const char*, unsigned line);

	void pragma (const char*, unsigned line);

	void module_begin (const std::string& id, const yy::parser::location_type& l)
	{
		builder_.module_begin (id, l.begin.line);
	}

	void module_end ()
	{
		builder_.scope_end ();
	}

	void native (const std::string& id, const yy::parser::location_type& l)
	{
		builder_.native (id, l.begin.line);
	}

	void interface_decl (const std::string& id, const yy::parser::location_type& l, AST::InterfaceKind ik = AST::InterfaceKind::UNCONSTRAINED)
	{
		builder_.interface_decl (id, l.begin.line, ik);
	}

	void interface_begin (const std::string& id, const yy::parser::location_type& l, AST::InterfaceKind ik = AST::InterfaceKind::UNCONSTRAINED)
	{
		builder_.interface_begin (id, l.begin.line, ik);
	}

	void interface_base (const AST::ScopedName& id, const yy::parser::location_type& l)
	{
		builder_.interface_base (id, l.begin.line);
	}

	void interface_end ()
	{
		builder_.scope_end ();
	}

private:
	AST::Builder builder_;
	yy::parser parser_;
};

inline yy::parser::symbol_type yylex (Driver& drv)
{
	return drv.yylex ();
}

#endif
