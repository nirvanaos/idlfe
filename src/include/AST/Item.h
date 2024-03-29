/// \file
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
#ifndef IDLFE_AST_ITEM_H_
#define IDLFE_AST_ITEM_H_
#pragma once

#include <utility>
#include <assert.h>

/// Abstract Syntax Tree namespace.
namespace AST {

template <class T> class Ptr;

/// \brief An %AST item.
class Item
{
public:
	/// The kind of item.
	enum class Kind
	{
		ROOT, ///< class Root
		INCLUDE, ///< class Include
		NATIVE, ///< class Native
		TYPE_DEF, ///< class TypeDef
		CONSTANT, ///< class Constant
		MODULE, ///< class Module
		MODULE_ITEMS, ///< class ModuleItems
		INTERFACE_DECL, ///< class InterfaceDecl
		INTERFACE, ///< class Interface
		OPERATION, ///< class Operation
		PARAMETER, ///< class Parameter
		ATTRIBUTE, ///< class Attribute
		EXCEPTION, ///< class Exception
		STRUCT_DECL, ///< class StructDecl
		STRUCT, ///< class Struct
		MEMBER, ///< class Member
		UNION_DECL, ///< class UnionDecl
		UNION, ///< class Union
		UNION_ELEMENT, ///< class UnionElement
		ENUM, ///< class Enum
		ENUM_ITEM, ///< class EnumItem
		VALUE_TYPE_DECL, ///< class ValueTypeDecl
		VALUE_TYPE, ///< class ValueType
		VALUE_BOX, ///< class ValueBox
		STATE_MEMBER, ///< class StateMember
		VALUE_FACTORY ///< class ValueFactory
	};

	/// \returns The kind of item.
	Kind kind () const noexcept
	{
		return kind_;
	}

	/// \returns `true` if the item represents a type.
	bool is_type () const noexcept;

	/// \returns `true` if the item is a forward declaration.
	bool is_forward_decl () const noexcept;

protected:
	Item (Kind kind) :
		kind_ (kind),
		ref_cnt_ (1)
	{}

	Item (const Item& src) :
		kind_ (src.kind_),
		ref_cnt_ (1)
	{}

	virtual ~Item () {}

	Item& operator = (const Item& src)
	{
		assert (kind_ == src.kind_);
		return *this;
	}

private:
	template <class T> friend class Ptr;

	void* operator new (std::size_t cb)
	{
		return ::operator new (cb);
	}

	void _add_ref () const noexcept
	{
		++ref_cnt_;
	}

	void _remove_ref () const noexcept
	{
		if (!--ref_cnt_)
			delete this;
	}

private:
	const Kind kind_;
	mutable unsigned ref_cnt_;
};

/// \brief %AST item smart pointer.
/// 
/// \tparam T the item type.
template <class T>
class Ptr
{
	template <class T1> friend class Ptr;
public:
	Ptr () noexcept :
		p_ (nullptr)
	{}

	template <class T1, class ... Args>
	static Ptr make (Args ... args)
	{
		return Ptr (new T1 (std::forward <Args> (args)...), Make ());
	}

	template <class T1>
	Ptr (const Ptr <T1>& src) noexcept :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}
	
	Ptr (const Ptr& src) noexcept :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}

	template <class T1>
	Ptr (Ptr <T1>&& src) noexcept :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	Ptr (Ptr&& src) noexcept :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	template <class T1>
	Ptr (T1* p) noexcept :
		p_ (p)
	{
		if (p_)
			p_->_add_ref ();
	}

	Ptr (T* p) noexcept :
		p_ (p)
	{
		if (p_)
			p_->_add_ref ();
	}

	~Ptr () noexcept
	{
		if (p_)
			p_->_remove_ref ();
	}

	template <class T1>
	Ptr& operator = (const Ptr <T1>& src) noexcept
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			if ((p_ = src.p_))
				p_->_add_ref ();
		}
		return *this;
	}

	Ptr& operator = (const Ptr <T>& src) noexcept
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			if ((p_ = src.p_))
				p_->_add_ref ();
		}
		return *this;
	}

	template <class T1>
	Ptr& operator = (Ptr <T1>&& src) noexcept
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	Ptr& operator = (Ptr <T>&& src) noexcept
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	template <class T1>
	Ptr& operator = (T1* p) noexcept
	{
		if (p_ != p) {
			if (p_)
				p_->_remove_ref ();
			p_ = p;
		}
		return *this;
	}

	Ptr& operator = (T* p) noexcept
	{
		if (p_ != p) {
			if (p_)
				p_->_remove_ref ();
			p_ = p;
		}
		return *this;
	}

	Ptr& operator = (std::nullptr_t) noexcept
	{
		if (p_) {
			p_->_remove_ref ();
			p_ = nullptr;
		}
		return *this;
	}

	T* operator -> () const noexcept
	{
		assert (p_);
		return p_;
	}

	operator T* () const noexcept
	{
		return p_;
	}

	T& operator * () const noexcept
	{
		assert (p_);
		return *p_;
	}

private:
	struct Make {};

	Ptr (T* p, const Make&) :
		p_ (p)
	{}

private:
	T* p_;
};

template <class T1, class T>
T1* scast (const Ptr <T>& ptr) noexcept
{
	return static_cast <T1*> ((T*)ptr);
}

}

#endif
