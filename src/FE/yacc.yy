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
%}

%require "3.7.1"
%language "c++"
%define api.value.type variant
%define api.token.constructor

%code requires {
	class Driver;
#include "../AST/ScopedName.h"
#include "../AST/Variant.h"
#include "../AST/Declarators.h"
#include "../AST/Parameter.h"
}

%param {Driver& drv}
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

%nterm <AST::FixedArraySizes> fixed_array_sizes;
%nterm <AST::Declarator> array_declarator;
%nterm <AST::Declarator> complex_declarator;
%nterm <AST::Declarator> declarator;
%nterm <AST::Declarator> simple_declarator;
%nterm <AST::Declarators> declarators;
%nterm <AST::Declarators> simple_declarators;

%nterm <bool> op_attribute;

%nterm <AST::Parameter::Attribute> param_attribute;

%%

/*1*/
specification
	: /*empty*/
	| definitions
	;

	
definitions
	: definition
	| definition definitions
	;

/*2*/
definition
	: type_dcl T_SEMICOLON
	| const_dcl T_SEMICOLON
	| except_dcl T_SEMICOLON
	| interface T_SEMICOLON
	| module T_SEMICOLON
	| value T_SEMICOLON
	;

/*3*/
module
	: T_MODULE T_IDENTIFIER { drv.module_begin ($2, @1.begin.line); }
	T_LEFT_CURLY_BRACKET
		definitions 
	T_RIGHT_CURLY_BRACKET { drv.module_end (); }
	;

/*4*/
interface
	: interface_dcl
	| forward_dcl
	;

/*5*/
interface_dcl
	: interface_header
	T_LEFT_CURLY_BRACKET
		interface_body
	T_RIGHT_CURLY_BRACKET { drv.interface_end (); }
	;

/*6*/
forward_dcl
	: T_INTERFACE T_IDENTIFIER { drv.interface_decl ($2, @1.begin.line); }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER { drv.interface_decl ($3, @1.begin.line, AST::InterfaceKind::ABSTRACT); }
	| T_LOCAL T_INTERFACE T_IDENTIFIER { drv.interface_decl ($3, @1.begin.line, AST::InterfaceKind::LOCAL); }
	;

/*7*/
interface_header
	: T_INTERFACE T_IDENTIFIER { drv.interface_begin ($2, @1.begin.line); }
	| T_INTERFACE T_IDENTIFIER interface_inheritance_spec { drv.interface_begin ($2, @1.begin.line); }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER { drv.interface_begin ($3, @1.begin.line, AST::InterfaceKind::ABSTRACT); }
	| T_ABSTRACT T_INTERFACE T_IDENTIFIER interface_inheritance_spec{ drv.interface_begin ($3, @1.begin.line, AST::InterfaceKind::ABSTRACT); }
	| T_LOCAL T_INTERFACE T_IDENTIFIER { drv.interface_begin ($3, @1.begin.line, AST::InterfaceKind::LOCAL); }
	| T_LOCAL T_INTERFACE T_IDENTIFIER interface_inheritance_spec { drv.interface_begin ($3, @1.begin.line, AST::InterfaceKind::LOCAL); }
	;

/*8*/
interface_body
	: /*empty*/
	| exports
	;

exports
	: export
	| export exports
	;

/*9*/
export
	: type_dcl T_SEMICOLON
	| const_dcl T_SEMICOLON 
	| except_dcl T_SEMICOLON
	| attr_dcl T_SEMICOLON
	| op_dcl T_SEMICOLON 
	;

/*10*/
interface_inheritance_spec
	: T_COLON scoped_names { drv.interface_bases ($2); }
	;

scoped_names
	: scoped_name { $$ = AST::ScopedNames (1, $1); }
	| scoped_name T_COMMA scoped_names { $$ = $3; $$.push_front ($1); }
	;

