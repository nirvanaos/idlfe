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

Install nirvanaos.idlfe.vc.1.#.# NuGet package.

## How to build

### MS Visual Studio

To build with MS Visual Studio use supermodule https://github.com/nirvanaos/idlfe.vc.git

### Other compilers

To create own compilation project you must have:

* C++17 compilant compiler
* Flex + Bison
* Fixed point arithmetic library https://github.com/nirvanaos/decNumber

## Used submodules

Simple C/C++ preprocessor https://github.com/danmar/simplecpp

SafeInt library https://github.com/dcleblanc/SafeInt.git
