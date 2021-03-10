// Printer.h Demonstrates the code generator class.
#ifndef NIDLFE_EXAMPLE_PRINTER_H_
#define NIDLFE_EXAMPLE_PRINTER_H_

#include <AST/CodeGen.h>
#include <filesystem>
#include <fstream>

class Printer : public AST::CodeGen
{
public:
	Printer (const std::filesystem::path& filename) :
		indent_ (0)
	{
		out_.exceptions (std::ofstream::failbit | std::ofstream::badbit);
		out_.open (filename);
	}

protected:
	virtual void include (const AST::Include& item);
	virtual void native (const AST::Native& item);
	virtual void type_def (const AST::TypeDef& item);
	virtual void constant (const AST::Constant& item);

	virtual void module_begin (const AST::ModuleItems& item);
	virtual void module_end (const AST::ModuleItems& item);

	virtual void interface_decl (const AST::InterfaceDecl& item);
	virtual void interface_begin (const AST::Interface& item);
	virtual void operation (const AST::Operation& item);
	virtual void parameter (const AST::Parameter& item);
	virtual void attribute (const AST::Attribute& item);
	virtual void interface_end (const AST::Interface& item);

	virtual void exception_begin (const AST::Exception& item);
	virtual void exception_end (const AST::Exception& item);

	virtual void struct_decl (const AST::StructDecl& item);
	virtual void struct_begin (const AST::Struct& item);
	virtual void struct_end (const AST::Struct& item);

	virtual void member (const AST::Member& item);

	virtual void union_decl (const AST::UnionDecl& item);
	virtual void union_begin (const AST::Union& item);
	virtual void union_element (const AST::UnionElement& item);
	virtual void union_end (const AST::Union& item);

	virtual void enum_type (const AST::Enum& item);

private:
	void indent ();
	void print_type (const AST::Type& t);

private:
	std::ofstream out_;
	unsigned indent_;
};



#endif
