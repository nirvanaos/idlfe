// IDL_Print.h Demonstrates the IDL compiler back-end class.
#ifndef NIDLFE_EXAMPLE_IDL_PRINT_H_
#define NIDLFE_EXAMPLE_IDL_PRINT_H_

#include <IDL_FrontEnd.h>
#include <filesystem>

// Derive the compiler class from IDL_FrontEnd.
class IDL_Print : public IDL_FrontEnd
{
private:
	// Override print_usage_info for additional usage information.
	virtual void print_usage_info (const char* exe_name);

	// Override parse_command_line to parse own command line switches.
	virtual bool parse_command_line (CmdLine& args);

	// Override generate_code to build output from the AST.
	virtual void generate_code (const AST::AST& tree);

private:
	std::filesystem::path out_dir_;
};

#endif
