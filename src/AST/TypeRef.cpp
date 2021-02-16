#include "TypeRef.h"
#include <assert.h>

namespace AST {

TypeRef::~TypeRef ()
{
	switch (kind_) {
		case Kind::BASIC_TYPE:
			break;
		case Kind::SCOPED_NAME:
			type_.scoped_name.~ScopedName ();
			break;
		default:
			type_.tpl.~Template ();
	}
}

TypeRef::TypeRef (BasicType bt) :
	kind_ (Kind::BASIC_TYPE),
	type_ (bt)
{}

TypeRef::TypeRef (const ScopedName& sn) :
	kind_ (Kind::SCOPED_NAME),
	type_ (sn)
{}

TypeRef::TypeRef (const TypeRef& src) :
	kind_ (src.kind_),
	type_ (src.kind_, src.type_)
{}

void TypeRef::set_size (unsigned margin)
{
	assert (Kind::STRING == kind_ || Kind::WSTRING == kind_ || Kind::SEQUENCE == kind_);
	assert (type_.tpl.margins.empty ());
	type_.tpl.margins.push_back (margin);
}

void TypeRef::add_dimension (unsigned dim)
{
	assert (Kind::ARRAY == kind_);
	type_.tpl.margins.push_back (dim);
}

}
