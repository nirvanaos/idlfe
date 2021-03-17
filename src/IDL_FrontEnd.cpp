/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include "include/IDL_FrontEnd.h"
#include "FE/Driver.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string.h>

#if __cplusplus < 201103L
#ifdef _MSC_BUILD
#error Use MSVC option /Zc:__cplusplus.
#endif
#endif

#include "simplecpp/simplecpp.h"

using namespace std;

struct IDL_FrontEnd::Arguments
{
	simplecpp::DUI preprocessor;
	vector <string> files;
};

bool IDL_FrontEnd::CmdLine::next () noexcept
{
	if (arg_ < end_) {
		++arg_;
		return true;
	} else
		return false;
}

const char* IDL_FrontEnd::CmdLine::parameter (const char* switch_end)
{
	if (*switch_end)
		return switch_end;
	else if (next ()) {
		const char* p = arg ();
		if ('-' != *p)
			return p;
	}
	throw invalid_argument (string ("Missing parameter for switch ") + arg ());
}

int IDL_FrontEnd::main (int argc, char* argv []) noexcept
{
	Arguments arguments;
	arguments_ = &arguments;
	int ret = 0;

	try {
		CmdLine cl (argc, argv);
		if (cl.end ())
			print_usage_info (cl.exe_file ());
		else {
			try {
				for (;;) {
					const char* a = cl.arg ();
					if (!parse_command_line (cl))
						throw invalid_argument (string ("Invalid switch: ") + cl.arg ());
					if (cl.end ())
						break;
					if (a == cl.arg ())
						throw runtime_error ("args.next () must be called for advance to the next argument.");
				}
			} catch (const invalid_argument& ex) {
				cerr << ex.what () << endl;
				print_usage_info (cl.exe_file ());
				ret = -1;
			}

			{
				const char* inc = getenv ("INCLUDE");
				if (inc) {
					const char* end = inc + strlen (inc);
					for (;;) {
						while (isspace (*inc))
							++inc;
						const char* sem = strchr (inc, ';');
						const char* endi = sem ? sem : end;
						while (inc < endi && isspace (*(endi - 1)))
							--endi;
						if (inc < endi)
							arguments_->preprocessor.includePaths.emplace_back (inc, endi);
						if (sem)
							inc = sem + 1;
						else
							break;
					}
				}
			}

			for (const auto& file : arguments_->files) {
				cout << file << endl;
				compile (file);
			}
		}
	} catch (const exception& ex) {
		cerr << ex.what () << endl;
		ret = -1;
	} catch (...) {
		cerr << "Unknown exception." << endl;
		ret = -1;
	}

	arguments_ = nullptr;
	return ret;
}

void IDL_FrontEnd::print_usage_info (const char* exe_name)
{
	cout << "Based on the Nirvana IDL compiler front-end library. Copyright (c) 2021 Igor Popov.\n";
	cout << "Usage: " << exe_name << " {file | option}.\n"
		"\toption = \"-\"{\"-\"}'...' [parameter].\n"
		"\tfile = parameter.\n"
		"\tparameter = (!\"-\")'...'.\n"
		"\n"
		"Preprocessor parameters:\n"
		"\t-D symbol Define symbol.\n"
		"\t-U symbol Undefine symbol.\n"
		"\t-I directory Add include directory.\n"
		"\t-FI file Include file.\n"
		;
}

bool IDL_FrontEnd::parse_command_line (CmdLine& args)
{
	const char* arg = args.arg ();
	bool recognized = true;
	if (arg [0] == '-') {
		++arg;
		switch (*arg) {
			case 'D': // D Define
				arguments_->preprocessor.defines.emplace_back (args.parameter (arg + 1));
				break;
			case 'U': // U Undefine
				arguments_->preprocessor.undefined.emplace (args.parameter (arg + 1));
				break;
			case 'I': // I Include path
				arguments_->preprocessor.includePaths.emplace_back (args.parameter (arg + 1));
				break;
			case 'F':
				if (arg [1] == 'I') // FI Include file
					arguments_->preprocessor.includes.emplace_back (args.parameter (arg + 2));
				else
					recognized = false;
				break;
			case 'h':
				print_usage_info (args.exe_file ());
				break;
			default:
				recognized = false;
		}
	} else
		arguments_->files.push_back (arg);

	if (recognized)
		args.next ();

	return recognized;
}

bool IDL_FrontEnd::compile (const string& file)
{
	istringstream preprocessed;

	{ // Perform preprocessing
		using namespace simplecpp;
		OutputList output_list;
		vector<string> files;
		ifstream f (file);
		if (!f.is_open ()) {
			cerr << "Can not open file " << file << endl;
			return false;
		}
		TokenList rawtokens (f, files, file, &output_list);

		rawtokens.removeComments ();
		map<string, simplecpp::TokenList*> included = simplecpp::load (rawtokens, files, arguments_->preprocessor, &output_list);
		for (auto i = included.begin (); i != included.end (); ++i)
			i->second->removeComments ();

		simplecpp::TokenList output_tokens (files);
		simplecpp::preprocess (output_tokens, rawtokens, files, included, arguments_->preprocessor, &output_list);

		if (!output_list.empty ()) {
			bool fatal = false;
			for (const simplecpp::Output& output : output_list) {
				cerr << output.location.file () << '(' << output.location.line << "): ";
				const char* type = "warning: ";
				switch (output.type) {
					case simplecpp::Output::ERROR:
					case simplecpp::Output::MISSING_HEADER:
					case simplecpp::Output::INCLUDE_NESTED_TOO_DEEPLY:
					case simplecpp::Output::SYNTAX_ERROR:
					case simplecpp::Output::UNHANDLED_CHAR_ERROR:
					case simplecpp::Output::EXPLICIT_INCLUDE_NOT_FOUND:
						fatal = true;
						type = "error: ";
						break;
				}
				cerr << type << output.msg << endl;
			}
			if (fatal)
				return false;
		}

		preprocessed.str (output_tokens.stringify ());
	}

	AST::Ptr <AST::Root> ast = FE::Driver::parse (file, preprocessed);

	if (ast)
		try {
			generate_code (*ast);
		} catch (const runtime_error& ex) {
			cerr << ex.what () << endl;
			return false;
		}

	return true;
}

const char* IDL_FrontEnd::filename (const char* path) noexcept
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
