// Printer.h Demonstrates the back-end code generator class.
#ifndef NIDLFE_EXAMPLE_PRINTER_H_
#define NIDLFE_EXAMPLE_PRINTER_H_

#include <AST/CodeGen.h>
#include <BE/IndentedOut.h>

class Printer : public AST::CodeGen
{
public:
	Printer (const std::filesystem::path& filename) :
		out_ (filename)
	{}

protected:
	virtual void end (const AST::Root& ast);

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

	virtual void leaf (const AST::Exception& item);

	virtual void leaf (const AST::StructDecl& item);
	virtual void leaf (const AST::Struct& item);

	virtual void leaf (const AST::UnionDecl& item);
	virtual void leaf (const AST::Union& item);
	
	virtual void leaf (const AST::Enum& item);
	
	virtual void leaf (const AST::ValueTypeDecl& item);
	virtual void begin (const AST::ValueType& item);
	virtual void end (const AST::ValueType& item);
	virtual void leaf (const AST::StateMember& item);
	virtual void leaf (const AST::ValueFactory& item);
	virtual void leaf (const AST::ValueBox& item);

private:
	void print_type (const AST::Type& t);
	void print_interface_kind (const AST::InterfaceKind ik);
	void print_op_base (const AST::OperationBase& item);
	void print (const AST::Parameter& p);
	void constructed_begin (const char* type, const AST::NamedItem& item);
	void constructed (const AST::StructBase& item);
	void complex_end ();
	void print (const AST::Raises& raises);

private:
	BE::IndentedOut out_;
};

#endif
