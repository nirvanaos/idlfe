# Changelog

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
