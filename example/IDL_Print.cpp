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

void IDL_Print::file_begin (const std::filesystem::path& file, Builder& builder)
{
	ami_map_.clear ();

	Location loc = builder.location ();
	builder.module_begin (SimpleDeclarator ("Messaging", loc));
	builder.type_prefix (ScopedName (Location (), true, "Messaging"), "omg.org", loc);

	builder.valuetype_begin (SimpleDeclarator ("Poller", loc), ValueType::Modifier::ABSTRACT);
	builder.valuetype_end ();

	builder.native (SimpleDeclarator ("ExceptionHolder", loc));
	builder.pragma_version (ScopedName (loc, false, "ExceptionHolder"), { 3, 1 }, loc);

	builder.interface_begin (SimpleDeclarator ("ReplyHandler", loc), InterfaceKind::UNCONSTRAINED);
	builder.interface_end ();
	builder.module_end ();

	builder.module_begin (SimpleDeclarator ("CORBA", loc));
	builder.type_prefix (ScopedName (loc, true, "CORBA"), "omg.org", loc);
	builder.exception_begin (SimpleDeclarator ("WrongTransaction", loc));
	builder.exception_end ();
	builder.module_end ();
}

void IDL_Print::interface_end (const Interface& itf, Builder& builder)
{
	static const char* AMI_POLLER = "Poller";
	static const char* AMI_HANDLER = "Handler";
	static const char* AMI_EXCEP = "_excep";

	if (async_support (itf)) {
		AMI_Identifiers ids{ make_ami_id (itf, AMI_POLLER), make_ami_id (itf, AMI_HANDLER) };

		Location loc = builder.location ();
		SimpleDeclarator ami_return_val ("ami_return_val", loc);

		AMI_Bases async_bases;
		for (const auto b : itf.bases ()) {
			if (async_support (*b))
				async_bases.push_back (&*ami_map_.find (b));
		}

		{ // Make Poller
			SimpleDeclarator ami_timeout ("ami_timeout", loc);

			builder.valuetype_begin (SimpleDeclarator (ids.poller, loc), ValueType::Modifier::ABSTRACT);

			ScopedNames bases;
			for (const auto b : async_bases) {
				ScopedName base = b->first->scoped_name ();
				base.pop_back ();
				base.push_back (b->second.poller);
				bases.push_front (std::move (base));
			}
			if (bases.empty ()) {
				ScopedName base (loc, true, { "Messaging", "Poller" });
				bases.push_front (std::move (base));
			}
			builder.valuetype_bases (false, bases);

			for (auto item : itf) {
				switch (item->kind ()) {
				case Item::Kind::OPERATION: {
					const Operation& op = static_cast <const Operation&> (*item);
					builder.operation_begin (false, Type (), SimpleDeclarator (op.name (), loc));

					builder.parameter (Parameter::Attribute::IN, Type (BasicType::ULONG), ami_timeout);
					if (op.tkind () != Type::Kind::VOID)
						builder.parameter (Parameter::Attribute::OUT, Type (op), ami_return_val);

					for (auto par : op) {
						if (par->attribute () != Parameter::Attribute::IN)
							builder.parameter (Parameter::Attribute::OUT, Type (*par), SimpleDeclarator (par->name (), loc));
					}

					builder.raises (poller_raises (loc, op.raises ()));
					builder.operation_end ();
				} break;

				case Item::Kind::ATTRIBUTE: {
					const Attribute& att = static_cast <const Attribute&> (*item);

					builder.operation_begin (false, Type (), SimpleDeclarator ("get_" + att.name (), loc));
					builder.parameter (Parameter::Attribute::IN, Type (BasicType::ULONG), ami_timeout);
					builder.parameter (Parameter::Attribute::OUT, Type (att), ami_return_val);
					builder.raises (poller_raises (loc, att.getraises ()));
					builder.operation_end ();

					if (!att.readonly ()) {
						builder.operation_begin (false, Type (), SimpleDeclarator ("set_" + att.name (), loc));
						builder.parameter (Parameter::Attribute::IN, Type (BasicType::ULONG), ami_timeout);
						builder.raises (poller_raises (loc, att.setraises ()));
						builder.operation_end ();
					}
				} break;
				}
			}

			builder.valuetype_end ();
		}

		{ // Make Handler

			const Type exception_holder = builder.lookup_type (ScopedName (loc, true, { "Messaging", "ExceptionHolder" }));
			SimpleDeclarator excep_holder ("excep_holder", loc);

			builder.interface_begin (SimpleDeclarator (ids.handler, loc), InterfaceKind::UNCONSTRAINED);

			ScopedNames bases;
			for (const auto b : async_bases) {
				ScopedName base = b->first->scoped_name ();
				base.pop_back ();
				base.push_back (b->second.handler);
				bases.push_front (std::move (base));
			}
			if (bases.empty ()) {
				ScopedName base (loc, true, { "Messaging", "ReplyHandler" });
				bases.push_front (std::move (base));
			}
			builder.interface_bases (bases);

			for (auto item : itf) {
				switch (item->kind ()) {
				case Item::Kind::OPERATION: {
					const Operation& op = static_cast <const Operation&> (*item);

					builder.operation_begin (false, Type (), SimpleDeclarator (op.name (), loc));

					if (op.tkind () != Type::Kind::VOID)
						builder.parameter (Parameter::Attribute::IN, Type (op), ami_return_val);

					for (auto par : op) {
						if (par->attribute () != Parameter::Attribute::IN)
							builder.parameter (Parameter::Attribute::IN, Type (*par), SimpleDeclarator (par->name (), loc));
					}

					builder.operation_end ();

					builder.operation_begin (false, Type (), SimpleDeclarator (op.name () + AMI_EXCEP, loc));
					builder.parameter (Parameter::Attribute::IN, Type (exception_holder), excep_holder);
					builder.operation_end ();

				} break;

				case Item::Kind::ATTRIBUTE: {
					const Attribute& att = static_cast <const Attribute&> (*item);

					builder.operation_begin (false, Type (), SimpleDeclarator ("get_" + att.name (), loc));
					builder.parameter (Parameter::Attribute::IN, Type (att), ami_return_val);
					builder.operation_end ();

					builder.operation_begin (false, Type (), SimpleDeclarator ("get_" + att.name () + AMI_EXCEP, loc));
					builder.parameter (Parameter::Attribute::IN, Type (exception_holder), excep_holder);
					builder.operation_end ();

					if (!att.readonly ()) {
						builder.operation_begin (false, Type (), SimpleDeclarator ("set_" + att.name (), loc));
						builder.operation_end ();

						builder.operation_begin (false, Type (), SimpleDeclarator ("set_" + att.name () + AMI_EXCEP, loc));
						builder.parameter (Parameter::Attribute::IN, Type (exception_holder), excep_holder);
						builder.operation_end ();
					}
				} break;
				}
			}

			builder.interface_end ();
		}

		ami_map_.emplace (&itf, std::move (ids));
	}
}

bool IDL_Print::async_support (const AST::Interface& itf)
{
	return (itf.interface_kind () == InterfaceKind::UNCONSTRAINED);
}

Identifier IDL_Print::make_ami_id (const Interface& itf, const char* suffix)
{
	std::string id = "AMI_" + itf.name () + suffix;
	const Symbols* scope = itf.parent_scope ();
	assert (scope);
	while (scope && scope->find (static_cast <const Identifier&> (id))) {
		id.insert (0, "AMI_");
	}
	return id;
}

ScopedNames IDL_Print::poller_raises (const AST::Location& loc, const AST::Raises& op_raises)
{
	bool wrong_transaction_found = false;
	ScopedName wrong_transaction (loc, true, { "CORBA", "WrongTransaction" });
	ScopedNames exceptions;
	for (auto ex : op_raises) {
		ScopedName sn = ex->scoped_name ();
		if (sn == wrong_transaction)
			wrong_transaction_found = true;
		exceptions.push_front (std::move (sn));
	}
	if (!wrong_transaction_found)
		exceptions.push_front (std::move (wrong_transaction));

	return exceptions;
}
