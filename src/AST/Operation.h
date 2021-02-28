#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"
#include "Type.h"

namespace AST {

class Operation :
	public NamedItem,
	public Type,
	public Container
{
public:
	Operation (const Location& loc, const ItemScope* parent, bool oneway, const Type& type, const std::string& name) :
		NamedItem (loc, Item::Kind::OPERATION, parent, name),
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

private:
	bool oneway_;

};

}

#endif
