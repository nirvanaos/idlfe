/// \file Item.h
#ifndef NIDL_AST_ITEM_H_
#define NIDL_AST_ITEM_H_

#include <utility>

namespace AST {

template <class T> class Ptr;

/// AST item.
class Item
{
public:
	/// The kind of item.
	enum class Kind
	{
		AST, /// class AST
		INCLUDE, /// class Include
		MODULE, /// class Module
		MODULE_ITEMS, /// class ModuleItems
		NATIVE, /// class Native
		INTERFACE, /// class Interface
		INTERFACE_DECL, /// class InterfaceDecl
		TYPEDEF, /// class TypeDef
		STRUCT, /// class Struct
		STRUCT_DECL, /// class StructDecl
		UNION, /// class Union
		UNION_DECL, /// class UnionDecl
		ENUM, /// class Enum
		ENUM_ITEM, /// class EnumItem
		OPERATION, /// class Operation
		ATTRIBUTE, /// class Attribute
		PARAMETER, /// class Parameter
		CONSTANT, /// class Constant
		EXCEPTION, /// class Exception
		MEMBER, /// class Member
		UNION_ELEMENT /// class UnionElement
	};

	/// \returns The kind of item.
	Kind kind () const
	{
		return kind_;
	}

	/// \returns `true` if the item represents a type.
	bool is_type () const
	{
		return Kind::NATIVE <= kind_ && kind_ <= Kind::ENUM;
	}

	/// \internals

	Item (Kind kind) :
		kind_ (kind),
		ref_cnt_ (1)
	{}

	Item (const Item& src) = delete;

	virtual ~Item () {}

	Item& operator = (const Item& src) = delete;

private:
	template <class T> friend class Ptr;

	void* operator new (size_t cb)
	{
		return ::operator new (cb);
	}

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		if (!--ref_cnt_)
			delete this;
	}

private:
	const Kind kind_;
	unsigned ref_cnt_;
	/// \endinternals
};

/// Item smart pointer.
/// 
/// \tparam T the item type.
template <class T>
class Ptr
{
	template <class T1> friend class Ptr;
public:
	Ptr () :
		p_ (nullptr)
	{}

	template <class T1, class ... Args>
	static Ptr make (Args ... args)
	{
		return Ptr (new T1 (std::forward <Args> (args)...), Make ());
	}

	template <class T1>
	Ptr (const Ptr <T1>& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}
	
	Ptr (const Ptr& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}

	template <class T1>
	Ptr (Ptr <T1>&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	Ptr (Ptr&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	template <class T1>
	Ptr (T1* p) :
		p_ (p)
	{
		if (p_)
			p_->_add_ref ();
	}

	Ptr (T* p) :
		p_ (p)
	{
		if (p_)
			p_->_add_ref ();
	}

	~Ptr ()
	{
		if (p_)
			p_->_remove_ref ();
	}

	template <class T1>
	Ptr& operator = (const Ptr <T1>& src)
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			if (p_ = src.p_)
				p_->_add_ref ();
		}
		return *this;
	}

	Ptr& operator = (const Ptr <T>& src)
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			if (p_ = src.p_)
				p_->_add_ref ();
		}
		return *this;
	}

	template <class T1>
	Ptr& operator = (Ptr <T1>&& src)
	{
		if (p_ != src.p_) {
			if (p_)
				p_->_remove_ref ();
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	Ptr& operator = (Ptr <T>&& src)
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
	Ptr& operator = (T1* p)
	{
		if (p_ != p) {
			if (p_)
				p_->_remove_ref ();
			p_ = p;
		}
		return *this;
	}

	Ptr& operator = (T* p)
	{
		if (p_ != p) {
			if (p_)
				p_->_remove_ref ();
			p_ = p;
		}
		return *this;
	}

	Ptr& operator = (nullptr_t)
	{
		if (p_) {
			p_->_remove_ref ();
			p_ = nullptr;
		}
		return *this;
	}

	T* operator -> () const
	{
		return p_;
	}

	operator T* () const
	{
		return p_;
	}

	T& operator * () const
	{
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
T1* scast (const Ptr <T>& ptr)
{
	return static_cast <T1*> ((T*)ptr);
}

}

#endif
