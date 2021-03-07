/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

%{
#include "Driver.h"

inline AST::Location::Location (const yy::location& loc) :
	file_ (loc.begin.filename),
	line_ (loc.begin.line)
{}

%}

%require "3.7.1"
%language "c++"
%define api.value.type variant
%define api.token.constructor

%code requires {
namespace FE {
class Driver;
}
#include "../AST/Builder/Declarators.h"
#include "../AST/ScopedName.h"
#include "../AST/Variant.h"
#include "../AST/Parameter.h"
}

%param {FE::Driver& drv}
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%token T_AMPERSAND
%token T_ANY
%token T_ASTERISK
%token T_ATTRIBUTE
%token T_BOOLEAN
%token T_CASE
%token T_CHAR
%token <std::string> T_CHARACTER_LITERAL
%token <std::string> T_WCHARACTER_LITERAL
%token T_CIRCUMFLEX
%token T_COLON
%token T_COMMA
%token T_CONST
%token T_CONTEXT
%token T_DEFAULT
%token T_DOUBLE
%token T_ENUM
%token T_EQUAL
%token T_EXCEPTION
%token T_FALSE
%token T_FIXED
%token <std::string> T_FIXED_PT_LITERAL
%token T_FLOAT
%token <std::string> T_FLOATING_PT_LITERAL
%token T_GREATER_THAN_SIGN
%token <std::string> T_IDENTIFIER
%token T_IN
%token T_INOUT
%token <std::string> T_INTEGER_LITERAL
%token T_INTERFACE
%token T_LEFT_CURLY_BRACKET
%token T_LEFT_PARANTHESIS
%token T_LEFT_SQUARE_BRACKET
%token T_LESS_THAN_SIGN
%token T_LONG
%token T_MINUS_SIGN
%token T_MODULE
%token T_OCTET
%token T_ONEWAY
%token T_OUT
%token T_PERCENT_SIGN
%token T_PLUS_SIGN
%token T_RAISES
%token T_READONLY
%token T_RIGHT_CURLY_BRACKET
%token T_RIGHT_PARANTHESIS
%token T_RIGHT_SQUARE_BRACKET
%token T_SCOPE
%token T_SEMICOLON
%token T_SEQUENCE
%token T_SHIFTLEFT
%token T_SHIFTRIGHT
%token T_SHORT
%token T_SOLIDUS
%token T_STRING
%token <std::string> T_STRING_LITERAL
%token <std::string> T_WSTRING_LITERAL
%token <std::string> T_PRAGMA
%token T_STRUCT
%token T_SWITCH
%token T_TILDE
%token T_TRUE
%token T_OBJECT
%token T_TYPEDEF
%token T_UNION
%token T_UNSIGNED
%token T_VERTICAL_LINE
%token T_VOID
%token T_WCHAR
%token T_WSTRING
%token T_UNKNOWN
%token T_ABSTRACT
%token T_LOCAL
%token T_VALUETYPE
%token T_TRUNCATABLE
%token T_SUPPORTS
%token T_CUSTOM
%token T_PUBLIC
%token T_PRIVATE
%token T_FACTORY
%token T_NATIVE
%token T_VALUEBASE
%token T_TYPEID
%token T_TYPEPREFIX

%nterm <AST::Variant> T_string_literal;
%nterm <AST::Variant> T_wstring_literal;

%nterm <AST::ScopedName> scoped_name;
%nterm <AST::ScopedNames> scoped_names;

%nterm <AST::BasicType> base_type_spec;
%nterm <AST::BasicType> floating_pt_type;
%nterm <AST::BasicType> integer_type;
%nterm <AST::BasicType> signed_int;
%nterm <AST::BasicType> unsigned_int;

%nterm <AST::Type> fixed_pt_type;

%nterm <AST::Type> string_type;
%nterm <AST::Type> wide_string_type;

%nterm <AST::Type> type_spec;
%nterm <AST::Type> simple_type_spec;
%nterm <AST::Type> param_type_spec;
%nterm <AST::Type> op_type_spec;
%nterm <AST::Type> template_type_spec;
%nterm <AST::Type> sequence_type;

