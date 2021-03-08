#ifndef NIDL_AST_ITEMSCOPE_H_
#define NIDL_AST_ITEMSCOPE_H_

#include "Symbols.h"
#include "ItemScope.h"

namespace AST {

class ItemScope :
	public NamedItem,
	public Symbols
{
public:
	ItemScope (Item::Kind kind, const Build::Builder& builder, const Build::SimpleDeclarator& name);

	static ItemScope* cast (NamedItem* item) noexcept;

	static const ItemScope* cast (const NamedItem* item) noexcept
	{
		return cast (const_cast <NamedItem*> (item));
	}

	const std::string& prefix () const noexcept
	{
		return prefix_;
	}

	virtual bool prefix (Build::Builder& builder, const std::string& pref, const Location& loc)
	{
		prefix_ = pref;
		return true;
	}

private:
	std::string prefix_;
};

}

#endif
