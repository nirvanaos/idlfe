#include "ItemScope.h"

using namespace std;

namespace AST {

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
