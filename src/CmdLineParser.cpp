#include "CmdLineParser.h"

using namespace std;

Options options;

void CmdLineParser::usage_info () const
{
	cout << "Usage: " << filename (exe_) << " [options] file... [options]" << endl;
}

const char* CmdLineParser::filename (const char* path)
{
	const char* p = path + strlen (path);
	while (p > path) {
		char c = *--p;
		if (c == '\\' || c == '/') {
			++p;
			break;
		}
	}
	return p;
}

const char* CmdLineParser::param (const char* switch_end)
{
	if (*switch_end)
		return switch_end;
	else if (arg_ < end_)
		return *++arg_;
	else
		throw invalid_argument (string ("Missing parameter for switch ") + *arg_);
}

void CmdLineParser::check_switch (const char* switch_end) const
{
	if (*switch_end)
		invalid_switch ();
}

void CmdLineParser::invalid_switch () const
{
	throw std::invalid_argument (std::string ("Invalid switch ") + *arg_);
}
