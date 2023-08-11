// IDL_Print.cpp Demonstrates the IDL compiler class.
#include "IDL_Print.h"
#include "Printer.h"
#include "AST/Builder.h"
#include <iostream>

using namespace std;
using namespace std::filesystem;

void IDL_Print::print_usage_info (const char* exe_name)
{
	cout << "Compile and print IDL.\n";
	IDL_FrontEnd::print_usage_info (exe_name);
	cout << "Output options:\n"
		"\t-d directory\tDirectory for output files.\n";
}

bool IDL_Print::parse_command_line (CmdLine& args)
{
	if (IDL_FrontEnd::parse_command_line (args))
		return true;

	if (args.arg () [1] == 'd') {
		out_dir_ = args.parameter (args.arg () + 2);
		assert (!out_dir_.empty ());
		args.next ();
		return true;
	}
	return false;
}

void IDL_Print::generate_code (const AST::Root& tree)
{
	path file = out_dir_ / tree.file ().filename ();
	file.replace_extension (".txt");
	Printer printer (file);
	if (tree.visit (printer))
		cerr << "Warning, some unsupported Building Blocks were ignored.\n";
}

using namespace AST;

void IDL_Print::start (Builder& builder)
{
	builder.module_begin (SimpleDeclarator ("Messaging", Location ()));
	builder.type_prefix (ScopedName (Location (), true, "Messaging"), "omg.org", Location ());
	builder.native (SimpleDeclarator ("ExceptionHolder", Location ()));
	builder.pragma_version (ScopedName (Location (), false, "ExceptionHolder"), { 3, 1 }, Location ());
	builder.interface_begin (SimpleDeclarator ("ReplyHandler", Location ()), InterfaceKind::UNCONSTRAINED);
	builder.interface_end ();
	builder.module_end ();
}
