// IDL_Print.h Demonstrates the IDL compiler class.
#ifndef NIDLFE_EXAMPLE_IDL_PRINT_H_
#define NIDLFE_EXAMPLE_IDL_PRINT_H_

#if __cplusplus < 201703L && _MSVC_LANG < 201703L
#error C++17 compliant compiler is required.
#endif

#include <IDL_FrontEnd.h>
#include "AST/Builder.h"
#include <filesystem>

// Derive the compiler class from IDL_FrontEnd.
class IDL_Print : public IDL_FrontEnd
{
private:
	// Override print_usage_info for additional usage information.
	virtual void print_usage_info (const char* exe_name) override;

	// Override parse_command_line to parse own command line switches.
	virtual bool parse_command_line (CmdLine& args) override;

	// Override generate_code to build output from the AST.
	virtual void generate_code (const AST::Root& tree) override;

	virtual void file_begin (const std::filesystem::path& file, AST::Builder& builder) override;
	virtual void interface_end (const AST::Interface& itf, AST::Builder& builder) override;

	AST::Identifier make_ami_id (const AST::Interface& itf, const char* suffix);

	static bool async_support (const AST::Interface& itf);

	static AST::ScopedNames poller_raises (const AST::Location& loc, const AST::Raises& op_raises);

private:
	std::filesystem::path out_dir_;

	struct AMI_Identifiers
	{
		AST::Identifier poller;
		AST::Identifier handler;
	};

	typedef std::unordered_map <const AST::Interface*, AMI_Identifiers> AMI_Map;

	typedef std::vector <const AMI_Map::value_type*> AMI_Bases;

	AMI_Map ami_map_;
};

#endif