%nterm <AST::Type> constr_type_spec;
%nterm <AST::Type> struct_type;
%nterm <AST::Type> union_type;
%nterm <AST::Type> switch_type_spec;
%nterm <AST::Type> enum_type;
%nterm <AST::Type> const_type;

%nterm <AST::Variant> const_exp;
%nterm <AST::Variant> or_expr;
%nterm <AST::Variant> xor_expr;
%nterm <AST::Variant> and_expr;
%nterm <AST::Variant> shift_expr;
%nterm <AST::Variant> add_expr;
%nterm <AST::Variant> mult_expr;
%nterm <AST::Variant> unary_expr;
%nterm <AST::Variant> primary_expr;
%nterm <AST::Variant> literal;
%nterm <unsigned> positive_int_const;
%nterm <unsigned> fixed_array_size;

%nterm <AST::Build::FixedArraySizes> fixed_array_sizes;
%nterm <AST::Build::Declarator> array_declarator;
%nterm <AST::Build::Declarator> complex_declarator;
%nterm <AST::Build::Declarator> declarator;
%nterm <AST::Build::SimpleDeclarator> simple_declarator;
%nterm <AST::Build::Declarators> declarators;
%nterm <AST::Build::SimpleDeclarators> simple_declarators;

%nterm <bool> op_attribute;

%nterm <AST::Parameter::Attribute> param_attribute;

%nterm <AST::Build::Variants> string_literals;

%%

specification
	: /*empty*/
	| definitions
	;

	
definitions
	: definition
	| definition definitions
	;

definition
	: type_dcl T_SEMICOLON
	| const_dcl T_SEMICOLON
	| except_dcl T_SEMICOLON
	| interface T_SEMICOLON
	| module T_SEMICOLON
	| value T_SEMICOLON
	| type_id_dcl T_SEMICOLON
	| type_prefix_dcl T_SEMICOLON
	;

module
	: T_MODULE simple_declarator { drv.module_begin ($2); }
	T_LEFT_CURLY_BRACKET
		definitions 
	T_RIGHT_CURLY_BRACKET { drv.module_end (); }
	;

interface
	: interface_dcl
	| forward_dcl
	;

interface_dcl
	: interface_header
	T_LEFT_CURLY_BRACKET
		interface_body
	T_RIGHT_CURLY_BRACKET { drv.interface_end (); }
	;

forward_dcl
	: T_INTERFACE simple_declarator { drv.interface_decl ($2); }
	| T_ABSTRACT T_INTERFACE simple_declarator { drv.interface_decl ($3, AST::InterfaceKind::ABSTRACT); }
	| T_LOCAL T_INTERFACE simple_declarator { drv.interface_decl ($3, AST::InterfaceKind::LOCAL); }
	;

interface_header
	: T_INTERFACE simple_declarator { drv.interface_begin ($2); }
	| T_INTERFACE simple_declarator { drv.interface_begin ($2); } interface_inheritance_spec
	| T_ABSTRACT T_INTERFACE simple_declarator { drv.interface_begin ($3, AST::InterfaceKind::ABSTRACT); }
	| T_ABSTRACT T_INTERFACE simple_declarator { drv.interface_begin ($3, AST::InterfaceKind::ABSTRACT); } interface_inheritance_spec
	| T_LOCAL T_INTERFACE simple_declarator { drv.interface_begin ($3, AST::InterfaceKind::LOCAL); }
	| T_LOCAL T_INTERFACE simple_declarator { drv.interface_begin ($3, AST::InterfaceKind::LOCAL); } interface_inheritance_spec
	;

interface_body
	: /*empty*/
	| exports
	;

exports
	: export
	| export exports
	;

export
	: type_dcl T_SEMICOLON
	| const_dcl T_SEMICOLON 
	| except_dcl T_SEMICOLON
	| attr_dcl T_SEMICOLON
	| op_dcl T_SEMICOLON 
	| type_id_dcl T_SEMICOLON
	| type_prefix_dcl T_SEMICOLON
	;

