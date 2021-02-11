#include "Interface.h"

using namespace std;

namespace AST {

const char* InterfaceDecl::kind_name (Kind ic)
{
	switch (ic) {
		case Kind::ABSTRACT:
			return "abstract";
		case Kind::LOCAL:
			return "local";
		default:
			return "";
	}
}

}
