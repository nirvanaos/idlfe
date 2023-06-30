# Changelog

## 3.1.0 - 2023-11-21
### Added
 - Equality operator for Type.
### Changed
 - Allow repeated typedef and native declarations.
### Fixed
 - Directory creation in BE::IndentedOut.
 - Check for collision with reserved words.

## 3.0.2 - 2022-11-21
### Fixed
 - Validate context strings.

## 3.0.1 - 2022-10-01
### Fixed
 - Union discriminator type.

## 3.0.0 - 2022-08-14
## Changed
 - Allow native type in the exception list, as it required by the standard.
 - Added class BE::MessageOut for the convenient compiler messages output.
 - Messages are redirected to `std::cerr` by default, instead of `std::cout`.
 - Added parameter `std::ostream& err_out` to `IDL_FrontEnd` constructor.
 
## 2.1.8 - 2022-08-13
### Fixed
 - Bug: enum from #include was empty.

## 2.1.7 - 2022-08-10
### Fixed
 - Overflow in `unsigned long long` constant calculation.

## 2.1.6 - 2022-07-12
### Changed
 - Fixed constants normalized. Fixed::scale () may be < 0.

## 2.1.5 - 2022-07-11
### Fixed
 - Bug in `fixed` type processing.

## 2.1.4 - 2022-07-08
### Fixed
 - Eliminated possible raise() on fixed-point calculation errors.

## 2.1.3 - 2022-07-06
### Fixed
 - Yet another bug in string literals.

## 2.1.2 - 2022-07-06
### Fixed
 - Bug in string literals.

## 2.1.1 - 2022-07-06
### Changed
 - `Fixed::to_BCD()` Redundant out parameter `scale` was removed.
### Fixed
 - Documentation.

## 2.1.0 - 2022-07-04
### Changed
 - StructDecl and UnionDecl now have attribute definition() to obtain definition for declaration.
### Fixed
 - Checking for type completeness was fixed.

## 2.0.0 - 2022-07-03
Library was completely refactored.

## 1.6.1 - 2022-06-30
### Fixed
 - Bug in checking type completeness.

## 1.6.0 - 2022-06-30
### Changed
 - AST::Fixed API.
### Fixed
 - Fixed point constants are normalized to <= 31 digits.

## 1.5.3 - 2022-06-28
### Fixed
 - Bug: Redundant error "incomplete type is not allowed" for interface reference.
 - Bug: Redundant assertion in `enum`.

## 1.5.2 - 2022-06-27
### Fixed
 - Bug in unions.

## 1.5.1 - 2022-06-27
### Changed
 - Class hierarchy optimized.

## 1.5.0 - 2022-06-24
### Changed
 - `class AST::Union`
     - Added property `default_element();`
     - Added property `default_label();`
 - Added `class AST::Fixed`.

## 1.4.8 - 2022-06-22
### Changed
- IndentedOut: unindent preprocessor directives.

## 1.4.7 - 2022-06-10
### Fixed
- Bug: The factory with empty parameter list caused a syntax error.

## 1.4.6 - 2022-03-27
### Changed
- Improved check of the valuetype inheritance rules.

## 1.4.4 - 2022-02-09
### Fixed
- Bug in bounded strings.

## 1.4.3 - 2022-02-06
### Fixed
- Bug in pseudo interface.

## 1.4.2 - 2021-12-26
### Fixed
- Bug in #include processing.

## 1.4.1 - 2021-12-26
### Fixed
- Bug in constant evaluation.

## 1.4.0 - 2021-04-26
### Added
- class ForwardDeclarable.

## 1.3.3 - 2021-04-21
### Fixed
- Bug.

## 1.3.2 - 2021-04-21 YANKED
### Fixed
- Bugs.
### Added
- size_t IndentedOut::size() const;

## 1.3.1 - 2021-04-18
### Fixed
- Bugs.

## 1.3.0 - 2021-04-14
### Added
- char IndentedOut::last_char() const;

## 1.2.1 - 2021-04-14
### Fixed
- The incompatibility with C-lang was fixed.

## 1.2.0 - 2021-04-05
### Added
- Added flag to disallow anonymous IDL types.

### Changed
- AST\::Parameter and AST\::Attribute classes now inherit AST\::Member.
- AST\::Type\::named_type() now returns reference instead of pointer.

## 1.1.1 - 2021-04-03
### Added
- class BE\::IndentedOut.
### Changed
- AST\::Interface\::get_all_bases () now returns AST\::Interfaces.
- pseudo interfaces may inherit other pseudo interfaces.

## 1.0.1 - 2021-03-28
### Fixed
- Bugs.
### Changed
- Rename AST\::Type\::string_size() => AST\::Type\::string_bound().
- Rename AST\::Sequence\::size() => AST\::Sequence\::bound().