/*12*/
scoped_name
	: T_IDENTIFIER { $$ = AST::ScopedName (AST::Location (drv.file (), @1.begin.line), false, $1); }
	| T_SCOPE T_IDENTIFIER { $$ = AST::ScopedName (AST::Location (drv.file (), @2.begin.line), true, $2); }
	| scoped_name T_SCOPE T_IDENTIFIER { $$ = $1; $$.push_back ($3); }
	;

/*13*/
value
	: value_dcl
	| value_abs_dcl
	| value_box_dcl
	| value_forward_dcl
	;

/*14*/
value_forward_dcl
	: T_VALUETYPE T_IDENTIFIER
	| T_ABSTRACT T_VALUETYPE T_IDENTIFIER
	;

/*15*/
value_box_dcl
	: T_VALUETYPE T_IDENTIFIER type_spec
	;

/*16*/
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

/*17*/
value_dcl
	: value_header T_LEFT_CURLY_BRACKET value_elements
		T_RIGHT_CURLY_BRACKET
	| value_header T_LEFT_CURLY_BRACKET T_RIGHT_CURLY_BRACKET
	;

value_elements
	: value_element
	| value_element value_elements
	;

/*18*/
value_header
	: T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER value_inheritance_spec
	| T_VALUETYPE T_IDENTIFIER
	| T_CUSTOM T_VALUETYPE T_IDENTIFIER
	;

/*19*/
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

/*20*/
value_name
	: scoped_name
	;

/*21*/
value_element
	: export
	| state_member
	| init_dcl
	;

/*22*/
state_member
	: T_PUBLIC type_spec declarators T_SEMICOLON
	| T_PRIVATE type_spec declarators T_SEMICOLON
	;

/*23*/
init_dcl
	: T_FACTORY T_IDENTIFIER
		T_LEFT_PARANTHESIS init_param_decls T_RIGHT_PARANTHESIS
		T_SEMICOLON
	;

/*24*/
init_param_decls
	: init_param_decl
	| init_param_decl T_COMMA init_param_decls
	;

/*25*/
init_param_decl
	: init_param_attribute param_type_spec simple_declarator
	;

/*26*/
init_param_attribute
	: T_IN
	;

/*27*/
const_dcl
	: T_CONST const_type T_IDENTIFIER T_EQUAL const_exp
	;

/*28*/
const_type
	: integer_type
	| char_type
	| wide_char_type
	| boolean_type
	| floating_pt_type
	| string_type
	| wide_string_type
	| fixed_pt_const_type
	| scoped_name 
	| octet_type
	;

/*29*/
const_exp
	: or_expr
	;

/*30*/
or_expr
	: xor_expr
	| or_expr T_VERTICAL_LINE xor_expr { $$ = drv.eval ().expr_or ($1, $3, @2.begin.line); }
	;

/*31*/
xor_expr
	: and_expr
	| xor_expr T_CIRCUMFLEX and_expr { $$ = drv.eval ().expr_xor ($1, $3, @2.begin.line); }
	;

/*32*/
and_expr
	: shift_expr
	| and_expr T_AMPERSAND shift_expr { $$ = drv.eval ().expr_and ($1, $3, @2.begin.line); }
	;

/*33*/
shift_expr
	: add_expr
	| shift_expr T_SHIFTRIGHT add_expr { $$ = drv.eval ().expr_shift_right ($1, $3, @2.begin.line); }
	| shift_expr T_SHIFTLEFT add_expr { $$ = drv.eval ().expr_shift_left ($1, $3, @2.begin.line); }
	;

/*34*/
add_expr
	: mult_expr
	| add_expr T_PLUS_SIGN mult_expr { $$ = drv.eval ().expr_add ($1, $3, @2.begin.line); }
	| add_expr T_MINUS_SIGN mult_expr { $$ = drv.eval ().expr_sub ($1, $3, @2.begin.line); }
	;

/*35*/
mult_expr
	: unary_expr
	| mult_expr T_ASTERISK unary_expr { $$ = drv.eval ().expr_mul ($1, $3, @2.begin.line); }
	| mult_expr T_SOLIDUS unary_expr { $$ = drv.eval ().expr_div ($1, $3, @2.begin.line); }
	| mult_expr T_PERCENT_SIGN unary_expr { $$ = drv.eval ().expr_rem ($1, $3, @2.begin.line); }
	;

