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

CORBA_Identifier	[a-zA-Z_][a-zA-Z0-9_]*

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
"{"			return yy::parser::make_T_LEFT_CURLY_BRACKET (driver ().location ());
"}"			return yy::parser::make_T_RIGHT_CURLY_BRACKET (driver ().location ());
"["		 	return yy::parser::make_T_LEFT_SQUARE_BRACKET (driver ().location ());
"]"			return yy::parser::make_T_RIGHT_SQUARE_BRACKET (driver ().location ());
"("			return yy::parser::make_T_LEFT_PARANTHESIS (driver ().location ());
")"			return yy::parser::make_T_RIGHT_PARANTHESIS (driver ().location ());
":"			return yy::parser::make_T_COLON (driver ().location ());
","			return yy::parser::make_T_COMMA (driver ().location ());
";"			return yy::parser::make_T_SEMICOLON (driver ().location ());
"="			return yy::parser::make_T_EQUAL (driver ().location ());
">>"		return yy::parser::make_T_SHIFTRIGHT (driver ().location ());
"<<"		return yy::parser::make_T_SHIFTLEFT (driver ().location ());
"+"			return yy::parser::make_T_PLUS_SIGN (driver ().location ());
"-"			return yy::parser::make_T_MINUS_SIGN (driver ().location ());
"*"			return yy::parser::make_T_ASTERISK (driver ().location ());
"/"			return yy::parser::make_T_SOLIDUS (driver ().location ());
"%"			return yy::parser::make_T_PERCENT_SIGN (driver ().location ());
"~"			return yy::parser::make_T_TILDE (driver ().location ());
"|"			return yy::parser::make_T_VERTICAL_LINE (driver ().location ());
"^"			return yy::parser::make_T_CIRCUMFLEX (driver ().location ());
"&"			return yy::parser::make_T_AMPERSAND (driver ().location ());
"<"			return yy::parser::make_T_LESS_THAN_SIGN (driver ().location ());
">"			return yy::parser::make_T_GREATER_THAN_SIGN (driver ().location ());

const       return yy::parser::make_T_CONST (driver ().location ());
typedef     return yy::parser::make_T_TYPEDEF (driver ().location ());
float       return yy::parser::make_T_FLOAT (driver ().location ());
double      return yy::parser::make_T_DOUBLE (driver ().location ());
char        return yy::parser::make_T_CHAR (driver ().location ());
wchar       return yy::parser::make_T_WCHAR (driver ().location ());
fixed       return yy::parser::make_T_FIXED (driver ().location ());
boolean     return yy::parser::make_T_BOOLEAN (driver ().location ());
string      return yy::parser::make_T_STRING (driver ().location ());
wstring     return yy::parser::make_T_WSTRING (driver ().location ());
void        return yy::parser::make_T_VOID (driver ().location ());
unsigned    return yy::parser::make_T_UNSIGNED (driver ().location ());
long        return yy::parser::make_T_LONG (driver ().location ());
short       return yy::parser::make_T_SHORT (driver ().location ());
FALSE       return yy::parser::make_T_FALSE (driver ().location ());
TRUE        return yy::parser::make_T_TRUE (driver ().location ());
struct      return yy::parser::make_T_STRUCT (driver ().location ());
union       return yy::parser::make_T_UNION (driver ().location ());
switch      return yy::parser::make_T_SWITCH (driver ().location ());
case        return yy::parser::make_T_CASE (driver ().location ());
default     return yy::parser::make_T_DEFAULT (driver ().location ());
enum        return yy::parser::make_T_ENUM (driver ().location ());
in          return yy::parser::make_T_IN (driver ().location ());
out         return yy::parser::make_T_OUT (driver ().location ());
interface   return yy::parser::make_T_INTERFACE (driver ().location ());
abstract    return yy::parser::make_T_ABSTRACT (driver ().location ());
local       return yy::parser::make_T_LOCAL (driver ().location ());
valuetype   return yy::parser::make_T_VALUETYPE (driver ().location ());
truncatable return yy::parser::make_T_TRUNCATABLE (driver ().location ());
supports    return yy::parser::make_T_SUPPORTS (driver ().location ());
custom      return yy::parser::make_T_CUSTOM (driver ().location ());
public      return yy::parser::make_T_PUBLIC (driver ().location ());
private     return yy::parser::make_T_PRIVATE (driver ().location ());
factory     return yy::parser::make_T_FACTORY (driver ().location ());
native      return yy::parser::make_T_NATIVE (driver ().location ());
ValueBase   return yy::parser::make_T_VALUEBASE (driver ().location ());

"::"        return yy::parser::make_T_SCOPE (driver ().location ()); 

module      return yy::parser::make_T_MODULE (driver ().location ());
octet       return yy::parser::make_T_OCTET (driver ().location ());
any         return yy::parser::make_T_ANY (driver ().location ());
sequence    return yy::parser::make_T_SEQUENCE (driver ().location ());
readonly    return yy::parser::make_T_READONLY (driver ().location ());
attribute   return yy::parser::make_T_ATTRIBUTE (driver ().location ());
exception   return yy::parser::make_T_EXCEPTION (driver ().location ());
oneway      return yy::parser::make_T_ONEWAY (driver ().location ());
inout       return yy::parser::make_T_INOUT (driver ().location ());
raises      return yy::parser::make_T_RAISES (driver ().location ());
context     return yy::parser::make_T_CONTEXT (driver ().location ());
typeid      return yy::parser::make_T_TYPEID (driver ().location ());
typeprefix  return yy::parser::make_T_TYPEPREFIX (driver ().location ());

Object      return yy::parser::make_T_OBJECT (driver ().location ());


{CORBA_Identifier} return yy::parser::make_T_IDENTIFIER (std::string (YYText (), YYLeng ()), driver ().location ());
{Float_Literal1} |
{Float_Literal2} |
{Float_Literal3} |
{Float_Literal4} |
{Float_Literal5} |
{Float_Literal6}   return yy::parser::make_T_FLOATING_PT_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{Fixed_Literal1} |
{Fixed_Literal2} |
{Fixed_Literal3} |
{Fixed_Literal4}   return yy::parser::make_T_FIXED_PT_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{Int_Literal}      return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{Oct_Literal}      return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{Hex_Literal}      return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{Char_Literal}     return yy::parser::make_T_CHARACTER_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{WChar_Literal}    return yy::parser::make_T_WCHARACTER_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{String_Literal}   return yy::parser::make_T_STRING_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
{WString_Literal}  return yy::parser::make_T_WSTRING_LITERAL (std::string (YYText (), YYLeng ()), driver ().location ());
.                       {
                          return yy::parser::make_T_UNKNOWN (driver ().location ());
                        }

%%

