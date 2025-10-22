/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
#if __cplusplus < 201103L
#ifdef _MSC_BUILD
#error Use MSVC option /Zc:__cplusplus.
#endif
#endif

#include "include/IDL_FrontEnd.h"

#include <string.h>

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class

#include "FE/Driver.h"

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wswitch"
#endif

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
	throw std::invalid_argument (std::string ("Missing parameter for switch ") + arg ());
}

int IDL_FrontEnd::run (const char* command, int argc, const char* const argv []) noexcept
{
	command_ = command;
	int ret = 0;
	CmdLine cl (argc, argv);
	if (cl.end ())
		print_usage_info (command_);
	else {
		try {
			try {
				parse_arguments (cl);
			} catch (const std::invalid_argument& ex) {
				std::cerr << ex.what () << std::endl;
				print_usage_info (command_);
				return -1;
			}

			for (const auto& file : files_) {
				std::cout << file << std::endl;
				if (!compile (file))
					ret = -1;
			}
			
		} catch (const std::exception& ex) {
			std::cerr << ex.what () << std::endl;
			ret = -1;
		} catch (...) {
			std::cerr << "Unknown exception." << std::endl;
			ret = -1;
		}
	}

	return ret;
}

void IDL_FrontEnd::parse_arguments (CmdLine& args)
{
	for (;;) {
		const char* a = args.arg ();
		if (!parse_command_line (args))
			throw std::invalid_argument (std::string ("Invalid switch: ") + args.arg ());
		if (args.end ())
			break;
		if (a == args.arg ())
			throw std::runtime_error ("args.next () must be called for advance to the next argument.");
	}
}

void IDL_FrontEnd::print_usage_info (const char* exe_name)
{
	std::cout << "Based on the Nirvana IDL compiler front-end library. Copyright (c) 2025 Igor Popov.\n"
		"Usage: " << exe_name << " {file | option}.\n"
		"\toption = \"-\"{\"-\"}'...' [parameter].\n"
		"\tfile = parameter.\n"
		"\tparameter = (!\"-\")'...'.\n"
		"\n"
		"Preprocessor parameters:\n"
		"\t-D symbol\tDefine symbol.\n"
		"\t-U symbol\tUndefine symbol.\n"
		"\t-I directory\tAdd include directory.\n"
		"\t-FI file\tInclude file.\n"
		"\t-E\tPrint preprocessed file to stdout.\n"
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
				defines_.emplace_back (args.parameter (arg + 1));
				break;
			case 'U': // U Undefine
				undefines_.emplace_back (args.parameter (arg + 1));
				break;
			case 'I': // I Include path
				include_paths_.emplace_back (args.parameter (arg + 1));
				break;
			case 'F':
				if (arg [1] == 'I') // FI Include file
					includes_.emplace_back (args.parameter (arg + 2));
				else
					recognized = false;
				break;
			case 'h':
				print_usage_info (command_);
				break;
			case 'E':
				preprocess_to_stdout_ = true;
				break;
			default:
				recognized = false;
		}
	} else
		files_.emplace_back (arg);

	if (recognized)
		args.next ();

	return recognized;
}

bool IDL_FrontEnd::compile (const std::string& file)
{
	std::stringstream preprocessed;

	{ // Perform preprocessing
		std::string input;

		for (const auto& fi : includes_) {
			std::filesystem::path file (fi);
			if (!file.is_absolute ()) {
				for (const auto& inc : include_paths_) {
					std::filesystem::path tmp (inc);
					tmp /= file;
					std::error_code ec;
					if (std::filesystem::exists (tmp, ec)) {
						file = tmp;
						break;
					}
				}
			}
			input += "#include \"";
			input += file.string ();
			input += "\"\n";
		}

		{
			std::ifstream f (file);
			if (!f.is_open ()) {
				std::cerr << "Can not open file " << file << std::endl;
				return false;
			}
			input.append (std::istreambuf_iterator <char> (f.rdbuf()),
				std::istreambuf_iterator <char> ());
		}

		typedef boost::wave::cpplexer::lex_iterator <boost::wave::cpplexer::lex_token <>>
			LexIterator;

		typedef boost::wave::context <std::string::iterator, LexIterator> Wave;

		Wave wave (input.begin (), input.end (), file.c_str ());

		wave.set_language (boost::wave::language_support::support_c99);

		for (const auto& def : defines_) {
			wave.add_macro_definition (def);
		}

		for (const auto& undef : undefines_) {
			wave.remove_macro_definition (undef, true);
		}

		for (const auto& inc : include_paths_) {
			wave.add_include_path (inc.c_str ());
		}

		bool fatal = false;
		for (Wave::iterator_type first = wave.begin (), last = wave.end (); first != last;) {
			try {
				preprocessed << (*first).get_value ();
				++first;
			} catch (const boost::wave::cpp_exception& err) {
				auto severity = err.get_severity ();
				std::cerr << err.file_name () << '(' << err.line_no () << ',' << err.column_no () << "): "
					<< boost::wave::util::get_severity (severity) << ": "
				 	<< err.description ();
				if (severity > boost::wave::util::severity::severity_warning)
					fatal = true;
				if (!err.is_recoverable ()) {
					fatal = true;
					break;
				}
			}
		}

		if (fatal)
			return false;

		preprocessed.seekg (0);
	}

	if (preprocess_to_stdout_) {
		std::cout << preprocessed.rdbuf ();
		return true;
	}

	auto ast = FE::Driver::parse (*this, file, preprocessed);

	if (ast)
		try {
			generate_code (*ast);
		} catch (const std::runtime_error& ex) {
			std::cerr << ex.what () << std::endl;
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
