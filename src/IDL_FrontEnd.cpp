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

			for (auto& fi : includes_) {
				std::filesystem::path file (fi);
				if (!file.is_absolute ()) {
					for (const auto& inc : include_paths_) {
						std::filesystem::path tmp (inc);
						tmp /= file;
						std::error_code ec;
						if (std::filesystem::exists (tmp, ec)) {
							fi = tmp.string ();
							break;
						}
					}
				}
			}

			simplecpp::DUI prep_params;
			for (const auto& def : defines_) {
				prep_params.defines.push_back (def);
			}
			for (const auto& undef : undefines_) {
				prep_params.undefined.insert (undef);
			}
			for (const auto& inc : include_paths_) {
				prep_params.includePaths.push_back (inc);
			}
			for (const auto& inc : includes_) {
				prep_params.includes.push_back (inc);
			}

			for (const auto& file : files_) {
				std::cout << file << std::endl;
				if (!compile (prep_params, file))
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
	std::cout << "Based on the Nirvana IDL compiler front-end library. Copyright (c) 2021 Igor Popov.\n"
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
			default:
				recognized = false;
		}
	} else
		files_.emplace_back (arg);

	if (recognized)
		args.next ();

	return recognized;
}

bool IDL_FrontEnd::compile (const simplecpp::DUI& prep_params, const std::string& file)
{
	std::istringstream preprocessed;

	{ // Perform preprocessing
		simplecpp::OutputList output_list;
		std::vector <std::string> files;
		std::ifstream f (file);
		if (!f.is_open ()) {
			std::cerr << "Can not open file " << file << std::endl;
			return false;
		}
		simplecpp::TokenList rawtokens (f, files, file, &output_list);

		rawtokens.removeComments ();
		std::map <std::string, simplecpp::TokenList*> included = simplecpp::load (rawtokens, files, prep_params, &output_list);
		for (auto i = included.begin (); i != included.end (); ++i)
			i->second->removeComments ();

		simplecpp::TokenList output_tokens (files);
		simplecpp::preprocess (output_tokens, rawtokens, files, included, prep_params, &output_list);

		if (!output_list.empty ()) {
			bool fatal = false;
			for (const simplecpp::Output& output : output_list) {
				std::cerr << output.location.file () << '(' << output.location.line << "): ";
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
				std::cerr << type << output.msg << std::endl;
			}
			if (fatal)
				return false;
		}

		preprocessed.str (output_tokens.stringify ());
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
