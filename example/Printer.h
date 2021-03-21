// Printer.h Demonstrates the back-end code generator class.
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
	virtual void leaf (const AST::Include& item);
	virtual void leaf (const AST::Native& item);
	virtual void leaf (const AST::TypeDef& item);
	virtual void leaf (const AST::Constant& item);

	virtual void begin (const AST::ModuleItems& item);
	virtual void end (const AST::ModuleItems& item);

	virtual void leaf (const AST::InterfaceDecl& item);
	
	virtual void begin (const AST::Interface& item);
	virtual void end (const AST::Interface& item);

	virtual void leaf (const AST::Operation& item);
	virtual void leaf (const AST::Attribute& item);

	virtual void begin (const AST::Exception& item);
	virtual void end (const AST::Exception& item);

	virtual void leaf (const AST::StructDecl& item);
	virtual void begin (const AST::Struct& item);
	virtual void end (const AST::Struct& item);

	virtual void leaf (const AST::Member& item);

	virtual void leaf (const AST::UnionDecl& item);
	virtual void begin (const AST::Union& item);
	virtual void end (const AST::Union& item);
	
	virtual void leaf (const AST::UnionElement& item);

	virtual void leaf (const AST::Enum& item);
	
	virtual void leaf (const AST::ValueTypeDecl& item);
	virtual void begin (const AST::ValueType& item);
	virtual void end (const AST::ValueType& item);
	virtual void leaf (const AST::StateMember& item);
	virtual void leaf (const AST::ValueFactory& item);
	virtual void leaf (const AST::ValueBox& item);

private:
	void indent ();
	void print_type (const AST::Type& t);
	void print_interface_kind (const AST::InterfaceKind ik);
	void print_op_base (const AST::OperationBase& item);
	void print (const AST::Parameter& p);
	void constructed_begin (const char* type, const AST::NamedItem& item);
	void complex_end ();
	void print (const AST::Raises& raises);

private:
	std::ofstream out_;
	unsigned indent_;
};



#endif
