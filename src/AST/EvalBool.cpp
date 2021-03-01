/// \file EvalBool.cpp Boolean expression evaluiator.
#include "EvalBool.h"

using namespace std;

namespace AST {

Variant EvalBool::literal_boolean (bool v, unsigned line)
{
	return Variant (v);
}

Variant EvalBool::constant (const ScopedName& constant)
{
	const Constant* pc = lookup_const (constant);
	if (pc) {
		const Type& t = pc->dereference ();
		if (t.kind () == Type::Kind::BASIC_TYPE && t.basic_type () == BasicType::BOOLEAN)
			return Variant (*pc);
		else {
			invalid_constant_type (constant);
			see_definition (*pc);
		}
	}
	return Variant ();
}

Variant EvalBool::expr (const Variant& l, char op, const Variant& r, unsigned line)
{
	if (l.kind () != Type::Kind::VOID && r.kind () != Type::Kind::VOID) {
		bool lv = l.as_bool (), rv = r.as_bool (), ret;
		switch (op) {
			case '|':
				ret = lv || rv;
				break;
			case '^':
				ret = lv != rv;
				break;
			case '&':
				ret = lv && rv;
				break;
			default:
				invalid_operation (op, line);
				return Variant ();
		}
		return ret;
	}
	return Variant ();
}

Variant EvalBool::expr (char op, const Variant& v, unsigned line)
{
	if (v.kind () != Type::Kind::VOID) {
		if ('~' == op)
			return !v.as_bool ();
		else
			invalid_operation (op, line);
	}
	return Variant ();
}

Variant EvalBool::cast (const Type& t, Variant&& v, unsigned line)
{
	assert (t.kind () == Type::Kind::BASIC_TYPE && t.basic_type () == BasicType::BOOLEAN);
	assert (v.kind () == Type::Kind::VOID || (v.kind () == Type::Kind::BASIC_TYPE && v.basic_type () == BasicType::BOOLEAN));
	return move (v);
}

}
