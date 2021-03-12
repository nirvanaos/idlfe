/// \file Type.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
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
#ifndef NIDL_AST_TYPE_H_
#define NIDL_AST_TYPE_H_

#include "BasicType.h"
#include "NamedItem.h"
#include <forward_list>

namespace AST {

typedef std::forward_list <unsigned> FixedArraySizes;

typedef uint32_t Dim;

class Sequence;
class Array;

/// An IDL type.
class Type
{
public:
	/// The kind of type.
	enum class Kind
	{
		VOID,
		BASIC_TYPE, ///< basic_type ()
		NAMED_TYPE, ///< named_type ()
		STRING,     ///< string_size ()
		WSTRING,    ///< string_size ()
		FIXED,      ///< fixed_digits (), fixed_scale ()
		SEQUENCE,   ///< sequence ()
		ARRAY       ///< array ()
	};

	/// \returns The kind of type.
	Kind tkind () const noexcept
	{
		return kind_;
	}

	/// \returns The BasicType.
	/// \invariant `kind () == Kind::BASIC_TYPE`.
	BasicType basic_type () const noexcept
	{
		assert (tkind () == Kind::BASIC_TYPE);
		return type_.basic_type;
	}

	/// \returns `const Ptr <NamedItem>&`.
	/// \invariant `kind () == Kind::NAMED_TYPE`.
	const Ptr <NamedItem>& named_type () const noexcept
	{
		assert (tkind () == Kind::NAMED_TYPE);
		return *type_.named_type;
	}

	// String

	/// \returns The string size limit if string has limited size.
	uint32_t string_size () const noexcept
	{
		assert (tkind () == Kind::STRING || tkind () == Kind::WSTRING);
		return type_.string_size;
	}

	// Sequence

	const Sequence& sequence () const noexcept
	{
		assert (tkind () == Kind::SEQUENCE);
		return *type_.sequence;
	}

	// Array

	const Array& array () const noexcept
	{
		assert (tkind () == Kind::ARRAY);
		return *type_.array;
	}

	// Fixed

	uint8_t fixed_digits () const noexcept
	{
		assert (tkind () == Kind::FIXED);
		return type_.fixed.digits;
	}

	uint8_t fixed_scale () const noexcept
	{
		assert (tkind () == Kind::FIXED);
		return type_.fixed.scale;
	}

	/// \internal

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

	size_t key_max () const noexcept;

private:
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
	/// \endinternal
};

}

#endif
