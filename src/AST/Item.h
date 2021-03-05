#ifndef NIDL_AST_ITEM_H_
#define NIDL_AST_ITEM_H_

#include <utility>

namespace AST {

template <class T> class Ptr;

class Item
{
public:
	enum class Kind
	{
		AST,
		INCLUDE,
		MODULE,
		MODULE_ITEMS,
		NATIVE,
		INTERFACE,
		INTERFACE_DECL,
		TYPEDEF,
		STRUCT,
		STRUCT_DECL,
		UNION,
		UNION_DECL,
		ENUM,
		ENUM_ITEM,
		OPERATION,
		ATTRIBUTE,
		PARAMETER,
		CONSTANT,
		EXCEPTION,
		MEMBER,
		CASE,
		DEFAULT
	};

	Item (Kind kind) :
		kind_ (kind),
		ref_cnt_ (1)
	{}

	Item (const Item& src) = delete;

	virtual ~Item () {}

	Item& operator = (const Item& src) = delete;

	Kind kind () const
	{
		return kind_;
	}

	bool is_type () const
	{
		return Kind::NATIVE <= kind_ && kind_ <= Kind::ENUM;
	}

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
};

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
