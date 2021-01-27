#ifndef NIDL_CMDLINEPARSER_H_
#define NIDL_CMDLINEPARSER_H_

#include "Options.h"
#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

class CmdLineParser
{
public:
	CmdLineParser (int argc, const char* const argv []) :
		exe_ (*argv),
		arg_ (argv + 1),
		end_ (argv + argc)
	{}

	bool parse (std::list <std::string>& files)
	{
		if (arg_ >= end_) {
			std::cout << "For usage info: " << filename (exe_) << " -h" << std::endl;
		} else {
			try {
				do {
					const char* arg = *arg_;
					if (arg [0] == '-' || arg [0] == '/') {
						++arg;
						switch (*arg) {
							case 'D': // D Define
								options.preprocessor.defines.push_back (param (arg + 1));
								break;
							case 'U': // U Undefine
								options.preprocessor.undefined.insert (param (arg + 1));
								break;
							case 'I': // I Include path
								options.preprocessor.includePaths.push_back (param (arg + 1));
								break;
							case 'F':
								if (arg [1] == 'I') // FI Include file
									options.preprocessor.includes.push_back (arg + 2);
								else
									invalid_switch ();
								break;
							case 'h':
								usage_info ();
								break;
							default:
								invalid_switch ();
						}
					} else
						files.push_back (*arg_);
				} while (++arg_ < end_);
			} catch (const std::invalid_argument& ex) {
				std::cerr << ex.what () << std::endl;
				return false;
			}
		}
		return true;
	}

private:
	static const char* filename (const char* path);
	const char* param (const char* switch_end);
	void check_switch (const char* switch_end) const;
	void invalid_switch () const;
	void usage_info () const;

private:
	const char* exe_;
	const char* const* arg_;
	const char* const* const end_;
};

#endif
