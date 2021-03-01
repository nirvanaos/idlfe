#ifndef NIDL_AST_TYPE_H_
#define NIDL_AST_TYPE_H_

#include "BasicType.h"
#include "NamedItem.h"
#include "Declarators.h"
#include <assert.h>

namespace AST {

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

	Kind kind () const noexcept
	{
		return kind_;
	}

	BasicType basic_type () const noexcept
	{
		assert (Kind::BASIC_TYPE == kind_);
		return type_.basic_type;
	}

	static bool is_integer (BasicType bt) noexcept
	{
		return BasicType::BOOLEAN < bt && bt <= BasicType::LONGLONG;
	}

	bool is_integer () const  noexcept
	{
		const Type& t = dereference ();
		return t.kind () == Kind::BASIC_TYPE && is_integer (t.basic_type ());
	}

	bool is_signed (BasicType bt) const noexcept
	{
		return BasicType::SHORT <= bt && bt <= BasicType::LONGDOUBLE;
	}

	bool is_signed () const noexcept;

	static bool is_floating_pt (BasicType bt) noexcept
	{
		return (BasicType::FLOAT <= bt && bt <= BasicType::LONGDOUBLE);
	}

	bool is_floating_pt () noexcept
	{
		const Type& t = dereference ();
		return t.kind () == Kind::BASIC_TYPE && is_floating_pt (t.basic_type ());
	}

	const Type& dereference () const noexcept;

	// Fixed

	uint8_t fixed_digits () const noexcept
	{
		assert (dereference ().kind () == Kind::FIXED);
		return type_.fixed.digits;
	}

	uint8_t fixed_scale () const noexcept
	{
		assert (dereference ().kind () == Kind::FIXED);
		return type_.fixed.scale;
	}

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
		Sequence* sequence;                // SEQUENCE
		Array* array;                      // ARRAY
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