interface_inheritance_spec
	: T_COLON scoped_names { drv.interface_bases ($2); }
	;

scoped_names
	: scoped_name { $$ = AST::ScopedNames (1, $1); }
	| scoped_name T_COMMA scoped_names { $$ = $3; $$.push_front ($1); }
	;

scoped_name
	: T_IDENTIFIER { $$ = AST::ScopedName (@1, false, $1); }
	| T_SCOPE T_IDENTIFIER { $$ = AST::ScopedName (@2, true, $2); }
	| scoped_name T_SCOPE T_IDENTIFIER { $$ = $1; $$.push_back ($3); }
	;

value
	: value_dcl
	| value_abs_dcl
	| value_box_dcl
	| value_forward_dcl
	;

value_forward_dcl
	: T_VALUETYPE T_IDENTIFIER
	| T_ABSTRACT T_VALUETYPE T_IDENTIFIER
	;

value_box_dcl
	: T_VALUETYPE T_IDENTIFIER type_spec
	;

value_abs_dcl
	: T_ABSTRACT T_VALUETYPE T_IDENTIFIER
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	| T_ABSTRACT T_VALUETYPE T_IDENTIFIER value_inheritance_spec
		T_LEFT_CURLY_BRACKET value_body T_RIGHT_CURLY_BRACKET
	;

value_body
	: /*empty*/
	| exports
	;

value_dcl
	: value_header T_LEFT_CURLY_BRACKET value_elements
		T_RIGHT_CURLY_BRACKET
	| value_header T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	;

value_elements
	: value_element
	| value_element value_elements
	;

value_header
	: T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	| T_VALUETYPE T_IDENTIFIER
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER
	;

value_inheritance_spec
	: T_COLON value_inheritance_bases
	| T_COLON value_inheritance_bases T_SUPPORTS scoped_names
	| T_SUPPORTS scoped_names
	;

value_inheritance_bases
	: value_name
	| value_name T_COMMA value_names
	| T_TRUNCATABLE value_name
	| T_TRUNCATABLE value_name T_COMMA value_names
	;

value_names
	: scoped_names
	;

value_name
	: scoped_name
	;

value_element
	: export
	| state_member
	| init_dcl
	;

state_member
	: T_PUBLIC type_spec declarators T_SEMICOLON
	| T_PRIVATE type_spec declarators T_SEMICOLON
	;

init_dcl
	: T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
		T_SEMICOLON
	;

init_param_decls
	: init_param_decl
	| init_param_decl T_COMMA init_param_decls
	;

init_param_decl
	: init_param_attribute param_type_spec simple_declarator
	;

init_param_attribute
	: T_IN
	;

const_dcl
	: T_CONST const_type simple_declarator T_EQUAL { drv.eval_push ($2, @2); } const_exp { drv.constant ($2, $3, std::move ($6), @6); }
	;

const_type
	: integer_type { $$ = $1; }
	| char_type { $$ = AST::BasicType::CHAR; }
	| wide_char_type { $$ = AST::BasicType::WCHAR; }
	| boolean_type { $$ = AST::BasicType::BOOLEAN; }
	| floating_pt_type { $$ = $1; }
	| string_type
	| wide_string_type
	| fixed_pt_const_type { $$ = AST::Type::make_fixed (0, 0); }
	| scoped_name { $$ = drv.lookup_type ($1); }
	| octet_type { $$ = AST::BasicType::OCTET; }
	;

const_exp
	: or_expr
	;

or_expr
	: xor_expr
	| or_expr T_VERTICAL_LINE xor_expr { $$ = drv.eval ().expr ($1, '|', $3, @2); }
	;

xor_expr
	: and_expr
	| xor_expr T_CIRCUMFLEX and_expr { $$ = drv.eval ().expr ($1, '^', $3, @2); }
	;

and_expr
	: shift_expr
	| and_expr T_AMPERSAND shift_expr { $$ = drv.eval ().expr ($1, '&', $3, @2); }
	;

