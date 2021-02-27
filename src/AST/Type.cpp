#include "Type.h"
#include "Sequence.h"
#include "Array.h"
#include "TypeDef.h"
#include <stdexcept>

using namespace std;

namespace AST {

Type::Type (BasicType bt) :
	kind_ (Kind::BASIC_TYPE)
{
	type_.basic_type = bt;
}

Type::Type (const Ptr <NamedItem>* named) :
	kind_ (Kind::NAMED_TYPE)
{
	if (named) {
		const NamedItem* p = *named;
		if (!p->is_type ())
			throw runtime_error (string ("Type name ") + p->name () + " is invalid.");
	}
	type_.named_type = named;
}

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
			type_.string_size = src.type_.string_size;
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

const Type& Type::dereference () const noexcept
{
	const Type* t = this;
	while (t->kind_ == Kind::NAMED_TYPE && t->type_.named_type) {
		const NamedItem* p = *(t->type_.named_type);
		if (Item::Kind::TYPEDEF == p->kind ())
			t = static_cast <const TypeDef*> (p);
		else
			break;
	}
	return *t;
}

}
