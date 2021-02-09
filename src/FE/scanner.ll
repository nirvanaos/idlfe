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
Int_Literal		[1-9][0-9]*
Oct_Literal		0{Oct_Digit}*
Hex_Literal		(0x|0X){Hex_Digit}*
Esc_Sequence1           "\\"[ntvbrfa\\\?\'\"]
Esc_Sequence2           "\\"{Oct_Digit}{1,3}
Esc_Sequence3           "\\"(x|X){Hex_Digit}{1,2}
Esc_Sequence            ({Esc_Sequence1}|{Esc_Sequence2}|{Esc_Sequence3})
Char                    ([^\n\t\"\'\\]|{Esc_Sequence})
Char_Literal            "'"({Char}|\")"'"
String_Literal		\"({Char}|"'")*\"
Float_Literal1		{Digits}"."{Digits}?(e|E)("+"|"-")?{Digits}  
Float_Literal2		{Digits}(e|E)("+"|"-")?{Digits}
Float_Literal3          {Digits}"."{Digits}
Float_Literal4          {Digits}"."
Float_Literal5		"."{Digits} 
Float_Literal6		"."{Digits}(e|E)("+"|"-")?{Digits}  
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
"#pragma"[^\n]*\n       {
                          return yy::parser::make_T_PRAGMA (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
                        }
"#"[^\n]*\n             {
                            static_cast <Driver*> (this)->preprocessor_directive (YYText (), YYLeng ());
                        }
"{"			return yy::parser::make_T_LEFT_CURLY_BRACKET (yy::parser::location_type (nullptr, lineno ()));
"}"			return yy::parser::make_T_RIGHT_CURLY_BRACKET (yy::parser::location_type (nullptr, lineno ()));
"["		 	return yy::parser::make_T_LEFT_SQUARE_BRACKET (yy::parser::location_type (nullptr, lineno ()));
"]"			return yy::parser::make_T_RIGHT_SQUARE_BRACKET (yy::parser::location_type (nullptr, lineno ()));
"("			return yy::parser::make_T_LEFT_PARANTHESIS (yy::parser::location_type (nullptr, lineno ()));
")"			return yy::parser::make_T_RIGHT_PARANTHESIS (yy::parser::location_type (nullptr, lineno ()));
":"			return yy::parser::make_T_COLON (yy::parser::location_type (nullptr, lineno ()));
","			return yy::parser::make_T_COMMA (yy::parser::location_type (nullptr, lineno ()));
";"			return yy::parser::make_T_SEMICOLON (yy::parser::location_type (nullptr, lineno ()));
"="			return yy::parser::make_T_EQUAL (yy::parser::location_type (nullptr, lineno ()));
">>"		return yy::parser::make_T_SHIFTRIGHT (yy::parser::location_type (nullptr, lineno ()));
"<<"		return yy::parser::make_T_SHIFTLEFT (yy::parser::location_type (nullptr, lineno ()));
"+"			return yy::parser::make_T_PLUS_SIGN (yy::parser::location_type (nullptr, lineno ()));
"-"			return yy::parser::make_T_MINUS_SIGN (yy::parser::location_type (nullptr, lineno ()));
"*"			return yy::parser::make_T_ASTERISK (yy::parser::location_type (nullptr, lineno ()));
"/"			return yy::parser::make_T_SOLIDUS (yy::parser::location_type (nullptr, lineno ()));
"%"			return yy::parser::make_T_PERCENT_SIGN (yy::parser::location_type (nullptr, lineno ()));
"~"			return yy::parser::make_T_TILDE (yy::parser::location_type (nullptr, lineno ()));
"|"			return yy::parser::make_T_VERTICAL_LINE (yy::parser::location_type (nullptr, lineno ()));
"^"			return yy::parser::make_T_CIRCUMFLEX (yy::parser::location_type (nullptr, lineno ()));
"&"			return yy::parser::make_T_AMPERSAND (yy::parser::location_type (nullptr, lineno ()));
"<"			return yy::parser::make_T_LESS_THAN_SIGN (yy::parser::location_type (nullptr, lineno ()));
">"			return yy::parser::make_T_GREATER_THAN_SIGN (yy::parser::location_type (nullptr, lineno ()));

const			return yy::parser::make_T_CONST (yy::parser::location_type (nullptr, lineno ()));
typedef		return yy::parser::make_T_TYPEDEF (yy::parser::location_type (nullptr, lineno ()));
float			return yy::parser::make_T_FLOAT (yy::parser::location_type (nullptr, lineno ()));
double		return yy::parser::make_T_DOUBLE (yy::parser::location_type (nullptr, lineno ()));
char			return yy::parser::make_T_CHAR (yy::parser::location_type (nullptr, lineno ()));
wchar			return yy::parser::make_T_WCHAR (yy::parser::location_type (nullptr, lineno ()));
fixed     return yy::parser::make_T_FIXED (yy::parser::location_type (nullptr, lineno ()));
boolean			return yy::parser::make_T_BOOLEAN (yy::parser::location_type (nullptr, lineno ()));
string			return yy::parser::make_T_STRING (yy::parser::location_type (nullptr, lineno ()));
wstring			return yy::parser::make_T_WSTRING (yy::parser::location_type (nullptr, lineno ()));
void			return yy::parser::make_T_VOID (yy::parser::location_type (nullptr, lineno ()));
unsigned		return yy::parser::make_T_UNSIGNED (yy::parser::location_type (nullptr, lineno ()));
long 			return yy::parser::make_T_LONG (yy::parser::location_type (nullptr, lineno ()));
short			return yy::parser::make_T_SHORT (yy::parser::location_type (nullptr, lineno ()));
FALSE			return yy::parser::make_T_FALSE (yy::parser::location_type (nullptr, lineno ()));
TRUE			return yy::parser::make_T_TRUE (yy::parser::location_type (nullptr, lineno ()));
struct			return yy::parser::make_T_STRUCT (yy::parser::location_type (nullptr, lineno ()));
union			return yy::parser::make_T_UNION (yy::parser::location_type (nullptr, lineno ()));
switch			return yy::parser::make_T_SWITCH (yy::parser::location_type (nullptr, lineno ()));
case			return yy::parser::make_T_CASE (yy::parser::location_type (nullptr, lineno ()));
default			return yy::parser::make_T_DEFAULT (yy::parser::location_type (nullptr, lineno ()));
enum			return yy::parser::make_T_ENUM (yy::parser::location_type (nullptr, lineno ()));
in			return yy::parser::make_T_IN (yy::parser::location_type (nullptr, lineno ()));
out			return yy::parser::make_T_OUT (yy::parser::location_type (nullptr, lineno ()));
interface		return yy::parser::make_T_INTERFACE (yy::parser::location_type (nullptr, lineno ()));
abstract		return yy::parser::make_T_ABSTRACT (yy::parser::location_type (nullptr, lineno ()));
valuetype		return yy::parser::make_T_VALUETYPE (yy::parser::location_type (nullptr, lineno ()));
truncatable		return yy::parser::make_T_TRUNCATABLE (yy::parser::location_type (nullptr, lineno ()));
supports		return yy::parser::make_T_SUPPORTS (yy::parser::location_type (nullptr, lineno ()));
custom			return yy::parser::make_T_CUSTOM (yy::parser::location_type (nullptr, lineno ()));
public			return yy::parser::make_T_PUBLIC (yy::parser::location_type (nullptr, lineno ()));
private			return yy::parser::make_T_PRIVATE (yy::parser::location_type (nullptr, lineno ()));
factory			return yy::parser::make_T_FACTORY (yy::parser::location_type (nullptr, lineno ()));
native			return yy::parser::make_T_NATIVE (yy::parser::location_type (nullptr, lineno ()));
ValueBase		return yy::parser::make_T_VALUEBASE (yy::parser::location_type (nullptr, lineno ()));

"::"			return yy::parser::make_T_SCOPE (yy::parser::location_type (nullptr, lineno ())); 

module			return yy::parser::make_T_MODULE (yy::parser::location_type (nullptr, lineno ()));
octet			return yy::parser::make_T_OCTET (yy::parser::location_type (nullptr, lineno ()));
any			return yy::parser::make_T_ANY (yy::parser::location_type (nullptr, lineno ()));
sequence		return yy::parser::make_T_SEQUENCE (yy::parser::location_type (nullptr, lineno ()));
readonly		return yy::parser::make_T_READONLY (yy::parser::location_type (nullptr, lineno ()));
attribute		return yy::parser::make_T_ATTRIBUTE (yy::parser::location_type (nullptr, lineno ()));
exception		return yy::parser::make_T_EXCEPTION (yy::parser::location_type (nullptr, lineno ()));
oneway			return yy::parser::make_T_ONEWAY (yy::parser::location_type (nullptr, lineno ()));
inout			return yy::parser::make_T_INOUT (yy::parser::location_type (nullptr, lineno ()));
raises			return yy::parser::make_T_RAISES (yy::parser::location_type (nullptr, lineno ()));
context			return yy::parser::make_T_CONTEXT (yy::parser::location_type (nullptr, lineno ()));

Object                  return yy::parser::make_T_OBJECT (yy::parser::location_type (nullptr, lineno ()));
Principal               return yy::parser::make_T_PRINCIPAL (yy::parser::location_type (nullptr, lineno ()));


{CORBA_Identifier}	return yy::parser::make_T_IDENTIFIER (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Float_Literal1}	|
{Float_Literal2}	|
{Float_Literal3}	|
{Float_Literal4}	|
{Float_Literal5}	|
{Float_Literal6}	return yy::parser::make_T_FLOATING_PT_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Fixed_Literal1}	|
{Fixed_Literal2}	|
{Fixed_Literal3}	|
{Fixed_Literal4}	return yy::parser::make_T_FIXED_PT_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Int_Literal}		return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Oct_Literal}		return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Hex_Literal}		return yy::parser::make_T_INTEGER_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{Char_Literal}		return yy::parser::make_T_CHARACTER_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
{String_Literal}	return yy::parser::make_T_STRING_LITERAL (std::string (YYText (), YYLeng ()), yy::parser::location_type (nullptr, lineno ()));
.                       {
                          return yy::parser::make_T_UNKNOWN (yy::parser::location_type (nullptr, lineno ()));
                        }

%%

