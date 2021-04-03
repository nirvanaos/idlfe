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

%{
#include "yacc.tab.h"
#include "Driver.h"
%}

%option noyywrap
%option yylineno

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
WChar_Literal           "L"Char_Literal
String_Literal          \"({Char}|"'")*\"
WString_Literal         "L"String_Literal
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
  driver ().pragma (YYText (), yy::location (&driver ().file (), driver ().lineno () - 1));
}
"#"[^\n]*\n {
  driver ().preprocessor_directive (YYText ());
}
"{"  return yy::parser::make_T_LEFT_CURLY_BRACKET (driver ().location ());
"}"  return yy::parser::make_T_RIGHT_CURLY_BRACKET (driver ().location ());
"["  return yy::parser::make_T_LEFT_SQUARE_BRACKET (driver ().location ());
"]"  return yy::parser::make_T_RIGHT_SQUARE_BRACKET (driver ().location ());
"("  return yy::parser::make_T_LEFT_PARANTHESIS (driver ().location ());
")"  return yy::parser::make_T_RIGHT_PARANTHESIS (driver ().location ());
":"  return yy::parser::make_T_COLON (driver ().location ());
","  return yy::parser::make_T_COMMA (driver ().location ());
";"  return yy::parser::make_T_SEMICOLON (driver ().location ());
"="  return yy::parser::make_T_EQUAL (driver ().location ());
">>" return yy::parser::make_T_SHIFTRIGHT (driver ().location ());
"<<" return yy::parser::make_T_SHIFTLEFT (driver ().location ());
"+"  return yy::parser::make_T_PLUS_SIGN (driver ().location ());
"-"  return yy::parser::make_T_MINUS_SIGN (driver ().location ());
"*"  return yy::parser::make_T_ASTERISK (driver ().location ());
"/"  return yy::parser::make_T_SOLIDUS (driver ().location ());
"%"  return yy::parser::make_T_PERCENT_SIGN (driver ().location ());
"~"  return yy::parser::make_T_TILDE (driver ().location ());
"|"  return yy::parser::make_T_VERTICAL_LINE (driver ().location ());
"^"  return yy::parser::make_T_CIRCUMFLEX (driver ().location ());
"&"  return yy::parser::make_T_AMPERSAND (driver ().location ());
"<"  return yy::parser::make_T_LESS_THAN_SIGN (driver ().location ());
">"  return yy::parser::make_T_GREATER_THAN_SIGN (driver ().location ());
"::" return yy::parser::make_T_SCOPE (driver ().location ());

const       return yy::parser::make_T_const (driver ().location ());
typedef     return yy::parser::make_T_typedef (driver ().location ());
float       return yy::parser::make_T_float (driver ().location ());
double      return yy::parser::make_T_double (driver ().location ());
char        return yy::parser::make_T_char (driver ().location ());
wchar       return yy::parser::make_T_wchar (driver ().location ());
fixed       return yy::parser::make_T_fixed (driver ().location ());
boolean     return yy::parser::make_T_boolean (driver ().location ());
string      return yy::parser::make_T_string (driver ().location ());
wstring     return yy::parser::make_T_wstring (driver ().location ());
void        return yy::parser::make_T_void (driver ().location ());
unsigned    return yy::parser::make_T_unsigned (driver ().location ());
long        return yy::parser::make_T_long (driver ().location ());
short       return yy::parser::make_T_short (driver ().location ());
FALSE       return yy::parser::make_T_FALSE (driver ().location ());
TRUE        return yy::parser::make_T_TRUE (driver ().location ());
struct      return yy::parser::make_T_struct (driver ().location ());
union       return yy::parser::make_T_union (driver ().location ());
switch      return yy::parser::make_T_switch (driver ().location ());
case        return yy::parser::make_T_case (driver ().location ());
default     return yy::parser::make_T_default (driver ().location ());
enum        return yy::parser::make_T_enum (driver ().location ());
in          return yy::parser::make_T_in (driver ().location ());
out         return yy::parser::make_T_out (driver ().location ());
interface   return yy::parser::make_T_interface (driver ().location ());
abstract    return yy::parser::make_T_abstract (driver ().location ());
local       return yy::parser::make_T_local (driver ().location ());
valuetype   return yy::parser::make_T_valuetype (driver ().location ());
truncatable return yy::parser::make_T_truncatable (driver ().location ());
supports    return yy::parser::make_T_supports (driver ().location ());
custom      return yy::parser::make_T_custom (driver ().location ());
public      return yy::parser::make_T_public (driver ().location ());
private     return yy::parser::make_T_private (driver ().location ());
factory     return yy::parser::make_T_factory (driver ().location ());
native      return yy::parser::make_T_native (driver ().location ());
ValueBase   return yy::parser::make_T_ValueBase (driver ().location ());
Object      return yy::parser::make_T_Object (driver ().location ());
module      return yy::parser::make_T_module (driver ().location ());
octet       return yy::parser::make_T_octet (driver ().location ());
any         return yy::parser::make_T_any (driver ().location ());
sequence    return yy::parser::make_T_sequence (driver ().location ());
readonly    return yy::parser::make_T_readonly (driver ().location ());
attribute   return yy::parser::make_T_attribute (driver ().location ());
exception   return yy::parser::make_T_exception (driver ().location ());
oneway      return yy::parser::make_T_oneway (driver ().location ());
inout       return yy::parser::make_T_inout (driver ().location ());
raises      return yy::parser::make_T_raises (driver ().location ());
context     return yy::parser::make_T_context (driver ().location ());
typeid      return yy::parser::make_T_typeid (driver ().location ());
typeprefix  return yy::parser::make_T_typeprefix (driver ().location ());
getraises   return yy::parser::make_T_getraises (driver ().location ());
setraises   return yy::parser::make_T_setraises (driver ().location ());
pseudo      return yy::parser::make_T_pseudo (driver ().location ());

{CORBA_Identifier} return yy::parser::make_T_identifier (AST::Identifier (YYText (), YYLeng ()), driver ().location ());
{Float_Literal1} |
{Float_Literal2} |
{Float_Literal3} |
{Float_Literal4} |
{Float_Literal5} |
{Float_Literal6}   return yy::parser::make_T_floating_point_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{Fixed_Literal1} |
{Fixed_Literal2} |
{Fixed_Literal3} |
{Fixed_Literal4}   return yy::parser::make_T_fixed_point_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{Int_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{Oct_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{Hex_Literal}      return yy::parser::make_T_integer_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{Char_Literal}     return yy::parser::make_T_character_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{WChar_Literal}    return yy::parser::make_T_wide_character_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{String_Literal}   return yy::parser::make_T_string_literal (std::string (YYText (), YYLeng ()), driver ().location ());
{WString_Literal}  return yy::parser::make_T_wide_string_literal (std::string (YYText (), YYLeng ()), driver ().location ());
.                       {
                          return yy::parser::make_T_UNKNOWN (driver ().location ());
                        }

%%