/*36*/
/*37*/
unary_expr
	: T_MINUS_SIGN primary_expr { $$ = drv.eval ().expr_minus ($2, @1.begin.line); }
	| T_PLUS_SIGN primary_expr { $$ = drv.eval ().expr_plus ($2, @1.begin.line); }
	| T_TILDE primary_expr { $$ = drv.eval ().expr_tilde ($2, @1.begin.line); }
	| primary_expr
	;

/*38*/
primary_expr
	: scoped_name { $$ = drv.eval ().constant ($1); }
	| literal { $$ = $1; }
	| T_LEFT_PARANTHESIS const_exp T_RIGHT_PARANTHESIS { $$ = $2; }
	;

/*39*/
/*40*/
literal
	: T_INTEGER_LITERAL { $$ = drv.eval ().literal_int ($1, @1.begin.line); }
	| T_string_literal { $$ = $1; }
	| T_wstring_literal { $$ = $1; }
	| T_CHARACTER_LITERAL { $$ = drv.eval ().literal_char ($1, @1.begin.line); }
	| T_WCHARACTER_LITERAL { $$ = drv.eval ().literal_wchar ($1, @1.begin.line); }
	| T_FIXED_PT_LITERAL { $$ = drv.eval ().literal_fixed ($1, @1.begin.line); }
	| T_FLOATING_PT_LITERAL { $$ = drv.eval ().literal_float ($1, @1.begin.line); }
	| T_TRUE { $$ = drv.eval ().literal_boolean (true, @1.begin.line); }
	| T_FALSE { $$ = drv.eval ().literal_boolean (false, @1.begin.line); }
	;

/*41*/
positive_int_const
	: { drv.set_eval <AST::EvalLong> (); } const_exp { $$ = drv.positive_int ($2, @1.begin.line); }
	;

/*42*/
/*43*/
type_dcl
	: T_TYPEDEF type_spec declarators { drv.type_def ($2, $3); }
	| struct_type
	| union_type
	| enum_type
	| T_NATIVE T_IDENTIFIER { drv.native ($2, @1.begin.line); }
	;

/*44*/
type_spec
	: simple_type_spec { $$ = $1; }
	| constr_type_spec
	;

/*45*/
simple_type_spec
	: base_type_spec { $$ = $1; }
  | template_type_spec { $$ = $1; }
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

/*46*/
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

/*47*/
template_type_spec
	: sequence_type
	| string_type
	| wide_string_type
	| fixed_pt_type
	;

/*48*/
constr_type_spec
	: struct_type
	| union_type
	| enum_type
	;

/*49*/
declarators
	: declarator { $$ = AST::Declarators (1, $1); }
	| declarator T_COMMA declarators { $$ = $3; $$.push_front ($1); }
	;

/*50*/
declarator
	: simple_declarator
	| complex_declarator
	;

/*51*/
simple_declarator
	: T_IDENTIFIER { $$ = AST::Declarator ($1, AST::Location (drv.file (), @1.begin.line)); }
	;

/*52*/
complex_declarator
	: array_declarator
	;

/*53*/
floating_pt_type
	: T_FLOAT { $$ = AST::BasicType::FLOAT; }
	| T_DOUBLE { $$ = AST::BasicType::DOUBLE; }
	| T_LONG T_DOUBLE { $$ = AST::BasicType::LONGDOUBLE; }
	;

/*54*/
integer_type
	: signed_int
	| unsigned_int
	;

/*55*/
signed_int
	: signed_long_int { $$ = AST::BasicType::LONG; }
	| signed_short_int { $$ = AST::BasicType::SHORT; }
	| signed_longlong_int { $$ = AST::BasicType::LONGLONG; }
	;

/*56*/
signed_short_int
	: T_SHORT
	;

/*57*/
signed_long_int
	: T_LONG
	;

