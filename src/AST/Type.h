#ifndef NIDL_AST_TYPE_H_
#define NIDL_AST_TYPE_H_

#include "BasicType.h"
#include "NamedItem.h"
#include <forward_list>
#include <assert.h>

namespace AST {

typedef std::forward_list <unsigned> FixedArraySizes;

typedef uint32_t Dim;

class Sequence;
class Array;

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
		FIXED,
		SEQUENCE,
		ARRAY
	};

	Kind kind () const noexcept
	{
		return kind_;
	}

	BasicType basic_type () const noexcept
	{
		assert (kind () == Kind::BASIC_TYPE);
		return type_.basic_type;
	}

	const Ptr <NamedItem>* named_type () const noexcept
	{
		assert (kind () == Kind::NAMED_TYPE);
		return type_.named_type;
	}

	// String

	uint32_t string_size () const noexcept
	{
		assert (kind () == Kind::STRING || kind () == Kind::WSTRING);
		return type_.string_size;
	}

	// Fixed

	uint8_t fixed_digits () const noexcept
	{
		assert (kind () == Kind::FIXED);
		return type_.fixed.digits;
	}

	uint8_t fixed_scale () const noexcept
	{
		assert (kind () == Kind::FIXED);
		return type_.fixed.scale;
	}

	// Sequence

	const Sequence& sequence () const noexcept
	{
		assert (kind () == Kind::SEQUENCE);
		return *type_.sequence;
	}

	// Array

	const Array& array () const noexcept
	{
		assert (kind () == Kind::ARRAY);
		return *type_.array;
	}

	// Internals

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

	static Type make_string (Dim size = 0)
	{
		return Type (Kind::STRING, size);
	}

	static Type make_wstring (Dim size = 0)
	{
		return Type (Kind::WSTRING, size);
	}

	static Type make_sequence (const Type& type, Dim size = 0);
	static Type make_array (const Type& type, const FixedArraySizes& sizes);

	static Type make_fixed (unsigned digits, unsigned scale)
	{
		return Type (digits, scale);
	}

	Type& operator = (const Type& src);
	Type& operator = (Type&& src) noexcept;

	const Type& dereference_type () const noexcept;

protected:
	void clear () noexcept;

	void reset ()
	{
		kind_ = Kind::VOID;
	}

	void copy (const Type& src);

	Type (Kind string_kind, Dim size = 0) :
		kind_ (string_kind),
		type_ (size)
	{}

	Type (unsigned digits, unsigned scale);

private:
	Type (Sequence* seq) :
		kind_ (Kind::SEQUENCE),
		type_ (seq)
	{}

	Type (Array* arr) :
		kind_ (Kind::ARRAY),
		type_ (arr)
	{}

private:
	Kind kind_;
	union U
	{
		BasicType basic_type;              // BASIC_TYPE
		const Ptr <NamedItem>* named_type; // NAMED_TYPE
		Dim string_size;                   // STRING, WSTRING
		const Sequence* sequence;          // SEQUENCE
		const Array* array;                // ARRAY
		struct
		{
			uint8_t digits;
			uint8_t scale;
		} fixed;                           // FIXED

		U ()
		{}

		U (BasicType bt) :
			basic_type (bt)
		{}

		U (const Ptr <NamedItem>* nt) :
			named_type (nt)
		{}

		U (Dim ssize) :
			string_size (ssize)
		{}

		U (Sequence* pseq) :
			sequence (pseq)
		{}

		U (Array* parr) :
			array (parr)
		{}

		~U () {}
	} type_;
};

}

#endif
