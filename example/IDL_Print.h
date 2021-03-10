// IDL_Print.h Demonstrates the compiler class.
#ifndef NIDLFE_EXAMPLE_IDL_PRINT_H_
#define NIDLFE_EXAMPLE_IDL_PRINT_H_

#include <IDL_FrontEnd.h>
#include <filesystem>

class IDL_Print : public IDL_FrontEnd
{
private:
	virtual void print_usage_info (const char* exe_name);
	virtual bool parse_command_line (CmdLine& args);
	virtual void generate_code (const AST::AST& tree);

private:
	std::filesystem::path out_dir_;
};

#endif
