#include "Builder/Builder.h"

using namespace std;
using namespace AST::Build;

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

ScopedName NamedItem::scoped_name () const
{
	ScopedName sn;
	if (parent_) {
		sn = parent_->scoped_name ();
		sn.push_back (name_);
	} else
		sn = ScopedName (*this, true, name_);
	return sn;
}

}
