#ifndef NIDL_AST_OPERATION_H_
#define NIDL_AST_OPERATION_H_

#include "NamedItem.h"
#include "Type.h"
#include "Exception.h"
#include "Variant.h"

namespace AST {

class Operation :
	public NamedItem,
	public Type,
	public Container
{
public:
	bool oneway () const
	{
		return oneway_;
	}

	typedef std::vector <const Exception*> Raises;

	const Raises& raises () const
	{
		return raises_;
	}

	typedef std::vector <std::string> Context;

	const Context context () const
	{
		return context_;
	}

	// Internals

	Operation (const Build::Builder& builder, bool oneway, const Type& type, const Build::SimpleDeclarator& name) :
		NamedItem (Item::Kind::OPERATION, builder, name),
		Type (type),
		oneway_ (oneway)
	{}

	void oneway_clear ()
	{
		oneway_ = false;
	}

	void add_exception (const Exception* ex)
	{
		raises_.push_back (ex);
	}

	void context (const Build::Variants& strings)
	{
		for (auto it = strings.begin (); it != strings.end (); ++it) {
			if (!it->empty ()) {
				assert (it->vtype () == Variant::VT::STRING);
				context_.push_back (it->as_string ());
			}
		}
	}

private:
	bool oneway_;
	Raises raises_;
	Context context_;
};

}

#endif
