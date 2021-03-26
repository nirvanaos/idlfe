// Printer.cpp Demonstrates the back-end code generator class.
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
		"long long",
		"float",
		"double",
		"long double",
		"Object",
		"ValueBase",
		"any"
	};

	switch (t.tkind ()) {
		case Type::Kind::BASIC_TYPE:
			out_ << basic_types [(size_t)t.basic_type ()];
			break;
		case Type::Kind::NAMED_TYPE:
			out_ << t.named_type ()->qualified_name ();
			break;
		case Type::Kind::STRING:
		case Type::Kind::WSTRING:
			out_ << (t.tkind () == Type::Kind::STRING ? "string" : "wstring");
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
		default:
			assert (false);
	}
}

void Printer::leaf (const Include& item)
{
	if (item.system ())
		out_ << "#include <" << item.file ().string () << ">\n";
	else
		out_ << "#include \"" << item.file ().string () << "\"\n";
}

void Printer::leaf (const Native& item)
{
	indent ();
	out_ << "native " << item.name () << ";\n";
}

void Printer::leaf (const TypeDef& item)
{
	indent ();
	out_ << "typedef ";
	print_type (item);
	out_ << ' ' << item.name () << ";\n";
}

void Printer::leaf (const Constant& item)
{
	indent ();
	out_ << "const ";
	print_type (item);
	out_ << ' ' << item.name () << " = " << item.to_string () <<";\n";
}

void Printer::begin (const ModuleItems& item)
{
	out_ << "module " << item.name () << " {\n";
}

void Printer::end (const ModuleItems& item)
{
	out_ << "};\n";
}

void Printer::print_interface_kind (const InterfaceKind ik)
{
	switch (ik.interface_kind ()) {
		case InterfaceKind::ABSTRACT:
			out_ << "abstract ";
			break;
		case InterfaceKind::LOCAL:
			out_ << "local ";
			break;
		case InterfaceKind::PSEUDO:
			out_ << "pseudo ";
			break;
	}
}

void Printer::leaf (const InterfaceDecl& item)
{
	indent ();
	print_interface_kind (item);
	out_ << "interface " << item.name () << ";\n";
}

void Printer::begin (const Interface& item)
{
	indent ();
	print_interface_kind (item);
	out_ << "interface " << item.name ();
	if (!item.bases ().empty ()) {
		out_ << " :\n";
		++indent_;
		auto base = item.bases ().begin ();
		indent ();
		out_ << (*base)->name ();
		++base;
		for (; base != item.bases ().end (); ++base) {
			out_ << ",\n";
			indent ();
			out_ << (*base)->name ();
		}
		--indent_;
	}
	out_ << '\n';
	indent ();
	out_ << "{\n";
	++indent_;
}

void Printer::end (const Interface& item)
{
	complex_end ();
}

void Printer::constructed_begin (const char* type, const NamedItem& item)
{
	indent ();
	out_ << type << item.name () << " {\n";
	++indent_;
}

void Printer::complex_end ()
{
	--indent_;
	indent ();
	out_ << "};\n";
}

void Printer::leaf (const Operation& item)
{
	indent ();
	if (item.oneway ())
		out_ << "oneway void";
	else if (item.tkind () == Type::Kind::VOID)
		out_ << "void";
	else
		print_type (item);

	print_op_base (item);

	if (!item.context ().empty ()) {
		out_ << " context (\"";
		auto s = item.context ().begin ();
		out_ << *s;
		for (++s; s != item.context ().end (); ++s) {
			out_ << "\", \"" << *s;
		}
		out_ << "\")";
	}
	out_ << ";\n";
}

void Printer::print_op_base (const AST::OperationBase& item)
{
	out_ << ' ' << item.name () << " (";
	if (!item.empty ()) {
		auto param = item.begin ();
		print (**param);
		for (++param; param != item.end (); ++param) {
			out_ << ", ";
			print (**param);
		}
	}
	out_ << ')';
	if (!item.raises ().empty ()) {
		out_ << " raises ";
		print (item.raises ());
	}
}

void Printer::print (const Raises& raises)
{
	auto ex = raises.begin ();
	out_ << '(' << (*ex)->name ();
	for (++ex; ex != raises.end (); ++ex) {
		out_ << ", " << (*ex)->name ();
	}
	out_ << ')';
}

void Printer::print (const Parameter& p)
{
	const char* att = "in";
	switch (p.attribute ()) {
		case Parameter::Attribute::OUT:
			att = "out";
			break;
		case Parameter::Attribute::INOUT:
			att = "inout";
			break;
	}
	out_ << att << ' ';
	print_type (p);
	out_ << ' ' << p.name ();
}