/*58*/
signed_longlong_int
	: T_LONG T_LONG
	;

/*59*/
unsigned_int
	: unsigned_long_int { $$ = AST::BasicType::ULONG; }
	| unsigned_short_int { $$ = AST::BasicType::USHORT; }
	| unsigned_longlong_int { $$ = AST::BasicType::ULONGLONG; }
	;

/*60*/
unsigned_short_int
	: T_UNSIGNED T_SHORT
	;

/*61*/
unsigned_long_int
	: T_UNSIGNED T_LONG
	;

/*62*/
unsigned_longlong_int
	: T_UNSIGNED T_LONG T_LONG
	;

/*63*/
char_type
	: T_CHAR
	;

/*64*/
wide_char_type
	: T_WCHAR
	;

/*65*/
boolean_type
	: T_BOOLEAN
	;

/*66*/
octet_type
	: T_OCTET
	;

/*67*/
any_type
	: T_ANY
	;

/*68*/
object_type
	: T_OBJECT
	;

/*69*/
struct_type
	: T_STRUCT T_IDENTIFIER
	T_LEFT_CURLY_BRACKET { drv.struct_begin ($2, @1.begin.line); }
		member_list
	T_RIGHT_CURLY_BRACKET
	{ $$ = drv.struct_end (); }
	| T_STRUCT T_IDENTIFIER { drv.struct_decl ($2, @1.begin.line); }
	;

/*70*/
member_list
	: member
	| member member_list
	;

/*71*/
member
	: type_spec declarators T_SEMICOLON
	;

/*72*/
union_type
	: T_UNION T_IDENTIFIER T_SWITCH T_LEFT_PARANTHESIS switch_type_spec T_RIGHT_PARANTHESIS { drv.union_begin ($2, $5, @1.begin.line); }
	T_LEFT_CURLY_BRACKET
		switch_body
	T_RIGHT_CURLY_BRACKET { $$ = drv.union_end (); }
	| T_UNION T_IDENTIFIER { drv.union_decl ($2, @1.begin.line); }
	; 

/*73*/
switch_type_spec
	: integer_type { $$ = AST::Type ($1); }
	| char_type { $$ = AST::BasicType::CHAR; }
	| boolean_type { $$ = AST::BasicType::BOOLEAN; }
	| enum_type
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

/*74*/
switch_body
	: case
	| case switch_body
	;

/*75*/
case	
	: case_label case
	| case_label element_spec T_SEMICOLON
	;

/*76*/
case_label
	: T_CASE const_exp T_COLON 
	| T_DEFAULT T_COLON
	;

/*77*/
element_spec
	: type_spec declarator
	;

/*78*/
enum_type
	: T_ENUM T_IDENTIFIER { drv.enum_begin ($2, @1.begin.line); }
	T_LEFT_CURLY_BRACKET
		enumerators
	T_RIGHT_CURLY_BRACKET { $$ = drv.enum_end (); }
	;

enumerators
	: enumerator
	| enumerator T_COMMA enumerators
	;

/*79*/
enumerator
	: T_IDENTIFIER { drv.enum_item ($1, @1.begin.line); }
	;

/*80*/
sequence_type
	: T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_COMMA
	positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_sequence ($3, $5); }
	| T_SEQUENCE T_LESS_THAN_SIGN simple_type_spec T_GREATER_THAN_SIGN { $$ = AST::Type::make_sequence ($3); }
	;

/*81*/
string_type
	: T_STRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_string (); }
	| T_STRING { $$ = AST::Type::make_string (); }
	;

/*82*/
wide_string_type
	: T_WSTRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN { $$ = AST::Type::make_wstring (); }
	| T_WSTRING { $$ = AST::Type::make_wstring (); }
	;

/*83*/
array_declarator
	: T_IDENTIFIER fixed_array_sizes { $$ = AST::Declarator ($1, AST::Location (drv.file (), @1.begin.line), $2); }
	;

fixed_array_sizes
	: fixed_array_size { $$ = AST::FixedArraySizes (1, $1); }
	| fixed_array_size fixed_array_sizes { $$ = $2; $$.push_front ($1); }
	;

