#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"
#include "Type.h"
#include "Exception.h"

namespace AST {

class Operation :
	public NamedItem,
	public Type,
	public Container
{
public:
	Operation (const Build::Builder& builder, bool oneway, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::OPERATION, builder, name),
		Type (type),
		oneway_ (oneway)
	{}

	bool oneway () const
	{
		return oneway_;
	}

	void oneway_clear ()
	{
		oneway_ = false;
	}

	const std::vector <const Exception*>& raises () const
	{
		return raises_;
	}

	void add_exception (const Exception* ex)
	{
		raises_.push_back (ex);
	}

private:
	bool oneway_;
	std::vector <const Exception*> raises_;
};

}

#endif
