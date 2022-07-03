/// \file CodeGen.h
/*
* Nirvana IDL front-end library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
*  popov.nirvana@gmail.com
*/
#ifndef NIDL_AST_CODEGEN_H_
#define NIDL_AST_CODEGEN_H_
#pragma once

#if __cplusplus < 201703L && _MSVC_LANG < 201703L
#error C++17 compliant compiler is required.
#endif

#include "Root.h"
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
#include "UnionElement.h"
#include "Enum.h"
#include "Exception.h"
#include "Array.h"
#include "Sequence.h"
#include "ValueType.h"
#include "StateMember.h"
#include "ValueFactory.h"
#include "ValueBox.h"

namespace AST {

/// \brief Base for code generators. Abstract class.
/// 
/// Supported Building Blocks (https://www.omg.org/spec/IDL/4.2/):
/// - Core Data Types
/// - Extended Data Types
/// - Any
/// - Interfaces Basic
/// - Interfaces Full
/// - CORBA - Specific Interfaces (except for `import`)
/// - Value Types
/// - CORBA - Specific - Value Types
/// - Anonimous Types
/// 
/// In future versions, it may be extended with derived classes CodeGen2, CodeGen3 etc.
/// 
class CodeGen
{
public:
	/// Begin code generation.
	/// 
	/// This method is not pure virtual and doed nothing by default.
	virtual void begin (const Root&) {}

	/// End code generation.
	/// 
	/// This method is not pure virtual and doed nothing by default.
	virtual void end (const Root&) {}

	/// `#include`
	virtual void leaf (const Include&) = 0;

	/// `native`
	virtual void leaf (const Native&) = 0;

	/// `typedef`
	virtual void leaf (const TypeDef&) = 0;

	/// `const`
	virtual void leaf (const Constant&) = 0;

	/// `module` begin.
	virtual void begin (const ModuleItems&) = 0;

	/// `module` end.
	virtual void end (const ModuleItems&) = 0;

	/// `interface` forward declaration.
	virtual void leaf (const InterfaceDecl&) = 0;

	/// `interface` begin.
	virtual void begin (const Interface&) = 0;

	/// `interface` end.
	virtual void end (const Interface&) = 0;

	/// Operation.
	virtual void leaf (const Operation&) = 0;

	/// `attribute`
	virtual void leaf (const Attribute&) = 0;

	/// `exception`
	virtual void leaf (const Exception&) = 0;

	/// `struct` forward declaration.
	virtual void leaf (const StructDecl&) = 0;

	/// `struct`
	virtual void leaf (const Struct&) = 0;

	/// `union` forward declaration.
	virtual void leaf (const UnionDecl&) = 0;

	/// `union`
	virtual void leaf (const Union&) = 0;

	/// `enum`
	virtual void leaf (const Enum&) = 0;

	/// `valuetype` forward declaration.
	virtual void leaf (const ValueTypeDecl&) = 0;

	/// `valuetype` begin.
	virtual void begin (const ValueType&) = 0;

	/// `valuetype` end.
	virtual void end (const ValueType&) = 0;

	/// `valuetype` state member.
	virtual void leaf (const StateMember&) = 0;

	/// `factory`
	virtual void leaf (const ValueFactory&) = 0;

	/// Boxed `valuetype`.
	virtual void leaf (const ValueBox&) = 0;
};

}

/// \example Printer.cpp
/// This is an example of how to use CodeGen class.
/// \include Printer.h

#endif