/*84*/
fixed_array_size
	: T_LEFT_SQUARE_BRACKET positive_int_const T_RIGHT_SQUARE_BRACKET { $$ = $2; }
	;

/*85*/
attr_dcl
	: T_ATTRIBUTE param_type_spec simple_declarators
	| T_READONLY T_ATTRIBUTE param_type_spec simple_declarators
	; 

simple_declarators
	: simple_declarator { $$ = AST::Declarators (1, $1); }
	| simple_declarator T_COMMA simple_declarators { $$ = $3; $$.push_front ($1); }
	;

/*86*/
except_dcl
	: T_EXCEPTION T_IDENTIFIER T_LEFT_CURLY_BRACKET members
                                          T_RIGHT_CURLY_BRACKET
	;

members
	: /*empty*/
	| member members
	;

/*87*/
op_dcl
	: op_attribute op_type_spec T_IDENTIFIER { drv.operation_begin ($1, $2, $3, @3.begin.line); }
	parameter_dcls raises_expr context_expr { drv.operation_end (); }
	;

/*88*/
op_attribute
	: /*empty*/ { $$ = false; }
	| T_ONEWAY{ $$ = true; }
	;

/*89*/
op_type_spec	
	: param_type_spec { $$ = $1; }
	| T_VOID { $$ = AST::Type (); }
	;

/*90*/
parameter_dcls
	: T_LEFT_PARANTHESIS param_dcls T_RIGHT_PARANTHESIS
	| T_LEFT_PARANTHESIS T_RIGHT_PARANTHESIS
	;

param_dcls
	: param_dcl
	| param_dcl T_COMMA param_dcls
	;

/*91*/
param_dcl
	: param_attribute param_type_spec T_IDENTIFIER { drv.operation_parameter ($1, $2, $3, @1.begin.line); }
	;

/*92*/
param_attribute
	: T_IN { $$ = AST::Parameter::Attribute::IN; }
	| T_OUT { $$ = AST::Parameter::Attribute::IN; }
	| T_INOUT { $$ = AST::Parameter::Attribute::IN; }
	;

/*93*/
raises_expr
	: /*empty*/
	| T_RAISES T_LEFT_PARANTHESIS scoped_names T_RIGHT_PARANTHESIS { drv.operation_raises ($3); }
	;

/*94*/
context_expr
	: /*empty*/
	| T_CONTEXT T_LEFT_PARANTHESIS string_literals T_RIGHT_PARANTHESIS
	;

string_literals
	: T_string_literal
	| T_string_literal T_COMMA string_literals
	;

T_string_literal
	: T_STRING_LITERAL { $$ = drv.eval ().literal_string ($1, @1.begin.line);  }
	| T_STRING_LITERAL T_string_literal { $$ = drv.eval ().literal_string ($1, @1.begin.line, &$2); }
	;

T_wstring_literal
	: T_WSTRING_LITERAL { $$ = drv.eval ().literal_wstring ($1, @1.begin.line); }
	| T_WSTRING_LITERAL T_wstring_literal { $$ = drv.eval ().literal_wstring ($1, @1.begin.line, &$2); }
	;

/*95*/
param_type_spec
	: base_type_spec { $$ = $1; }
	| string_type { $$ = $1; }
	| wide_string_type { $$ = $1; }
	| scoped_name { $$ = drv.lookup_type ($1); }
	;

/*96*/
fixed_pt_type
	: T_FIXED T_LESS_THAN_SIGN positive_int_const T_COMMA
	positive_int_const T_GREATER_THAN_SIGN { $$ = drv.fixed_pt_type ($3, $5, @1.begin.line); }
	;

/*97*/
fixed_pt_const_type
	: T_FIXED
	;

/*98*/
value_base_type
	: T_VALUEBASE
	;

%%

namespace yy {

// Report an error to the user.
void parser::error (const location& l, const std::string& msg)
{
	drv.parser_error (l.begin.line, msg);
}

}
