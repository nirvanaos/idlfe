#ifndef NIDL_AST_ITEM_H_
#define NIDL_AST_ITEM_H_

#include <assert.h>
#include <utility>
#include <vector>

namespace AST {

template <class T> class Ptr;

template <class T, class ... Args>
Ptr <T> make_item (Args ... args);

class Item
{
public:
	enum class Kind
	{
		INCLUDE,
		MODULE,
		NATIVE,
		INTERFACE_DECL,
		INTERFACE,
		OPERATION
	};

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

	Kind kind () const
	{
		return kind_;
	}

private:
	template <class T> friend class Ptr;
	template <class T, class ... Args> friend Ptr <T> make_item (Args...);

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
	template <class T1, class ... Args> friend Ptr <T1> make_item (Args...);
public:
	Ptr () :
		p_ (nullptr)
	{}

	template <class T1>
	Ptr (const Ptr <T1>& src) :
		p_ (src.p_)
	{
		if (p_)
			p_->_add_ref ();
	}

	Ptr (const Ptr <T>& src) :
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

	Ptr (Ptr <T>&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
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

	T* operator -> () const
	{
		return p_;
	}

	T* get () const
	{
		return p_;
	}

private:
	Ptr (T* p) :
		p_ (p)
	{}

private:
	T* p_;
};

template <class T, class ... Args>
inline Ptr <T> make_item (Args ... args)
{
	return Ptr <T> (new T (std::forward <Args> (args)...));
}

}

#endif
