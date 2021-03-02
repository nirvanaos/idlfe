#include "Builder.h"

using namespace std;

namespace AST {

NamedItem::NamedItem (Kind kind, const Builder& builder, const SimpleDeclarator& name) :
	Item (kind),
	Location (name),
	parent_ (builder.cur_scope ()),
	name_ (name)
{}

std::string NamedItem::qualified_name () const
{
	string qn;
	if (parent_)
		qn = parent_->qualified_name ();
	qn += "::";
	qn += name_;
	return qn;
}

}
