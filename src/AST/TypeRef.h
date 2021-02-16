#ifndef NIDL_AST_TYPEREF_H_
#define NIDL_AST_TYPEREF_H_

#include "BasicType.h"
#include "ScopedName.h"

namespace AST {

class TypeRef
{
public:
	enum class Kind
	{
		BASIC_TYPE,
		SCOPED_NAME,
		STRING,
		WSTRING,
		SEQUENCE,
		ARRAY
	};

	~TypeRef ();

	TypeRef (BasicType bt);

	TypeRef (const ScopedName& sn);

	TypeRef (const TypeRef& src);

	void set_size (unsigned margin);

	void add_dimension (unsigned dim);

private:
	Kind kind_;

	struct Template
	{
		std::vector <unsigned> margins;
		TypeRef* type_ref;

		Template () :
			type_ref (nullptr)
		{}

		Template (const TypeRef& tr) :
			type_ref (new TypeRef (tr))
		{}

		Template (const Template& src) :
			margins (src.margins),
			type_ref (nullptr)
		{
			if (src.type_ref)
				type_ref = new TypeRef (*src.type_ref);
		}

		~Template ()
		{
			delete type_ref;
		}
	};

	union Type
	{
		BasicType basic_type;
		ScopedName scoped_name;
		Template tpl;

		Type (BasicType bt) :
			basic_type (bt)
		{}

		Type (const ScopedName& sn) :
			scoped_name (sn)
		{}

		Type (Kind kind, const Type& src)
		{
			switch (kind) {
				case Kind::BASIC_TYPE:
					basic_type = src.basic_type;
					break;
				case Kind::SCOPED_NAME:
					new (&scoped_name) ScopedName (src.scoped_name);
					break;
				default:
					new (&tpl) Template (src.tpl);
			}
		}

		~Type () {}

	} type_;
};

}

#endif