shift_expr
	: add_expr
	| shift_expr T_SHIFTRIGHT add_expr { $$ = drv.eval ().expr ($1, '>', $3, @2); }
	| shift_expr T_SHIFTLEFT add_expr { $$ = drv.eval ().expr ($1, '<', $3, @2); }
	;

add_expr
	: mult_expr
	| add_expr T_PLUS_SIGN mult_expr { $$ = drv.eval ().expr ($1, '+', $3, @2); }
	| add_expr T_MINUS_SIGN mult_expr { $$ = drv.eval ().expr ($1, '-', $3, @2); }
	;

mult_expr
	: unary_expr
	| mult_expr T_ASTERISK unary_expr { $$ = drv.eval ().expr ($1, '*', $3, @2); }
	| mult_expr T_SOLIDUS unary_expr { $$ = drv.eval ().expr ($1, '/', $3, @2); }
	| mult_expr T_PERCENT_SIGN unary_expr { $$ = drv.eval ().expr ($1, '%', $3, @2); }
	;

unary_expr
	: T_MINUS_SIGN primary_expr { $$ = drv.eval ().expr ('-', $2, @1); }
	| T_PLUS_SIGN primary_expr { $$ = drv.eval ().expr ('+', $2, @1); }
	| T_TILDE primary_expr { $$ = drv.eval ().expr ('~', $2, @1); }
	| primary_expr
	;

primary_expr
	: scoped_name { $$ = drv.eval ().constant ($1); }
	| literal { $$ = $1; }
	| T_LEFT_PARANTHESIS const_exp T_RIGHT_PARANTHESIS { $$ = $2; }
	;

literal
	: T_INTEGER_LITERAL { $$ = drv.eval ().literal_int ($1, @1); }
	| T_string_literal { $$ = $1; }
	| T_wstring_literal { $$ = $1; }
	| T_CHARACTER_LITERAL { $$ = drv.eval ().literal_char ($1, @1); }
	| T_WCHARACTER_LITERAL { $$ = drv.eval ().literal_wchar ($1, @1); }
	| T_FIXED_PT_LITERAL { $$ = drv.eval ().literal_fixed ($1, @1); }
	| T_FLOATING_PT_LITERAL { $$ = drv.eval ().literal_float ($1, @1); }
	| T_TRUE { $$ = drv.eval ().literal_boolean (true, @1); }
	| T_FALSE { $$ = drv.eval ().literal_boolean (false, @1); }
	;

positive_int_const
	: { drv.eval_push (AST::BasicType::ULONG, AST::Location ()); } const_exp { $$ = drv.positive_int ($2, @1); drv.eval_pop (); }
	;

type_dcl
	: T_TYPEDEF type_spec declarators { drv.type_def ($2, $3); }
	| struct_type
	| union_type
	| enum_type
	| T_NATIVE simple_declarator { drv.native ($2); }
	;

type_spec
	: simple_type_spec { $$ = $1; }
	| constr_type_spec
	;

simple_type_spec
	: base_type_spec { $$ = $1; }
  | template_type_spec { $$ = $1; }
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

base_type_spec
	: floating_pt_type { $$ = $1; }
	| integer_type { $$ = $1; }
	| char_type { $$ = AST::BasicType::CHAR; }
	| wide_char_type { $$ = AST::BasicType::WCHAR; }
	| boolean_type { $$ = AST::BasicType::BOOLEAN; }
	| octet_type { $$ = AST::BasicType::OCTET; }
	| any_type { $$ = AST::BasicType::ANY; }
	| object_type { $$ = AST::BasicType::OBJECT; }
	| value_base_type { $$ = AST::BasicType::VALUE_BASE; }
	;

template_type_spec
	: sequence_type
	| string_type
	| wide_string_type
	| fixed_pt_type
	;

constr_type_spec
	: struct_type
	| union_type
	| enum_type
	;

declarators
	: declarator { $$ = AST::Build::Declarators (1, $1); }
	| declarator T_COMMA declarators { $$ = $3; $$.push_front ($1); }
	;

declarator
	: simple_declarator { $$ = $1; }
	| complex_declarator
	;

simple_declarator
	: T_IDENTIFIER { $$ = AST::Build::SimpleDeclarator ($1, @1); }
	;

