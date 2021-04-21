# Changelog

## Unreleased
### Fixed
- Bugs.
### Added
- size_t IndentedOut::size() const;

## 1.3.1 - 18.04.2021
### Fixed
- Bugs.

## 1.3.0 - 14.04.2021
### Added
- char IndentedOut::last_char() const;

## 1.2.1 - 14.04.2021
### Fixed
- The incompatibility with C-lang was fixed.

## 1.2.0 - 05.04.2021
### Added
- Added flag to disallow anonymous IDL types.

### Changed
- AST\::Parameter and AST\::Attribute classes now inherit AST\::Member.
- AST\::Type\::named_type() now returns reference instead of pointer.

## 1.1.1 - 03.04.2021
### Added
- class BE\::IndentedOut.
### Changed
- AST\::Interface\::get_all_bases () now returns AST\::Interfaces.
- pseudo interfaces may inherit other pseudo interfaces.

## 1.0.1 - 28.03.2021
### Fixed
- Bugs.
### Changed
- Rename AST\::Type\::string_size() => AST\::Type\::string_bound().
- Rename AST\::Sequence\::size() => AST\::Sequence\::bound().
