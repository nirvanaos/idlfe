/// \file CodeGen.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  silver.popov@google.com
*/
#ifndef NIDL_AST_CODEGEN_H_
#define NIDL_AST_CODEGEN_H_

#include "AST.h"
#include "Include.h"
#include "Native.h"
#include "TypeDef.h"
#include "Constant.h"
#include "Module.h"
#include "Interface.h"
#include "Operation.h"
#include "Parameter.h"
#include "Attribute.h"
#include "Struct.h"
#include "Union.h"
#include "Member.h"
#include "Enum.h"
#include "Exception.h"
#include "Array.h"
#include "Sequence.h"

namespace AST {

/// Base for code generators. Abstract class.
/// In future versions, it will be extended with derived classes CodeGen2, CodeGen3 etc.
/// 
/// Supported Building Blocks:
/// - Core Data Types
/// - Extended Data Types
/// - Any
/// - Interfaces Basic
/// - Interfaces Full
/// - CORBA - Specific Interfaces (except for `import`)
/// - Anonimous Types

class CodeGen
{
public:
	virtual void begin () {}
	virtual void end () {}

	virtual void include (const Include& item) = 0;
	virtual void native (const Native& item) = 0;
	virtual void type_def (const TypeDef& item) = 0;
	virtual void constant (const Constant& item) = 0;

	virtual void module_begin (const ModuleItems& item) = 0;
	virtual void module_end (const ModuleItems& item) = 0;

	virtual void interface_decl (const InterfaceDecl& item) = 0;
	virtual void interface_begin (const Interface& item) = 0;
	virtual void operation (const Operation& item) = 0;
	virtual void parameter (const Parameter& item) = 0;
	virtual void attribute (const Attribute& item) = 0;
	virtual void interface_end (const Interface& item) = 0;

	virtual void exception_begin (const Exception& item) = 0;
	virtual void exception_end (const Exception& item) = 0;

	virtual void struct_decl (const StructDecl& item) = 0;
	virtual void struct_begin (const Struct& item) = 0;
	virtual void struct_end (const Struct& item) = 0;

	virtual void member (const Member& item) = 0;

	virtual void union_decl (const UnionDecl& item) = 0;
	virtual void union_begin (const Union& item) = 0;
	virtual void union_element (const UnionElement& item) = 0;
	virtual void union_end (const Union& item) = 0;

	virtual void enum_type (const Enum& item) = 0;
};

}

#endif