void Printer::leaf (const Attribute& item)
{
	indent ();
	if (item.readonly ()) {
		out_ << "readonly attribute ";
		print_type (item);
		out_ << ' ' << item.name ();
		if (!item.getraises ().empty ()) {
			out_ << " raises ";
			print (item.getraises ());
		}
	} else {
		out_ << "attribute ";
		print_type (item);
		out_ << ' ' << item.name ();
		if (!item.getraises ().empty ()) {
			out_ << " getraises ";
			print (item.getraises ());
		}
		if (!item.setraises ().empty ()) {
			out_ << " setraises ";
			print (item.setraises ());
		}
	}
	out_ << ";\n";
}

void Printer::begin (const Exception& item)
{
	constructed_begin ("exception ", item);
}

void Printer::end (const Exception& item)
{
	complex_end ();
}

void Printer::leaf (const StructDecl& item)
{
	indent ();
	out_ << "struct " << item.name () << ";\n";
}

void Printer::begin (const Struct& item)
{
	constructed_begin ("struct ", item);
}

void Printer::end (const Struct& item)
{
	complex_end ();
}

void Printer::leaf (const Member& item)
{
	indent ();
	print_type (item);
	out_ << ' ' << item.name () << ";\n";
}

void Printer::leaf (const UnionDecl& item)
{
	indent ();
	out_ << "union " << item.name () << ";\n";
}

void Printer::begin (const Union& item)
{
	indent ();
	out_ << "union " << item.name () << " switch (";
	print_type (item.discriminator_type ());
	out_ << ") {\n";
	++indent_;
}

void Printer::leaf (const UnionElement& item)
{
	indent ();
	if (!item.is_default ()) {
		for (const auto& label : item.labels ()) {
			indent ();
			out_ << "case " << label.to_string () << ":\n";
		}
	} else
		out_ << "default:\n";
	++indent_;
	indent ();
	print_type (item);
	out_ << ' ' << item.name () << ";\n";
	--indent_;
}

void Printer::end (const Union& item)
{
	complex_end ();
}

void Printer::leaf (const Enum& item)
{
	constructed_begin ("enum ", item);
	auto it = item.begin ();
	indent ();
	out_ << (*it)->name ();
	for (++it; it != item.end (); ++it) {
		out_ << ",\n";
		indent ();
		out_ << (*it)->name ();
	}
	out_ << endl;
	complex_end ();
}

void Printer::leaf (const ValueTypeDecl& item)
{
	indent ();
	if (item.is_abstract ())
		out_ << "abstract ";
	out_ << "valuetype " << item.name () << ";\n";
}

void Printer::begin (const ValueType& item)
{
	indent ();
	switch (item.modifier ()) {
		case ValueType::Modifier::ABSTRACT:
			out_ << "abstract ";
			break;
		case ValueType::Modifier::CUSTOM:
			out_ << "custom ";
			break;
	}
	out_ << "valuetype " << item.name ();

	if (!item.bases ().empty ()) {
		out_ << ":\n";
		++indent_;
		auto base = item.bases ().begin ();
		indent ();
		out_ << (*base)->name ();
		++base;
		for (; base != item.bases ().end (); ++base) {
			out_ << ",\n";
			indent ();
			out_ << (*base)->name ();
		}
		--indent_;
	}

	if (!item.supports ().empty ()) {
		out_ << "supports\n";
		++indent_;
		auto base = item.supports ().begin ();
		indent ();
		if (item.modifier () == ValueType::Modifier::TRUNCATABLE)
			out_ << "truncatable ";
		out_ << (*base)->name ();
		++base;
		for (; base != item.supports ().end (); ++base) {
			out_ << ",\n";
			indent ();
			out_ << (*base)->name ();
		}
		--indent_;
	}

	out_ << '\n';
	indent ();
	out_ << "{\n";
	++indent_;
}

void Printer::end (const ValueType& item)
{
	complex_end ();
}

void Printer::leaf (const StateMember& item)
{
	indent ();
	if (item.is_public ())
		out_ << "public ";
	else
		out_ << "private ";
	print_type (item);
	out_ << ' ' << item.name () << ";\n";
}

void Printer::leaf (const ValueFactory& item)
{
	indent ();
	out_ << "factory ";
	print_op_base (item);
	out_ << ";\n";
}

void Printer::leaf (const ValueBox& item)
{
	indent ();
	out_ << "valuetype " << item.name () << ' ';
	print_type (item);
	out_ << ";\n";
}
