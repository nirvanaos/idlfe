#include "AST.h"

using namespace std;

namespace AST {

const string& AST::add_file (const string& name)
{
	return *files_.insert (name).first;
}

}
