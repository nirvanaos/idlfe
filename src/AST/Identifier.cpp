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
#include "../include/AST/Identifier.h"
#include <algorithm>

namespace AST {

const char* const Identifier::reserved_words_ [] = {
	"abstract",
	"any",
	"alias",
	"attribute",
	"bitfield",
	"bitmask",
	"bitset",
	"boolean",
	"case",
	"char",
	"component",
	"connector",
	"const",
	"consumes",
	"context",
	"custom",
	"default",
	"double",
	"exception",
	"emits",
	"enum",
	"eventtype",
	"factory",
	"FALSE",
	"finder",
	"fixed",
	"float",
	"getraises",
	"home",
	"import",
	"in",
	"inout",
	"interface",
	"local",
	"long",
	"manages",
	"map",
	"mirrorport",
	"module",
	"multiple",
	"native",
	"Object",
	"octet",
	"oneway",
	"out",
	"primarykey",
	"private",
	"port",
	"porttype",
	"provides",
	"public",
	"publishes",
	"raises",
	"readonly",
	"setraises",
	"sequence",
	"short",
	"string",
	"struct",
	"supports",
	"switch",
	"TRUE",
	"truncatable",
	"typedef",
	"typeid",
	"typename",
	"typeprefix",
	"unsigned",
	"union",
	"uses",
	"ValueBase",
	"valuetype",
	"void",
	"wchar",
	"wstring",
	"int8",
	"uint8",
	"int16",
	"int32",
	"int64",
	"uint16",
	"uint32",
	"uint64"
};

inline bool ci_compare (char l, char r) noexcept
{
	return Identifier::tolower (l) < Identifier::tolower (r);
}

bool Identifier::operator < (const Identifier& r) const noexcept
{
	return std::lexicographical_compare (begin (), end (), r.begin (), r.end (), ci_compare);
}

bool operator < (const Identifier& l, const char* r) noexcept
{
	return std::lexicographical_compare (l.begin (), l.end (), r, r + strlen (r), ci_compare);
}

bool operator < (const char* l, const Identifier& r) noexcept
{
	return std::lexicographical_compare (l, l + strlen (l), r.begin (), r.end (), ci_compare);
}

bool Identifier::operator == (const char* s) const noexcept
{
	size_t len = strlen (s);
	return size () == len && std::equal (begin (), end (), s, ci_compare);
}

bool Identifier::valid () const noexcept
{
	return ('_' == front () || !std::binary_search (reserved_words_, std::end (reserved_words_), *this));
}

}
