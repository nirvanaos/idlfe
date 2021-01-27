#include "CmdLineParser.h"
#include "compile.h"

using namespace std;

int main (int argc, char* argv [])
{
	list <string> files;
	if (CmdLineParser (argc, argv).parse (files)) {
		for (auto f = files.begin (); f != files.end (); ++f)
			compile (*f);
		return 0;
	} else
		return -1;
}