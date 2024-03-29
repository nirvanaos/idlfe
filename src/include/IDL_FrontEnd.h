/// \file
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
#ifndef IDLFE_IDL_FRONTEND_H_
#define IDLFE_IDL_FRONTEND_H_
#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace AST {
class Root;
class Builder;
class Interface;
}

namespace FE {
class Driver;
}

namespace simplecpp {
struct DUI;
}

/// \brief IDL front-end.
/// 
/// Performs:
///   - Command line parsing
///   - Preprocessing
///   - IDL files parsing
///   - Building of the Abstract Syntax Tree (AST)
///
/// The compiler class should derive from this class and implement
///   virtual void generate_code ();
/// 
class IDL_FrontEnd
{
public:
	/// \brief Call this method from the main() function.
	/// 
	/// \param argc Count of command line arguments.
	/// \param argv Command line arguments.
	/// \returns The 0 if no errors, otherwise -1.
	int main (int argc, const char* const argv []) noexcept
	{
		return run (filename (argv [0]), argc - 1, argv + 1);
	}

	/// \brief The same as main (), but aruments must not include the first main() argument
	///    (executable file name).
	/// \param command The command name for usage info.
	///                You can use filename (argv [0]).
	/// \param argc Count of command line arguments.
	/// \param argv Command line arguments.
	/// \returns The 0 if no errors, otherwise -1.
	int run (const char* command, int argc, const char* const argv []) noexcept;

	/// \brief Get file name from path.
	/// 
	/// \param path The file path.
	/// \returns The file name.
	static const char* filename (const char* path) noexcept;

	/// \returns Flags parameter specified in constructor.
	unsigned flags () const noexcept
	{
		return flags_;
	}

	/// Disallows anonymous IDL types as required by the C++11 Language Mapping Specification.
	static const unsigned FLAG_DEPRECATE_ANONYMOUS_TYPES = 1;

	/// Disallows pseudo interfaces.
	static const unsigned FLAG_DEPRECATE_PSEUDO_INTERFACES = 2;

	/// Allow Nirvana const interface references.
	static const unsigned FLAG_ENABLE_CONST_OBJREF = 4;

	/// \returns Compiler messages output stream.
	std::ostream& err_out () const noexcept
	{
		return err_out_;
	}

protected:
	friend class FE::Driver;
	friend class AST::Builder;

	/// \brief Command line argument iterator.
	class CmdLine
	{
	public:
		/// \returns The current argument
		const char* arg () const noexcept
		{
			assert (!end ());
			return *arg_;
		}

		/// \returns `false` if there are no more arguments.
		bool end () const noexcept
		{
			return arg_ >= end_;
		}

		/// \brief Advances to the next argument.
		/// 
		/// \returns `true` on success, `false` if no more arguments.
		bool next () noexcept;

		/// \brief Get the parameter followed to the switch.
		/// 
		/// The command line parameter may follow the switch immediately (-DMYDEFINE)
		/// or as the next argument (-D MYDEFINE). The parameter must not start from '-'.
		/// 
		/// \param switch_end Pointer to the character immediately after the switch.
		/// \returns The parameter string.
		/// \throws std::invalid_argument if the parameter is missing.
		const char* parameter (const char* switch_end);

		/// Constructor.
		/// 
		/// \param argc Argument count.
		/// \param argv Argument array.
		CmdLine (int argc, const char* const argv []) :
			arg_ (argv),
			end_ (argv + argc)
		{}

	private:
		const char* const* arg_;
		const char* const* end_;
	};

	/// \brief Constructor
	/// 
	/// \param flags The flags.
	///              Currently only IDL_FrontEnd::FLAG_DEPRECATE_ANONYMOUS_TYPES is supported.
	/// \param err_out Compiler messages output stream. Default is std::cerr.
	IDL_FrontEnd (unsigned flags = 0, std::ostream& err_out = std::cerr) :
		command_ (nullptr),
		flags_ (flags),
		err_out_ (err_out),
		preprocess_to_stdout_ (false)
	{}

	/// \brief Parse command line parameters.
	///
	/// User can override this method to do some parameter fix/check.
	/// For example, user can add include paths from the environment.
	/// 
	/// \param args CmdLine object.
	virtual void parse_arguments (CmdLine& args);

	/// \brief Parse command line parameter.
	/// 
	/// User can override this method to parse additional parameters.
	/// The user method must call IDL_FrontEnd::parse_command_line () first to let the IDL_FrontEnd parse
	/// it's arguments. If the IDL_FrontEnd::parse_command_line () returns `false`, user method may parse
	/// it's own arguments.
	/// If the argument is recognized, args.next () must be called for advance to the next argument.
	/// 
	/// Switches recognized by IDL_FrontEnd:
	///   - -D definition Define a symbol.
	///   - -U definition Undefine a sy
	///   - -I include_path Add directory to the include directories.
	///   - -FI include_file Include a file.
	///   - -h Display usage information.
	/// 
	/// \param args CmdLine object.
	/// \returns `true` if command line parameter was recognized.
	/// \throw std::invalid_argument If the argument was recognized but the syntax is invalid.
	virtual bool parse_command_line (CmdLine& args);

	/// \brief Prints usage information to std::cout.
	/// 
	/// User can override this method to print additional information.
	/// 
	/// \param exe_name The name of program executable file obtained from argv[0].
	virtual void print_usage_info (const char* exe_name);

	/// \return Macros to define.
	std::vector <std::string>& defines () noexcept
	{
		return defines_;
	}
	
	/// \return Macros to undefine.
	std::vector <std::string>& undefines () noexcept
	{
		return undefines_;
	}

	/// \return Paths to find included files.
	std::vector <std::string>& include_paths () noexcept
	{
		return include_paths_;
	}

	/// \return Files to include.
	std::vector <std::string>& includes () noexcept
	{
		return includes_;
	}

	/// \return Files to compile.
	std::vector <std::string>& files () noexcept
	{
		return files_;
	}

	/// \brief Generate user code from %AST.
	/// 
	/// User must override this method.
	/// 
	/// \param tree AST::Root object.
	/// \throw std::runtime_error For displaying the error message and compile next file.
	///                           Other exceptions will cause the compilation interruption.
	virtual void generate_code (const AST::Root& tree) = 0;

	/// \brief Begin of the file parsing.
	/// 
	/// \param file The IDL file path.
	/// \param builder The %AST builder.
	///   User can use this object to inject some initial items to the empty %AST.
	virtual void file_begin (const std::filesystem::path& file, AST::Builder& builder)
	{}

	/// \brief End of the interface parsing.
	/// 
	/// User can override this method for interface post-processing, e. g. AMI IDL generation.
	/// This method is called on the end of interface from the IDL file. It is not called
	/// on Builder::interface_end () call.
	/// 
	/// \param itf The parsed interface.
	/// \param builder The %AST builder.
	virtual void interface_end (const AST::Interface& itf, AST::Builder& builder)
	{}

private:
	bool compile (const simplecpp::DUI& prep_params, const std::string& file);

private:
	const char* command_;
	unsigned flags_;
	std::vector <std::string> defines_;
	std::vector <std::string> undefines_;
	std::vector <std::string> include_paths_;
	std::vector <std::string> includes_;
	std::vector <std::string> files_;
	std::ostream& err_out_;
	bool preprocess_to_stdout_;
};

/// \example IDL_Print.cpp
/// This is an example of how to use IDL_FrontEnd class.
/// \include IDL_Print.h

#endif