complex_declarator
	: array_declarator
	;

floating_pt_type
	: T_FLOAT { $$ = AST::BasicType::FLOAT; }
	| T_DOUBLE { $$ = AST::BasicType::DOUBLE; }
	| T_LONG T_DOUBLE { $$ = AST::BasicType::LONGDOUBLE; }
	;

integer_type
	: signed_int
	| unsigned_int
	;

signed_int
	: signed_long_int { $$ = AST::BasicType::LONG; }
	| signed_short_int { $$ = AST::BasicType::SHORT; }
	| signed_longlong_int { $$ = AST::BasicType::LONGLONG; }
	;

signed_short_int
	: T_SHORT
	;

signed_long_int
	: T_LONG
	;

signed_longlong_int
	: T_LONG T_LONG
	;

unsigned_int
	: unsigned_long_int { $$ = AST::BasicType::ULONG; }
	| unsigned_short_int { $$ = AST::BasicType::USHORT; }
	| unsigned_longlong_int { $$ = AST::BasicType::ULONGLONG; }
	;

unsigned_short_int
	: T_UNSIGNED T_SHORT
	;

unsigned_long_int
	: T_UNSIGNED T_LONG
	;

unsigned_longlong_int
	: T_UNSIGNED T_LONG T_LONG
	;

char_type
	: T_CHAR
	;

wide_char_type
	: T_WCHAR
	;

boolean_type
	: T_BOOLEAN
	;

octet_type
	: T_OCTET
	;

any_type
	: T_ANY
	;

object_type
	: T_OBJECT
	;

struct_type
	: T_STRUCT simple_declarator
	T_LEFT_CURLY_BRACKET { drv.struct_begin ($2); }
		member_list
	T_RIGHT_CURLY_BRACKET
	{ $$ = drv.struct_end (); }
	| T_STRUCT simple_declarator { drv.struct_decl ($2); }
	;

member_list
	: member
	| member member_list
	;

member
	: type_spec declarators T_SEMICOLON { drv.member ($1, $2); }
	;

union_type
	: T_UNION simple_declarator T_SWITCH T_LEFT_PARANTHESIS switch_type_spec T_RIGHT_PARANTHESIS { drv.union_begin ($2, $5, @5); }
	T_LEFT_CURLY_BRACKET
		switch_body
	T_RIGHT_CURLY_BRACKET { $$ = drv.union_end (); }
	| T_UNION simple_declarator { drv.union_decl ($2); }
	; 

switch_type_spec
	: integer_type { $$ = AST::Type ($1); }
	| char_type { $$ = AST::BasicType::CHAR; }
	| boolean_type { $$ = AST::BasicType::BOOLEAN; }
	| enum_type
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

switch_body
	: case
	| case switch_body
	;

case	
	: case_label case
	| case_label element_spec T_SEMICOLON
	;

case_label
	: T_CASE const_exp T_COLON { drv.union_label ($2, @2); }
	| T_DEFAULT T_COLON { drv.union_default (@1); }
	;

element_spec
	: type_spec declarator { drv.union_element ($1, $2); }
	;

/*78*/
enum_type
	: T_ENUM simple_declarator
	T_LEFT_CURLY_BRACKET
		simple_declarators
	T_RIGHT_CURLY_BRACKET { $$ = drv.enum_type ($2, $4); }
	;

sequence_type
	: T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_COMMA
		positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_sequence ($3, $5); }
	| T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_GREATER_THAN_SIGN { $$ = AST::Type::make_sequence ($3); }
	;

string_type
	: T_STRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_string (); }
	| T_STRING { $$ = AST::Type::make_string (); }
	;

wide_string_type
	: T_WSTRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_wstring (); }
	| T_WSTRING { $$ = AST::Type::make_wstring (); }
	;

array_declarator
	: T_IDENTIFIER fixed_array_sizes { $$ = AST::Build::Declarator ($1, @1, $2); }
	;

fixed_array_sizes
	: fixed_array_size { $$ = AST::FixedArraySizes (1, $1); }
	| fixed_array_size fixed_array_sizes { $$ = $2; $$.push_front ($1); }
	;

