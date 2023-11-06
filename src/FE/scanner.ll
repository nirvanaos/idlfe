/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/

%option c++
%option noyywrap
%option yylineno
%option yyclass="FE::Driver"

%{
#include "Driver.h"
#undef  YY_DECL
#define YY_DECL yy::parser::symbol_type FE::Driver::yylex_sym()
#define yyterminate() return yy::parser::make_YYEOF(location ())
%}

/*--------------------------------------------------------------------------*/

Digits                  [0-9]+
Oct_Digit               [0-7]
Hex_Digit               [a-fA-F0-9]
Int_Literal             [1-9][0-9]*
Oct_Literal             0{Oct_Digit}*
Hex_Literal             (0x|0X){Hex_Digit}*
Esc_Sequence1           "\\"[ntvbrfa\\\?\'\"]
Esc_Sequence2           "\\"{Oct_Digit}{1,3}
Esc_Sequence3           "\\"(x|X){Hex_Digit}{1,2}
Esc_Sequence            ({Esc_Sequence1}|{Esc_Sequence2}|{Esc_Sequence3})
Char                    ([^\n\t\"\'\\]|{Esc_Sequence})
Char_Literal            "'"({Char}|\")"'"
WChar_Literal           "L"{Char_Literal}
String_Literal          \"({Char}|"'")*\"
WString_Literal         "L"{String_Literal}
Float_Literal1          {Digits}"."{Digits}?(e|E)("+"|"-")?{Digits}  
Float_Literal2          {Digits}(e|E)("+"|"-")?{Digits}
Float_Literal3          {Digits}"."{Digits}
Float_Literal4          {Digits}"."
Float_Literal5          "."{Digits} 
Float_Literal6          "."{Digits}(e|E)("+"|"-")?{Digits}  
Fixed_Literal1          {Digits}(d|D)
Fixed_Literal2          {Digits}"."(d|D)
Fixed_Literal3          "."{Digits}(d|D)
Fixed_Literal4          {Digits}"."{Digits}(d|D)

/*--------------------------------------------------------------------------*/

CORBA_Identifier	_?[a-zA-Z][a-zA-Z0-9_]*

/*--------------------------------------------------------------------------*/

%%

[ \t]			;
[\n]			;
"//"[^\n]*		;
"#pragma"[^\n]*\n {
  pragma (YYText (), yy::location (file (), lineno () - 1));
}
"#"[^\n]*\n {
  preprocessor_directive (YYText ());
}
"{"  return yy::parser::make_T_LEFT_CURLY_BRACKET (location ());
"}"  return yy::parser::make_T_RIGHT_CURLY_BRACKET (location ());
"["  return yy::parser::make_T_LEFT_SQUARE_BRACKET (location ());
"]"  return yy::parser::make_T_RIGHT_SQUARE_BRACKET (location ());
"("  return yy::parser::make_T_LEFT_PARANTHESIS (location ());
")"  return yy::parser::make_T_RIGHT_PARANTHESIS (location ());
":"  return yy::parser::make_T_COLON (location ());
","  return yy::parser::make_T_COMMA (location ());
";"  return yy::parser::make_T_SEMICOLON (location ());
"="  return yy::parser::make_T_EQUAL (location ());
">>" return yy::parser::make_T_SHIFTRIGHT (location ());
"<<" return yy::parser::make_T_SHIFTLEFT (location ());
"+"  return yy::parser::make_T_PLUS_SIGN (location ());
"-"  return yy::parser::make_T_MINUS_SIGN (location ());
"*"  return yy::parser::make_T_ASTERISK (location ());
"/"  return yy::parser::make_T_SOLIDUS (location ());
"%"  return yy::parser::make_T_PERCENT_SIGN (location ());
"~"  return yy::parser::make_T_TILDE (location ());
"|"  return yy::parser::make_T_VERTICAL_LINE (location ());
"^"  return yy::parser::make_T_CIRCUMFLEX (location ());
"&"  return yy::parser::make_T_AMPERSAND (location ());
"<"  return yy::parser::make_T_LESS_THAN_SIGN (location ());
">"  return yy::parser::make_T_GREATER_THAN_SIGN (location ());
"::" return yy::parser::make_T_SCOPE (location ());

const       return yy::parser::make_T_const (location ());
typedef     return yy::parser::make_T_typedef (location ());
float       return yy::parser::make_T_float (location ());
double      return yy::parser::make_T_double (location ());
char        return yy::parser::make_T_char (location ());
wchar       return yy::parser::make_T_wchar (location ());
fixed       return yy::parser::make_T_fixed (location ());
boolean     return yy::parser::make_T_boolean (location ());
string      return yy::parser::make_T_string (location ());
wstring     return yy::parser::make_T_wstring (location ());
void        return yy::parser::make_T_void (location ());
unsigned    return yy::parser::make_T_unsigned (location ());
long        return yy::parser::make_T_long (location ());
short       return yy::parser::make_T_short (location ());
FALSE       return yy::parser::make_T_FALSE (location ());
TRUE        return yy::parser::make_T_TRUE (location ());
struct      return yy::parser::make_T_struct (location ());
union       return yy::parser::make_T_union (location ());
switch      return yy::parser::make_T_switch (location ());
case        return yy::parser::make_T_case (location ());
default     return yy::parser::make_T_default (location ());
enum        return yy::parser::make_T_enum (location ());
in          return yy::parser::make_T_in (location ());
out         return yy::parser::make_T_out (location ());
interface   return yy::parser::make_T_interface (location ());
abstract    return yy::parser::make_T_abstract (location ());
local       return yy::parser::make_T_local (location ());
valuetype   return yy::parser::make_T_valuetype (location ());
truncatable return yy::parser::make_T_truncatable (location ());
supports    return yy::parser::make_T_supports (location ());
custom      return yy::parser::make_T_custom (location ());
public      return yy::parser::make_T_public (location ());
private     return yy::parser::make_T_private (location ());
factory     return yy::parser::make_T_factory (location ());
native      return yy::parser::make_T_native (location ());
ValueBase   return yy::parser::make_T_ValueBase (location ());
Object      return yy::parser::make_T_Object (location ());
module      return yy::parser::make_T_module (location ());
octet       return yy::parser::make_T_octet (location ());
any         return yy::parser::make_T_any (location ());
sequence    return yy::parser::make_T_sequence (location ());
readonly    return yy::parser::make_T_readonly (location ());
attribute   return yy::parser::make_T_attribute (location ());
exception   return yy::parser::make_T_exception (location ());
oneway      return yy::parser::make_T_oneway (location ());
inout       return yy::parser::make_T_inout (location ());
raises      return yy::parser::make_T_raises (location ());
context     return yy::parser::make_T_context (location ());
typeid      return yy::parser::make_T_typeid (location ());
typeprefix  return yy::parser::make_T_typeprefix (location ());
getraises   return yy::parser::make_T_getraises (location ());
setraises   return yy::parser::make_T_setraises (location ());
pseudo      return yy::parser::make_T_pseudo (location ());

{CORBA_Identifier} return yy::parser::make_T_identifier (AST::Identifier (YYText (), YYLeng ()), location ());
{Float_Literal1} |
{Float_Literal2} |
{Float_Literal3} |
{Float_Literal4} |
{Float_Literal5} |
{Float_Literal6}   return yy::parser::make_T_floating_point_literal (std::string (YYText (), YYLeng ()), location ());
{Fixed_Literal1} |
{Fixed_Literal2} |
{Fixed_Literal3} |
{Fixed_Literal4}   return yy::parser::make_T_fixed_point_literal (std::string (YYText (), YYLeng ()), location ());
{Int_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), location ());
{Oct_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), location ());
{Hex_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), location ());
{Char_Literal}     return yy::parser::make_T_character_literal (std::string (YYText (), YYLeng ()), location ());
{WChar_Literal}    return yy::parser::make_T_wide_character_literal (std::string (YYText (), YYLeng ()), location ());
{String_Literal}   return yy::parser::make_T_string_literal (std::string (YYText (), YYLeng ()), location ());
{WString_Literal}  return yy::parser::make_T_wide_string_literal (std::string (YYText (), YYLeng ()), location ());
.                       {
                          return yy::parser::make_T_UNKNOWN (location ());
                        }

%%
