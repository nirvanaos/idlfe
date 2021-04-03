/// \file IDL_FrontEnd.h
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
#ifndef NIDL_IDL_FRONTEND_H_
#define NIDL_IDL_FRONTEND_H_

#include <string>
#include <assert.h>

namespace AST {
class Root;
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

	/// Call this method from the main() function.
	/// 
	/// \param argc Count of command line arguments.
	/// \param argv Command line arguments.
	/// \returns The 0 if no errors, otherwise -1.
	int main (int argc, char* argv []) noexcept;

	/// Get file name from path.
	/// 
	/// \param path The file path.
	/// \returns The file name.
	static const char* filename (const char* path) noexcept;

protected:

	/// Command line argument iterator.
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

		/// Advances to the next argument.
		/// 
		/// \returns `true` on success, `false` if no more arguments.
		bool next () noexcept;

		/// Get the parameter followed to switch.
		/// 
		/// The command line parameter may follow the switch immediately (-DMYDEFINE)
		/// or as the next argument (-D MYDEFINE). The parameter must not start from '-'.
		/// 
		/// \param switch_end Pointer to the character immediately after the switch.
		/// \returns The parameter string.
		/// \throws std::invalid_argument if the parameter is missing.
		const char* parameter (const char* switch_end);

		/// \returns The compiler executable file name.
		const char* exe_file () const
		{
			return filename (exe_);
		}

		CmdLine (int argc, char* argv []) :
			exe_ (*argv),
			arg_ (argv + 1),
			end_ (argv + argc)
		{}

	private:
		const char* exe_;
		const char* const* arg_;
		const char* const* end_;
	};

	IDL_FrontEnd () :
		arguments_ (nullptr)
	{}

	/// Parse command line parameter.
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
	/// \returns `true` if aommand line parameter was recognized.
	/// \throw std::invalid_argument If the argument was recognized but the syntax is invalid.
	virtual bool parse_command_line (CmdLine& args);

	/// Prints usage information to std::cout.
	/// 
	/// User can override this method to print additional information.
	/// 
	/// \param exe_name The name of program executable file obtained from argv[0].
	virtual void print_usage_info (const char* exe_name);

	/// Generate user code from AST.
	/// 
	/// \param tree AST.
	/// \throw std::runtime_error For displaying the error message and compile next file.
	///                           Other exceptions will cause the compilation interruption.
	virtual void generate_code (const ::AST::Root& tree) = 0;

private:
	bool compile (const std::string& file);

private:
	struct Arguments;
	Arguments* arguments_;
};

/// \example IDL_Print.cpp
/// This is an example of how to use IDL_FrontEnd class.
/// \include IDL_Print.h

#endif
