#ifndef NIDL_AST_TYPE_H_
#define NIDL_AST_TYPE_H_

#include "BasicType.h"
#include "NamedItem.h"
#include <assert.h>

namespace AST {

class Sequence;
class Array;

typedef uint32_t Dim;

class Type
{
public:
	enum class Kind
	{
		VOID,
		BASIC_TYPE,
		NAMED_TYPE,
		STRING,
		WSTRING,
		SEQUENCE,
		ARRAY,
		FIXED
	};

	~Type ()
	{
		clear ();
	}

	Type () :
		kind_ (Kind::VOID)
	{}

	Type (BasicType bt);

	Type (const Ptr <NamedItem>* named);

	Type (const Type& src)
	{
		copy (src);
	}

	Type (Type&& src) noexcept;

	Type (Sequence* seq) :
		kind_ (Kind::SEQUENCE)
	{
		type_.sequence = seq;
	}

	Type (Array* arr) :
		kind_ (Kind::ARRAY)
	{
		type_.array = arr;
	}

	static Type make_string (Dim size = 0)
	{
		return Type (Kind::STRING, size);
	}

	static Type make_wstring (Dim size = 0)
	{
		return Type (Kind::WSTRING, size);
	}

	static Type make_fixed (uint16_t digits, uint16_t scale)
	{
		return Type (digits, scale);
	}

	Type& operator = (const Type& src);
	Type& operator = (Type&& src) noexcept;

	Kind kind () const
	{
		return kind_;
	}

	BasicType basic_type () const
	{
		assert (Kind::BASIC_TYPE == kind_);
		return type_.basic_type;
	}

	static bool is_int (BasicType bt)
	{
		return bt <= BasicType::ULONGLONG;
	}

	const Type& dereference () const noexcept;

protected:
	void clear () noexcept;

	void reset ()
	{
		kind_ = Kind::VOID;
	}

	void copy (const Type& src);

private:
	Type (Kind string_kind, Dim size) :
		kind_ (string_kind)
	{
		type_.string_size = size;
	}

	Type (uint16_t digits, uint16_t scale);

private:
	Kind kind_;
	union U
	{
		BasicType basic_type;              // BASIC_TYPE
		const Ptr <NamedItem>* named_type; // NAMED_TYPE
		Dim string_size;                   // STRING, WSTRING
		Sequence* sequence;                // SEQUENCE
		Array* array;                      // ARRAY
		struct
		{
			uint16_t digits;
			uint16_t scale;
		} fixed;                           // FIXED

		U () {}
		~U () {}
	} type_;
};

}

#endif
