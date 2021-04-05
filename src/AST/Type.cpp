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
#include "../include/AST/Type.h"
#include "../include/AST/Sequence.h"
#include "../include/AST/Array.h"
#include "../include/AST/TypeDef.h"
#include "../include/AST/Enum.h"
#include <stdexcept>

using namespace std;

namespace AST {

Type::Type (BasicType bt) :
	kind_ (Kind::BASIC_TYPE),
	type_ (bt)
{}

Type::Type (const Ptr <NamedItem>* named) :
	kind_ (named ? Kind::NAMED_TYPE : Kind::VOID),
	type_ (named ? named : (const Ptr <NamedItem>*)nullptr)
{
	assert (!named || (*named)->is_type ());
}

Type Type::make_sequence (const Type& type, Dim size)
{
	return Type (new Sequence (type, size));
}

Type::Type (const Type& type, const FixedArraySizes& sizes) :
	kind_ (Kind::ARRAY),
	type_ (new Array (type, vector <Dim> (sizes.begin (), sizes.end ())))
{}

Type::Type (unsigned digits, unsigned scale) :
	kind_ (Kind::FIXED)
{
	assert (digits <= 62 && scale <= digits);
	type_.fixed.digits = (uint8_t)digits;
	type_.fixed.scale = (uint8_t)scale;
}

Type::Type (Type&& src) noexcept :
	kind_ (src.kind_),
	type_ (src.type_)
{
	src.reset ();
}

void Type::copy (const Type& src)
{
	kind_ = src.kind_;
	switch (kind_) {
		case Kind::BASIC_TYPE:
			type_.basic_type = src.type_.basic_type;
			break;
		case Kind::NAMED_TYPE:
			type_.named_type = src.type_.named_type;
			break;
		case Kind::STRING:
		case Kind::WSTRING:
			type_.string_bound = src.type_.string_bound;
			break;
		case Kind::SEQUENCE:
			type_.sequence = nullptr;
			type_.sequence = new Sequence (*src.type_.sequence);
			break;
		case Kind::ARRAY:
			type_.array = nullptr;
			type_.array = new Array (*src.type_.array);
			break;
	}
}

void Type::clear () noexcept
{
	switch (kind_) {
		case Kind::SEQUENCE:
			delete type_.sequence;
			break;
		case Kind::ARRAY:
			delete type_.array;
			break;
	}
}

Type& Type::operator = (const Type& src)
{
	clear ();
	copy (src);
	return *this;
}

Type& Type::operator = (Type&& src) noexcept
{
	kind_ = src.kind_;
	type_ = src.type_;
	src.reset ();
	return *this;
}

const Type& Type::dereference_type () const noexcept
{
	const Type* t = this;
	while (t->kind_ == Kind::NAMED_TYPE) {
		const NamedItem& nt = t->named_type ();
		if (Item::Kind::TYPE_DEF == nt.kind ())
			t = &static_cast <const TypeDef&> (nt);
		else
			break;
	}
	return *t;
}

size_t Type::key_max () const noexcept
{
	const Type& t = dereference_type ();
	if (t.tkind () == Kind::BASIC_TYPE) {
		switch (t.basic_type ()) {
			case BasicType::BOOLEAN:
				return 1;
			case BasicType::OCTET:
			case BasicType::CHAR:
				return 255;
			case BasicType::WCHAR:
			case BasicType::USHORT:
			case BasicType::SHORT:
				return numeric_limits <uint16_t>::max ();
			case BasicType::ULONG:
			case BasicType::LONG:
				return numeric_limits <uint32_t>::max ();
			case BasicType::ULONGLONG:
			case BasicType::LONGLONG:
				return numeric_limits <size_t>::max ();
		}
	} else if (t.tkind () == Kind::NAMED_TYPE) {
		const NamedItem& en = named_type ();
		if (en.kind () == Item::Kind::ENUM) {
			size_t item_cnt = static_cast <const Enum&> (en).size ();
			if (item_cnt > 0)
				--item_cnt;
			return item_cnt;
		}
	}
	assert (false);
	return 0;
}

bool Type::is_complete_or_ref () const noexcept
{
	const Type& t = dereference_type ();
	if (t.tkind () == Kind::NAMED_TYPE) {
		Item::Kind k = named_type ().kind ();
		return k != Item::Kind::STRUCT_DECL && k != Item::Kind::UNION_DECL;
	}
	return true;
}

bool Type::is_complete () const noexcept
{
	const Type& t = dereference_type ();
	switch (t.tkind ()) {
		case Kind::NAMED_TYPE:
			return !t.named_type ().is_forward_decl ();
		case Kind::SEQUENCE:
			return sequence ().is_complete ();
		case Kind::ARRAY:
			return array ().is_complete ();
	}
	return true;
}

}
