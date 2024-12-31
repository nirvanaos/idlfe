# IDL compiler front-end library

This is a part of the [Nirvana](https://github.com/nirvanaos/home) project.

## Purpose

This library is intended for the easy creation of the CORBA IDL compilers.

It performs:

* Command line parsing
* Preprocessing
* Parsing IDL files
* Building the Abstract Syntax Tree

To create an IDL compiler:

* Derive your compiler class from `class IDL_FrontEnd`.
* Derive your back-end code generator from `class AST::CodeGen`.
* Override pure virtual method `IDL_FrontEnd::generate_code(AST::Root& tree)`.

For the example see: https://github.com/nirvanaos/idlfe/tree/master/example

API reference: https://nirvanaos.github.io/idlfe/

## Implemented Building Blocks

See: https://www.omg.org/spec/IDL/4.2/

* Core Data Types
* Extended Data Types
* Any
* Interfaces Basic
* Interfaces Full
* CORBA-Specific - Interfaces (except for `import`)
* Value Types
* CORBA-Specific - Value Types
* Anonimous Types (optional)

## How to use

### vcpkg Package manager
Use `idlfe` port from https://github.com/nirvanaos/vcpkg-registry.git.

## Build prerequisites

* CMake
* C++17 compilant compiler

## Used libraries

* Fixed point arithmetic library https://github.com/nirvanaos/decNumber
* SafeInt library https://github.com/dcleblanc/SafeInt.git

## Used submodules

* Danmar simple C/C++ preprocessor https://github.com/silver-popov/simplecpp
