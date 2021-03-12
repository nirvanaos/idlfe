# IDL compiler front-end library

This is a part of the Nirvana project.

## Purpose

This library is intended for the easy creation of the CORBA IDL compilers.

It performs:

* Command line parsing
* Preprocessing
* Parsing IDL files
* Building the Abstract Syntax Tree

To create an IDL compiler, the user just derives his own compiler class from `class IDL_FrontEnd`.

## Implemented Building Blocks

* Core Data Types
* Extended Data Types
* Any
* Interfaces Basic
* Interfaces Full
* CORBA-Specific Interfaces (except for `import`)
* Anonimous Types

## How to build
Requires a C++17 compilant compiler.
To build with MS Visual Studio use supermodule https://github.com/nirvanaos/idlfe.vc.git