fixed_array_size
	: T_LEFT_SQUARE_BRACKET positive_int_const T_RIGHT_SQUARE_BRACKET { $$ = $2; }
	;

attr_dcl
	: T_ATTRIBUTE param_type_spec simple_declarators { drv.attribute (false, $2, $3); }
	| T_READONLY T_ATTRIBUTE param_type_spec simple_declarators { drv.attribute (true, $3, $4); }
	; 

simple_declarators
	: simple_declarator { $$ = AST::Build::SimpleDeclarators (1, $1); }
	| simple_declarator T_COMMA simple_declarators { $$ = $3; $$.push_front ($1); }
	;

except_dcl
	: T_EXCEPTION simple_declarator
	T_LEFT_CURLY_BRACKET { drv.exception_begin ($2); }
		members
	T_RIGHT_CURLY_BRACKET { drv.exception_end (); }
	;

members
	: /*empty*/
	| member members
	;

op_dcl
	: op_attribute op_type_spec simple_declarator { drv.operation_begin ($1, $2, $3); }
	parameter_dcls raises_expr context_expr { drv.operation_end (); }
	;

op_attribute
	: /*empty*/ { $$ = false; }
	| T_ONEWAY{ $$ = true; }
	;

op_type_spec	
	: param_type_spec { $$ = $1; }
	| T_VOID { $$ = AST::Type (); }
	;

parameter_dcls
	: T_LEFT_PARANTHESIS param_dcls T_RIGHT_PARANTHESIS
	| T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
	;

param_dcls
	: param_dcl
	| param_dcl T_COMMA param_dcls
	;

param_dcl
	: param_attribute param_type_spec simple_declarator { drv.operation_parameter ($1, $2, $3); }
	;

param_attribute
	: T_IN { $$ = AST::Parameter::Attribute::IN; }
	| T_OUT { $$ = AST::Parameter::Attribute::IN; }
	| T_INOUT { $$ = AST::Parameter::Attribute::IN; }
	;

raises_expr
	: /*empty*/
	| T_RAISES T_LEFT_PARANTHESIS scoped_names T_RIGHT_PARANTHESIS { drv.operation_raises ($3); }
	;

context_expr
	: /*empty*/
	| T_CONTEXT T_LEFT_PARANTHESIS { drv.eval_push (AST::Type::make_string (), AST::Location ()); }
			string_literals
		T_RIGHT_PARANTHESIS { drv.eval_pop (); drv.operation_context ($4); }
	;

string_literals
	: T_string_literal { $$ = AST::Build::Variants (1, $1); }
	| T_string_literal T_COMMA string_literals { $$ = $3; $$.push_front ($1); }
	;

T_string_literal
	: T_STRING_LITERAL { $$ = drv.eval ().literal_string ($1, @1);  }
	| T_STRING_LITERAL T_string_literal { $$ = drv.eval ().literal_string ($1, @1, &$2); }
	;

T_wstring_literal
	: T_WSTRING_LITERAL { $$ = drv.eval ().literal_wstring ($1, @1); }
	| T_WSTRING_LITERAL T_wstring_literal { $$ = drv.eval ().literal_wstring ($1, @1, &$2); }
	;

param_type_spec
	: base_type_spec { $$ = $1; }
	| string_type { $$ = $1; }
	| wide_string_type { $$ = $1; }
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

fixed_pt_type
	: T_FIXED T_LESS_THAN_SIGN positive_int_const T_COMMA
	positive_int_const T_GREATER_THAN_SIGN { $$ = drv.fixed_pt_type ($3, $5, @1); }
	;

fixed_pt_const_type
	: T_FIXED
	;

value_base_type
	: T_VALUEBASE
	;

type_id_dcl
	: T_TYPEID scoped_name T_string_literal

type_prefix_dcl
	: T_TYPEPREFIX scoped_name T_string_literal

%%

namespace yy {

// Report an error to the user.
void parser::error (const location& l, const std::string& msg)
{
	drv.parser_error (l, msg);
}

}
