// Printer.cpp Demonstrates the code generator.
#include "Printer.h"

using namespace std;
using namespace AST;

void Printer::indent ()
{
	for (unsigned cnt = indent_; cnt; --cnt)
		out_ << '\t';
}

void Printer::print_type (const Type& t)
{
	static const char* const basic_types [(size_t)BasicType::ANY + 1] = {
		"boolean",
		"octet",
		"char",
		"wchar",
		"unsigned short",
		"unsigned long",
		"unsigned longlong",
		"short",
		"long",
		"float",
		"double",
		"long double",
		"Object",
		"ValueBase",
		"any"
	};

	switch (t.kind ()) {
		case Type::Kind::BASIC_TYPE:
			out_ << basic_types [(size_t)t.basic_type ()];
			break;
		case Type::Kind::NAMED_TYPE:
			out_ << t.named_type ()->name ();
			break;
		case Type::Kind::STRING:
		case Type::Kind::WSTRING:
			out_ << (t.kind () == Type::Kind::STRING) ? "string" : "wstring";
			if (t.string_size ())
				out_ << " <" << t.string_size () << '>';
			break;
		case Type::Kind::FIXED:
			out_ << "fixed";
			if (t.fixed_digits ())
				out_ << " <" << t.fixed_digits () << ", " << t.fixed_scale () << '>';
			break;
		case Type::Kind::SEQUENCE: {
			const Sequence& seq = t.sequence ();
			out_ << "sequence <";
			print_type (seq);
			if (seq.size ())
				out_ << ", " << seq.size ();
			out_ << '>';
		} break;
		case Type::Kind::ARRAY: {
			const Array& arr = t.array ();
			print_type (arr);
			for (auto dim : arr.dimensions ()) {
				out_ << '[' << dim << ']';
			}
		} break;
	}
}

void Printer::include (const Include& item)
{
	out_ << "#include \"" << item.file () << "\"\n";
}

void Printer::native (const Native& item)
{
	indent ();
	out_ << "native " << item.name () << ";\n";
}

void Printer::type_def (const TypeDef& item)
{
	indent ();
	out_ << "typedef ";
	print_type (item);
	out_ << ' ' << item.name () << ";\n";
}

void Printer::constant (const Constant& item) {}

void Printer::module_begin (const ModuleItems& item) {}
void Printer::module_end (const ModuleItems& item) {}

void Printer::interface_decl (const InterfaceDecl& item) {}
void Printer::interface_begin (const Interface& item) {}
void Printer::operation (const Operation& item) {}
void Printer::parameter (const Parameter& item) {}
void Printer::attribute (const Attribute& item) {}
void Printer::interface_end (const Interface& item) {}

void Printer::exception_begin (const Exception& item) {}
void Printer::exception_end (const Exception& item) {}

void Printer::struct_decl (const StructDecl& item) {}
void Printer::struct_begin (const Struct& item) {}
void Printer::struct_end (const Struct& item) {}

void Printer::member (const Member& item) {}

void Printer::union_decl (const UnionDecl& item) {}
void Printer::union_begin (const Union& item) {}
void Printer::union_element (const UnionElement& item) {}
void Printer::union_end (const Union& item) {}

void Printer::enum_type (const Enum& item) {}
