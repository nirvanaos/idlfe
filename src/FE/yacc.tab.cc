// A Bison parser, made by GNU Bison 3.7.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 23 "yacc.yy"

#include "Driver.h"

#line 45 "yacc.tab.cc"


// "%code requires" blocks.
#line 32 "yacc.yy"

	class Driver;
#include "../AST/ScopedName.h"
#include "../AST/Type.h"

#line 55 "yacc.tab.cc"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif



#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

namespace yy {
#line 184 "yacc.tab.cc"


  /// A point in a source file.
  class position
  {
  public:
    /// Type for file name.
    typedef const std::string filename_type;
    /// Type for line and column numbers.
    typedef int counter_type;

    /// Construct a position.
    explicit position (filename_type* f = YY_NULLPTR,
                       counter_type l = 1,
                       counter_type c = 1)
      : filename (f)
      , line (l)
      , column (c)
    {}


    /// Initialization.
    void initialize (filename_type* fn = YY_NULLPTR,
                     counter_type l = 1,
                     counter_type c = 1)
    {
      filename = fn;
      line = l;
      column = c;
    }

    /** \name Line and Column related manipulators
     ** \{ */
    /// (line related) Advance to the COUNT next lines.
    void lines (counter_type count = 1)
    {
      if (count)
        {
          column = 1;
          line = add_ (line, count, 1);
        }
    }

    /// (column related) Advance to the COUNT next columns.
    void columns (counter_type count = 1)
    {
      column = add_ (column, count, 1);
    }
    /** \} */

    /// File name to which this position refers.
    filename_type* filename;
    /// Current line number.
    counter_type line;
    /// Current column number.
    counter_type column;

  private:
    /// Compute max (min, lhs+rhs).
    static counter_type add_ (counter_type lhs, counter_type rhs, counter_type min)
    {
      return lhs + rhs < min ? min : lhs + rhs;
    }
  };

  /// Add \a width columns, in place.
  inline position&
  operator+= (position& res, position::counter_type width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns.
  inline position
  operator+ (position res, position::counter_type width)
  {
    return res += width;
  }

  /// Subtract \a width columns, in place.
  inline position&
  operator-= (position& res, position::counter_type width)
  {
    return res += -width;
  }

  /// Subtract \a width columns.
  inline position
  operator- (position res, position::counter_type width)
  {
    return res -= width;
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param pos a reference to the position to redirect
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const position& pos)
  {
    if (pos.filename)
      ostr << *pos.filename << ':';
    return ostr << pos.line << '.' << pos.column;
  }

  /// Two points in a source file.
  class location
  {
  public:
    /// Type for file name.
    typedef position::filename_type filename_type;
    /// Type for line and column numbers.
    typedef position::counter_type counter_type;

    /// Construct a location from \a b to \a e.
    location (const position& b, const position& e)
      : begin (b)
      , end (e)
    {}

    /// Construct a 0-width location in \a p.
    explicit location (const position& p = position ())
      : begin (p)
      , end (p)
    {}

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit location (filename_type* f,
                       counter_type l = 1,
                       counter_type c = 1)
      : begin (f, l, c)
      , end (f, l, c)
    {}


    /// Initialization.
    void initialize (filename_type* f = YY_NULLPTR,
                     counter_type l = 1,
                     counter_type c = 1)
    {
      begin.initialize (f, l, c);
      end = begin;
    }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step ()
    {
      begin = end;
    }

    /// Extend the current location to the COUNT next columns.
    void columns (counter_type count = 1)
    {
      end += count;
    }

    /// Extend the current location to the COUNT next lines.
    void lines (counter_type count = 1)
    {
      end.lines (count);
    }
    /** \} */


  public:
    /// Beginning of the located region.
    position begin;
    /// End of the located region.
    position end;
  };

  /// Join two locations, in place.
  inline location&
  operator+= (location& res, const location& end)
  {
    res.end = end.end;
    return res;
  }

  /// Join two locations.
  inline location
  operator+ (location res, const location& end)
  {
    return res += end;
  }

  /// Add \a width columns to the end position, in place.
  inline location&
  operator+= (location& res, location::counter_type width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns to the end position.
  inline location
  operator+ (location res, location::counter_type width)
  {
    return res += width;
  }

  /// Subtract \a width columns to the end position, in place.
  inline location&
  operator-= (location& res, location::counter_type width)
  {
    return res += -width;
  }

  /// Subtract \a width columns to the end position.
  inline location
  operator- (location res, location::counter_type width)
  {
    return res -= width;
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param loc a reference to the location to redirect
   **
   ** Avoid duplicate information.
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const location& loc)
  {
    location::counter_type end_col
      = 0 < loc.end.column ? loc.end.column - 1 : 0;
    ostr << loc.begin;
    if (loc.end.filename
        && (!loc.begin.filename
            || *loc.begin.filename != *loc.end.filename))
      ostr << '-' << loc.end.filename << ':' << loc.end.line << '.' << end_col;
    else if (loc.begin.line < loc.end.line)
      ostr << '-' << loc.end.line << '.' << end_col;
    else if (loc.begin.column < end_col)
      ostr << '-' << end_col;
    return ostr;
  }


  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
    {
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    semantic_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    semantic_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // base_type_spec
      // floating_pt_type
      // integer_type
      // signed_int
      // unsigned_int
      char dummy1[sizeof (AST::BasicType)];

      // scoped_name
      char dummy2[sizeof (AST::ScopedName)];

      // simple_type_spec
      // string_type
      // wide_string_type
      // op_type_spec
      // param_type_spec
      char dummy3[sizeof (AST::Type)];

      // T_CHARACTER_LITERAL
      // T_FIXED_PT_LITERAL
      // T_FLOATING_PT_LITERAL
      // T_IDENTIFIER
      // T_INTEGER_LITERAL
      // T_STRING_LITERAL
      // T_PRAGMA
      char dummy4[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    T_AMPERSAND = 258,             // T_AMPERSAND
    T_ANY = 259,                   // T_ANY
    T_ASTERISK = 260,              // T_ASTERISK
    T_ATTRIBUTE = 261,             // T_ATTRIBUTE
    T_BOOLEAN = 262,               // T_BOOLEAN
    T_CASE = 263,                  // T_CASE
    T_CHAR = 264,                  // T_CHAR
    T_CHARACTER_LITERAL = 265,     // T_CHARACTER_LITERAL
    T_CIRCUMFLEX = 266,            // T_CIRCUMFLEX
    T_COLON = 267,                 // T_COLON
    T_COMMA = 268,                 // T_COMMA
    T_CONST = 269,                 // T_CONST
    T_CONTEXT = 270,               // T_CONTEXT
    T_DEFAULT = 271,               // T_DEFAULT
    T_DOUBLE = 272,                // T_DOUBLE
    T_ENUM = 273,                  // T_ENUM
    T_EQUAL = 274,                 // T_EQUAL
    T_EXCEPTION = 275,             // T_EXCEPTION
    T_FALSE = 276,                 // T_FALSE
    T_FIXED = 277,                 // T_FIXED
    T_FIXED_PT_LITERAL = 278,      // T_FIXED_PT_LITERAL
    T_FLOAT = 279,                 // T_FLOAT
    T_FLOATING_PT_LITERAL = 280,   // T_FLOATING_PT_LITERAL
    T_GREATER_THAN_SIGN = 281,     // T_GREATER_THAN_SIGN
    T_IDENTIFIER = 282,            // T_IDENTIFIER
    T_IN = 283,                    // T_IN
    T_INOUT = 284,                 // T_INOUT
    T_INTEGER_LITERAL = 285,       // T_INTEGER_LITERAL
    T_INTERFACE = 286,             // T_INTERFACE
    T_LEFT_CURLY_BRACKET = 287,    // T_LEFT_CURLY_BRACKET
    T_LEFT_PARANTHESIS = 288,      // T_LEFT_PARANTHESIS
    T_LEFT_SQUARE_BRACKET = 289,   // T_LEFT_SQUARE_BRACKET
    T_LESS_THAN_SIGN = 290,        // T_LESS_THAN_SIGN
    T_LONG = 291,                  // T_LONG
    T_MINUS_SIGN = 292,            // T_MINUS_SIGN
    T_MODULE = 293,                // T_MODULE
    T_OCTET = 294,                 // T_OCTET
    T_ONEWAY = 295,                // T_ONEWAY
    T_OUT = 296,                   // T_OUT
    T_PERCENT_SIGN = 297,          // T_PERCENT_SIGN
    T_PLUS_SIGN = 298,             // T_PLUS_SIGN
    T_RAISES = 299,                // T_RAISES
    T_READONLY = 300,              // T_READONLY
    T_RIGHT_CURLY_BRACKET = 301,   // T_RIGHT_CURLY_BRACKET
    T_RIGHT_PARANTHESIS = 302,     // T_RIGHT_PARANTHESIS
    T_RIGHT_SQUARE_BRACKET = 303,  // T_RIGHT_SQUARE_BRACKET
    T_SCOPE = 304,                 // T_SCOPE
    T_SEMICOLON = 305,             // T_SEMICOLON
    T_SEQUENCE = 306,              // T_SEQUENCE
    T_SHIFTLEFT = 307,             // T_SHIFTLEFT
    T_SHIFTRIGHT = 308,            // T_SHIFTRIGHT
    T_SHORT = 309,                 // T_SHORT
    T_SOLIDUS = 310,               // T_SOLIDUS
    T_STRING = 311,                // T_STRING
    T_STRING_LITERAL = 312,        // T_STRING_LITERAL
    T_PRAGMA = 313,                // T_PRAGMA
    T_STRUCT = 314,                // T_STRUCT
    T_SWITCH = 315,                // T_SWITCH
    T_TILDE = 316,                 // T_TILDE
    T_TRUE = 317,                  // T_TRUE
    T_OBJECT = 318,                // T_OBJECT
    T_TYPEDEF = 319,               // T_TYPEDEF
    T_UNION = 320,                 // T_UNION
    T_UNSIGNED = 321,              // T_UNSIGNED
    T_VERTICAL_LINE = 322,         // T_VERTICAL_LINE
    T_VOID = 323,                  // T_VOID
    T_WCHAR = 324,                 // T_WCHAR
    T_WSTRING = 325,               // T_WSTRING
    T_UNKNOWN = 326,               // T_UNKNOWN
    T_ABSTRACT = 327,              // T_ABSTRACT
    T_LOCAL = 328,                 // T_LOCAL
    T_VALUETYPE = 329,             // T_VALUETYPE
    T_TRUNCATABLE = 330,           // T_TRUNCATABLE
    T_SUPPORTS = 331,              // T_SUPPORTS
    T_CUSTOM = 332,                // T_CUSTOM
    T_PUBLIC = 333,                // T_PUBLIC
    T_PRIVATE = 334,               // T_PRIVATE
    T_FACTORY = 335,               // T_FACTORY
    T_NATIVE = 336,                // T_NATIVE
    T_VALUEBASE = 337              // T_VALUEBASE
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::yytokentype token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 83, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_T_AMPERSAND = 3,                       // T_AMPERSAND
        S_T_ANY = 4,                             // T_ANY
        S_T_ASTERISK = 5,                        // T_ASTERISK
        S_T_ATTRIBUTE = 6,                       // T_ATTRIBUTE
        S_T_BOOLEAN = 7,                         // T_BOOLEAN
        S_T_CASE = 8,                            // T_CASE
        S_T_CHAR = 9,                            // T_CHAR
        S_T_CHARACTER_LITERAL = 10,              // T_CHARACTER_LITERAL
        S_T_CIRCUMFLEX = 11,                     // T_CIRCUMFLEX
        S_T_COLON = 12,                          // T_COLON
        S_T_COMMA = 13,                          // T_COMMA
        S_T_CONST = 14,                          // T_CONST
        S_T_CONTEXT = 15,                        // T_CONTEXT
        S_T_DEFAULT = 16,                        // T_DEFAULT
        S_T_DOUBLE = 17,                         // T_DOUBLE
        S_T_ENUM = 18,                           // T_ENUM
        S_T_EQUAL = 19,                          // T_EQUAL
        S_T_EXCEPTION = 20,                      // T_EXCEPTION
        S_T_FALSE = 21,                          // T_FALSE
        S_T_FIXED = 22,                          // T_FIXED
        S_T_FIXED_PT_LITERAL = 23,               // T_FIXED_PT_LITERAL
        S_T_FLOAT = 24,                          // T_FLOAT
        S_T_FLOATING_PT_LITERAL = 25,            // T_FLOATING_PT_LITERAL
        S_T_GREATER_THAN_SIGN = 26,              // T_GREATER_THAN_SIGN
        S_T_IDENTIFIER = 27,                     // T_IDENTIFIER
        S_T_IN = 28,                             // T_IN
        S_T_INOUT = 29,                          // T_INOUT
        S_T_INTEGER_LITERAL = 30,                // T_INTEGER_LITERAL
        S_T_INTERFACE = 31,                      // T_INTERFACE
        S_T_LEFT_CURLY_BRACKET = 32,             // T_LEFT_CURLY_BRACKET
        S_T_LEFT_PARANTHESIS = 33,               // T_LEFT_PARANTHESIS
        S_T_LEFT_SQUARE_BRACKET = 34,            // T_LEFT_SQUARE_BRACKET
        S_T_LESS_THAN_SIGN = 35,                 // T_LESS_THAN_SIGN
        S_T_LONG = 36,                           // T_LONG
        S_T_MINUS_SIGN = 37,                     // T_MINUS_SIGN
        S_T_MODULE = 38,                         // T_MODULE
        S_T_OCTET = 39,                          // T_OCTET
        S_T_ONEWAY = 40,                         // T_ONEWAY
        S_T_OUT = 41,                            // T_OUT
        S_T_PERCENT_SIGN = 42,                   // T_PERCENT_SIGN
        S_T_PLUS_SIGN = 43,                      // T_PLUS_SIGN
        S_T_RAISES = 44,                         // T_RAISES
        S_T_READONLY = 45,                       // T_READONLY
        S_T_RIGHT_CURLY_BRACKET = 46,            // T_RIGHT_CURLY_BRACKET
        S_T_RIGHT_PARANTHESIS = 47,              // T_RIGHT_PARANTHESIS
        S_T_RIGHT_SQUARE_BRACKET = 48,           // T_RIGHT_SQUARE_BRACKET
        S_T_SCOPE = 49,                          // T_SCOPE
        S_T_SEMICOLON = 50,                      // T_SEMICOLON
        S_T_SEQUENCE = 51,                       // T_SEQUENCE
        S_T_SHIFTLEFT = 52,                      // T_SHIFTLEFT
        S_T_SHIFTRIGHT = 53,                     // T_SHIFTRIGHT
        S_T_SHORT = 54,                          // T_SHORT
        S_T_SOLIDUS = 55,                        // T_SOLIDUS
        S_T_STRING = 56,                         // T_STRING
        S_T_STRING_LITERAL = 57,                 // T_STRING_LITERAL
        S_T_PRAGMA = 58,                         // T_PRAGMA
        S_T_STRUCT = 59,                         // T_STRUCT
        S_T_SWITCH = 60,                         // T_SWITCH
        S_T_TILDE = 61,                          // T_TILDE
        S_T_TRUE = 62,                           // T_TRUE
        S_T_OBJECT = 63,                         // T_OBJECT
        S_T_TYPEDEF = 64,                        // T_TYPEDEF
        S_T_UNION = 65,                          // T_UNION
        S_T_UNSIGNED = 66,                       // T_UNSIGNED
        S_T_VERTICAL_LINE = 67,                  // T_VERTICAL_LINE
        S_T_VOID = 68,                           // T_VOID
        S_T_WCHAR = 69,                          // T_WCHAR
        S_T_WSTRING = 70,                        // T_WSTRING
        S_T_UNKNOWN = 71,                        // T_UNKNOWN
        S_T_ABSTRACT = 72,                       // T_ABSTRACT
        S_T_LOCAL = 73,                          // T_LOCAL
        S_T_VALUETYPE = 74,                      // T_VALUETYPE
        S_T_TRUNCATABLE = 75,                    // T_TRUNCATABLE
        S_T_SUPPORTS = 76,                       // T_SUPPORTS
        S_T_CUSTOM = 77,                         // T_CUSTOM
        S_T_PUBLIC = 78,                         // T_PUBLIC
        S_T_PRIVATE = 79,                        // T_PRIVATE
        S_T_FACTORY = 80,                        // T_FACTORY
        S_T_NATIVE = 81,                         // T_NATIVE
        S_T_VALUEBASE = 82,                      // T_VALUEBASE
        S_YYACCEPT = 83,                         // $accept
        S_specification = 84,                    // specification
        S_definitions = 85,                      // definitions
        S_definition = 86,                       // definition
        S_module = 87,                           // module
        S_88_1 = 88,                             // $@1
        S_interface = 89,                        // interface
        S_interface_dcl = 90,                    // interface_dcl
        S_forward_dcl = 91,                      // forward_dcl
        S_interface_header = 92,                 // interface_header
        S_93_2 = 93,                             // $@2
        S_94_3 = 94,                             // $@3
        S_95_4 = 95,                             // $@4
        S_interface_body = 96,                   // interface_body
        S_exports = 97,                          // exports
        S_export = 98,                           // export
        S_interface_inheritance_spec = 99,       // interface_inheritance_spec
        S_interface_names = 100,                 // interface_names
        S_scoped_names = 101,                    // scoped_names
        S_interface_name = 102,                  // interface_name
        S_scoped_name = 103,                     // scoped_name
        S_value = 104,                           // value
        S_value_forward_dcl = 105,               // value_forward_dcl
        S_value_box_dcl = 106,                   // value_box_dcl
        S_value_abs_dcl = 107,                   // value_abs_dcl
        S_value_body = 108,                      // value_body
        S_value_dcl = 109,                       // value_dcl
        S_value_elements = 110,                  // value_elements
        S_value_header = 111,                    // value_header
        S_value_inheritance_spec = 112,          // value_inheritance_spec
        S_value_inheritance_bases = 113,         // value_inheritance_bases
        S_value_names = 114,                     // value_names
        S_value_name = 115,                      // value_name
        S_value_element = 116,                   // value_element
        S_state_member = 117,                    // state_member
        S_init_dcl = 118,                        // init_dcl
        S_init_param_decls = 119,                // init_param_decls
        S_init_param_decl = 120,                 // init_param_decl
        S_init_param_attribute = 121,            // init_param_attribute
        S_const_dcl = 122,                       // const_dcl
        S_const_type = 123,                      // const_type
        S_const_exp = 124,                       // const_exp
        S_or_expr = 125,                         // or_expr
        S_xor_expr = 126,                        // xor_expr
        S_and_expr = 127,                        // and_expr
        S_shift_expr = 128,                      // shift_expr
        S_add_expr = 129,                        // add_expr
        S_mult_expr = 130,                       // mult_expr
        S_unary_expr = 131,                      // unary_expr
        S_primary_expr = 132,                    // primary_expr
        S_literal = 133,                         // literal
        S_positive_int_const = 134,              // positive_int_const
        S_type_dcl = 135,                        // type_dcl
        S_type_spec = 136,                       // type_spec
        S_simple_type_spec = 137,                // simple_type_spec
        S_base_type_spec = 138,                  // base_type_spec
        S_declarators = 139,                     // declarators
        S_declarator = 140,                      // declarator
        S_simple_declarator = 141,               // simple_declarator
        S_complex_declarator = 142,              // complex_declarator
        S_floating_pt_type = 143,                // floating_pt_type
        S_integer_type = 144,                    // integer_type
        S_signed_int = 145,                      // signed_int
        S_signed_short_int = 146,                // signed_short_int
        S_signed_long_int = 147,                 // signed_long_int
        S_signed_longlong_int = 148,             // signed_longlong_int
        S_unsigned_int = 149,                    // unsigned_int
        S_unsigned_short_int = 150,              // unsigned_short_int
        S_unsigned_long_int = 151,               // unsigned_long_int
        S_unsigned_longlong_int = 152,           // unsigned_longlong_int
        S_char_type = 153,                       // char_type
        S_wide_char_type = 154,                  // wide_char_type
        S_boolean_type = 155,                    // boolean_type
        S_octet_type = 156,                      // octet_type
        S_any_type = 157,                        // any_type
        S_object_type = 158,                     // object_type
        S_struct_type = 159,                     // struct_type
        S_member_list = 160,                     // member_list
        S_member = 161,                          // member
        S_union_type = 162,                      // union_type
        S_switch_type_spec = 163,                // switch_type_spec
        S_switch_body = 164,                     // switch_body
        S_case = 165,                            // case
        S_case_label = 166,                      // case_label
        S_element_spec = 167,                    // element_spec
        S_enum_type = 168,                       // enum_type
        S_enumerators = 169,                     // enumerators
        S_enumerator = 170,                      // enumerator
        S_string_type = 171,                     // string_type
        S_wide_string_type = 172,                // wide_string_type
        S_array_declarator = 173,                // array_declarator
        S_fixed_array_sizes = 174,               // fixed_array_sizes
        S_fixed_array_size = 175,                // fixed_array_size
        S_attr_dcl = 176,                        // attr_dcl
        S_simple_declarators = 177,              // simple_declarators
        S_except_dcl = 178,                      // except_dcl
        S_members = 179,                         // members
        S_op_dcl = 180,                          // op_dcl
        S_op_attribute = 181,                    // op_attribute
        S_op_type_spec = 182,                    // op_type_spec
        S_parameter_dcls = 183,                  // parameter_dcls
        S_param_dcls = 184,                      // param_dcls
        S_param_dcl = 185,                       // param_dcl
        S_param_attribute = 186,                 // param_attribute
        S_raises_expr = 187,                     // raises_expr
        S_context_expr = 188,                    // context_expr
        S_string_literals = 189,                 // string_literals
        S_T_string_literal = 190,                // T_string_literal
        S_param_type_spec = 191,                 // param_type_spec
        S_fixed_pt_const_type = 192,             // fixed_pt_const_type
        S_value_base_type = 193                  // value_base_type
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.move< AST::BasicType > (std::move (that.value));
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.move< AST::ScopedName > (std::move (that.value));
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.move< AST::Type > (std::move (that.value));
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AST::BasicType&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AST::BasicType& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AST::ScopedName&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AST::ScopedName& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AST::Type&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AST::Type& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.template destroy< AST::BasicType > ();
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.template destroy< AST::ScopedName > ();
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.template destroy< AST::Type > ();
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// Default constructor.
      by_kind ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that);
#endif

      /// Copy constructor.
      by_kind (const by_kind& that);

      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
#endif
      {}
    };

    /// Build a parser object.
    parser (Driver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0


    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_AMPERSAND (location_type l)
      {
        return symbol_type (token::T_AMPERSAND, std::move (l));
      }
#else
      static
      symbol_type
      make_T_AMPERSAND (const location_type& l)
      {
        return symbol_type (token::T_AMPERSAND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ANY (location_type l)
      {
        return symbol_type (token::T_ANY, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ANY (const location_type& l)
      {
        return symbol_type (token::T_ANY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ASTERISK (location_type l)
      {
        return symbol_type (token::T_ASTERISK, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ASTERISK (const location_type& l)
      {
        return symbol_type (token::T_ASTERISK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ATTRIBUTE (location_type l)
      {
        return symbol_type (token::T_ATTRIBUTE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ATTRIBUTE (const location_type& l)
      {
        return symbol_type (token::T_ATTRIBUTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_BOOLEAN (location_type l)
      {
        return symbol_type (token::T_BOOLEAN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_BOOLEAN (const location_type& l)
      {
        return symbol_type (token::T_BOOLEAN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CASE (location_type l)
      {
        return symbol_type (token::T_CASE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CASE (const location_type& l)
      {
        return symbol_type (token::T_CASE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CHAR (location_type l)
      {
        return symbol_type (token::T_CHAR, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CHAR (const location_type& l)
      {
        return symbol_type (token::T_CHAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CHARACTER_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::T_CHARACTER_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_CHARACTER_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_CHARACTER_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CIRCUMFLEX (location_type l)
      {
        return symbol_type (token::T_CIRCUMFLEX, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CIRCUMFLEX (const location_type& l)
      {
        return symbol_type (token::T_CIRCUMFLEX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_COLON (location_type l)
      {
        return symbol_type (token::T_COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_T_COLON (const location_type& l)
      {
        return symbol_type (token::T_COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_COMMA (location_type l)
      {
        return symbol_type (token::T_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_T_COMMA (const location_type& l)
      {
        return symbol_type (token::T_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CONST (location_type l)
      {
        return symbol_type (token::T_CONST, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CONST (const location_type& l)
      {
        return symbol_type (token::T_CONST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CONTEXT (location_type l)
      {
        return symbol_type (token::T_CONTEXT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CONTEXT (const location_type& l)
      {
        return symbol_type (token::T_CONTEXT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_DEFAULT (location_type l)
      {
        return symbol_type (token::T_DEFAULT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_DEFAULT (const location_type& l)
      {
        return symbol_type (token::T_DEFAULT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_DOUBLE (location_type l)
      {
        return symbol_type (token::T_DOUBLE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_DOUBLE (const location_type& l)
      {
        return symbol_type (token::T_DOUBLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ENUM (location_type l)
      {
        return symbol_type (token::T_ENUM, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ENUM (const location_type& l)
      {
        return symbol_type (token::T_ENUM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EQUAL (location_type l)
      {
        return symbol_type (token::T_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EQUAL (const location_type& l)
      {
        return symbol_type (token::T_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EXCEPTION (location_type l)
      {
        return symbol_type (token::T_EXCEPTION, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EXCEPTION (const location_type& l)
      {
        return symbol_type (token::T_EXCEPTION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FALSE (location_type l)
      {
        return symbol_type (token::T_FALSE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_FALSE (const location_type& l)
      {
        return symbol_type (token::T_FALSE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FIXED (location_type l)
      {
        return symbol_type (token::T_FIXED, std::move (l));
      }
#else
      static
      symbol_type
      make_T_FIXED (const location_type& l)
      {
        return symbol_type (token::T_FIXED, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FIXED_PT_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::T_FIXED_PT_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_FIXED_PT_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_FIXED_PT_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FLOAT (location_type l)
      {
        return symbol_type (token::T_FLOAT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_FLOAT (const location_type& l)
      {
        return symbol_type (token::T_FLOAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FLOATING_PT_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::T_FLOATING_PT_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_FLOATING_PT_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_FLOATING_PT_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_GREATER_THAN_SIGN (location_type l)
      {
        return symbol_type (token::T_GREATER_THAN_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_GREATER_THAN_SIGN (const location_type& l)
      {
        return symbol_type (token::T_GREATER_THAN_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::T_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_IN (location_type l)
      {
        return symbol_type (token::T_IN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_IN (const location_type& l)
      {
        return symbol_type (token::T_IN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_INOUT (location_type l)
      {
        return symbol_type (token::T_INOUT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_INOUT (const location_type& l)
      {
        return symbol_type (token::T_INOUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_INTEGER_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::T_INTEGER_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_INTEGER_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_INTEGER_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_INTERFACE (location_type l)
      {
        return symbol_type (token::T_INTERFACE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_INTERFACE (const location_type& l)
      {
        return symbol_type (token::T_INTERFACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LEFT_CURLY_BRACKET (location_type l)
      {
        return symbol_type (token::T_LEFT_CURLY_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LEFT_CURLY_BRACKET (const location_type& l)
      {
        return symbol_type (token::T_LEFT_CURLY_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LEFT_PARANTHESIS (location_type l)
      {
        return symbol_type (token::T_LEFT_PARANTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LEFT_PARANTHESIS (const location_type& l)
      {
        return symbol_type (token::T_LEFT_PARANTHESIS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LEFT_SQUARE_BRACKET (location_type l)
      {
        return symbol_type (token::T_LEFT_SQUARE_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LEFT_SQUARE_BRACKET (const location_type& l)
      {
        return symbol_type (token::T_LEFT_SQUARE_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LESS_THAN_SIGN (location_type l)
      {
        return symbol_type (token::T_LESS_THAN_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LESS_THAN_SIGN (const location_type& l)
      {
        return symbol_type (token::T_LESS_THAN_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LONG (location_type l)
      {
        return symbol_type (token::T_LONG, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LONG (const location_type& l)
      {
        return symbol_type (token::T_LONG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_MINUS_SIGN (location_type l)
      {
        return symbol_type (token::T_MINUS_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_MINUS_SIGN (const location_type& l)
      {
        return symbol_type (token::T_MINUS_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_MODULE (location_type l)
      {
        return symbol_type (token::T_MODULE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_MODULE (const location_type& l)
      {
        return symbol_type (token::T_MODULE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OCTET (location_type l)
      {
        return symbol_type (token::T_OCTET, std::move (l));
      }
#else
      static
      symbol_type
      make_T_OCTET (const location_type& l)
      {
        return symbol_type (token::T_OCTET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ONEWAY (location_type l)
      {
        return symbol_type (token::T_ONEWAY, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ONEWAY (const location_type& l)
      {
        return symbol_type (token::T_ONEWAY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OUT (location_type l)
      {
        return symbol_type (token::T_OUT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_OUT (const location_type& l)
      {
        return symbol_type (token::T_OUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PERCENT_SIGN (location_type l)
      {
        return symbol_type (token::T_PERCENT_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_PERCENT_SIGN (const location_type& l)
      {
        return symbol_type (token::T_PERCENT_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PLUS_SIGN (location_type l)
      {
        return symbol_type (token::T_PLUS_SIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_PLUS_SIGN (const location_type& l)
      {
        return symbol_type (token::T_PLUS_SIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_RAISES (location_type l)
      {
        return symbol_type (token::T_RAISES, std::move (l));
      }
#else
      static
      symbol_type
      make_T_RAISES (const location_type& l)
      {
        return symbol_type (token::T_RAISES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_READONLY (location_type l)
      {
        return symbol_type (token::T_READONLY, std::move (l));
      }
#else
      static
      symbol_type
      make_T_READONLY (const location_type& l)
      {
        return symbol_type (token::T_READONLY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_RIGHT_CURLY_BRACKET (location_type l)
      {
        return symbol_type (token::T_RIGHT_CURLY_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_T_RIGHT_CURLY_BRACKET (const location_type& l)
      {
        return symbol_type (token::T_RIGHT_CURLY_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_RIGHT_PARANTHESIS (location_type l)
      {
        return symbol_type (token::T_RIGHT_PARANTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_T_RIGHT_PARANTHESIS (const location_type& l)
      {
        return symbol_type (token::T_RIGHT_PARANTHESIS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_RIGHT_SQUARE_BRACKET (location_type l)
      {
        return symbol_type (token::T_RIGHT_SQUARE_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_T_RIGHT_SQUARE_BRACKET (const location_type& l)
      {
        return symbol_type (token::T_RIGHT_SQUARE_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SCOPE (location_type l)
      {
        return symbol_type (token::T_SCOPE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SCOPE (const location_type& l)
      {
        return symbol_type (token::T_SCOPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SEMICOLON (location_type l)
      {
        return symbol_type (token::T_SEMICOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SEMICOLON (const location_type& l)
      {
        return symbol_type (token::T_SEMICOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SEQUENCE (location_type l)
      {
        return symbol_type (token::T_SEQUENCE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SEQUENCE (const location_type& l)
      {
        return symbol_type (token::T_SEQUENCE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SHIFTLEFT (location_type l)
      {
        return symbol_type (token::T_SHIFTLEFT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SHIFTLEFT (const location_type& l)
      {
        return symbol_type (token::T_SHIFTLEFT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SHIFTRIGHT (location_type l)
      {
        return symbol_type (token::T_SHIFTRIGHT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SHIFTRIGHT (const location_type& l)
      {
        return symbol_type (token::T_SHIFTRIGHT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SHORT (location_type l)
      {
        return symbol_type (token::T_SHORT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SHORT (const location_type& l)
      {
        return symbol_type (token::T_SHORT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SOLIDUS (location_type l)
      {
        return symbol_type (token::T_SOLIDUS, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SOLIDUS (const location_type& l)
      {
        return symbol_type (token::T_SOLIDUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_STRING (location_type l)
      {
        return symbol_type (token::T_STRING, std::move (l));
      }
#else
      static
      symbol_type
      make_T_STRING (const location_type& l)
      {
        return symbol_type (token::T_STRING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_STRING_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::T_STRING_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_STRING_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_STRING_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PRAGMA (std::string v, location_type l)
      {
        return symbol_type (token::T_PRAGMA, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_PRAGMA (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_PRAGMA, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_STRUCT (location_type l)
      {
        return symbol_type (token::T_STRUCT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_STRUCT (const location_type& l)
      {
        return symbol_type (token::T_STRUCT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SWITCH (location_type l)
      {
        return symbol_type (token::T_SWITCH, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SWITCH (const location_type& l)
      {
        return symbol_type (token::T_SWITCH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_TILDE (location_type l)
      {
        return symbol_type (token::T_TILDE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_TILDE (const location_type& l)
      {
        return symbol_type (token::T_TILDE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_TRUE (location_type l)
      {
        return symbol_type (token::T_TRUE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_TRUE (const location_type& l)
      {
        return symbol_type (token::T_TRUE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OBJECT (location_type l)
      {
        return symbol_type (token::T_OBJECT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_OBJECT (const location_type& l)
      {
        return symbol_type (token::T_OBJECT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_TYPEDEF (location_type l)
      {
        return symbol_type (token::T_TYPEDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_T_TYPEDEF (const location_type& l)
      {
        return symbol_type (token::T_TYPEDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_UNION (location_type l)
      {
        return symbol_type (token::T_UNION, std::move (l));
      }
#else
      static
      symbol_type
      make_T_UNION (const location_type& l)
      {
        return symbol_type (token::T_UNION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_UNSIGNED (location_type l)
      {
        return symbol_type (token::T_UNSIGNED, std::move (l));
      }
#else
      static
      symbol_type
      make_T_UNSIGNED (const location_type& l)
      {
        return symbol_type (token::T_UNSIGNED, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_VERTICAL_LINE (location_type l)
      {
        return symbol_type (token::T_VERTICAL_LINE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_VERTICAL_LINE (const location_type& l)
      {
        return symbol_type (token::T_VERTICAL_LINE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_VOID (location_type l)
      {
        return symbol_type (token::T_VOID, std::move (l));
      }
#else
      static
      symbol_type
      make_T_VOID (const location_type& l)
      {
        return symbol_type (token::T_VOID, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_WCHAR (location_type l)
      {
        return symbol_type (token::T_WCHAR, std::move (l));
      }
#else
      static
      symbol_type
      make_T_WCHAR (const location_type& l)
      {
        return symbol_type (token::T_WCHAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_WSTRING (location_type l)
      {
        return symbol_type (token::T_WSTRING, std::move (l));
      }
#else
      static
      symbol_type
      make_T_WSTRING (const location_type& l)
      {
        return symbol_type (token::T_WSTRING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_UNKNOWN (location_type l)
      {
        return symbol_type (token::T_UNKNOWN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_UNKNOWN (const location_type& l)
      {
        return symbol_type (token::T_UNKNOWN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ABSTRACT (location_type l)
      {
        return symbol_type (token::T_ABSTRACT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_ABSTRACT (const location_type& l)
      {
        return symbol_type (token::T_ABSTRACT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_LOCAL (location_type l)
      {
        return symbol_type (token::T_LOCAL, std::move (l));
      }
#else
      static
      symbol_type
      make_T_LOCAL (const location_type& l)
      {
        return symbol_type (token::T_LOCAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_VALUETYPE (location_type l)
      {
        return symbol_type (token::T_VALUETYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_VALUETYPE (const location_type& l)
      {
        return symbol_type (token::T_VALUETYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_TRUNCATABLE (location_type l)
      {
        return symbol_type (token::T_TRUNCATABLE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_TRUNCATABLE (const location_type& l)
      {
        return symbol_type (token::T_TRUNCATABLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SUPPORTS (location_type l)
      {
        return symbol_type (token::T_SUPPORTS, std::move (l));
      }
#else
      static
      symbol_type
      make_T_SUPPORTS (const location_type& l)
      {
        return symbol_type (token::T_SUPPORTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CUSTOM (location_type l)
      {
        return symbol_type (token::T_CUSTOM, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CUSTOM (const location_type& l)
      {
        return symbol_type (token::T_CUSTOM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PUBLIC (location_type l)
      {
        return symbol_type (token::T_PUBLIC, std::move (l));
      }
#else
      static
      symbol_type
      make_T_PUBLIC (const location_type& l)
      {
        return symbol_type (token::T_PUBLIC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PRIVATE (location_type l)
      {
        return symbol_type (token::T_PRIVATE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_PRIVATE (const location_type& l)
      {
        return symbol_type (token::T_PRIVATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_FACTORY (location_type l)
      {
        return symbol_type (token::T_FACTORY, std::move (l));
      }
#else
      static
      symbol_type
      make_T_FACTORY (const location_type& l)
      {
        return symbol_type (token::T_FACTORY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_NATIVE (location_type l)
      {
        return symbol_type (token::T_NATIVE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_NATIVE (const location_type& l)
      {
        return symbol_type (token::T_NATIVE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_VALUEBASE (location_type l)
      {
        return symbol_type (token::T_VALUEBASE, std::move (l));
      }
#else
      static
      symbol_type
      make_T_VALUEBASE (const location_type& l)
      {
        return symbol_type (token::T_VALUEBASE, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static symbol_kind_type yytranslate_ (int t);

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const unsigned char yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yylast_ = 577,     ///< Last index in yytable_.
      yynnts_ = 111,  ///< Number of nonterminal symbols.
      yyfinal_ = 95 ///< Termination state number.
    };


    // User arguments.
    Driver& drv;

  };

  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82
    };
    // Last valid token kind.
    const int code_max = 337;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.copy< AST::BasicType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.copy< AST::ScopedName > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.copy< AST::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.move< AST::BasicType > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.move< AST::ScopedName > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.move< AST::Type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
  parser::by_kind::clear ()
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

} // yy
#line 3101 "yacc.tab.cc"








#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 3199 "yacc.tab.cc"

  /// Build a parser object.
  parser::parser (Driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.YY_MOVE_OR_COPY< AST::BasicType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.YY_MOVE_OR_COPY< AST::ScopedName > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.YY_MOVE_OR_COPY< AST::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.move< AST::BasicType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.move< AST::ScopedName > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.move< AST::Type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.copy< AST::BasicType > (that.value);
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.copy< AST::ScopedName > (that.value);
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.copy< AST::Type > (that.value);
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        value.move< AST::BasicType > (that.value);
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        value.move< AST::ScopedName > (that.value);
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        value.move< AST::Type > (that.value);
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YYUSE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_base_type_spec: // base_type_spec
      case symbol_kind::S_floating_pt_type: // floating_pt_type
      case symbol_kind::S_integer_type: // integer_type
      case symbol_kind::S_signed_int: // signed_int
      case symbol_kind::S_unsigned_int: // unsigned_int
        yylhs.value.emplace< AST::BasicType > ();
        break;

      case symbol_kind::S_scoped_name: // scoped_name
        yylhs.value.emplace< AST::ScopedName > ();
        break;

      case symbol_kind::S_simple_type_spec: // simple_type_spec
      case symbol_kind::S_string_type: // string_type
      case symbol_kind::S_wide_string_type: // wide_string_type
      case symbol_kind::S_op_type_spec: // op_type_spec
      case symbol_kind::S_param_type_spec: // param_type_spec
        yylhs.value.emplace< AST::Type > ();
        break;

      case symbol_kind::S_T_CHARACTER_LITERAL: // T_CHARACTER_LITERAL
      case symbol_kind::S_T_FIXED_PT_LITERAL: // T_FIXED_PT_LITERAL
      case symbol_kind::S_T_FLOATING_PT_LITERAL: // T_FLOATING_PT_LITERAL
      case symbol_kind::S_T_IDENTIFIER: // T_IDENTIFIER
      case symbol_kind::S_T_INTEGER_LITERAL: // T_INTEGER_LITERAL
      case symbol_kind::S_T_STRING_LITERAL: // T_STRING_LITERAL
      case symbol_kind::S_T_PRAGMA: // T_PRAGMA
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 12: // $@1: %empty
#line 159 "yacc.yy"
                                                     { drv.module_begin (yystack_[1].value.as < std::string > (), yystack_[2].location); }
#line 3743 "yacc.tab.cc"
    break;

  case 13: // module: T_MODULE T_IDENTIFIER T_LEFT_CURLY_BRACKET $@1 definitions T_RIGHT_CURLY_BRACKET
#line 160 "yacc.yy"
                                  { drv.module_end (); }
#line 3749 "yacc.tab.cc"
    break;

  case 16: // interface_dcl: interface_header T_LEFT_CURLY_BRACKET interface_body T_RIGHT_CURLY_BRACKET
#line 172 "yacc.yy"
                              { drv.interface_end (); }
#line 3755 "yacc.tab.cc"
    break;

  case 17: // forward_dcl: T_INTERFACE T_IDENTIFIER
#line 177 "yacc.yy"
                                   { drv.interface_decl (yystack_[0].value.as < std::string > (), yystack_[1].location); }
#line 3761 "yacc.tab.cc"
    break;

  case 18: // forward_dcl: T_ABSTRACT T_INTERFACE T_IDENTIFIER
#line 178 "yacc.yy"
                                              { drv.interface_decl (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::ABSTRACT); }
#line 3767 "yacc.tab.cc"
    break;

  case 19: // forward_dcl: T_LOCAL T_INTERFACE T_IDENTIFIER
#line 179 "yacc.yy"
                                           { drv.interface_decl (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::LOCAL); }
#line 3773 "yacc.tab.cc"
    break;

  case 20: // interface_header: T_INTERFACE T_IDENTIFIER
#line 184 "yacc.yy"
                                   { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[1].location); }
#line 3779 "yacc.tab.cc"
    break;

  case 21: // $@2: %empty
#line 185 "yacc.yy"
                                   { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[1].location); }
#line 3785 "yacc.tab.cc"
    break;

  case 23: // interface_header: T_ABSTRACT T_INTERFACE T_IDENTIFIER
#line 186 "yacc.yy"
                                              { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::ABSTRACT); }
#line 3791 "yacc.tab.cc"
    break;

  case 24: // $@3: %empty
#line 187 "yacc.yy"
                                              { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::ABSTRACT); }
#line 3797 "yacc.tab.cc"
    break;

  case 26: // interface_header: T_LOCAL T_INTERFACE T_IDENTIFIER
#line 188 "yacc.yy"
                                           { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::LOCAL); }
#line 3803 "yacc.tab.cc"
    break;

  case 27: // $@4: %empty
#line 189 "yacc.yy"
                                           { drv.interface_begin (yystack_[0].value.as < std::string > (), yystack_[2].location, AST::InterfaceKind::LOCAL); }
#line 3809 "yacc.tab.cc"
    break;

  case 43: // interface_name: scoped_name
#line 229 "yacc.yy"
                      { drv.interface_base (yystack_[0].value.as < AST::ScopedName > (), yystack_[0].location); }
#line 3815 "yacc.tab.cc"
    break;

  case 44: // scoped_name: T_IDENTIFIER
#line 234 "yacc.yy"
                       { yylhs.value.as < AST::ScopedName > () = AST::ScopedName (false, yystack_[0].value.as < std::string > ()); }
#line 3821 "yacc.tab.cc"
    break;

  case 45: // scoped_name: T_SCOPE T_IDENTIFIER
#line 235 "yacc.yy"
                               { yylhs.value.as < AST::ScopedName > () = AST::ScopedName (true, yystack_[0].value.as < std::string > ()); }
#line 3827 "yacc.tab.cc"
    break;

  case 46: // scoped_name: scoped_name T_SCOPE T_IDENTIFIER
#line 236 "yacc.yy"
                                           { yylhs.value.as < AST::ScopedName > () = yystack_[2].value.as < AST::ScopedName > (); yylhs.value.as < AST::ScopedName > ().push_back (yystack_[0].value.as < std::string > ()); }
#line 3833 "yacc.tab.cc"
    break;

  case 132: // type_dcl: T_NATIVE T_IDENTIFIER
#line 454 "yacc.yy"
                                { drv.native (yystack_[0].value.as < std::string > (), yystack_[1].location); }
#line 3839 "yacc.tab.cc"
    break;

  case 134: // simple_type_spec: base_type_spec
#line 465 "yacc.yy"
                         { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::BasicType > (); }
#line 3845 "yacc.tab.cc"
    break;

  case 135: // simple_type_spec: scoped_name
#line 467 "yacc.yy"
                      { yylhs.value.as < AST::Type > () = drv.lookup (yystack_[0].value.as < AST::ScopedName > (), yystack_[0].location); }
#line 3851 "yacc.tab.cc"
    break;

  case 136: // base_type_spec: floating_pt_type
#line 472 "yacc.yy"
                           { yylhs.value.as < AST::BasicType > () = yystack_[0].value.as < AST::BasicType > (); }
#line 3857 "yacc.tab.cc"
    break;

  case 137: // base_type_spec: integer_type
#line 473 "yacc.yy"
                       { yylhs.value.as < AST::BasicType > () = yystack_[0].value.as < AST::BasicType > (); }
#line 3863 "yacc.tab.cc"
    break;

  case 138: // base_type_spec: char_type
#line 474 "yacc.yy"
                    { yylhs.value.as < AST::BasicType > () = AST::BasicType::CHAR; }
#line 3869 "yacc.tab.cc"
    break;

  case 139: // base_type_spec: wide_char_type
#line 475 "yacc.yy"
                         { yylhs.value.as < AST::BasicType > () = AST::BasicType::WCHAR; }
#line 3875 "yacc.tab.cc"
    break;

  case 140: // base_type_spec: boolean_type
#line 476 "yacc.yy"
                       { yylhs.value.as < AST::BasicType > () = AST::BasicType::BOOLEAN; }
#line 3881 "yacc.tab.cc"
    break;

  case 141: // base_type_spec: octet_type
#line 477 "yacc.yy"
                     { yylhs.value.as < AST::BasicType > () = AST::BasicType::OCTET; }
#line 3887 "yacc.tab.cc"
    break;

  case 142: // base_type_spec: any_type
#line 478 "yacc.yy"
                   { yylhs.value.as < AST::BasicType > () = AST::BasicType::ANY; }
#line 3893 "yacc.tab.cc"
    break;

  case 143: // base_type_spec: object_type
#line 479 "yacc.yy"
                      { yylhs.value.as < AST::BasicType > () = AST::BasicType::OBJECT; }
#line 3899 "yacc.tab.cc"
    break;

  case 144: // base_type_spec: value_base_type
#line 480 "yacc.yy"
                          { yylhs.value.as < AST::BasicType > () = AST::BasicType::VALUE_BASE; }
#line 3905 "yacc.tab.cc"
    break;

  case 151: // floating_pt_type: T_FLOAT
#line 522 "yacc.yy"
                  { yylhs.value.as < AST::BasicType > () = AST::BasicType::FLOAT; }
#line 3911 "yacc.tab.cc"
    break;

  case 152: // floating_pt_type: T_DOUBLE
#line 523 "yacc.yy"
                   { yylhs.value.as < AST::BasicType > () = AST::BasicType::DOUBLE; }
#line 3917 "yacc.tab.cc"
    break;

  case 153: // floating_pt_type: T_LONG T_DOUBLE
#line 524 "yacc.yy"
                          { yylhs.value.as < AST::BasicType > () = AST::BasicType::LONGDOUBLE; }
#line 3923 "yacc.tab.cc"
    break;

  case 154: // integer_type: signed_int
#line 529 "yacc.yy"
                     { yylhs.value.as < AST::BasicType > () = yystack_[0].value.as < AST::BasicType > (); }
#line 3929 "yacc.tab.cc"
    break;

  case 155: // integer_type: unsigned_int
#line 530 "yacc.yy"
                       { yylhs.value.as < AST::BasicType > () = yystack_[0].value.as < AST::BasicType > (); }
#line 3935 "yacc.tab.cc"
    break;

  case 156: // signed_int: signed_long_int
#line 535 "yacc.yy"
                          { yylhs.value.as < AST::BasicType > () = AST::BasicType::LONG; }
#line 3941 "yacc.tab.cc"
    break;

  case 157: // signed_int: signed_short_int
#line 536 "yacc.yy"
                           { yylhs.value.as < AST::BasicType > () = AST::BasicType::SHORT; }
#line 3947 "yacc.tab.cc"
    break;

  case 158: // signed_int: signed_longlong_int
#line 537 "yacc.yy"
                              { yylhs.value.as < AST::BasicType > () = AST::BasicType::LONGLONG; }
#line 3953 "yacc.tab.cc"
    break;

  case 162: // unsigned_int: unsigned_long_int
#line 557 "yacc.yy"
                            { yylhs.value.as < AST::BasicType > () = AST::BasicType::ULONG; }
#line 3959 "yacc.tab.cc"
    break;

  case 163: // unsigned_int: unsigned_short_int
#line 558 "yacc.yy"
                             { yylhs.value.as < AST::BasicType > () = AST::BasicType::USHORT; }
#line 3965 "yacc.tab.cc"
    break;

  case 164: // unsigned_int: unsigned_longlong_int
#line 559 "yacc.yy"
                                { yylhs.value.as < AST::BasicType > () = AST::BasicType::ULONGLONG; }
#line 3971 "yacc.tab.cc"
    break;

  case 195: // string_type: T_STRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN
#line 688 "yacc.yy"
                                                                           { yylhs.value.as < AST::Type > () = AST::Type::make_string (); }
#line 3977 "yacc.tab.cc"
    break;

  case 196: // string_type: T_STRING
#line 689 "yacc.yy"
                   { yylhs.value.as < AST::Type > () = AST::Type::make_string (); }
#line 3983 "yacc.tab.cc"
    break;

  case 197: // wide_string_type: T_WSTRING T_LESS_THAN_SIGN positive_int_const T_GREATER_THAN_SIGN
#line 694 "yacc.yy"
                                                                            { yylhs.value.as < AST::Type > () = AST::Type::make_wstring (); }
#line 3989 "yacc.tab.cc"
    break;

  case 198: // wide_string_type: T_WSTRING
#line 695 "yacc.yy"
                    { yylhs.value.as < AST::Type > () = AST::Type::make_wstring (); }
#line 3995 "yacc.tab.cc"
    break;

  case 213: // op_type_spec: param_type_spec
#line 749 "yacc.yy"
                          { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::Type > (); }
#line 4001 "yacc.tab.cc"
    break;

  case 214: // op_type_spec: T_VOID
#line 750 "yacc.yy"
                 { yylhs.value.as < AST::Type > () = AST::BasicType::VOID; }
#line 4007 "yacc.tab.cc"
    break;

  case 231: // param_type_spec: base_type_spec
#line 800 "yacc.yy"
                         { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::BasicType > (); }
#line 4013 "yacc.tab.cc"
    break;

  case 232: // param_type_spec: string_type
#line 801 "yacc.yy"
                      { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::Type > (); }
#line 4019 "yacc.tab.cc"
    break;

  case 233: // param_type_spec: wide_string_type
#line 802 "yacc.yy"
                           { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::Type > (); }
#line 4025 "yacc.tab.cc"
    break;

  case 234: // param_type_spec: scoped_name
#line 803 "yacc.yy"
                      { yylhs.value.as < AST::Type > () = yystack_[0].value.as < AST::ScopedName > (); }
#line 4031 "yacc.tab.cc"
    break;


#line 4035 "yacc.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0





  const short parser::yypact_ninf_ = -290;

  const signed char parser::yytable_ninf_ = -65;

  const short
  parser::yypact_[] =
  {
      89,   481,    13,    39,    52,    84,   105,   310,   111,   -14,
     112,   118,   -45,   133,   168,  -290,    89,   121,   122,  -290,
    -290,    24,   123,  -290,  -290,  -290,  -290,   142,   125,   126,
    -290,  -290,  -290,   127,  -290,  -290,  -290,  -290,  -290,  -290,
      70,  -290,   151,  -290,   144,    82,  -290,   145,   132,   155,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,   152,   153,    29,
     157,   158,  -290,  -290,  -290,   132,   156,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,   134,   181,
     184,   186,    26,   187,  -290,  -290,  -290,  -290,  -290,   389,
    -290,   337,  -290,  -290,  -290,  -290,  -290,  -290,   515,   180,
    -290,   515,   190,   200,   195,   310,   212,  -290,   310,   191,
    -290,   213,  -290,  -290,  -290,   196,    56,    20,    92,    -2,
      22,  -290,  -290,    25,   214,  -290,   222,   188,  -290,   418,
     182,   183,   185,   193,   194,   203,  -290,   310,   310,   209,
    -290,   199,   366,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
     515,    21,    21,   189,    21,  -290,   132,  -290,   170,   229,
     244,    73,    76,    17,  -290,  -290,  -290,   223,  -290,  -290,
     225,  -290,   515,  -290,   202,   241,   156,   310,   210,    22,
    -290,    89,   215,   310,   515,  -290,   191,   156,   103,   212,
     451,   226,   212,    22,   132,   179,   247,  -290,   249,   132,
    -290,   132,  -290,  -290,  -290,   237,   214,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,   238,  -290,   156,   156,   234,
    -290,  -290,   231,  -290,  -290,  -290,  -290,   515,   515,   515,
     515,   515,   515,   515,   515,   515,   515,  -290,  -290,  -290,
    -290,   195,   232,  -290,  -290,  -290,   233,  -290,  -290,   239,
    -290,  -290,   245,   132,  -290,  -290,  -290,   242,  -290,  -290,
    -290,   240,   451,  -290,   277,    22,    22,    22,  -290,   281,
    -290,   237,   262,   248,   252,   269,  -290,   229,   244,    73,
      76,    76,    17,    17,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,   267,  -290,   258,    22,  -290,  -290,    14,  -290,  -290,
     237,  -290,    53,   261,  -290,  -290,  -290,   259,   294,   214,
      61,  -290,  -290,    22,  -290,  -290,  -290,  -290,  -290,   263,
     296,   214,   279,   298,   265,   269,   237,   515,   304,   275,
      61,   284,  -290,  -290,    94,   237,    22,   291,  -290,  -290,
    -290,  -290,   313,  -290,  -290,  -290,   156,  -290,   276,  -290,
    -290,   282,   189,  -290,  -290,  -290,  -290,   285,   315,  -290,
     189,  -290
  };

  const unsigned char
  parser::yydefact_[] =
  {
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     0,     0,    14,
      15,     0,     0,    50,    49,    48,    47,     0,     0,     0,
     129,   130,   131,     0,   170,   168,   152,   235,   151,    44,
     160,   171,     0,   159,   196,     0,   169,   198,    94,     0,
      90,    86,   154,   157,   156,   158,   155,   163,   162,   164,
      87,    88,    89,    95,    91,    92,    93,     0,     0,    17,
       0,     0,   172,   173,   236,   135,     0,   133,   134,   136,
     137,   138,   139,   140,   141,   142,   143,   144,     0,     0,
       0,     0,    51,     0,   132,     1,     5,    10,     9,   211,
      11,   211,     7,     6,     8,   153,   161,    45,     0,   166,
     165,     0,     0,     0,     0,   208,     0,    12,     0,   149,
     128,   145,   147,   148,   150,     0,    18,    52,    19,     0,
       0,    62,    53,    65,     0,   212,     0,     0,    30,   211,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
      75,     0,   211,    76,    77,   122,   126,   123,   124,   120,
       0,     0,     0,   229,     0,   125,   117,   127,    96,    97,
      99,   101,   103,   106,   109,   116,   118,     0,   121,   167,
       0,    46,     0,   194,     0,   192,     0,   208,     0,     0,
      22,     0,     0,   175,     0,   199,   200,     0,     0,     0,
     211,     0,     0,     0,    74,    66,    69,    68,    39,    43,
      63,   234,   231,   232,   233,     0,     0,    16,    32,    34,
      33,    36,    35,    37,   214,     0,   213,     0,     0,     0,
      58,    61,     0,   113,   114,   230,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   195,   197,    85,
     191,     0,     0,   209,   207,    38,     0,   174,   176,     0,
     201,   146,   160,   183,   179,   180,   181,     0,   182,    25,
      57,     0,   211,    28,    71,     0,     0,     0,   149,   205,
     203,     0,     0,     0,     0,     0,   119,    98,   100,   102,
     105,   104,   108,   107,   110,   112,   111,   193,   177,    13,
     202,     0,    54,     0,     0,    67,    73,    41,    70,    40,
       0,   204,     0,   223,    78,    79,    84,     0,    81,     0,
       0,    55,    72,     0,   206,   220,   222,   221,   216,     0,
     217,     0,     0,   225,     0,     0,     0,     0,     0,     0,
     184,     0,    42,   215,     0,     0,     0,     0,   210,    80,
      82,    83,     0,   189,   178,   185,     0,   186,     0,   218,
     219,     0,     0,   188,   190,   187,   224,     0,   227,   226,
       0,   228
  };

  const short
  parser::yypgoto_[] =
  {
    -290,  -290,     7,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,   -75,   -85,   -65,  -178,  -289,  -290,
      -1,  -290,  -290,  -290,  -290,    59,  -290,   192,  -290,   -88,
    -290,    32,   136,  -290,  -290,  -290,    19,  -290,  -290,     4,
    -290,  -146,  -290,   104,   110,   113,   -82,   -87,   -95,     3,
    -290,   -96,     5,    -6,  -290,  -126,  -112,     0,  -205,  -290,
     341,     2,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
       6,   357,     8,   359,  -290,  -290,  -290,   169,  -100,  -290,
    -290,    23,    27,  -290,  -290,   163,   114,  -290,   368,   369,
    -290,   171,  -290,  -290,  -255,    12,   198,  -290,  -290,  -290,
    -290,    30,  -290,  -290,  -290,  -290,     1,  -161,  -132,  -290,
    -290
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    17,   191,    18,    19,    20,    21,
     116,   199,   202,   137,   270,   139,   190,   207,   306,   208,
     166,    22,    23,    24,    25,   271,    26,   151,    27,   131,
     205,   308,   206,   152,   153,   154,   317,   318,   319,   140,
      49,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   141,   186,    77,    78,   120,   121,   122,   123,
      79,    80,    52,    53,    54,    55,    56,    57,    58,    59,
      81,    82,    83,    84,    85,    86,    30,   192,   187,    31,
     267,   339,   340,   341,   358,    32,   184,   185,   213,   214,
     124,   195,   196,   142,   280,   143,   188,   144,   145,   225,
     313,   329,   330,   331,   333,   348,   367,   178,   215,    66,
      87
  };

  const short
  parser::yytable_[] =
  {
      48,    76,   235,    51,    28,    29,    75,    60,   212,    62,
     279,   255,    33,   226,   232,   180,   150,    89,   193,   212,
      28,    29,   244,    96,   138,    39,   311,   323,    33,    93,
      72,   155,   129,    34,   342,    35,   249,   129,   129,   201,
      67,   -21,   156,    36,   157,   210,   158,    42,    39,    39,
      38,   159,   200,    39,   160,   324,    99,   361,   -64,   245,
      90,   -20,    40,   112,   218,    41,    68,   150,   -24,   337,
      42,    42,   246,   203,   252,    42,   279,   338,   163,    69,
      43,   325,   326,   165,   281,   261,   132,   105,   -23,    73,
     212,    75,    45,   193,   327,    46,   130,   305,   259,   309,
     328,   130,   130,     1,   -27,   279,   106,     2,    74,     3,
      34,    70,    35,   242,    75,   283,   284,    75,   109,   243,
       4,     2,   325,   326,   -26,   240,   241,     5,   204,   209,
      39,   351,    71,   211,   269,   327,   110,   273,    88,   262,
     360,   227,   228,    91,   211,    92,    75,    75,     6,   294,
     295,   296,    42,     7,     8,   292,   293,    43,   290,   291,
      94,     9,    10,    11,   233,   234,    12,   236,    95,    45,
      13,    97,    98,   100,   101,   102,   103,   104,   107,   108,
     111,   112,   113,   119,   114,   115,    75,   336,   209,   117,
     118,   352,    75,   212,   125,    28,    29,   263,   256,   345,
     264,   368,   204,    33,   265,   212,   266,    72,   126,   368,
      34,   127,    35,   128,   133,   211,   179,   181,    72,   182,
      36,    34,   183,    35,   189,   194,   197,    38,   216,   198,
      39,    36,   219,   220,   217,   221,   229,   237,    38,    40,
     238,    39,    41,   222,   223,   230,   163,   239,   250,   247,
      40,   248,    42,    41,   251,   275,   254,    43,   272,    44,
     276,   257,   277,    42,   278,   282,    73,   285,    43,    45,
      44,   224,    46,    47,   209,   307,   209,    73,   286,   299,
      45,   106,   298,    46,    47,    74,   302,   300,    72,   301,
     304,    34,   337,    35,   310,   312,    74,   316,   314,   320,
     338,    36,   315,   307,   321,   332,   334,   335,    38,   344,
     343,    39,   346,   347,    72,   349,   353,    34,   211,    35,
      40,   354,   307,    41,   362,   363,   365,    36,   370,   366,
     211,   303,   369,    42,    38,   356,   322,    39,    43,   274,
      75,   287,    50,   134,   231,   307,    40,    73,   288,    41,
      45,     1,   289,    46,   350,     2,   364,     3,    61,    42,
      63,   268,   258,   355,    43,   297,    74,   260,   357,    64,
      65,   371,   134,    73,   359,     0,    45,   135,     0,    46,
       1,     0,   136,   146,     2,   253,     3,     0,     0,     0,
       0,     0,    74,     0,     0,   134,     6,     0,     0,     0,
       0,     7,     8,     1,     0,     0,   135,     2,     0,     3,
       0,   136,   -60,     0,     0,   147,   148,   149,    13,     0,
       0,     0,     0,     0,   134,     6,     0,     0,     0,   135,
       7,     8,     1,     0,   136,   -29,     2,     0,     3,     0,
       0,     0,     0,     0,   147,   148,   149,    13,     6,     0,
       0,     0,     0,     7,     8,     0,     0,   134,   135,     0,
       0,     0,     0,   136,   -31,     1,     0,     0,     0,     2,
      13,     3,     0,     0,     0,     0,     0,     6,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,    34,     0,
      35,   135,     0,     0,     0,     0,   136,   -56,    36,    13,
       0,     0,     0,    37,     0,    38,     0,     0,    39,     0,
       6,     0,     0,     0,     0,     7,     8,    40,     0,     0,
      41,     0,     0,     0,     0,   155,     0,     0,     0,     0,
      42,     0,    13,     0,     0,    43,   156,    44,   157,     0,
     158,     0,    39,     0,     0,   159,     0,    45,   160,     0,
      46,    47,   161,     0,     0,     0,     0,     0,   162,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,   163,     0,     0,     0,   164,   165
  };

  const short
  parser::yycheck_[] =
  {
       1,     7,   163,     1,     0,     0,     7,     1,   134,     1,
     215,   189,     0,   145,   160,   111,   101,    31,   118,   145,
      16,    16,     5,    16,    99,    27,   281,    13,    16,    74,
       4,    10,    12,     7,   323,     9,   182,    12,    12,   127,
      27,    12,    21,    17,    23,   133,    25,    49,    27,    27,
      24,    30,    32,    27,    33,   310,    32,   346,    32,    42,
      74,    32,    36,    49,   139,    39,    27,   152,    12,     8,
      49,    49,    55,    75,   186,    49,   281,    16,    57,    27,
      54,    28,    29,    62,   216,   197,    92,    17,    32,    63,
     216,    92,    66,   193,    41,    69,    76,   275,   194,   277,
      47,    76,    76,    14,    12,   310,    36,    18,    82,    20,
       7,    27,     9,    37,   115,   227,   228,   118,    36,    43,
      31,    18,    28,    29,    32,    52,    53,    38,   129,   130,
      27,   336,    27,   134,   199,    41,    54,   202,    27,    36,
     345,   147,   148,    31,   145,    27,   147,   148,    59,   244,
     245,   246,    49,    64,    65,   242,   243,    54,   240,   241,
      27,    72,    73,    74,   161,   162,    77,   164,     0,    66,
      81,    50,    50,    50,    32,    50,    50,    50,    27,    35,
      35,    49,    27,    27,    32,    32,   187,   319,   189,    32,
      32,   337,   193,   319,    60,   191,   191,   198,   191,   331,
     198,   362,   203,   191,   198,   331,   198,     4,    27,   370,
       7,    27,     9,    27,    27,   216,    36,    27,     4,    19,
      17,     7,    27,     9,    12,    34,    13,    24,     6,    33,
      27,    17,    50,    50,    46,    50,    27,    67,    24,    36,
      11,    27,    39,    50,    50,    46,    57,     3,    46,    26,
      36,    26,    49,    39,    13,    76,    46,    54,    32,    56,
      13,    46,    13,    49,    27,    27,    63,    33,    54,    66,
      56,    68,    69,    70,   275,   276,   277,    63,    47,    46,
      66,    36,    50,    69,    70,    82,    46,    48,     4,    47,
      13,     7,     8,     9,    13,    33,    82,    28,    50,    32,
      16,    17,    50,   304,    46,    44,    47,    13,    24,    13,
      47,    27,    33,    15,     4,    50,    12,     7,   319,     9,
      36,    46,   323,    39,    33,    12,    50,    17,    13,    47,
     331,   272,    47,    49,    24,   341,   304,    27,    54,   203,
     341,   237,     1,     6,   152,   346,    36,    63,   238,    39,
      66,    14,   239,    69,   335,    18,   356,    20,     1,    49,
       1,   198,   193,   340,    54,   251,    82,   196,   341,     1,
       1,   370,     6,    63,   344,    -1,    66,    40,    -1,    69,
      14,    -1,    45,    46,    18,   187,    20,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,     6,    59,    -1,    -1,    -1,
      -1,    64,    65,    14,    -1,    -1,    40,    18,    -1,    20,
      -1,    45,    46,    -1,    -1,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,     6,    59,    -1,    -1,    -1,    40,
      64,    65,    14,    -1,    45,    46,    18,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    59,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,     6,    40,    -1,
      -1,    -1,    -1,    45,    46,    14,    -1,    -1,    -1,    18,
      81,    20,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,     7,    -1,
       9,    40,    -1,    -1,    -1,    -1,    45,    46,    17,    81,
      -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    27,    -1,
      59,    -1,    -1,    -1,    -1,    64,    65,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      49,    -1,    81,    -1,    -1,    54,    21,    56,    23,    -1,
      25,    -1,    27,    -1,    -1,    30,    -1,    66,    33,    -1,
      69,    70,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    61,    62
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    14,    18,    20,    31,    38,    59,    64,    65,    72,
      73,    74,    77,    81,    84,    85,    86,    87,    89,    90,
      91,    92,   104,   105,   106,   107,   109,   111,   122,   135,
     159,   162,   168,   178,     7,     9,    17,    22,    24,    27,
      36,    39,    49,    54,    56,    66,    69,    70,   103,   123,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   171,   172,   192,    27,    27,    27,
      27,    27,     4,    63,    82,   103,   136,   137,   138,   143,
     144,   153,   154,   155,   156,   157,   158,   193,    27,    31,
      74,    31,    27,    74,    27,     0,    85,    50,    50,    32,
      50,    32,    50,    50,    50,    17,    36,    27,    35,    36,
      54,    35,    49,    27,    32,    32,    93,    32,    32,    27,
     139,   140,   141,   142,   173,    60,    27,    27,    27,    12,
      76,   112,   136,    27,     6,    40,    45,    96,    97,    98,
     122,   135,   176,   178,   180,   181,    46,    78,    79,    80,
      98,   110,   116,   117,   118,    10,    21,    23,    25,    30,
      33,    37,    43,    57,    61,    62,   103,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   190,    36,
     134,    27,    19,    27,   169,   170,   136,   161,   179,    12,
      99,    88,   160,   161,    34,   174,   175,    13,    33,    94,
      32,   112,    95,    75,   103,   113,   115,   100,   102,   103,
     112,   103,   138,   171,   172,   191,     6,    46,    97,    50,
      50,    50,    50,    50,    68,   182,   191,   136,   136,    27,
      46,   110,   124,   132,   132,   190,   132,    67,    11,     3,
      52,    53,    37,    43,     5,    42,    55,    26,    26,   124,
      46,    13,   139,   179,    46,   100,    85,    46,   160,   134,
     174,   139,    36,   103,   144,   153,   155,   163,   168,    99,
      97,   108,    32,    99,   115,    76,    13,    13,    27,   141,
     177,   191,    27,   139,   139,    33,    47,   126,   127,   128,
     129,   129,   130,   130,   131,   131,   131,   169,    50,    46,
      48,    47,    46,   108,    13,   100,   101,   103,   114,   100,
      13,   177,    33,   183,    50,    50,    28,   119,   120,   121,
      32,    46,   114,    13,   177,    28,    29,    41,    47,   184,
     185,   186,    44,   187,    47,    13,   191,     8,    16,   164,
     165,   166,   101,    47,    13,   191,    33,    15,   188,    50,
     119,   141,   124,    12,    46,   164,   136,   165,   167,   184,
     141,   101,    33,    12,   140,    50,    47,   189,   190,    47,
      13,   189
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    83,    84,    84,    85,    85,    86,    86,    86,    86,
      86,    86,    88,    87,    89,    89,    90,    91,    91,    91,
      92,    93,    92,    92,    94,    92,    92,    95,    92,    96,
      96,    97,    97,    98,    98,    98,    98,    98,    99,   100,
     100,   101,   101,   102,   103,   103,   103,   104,   104,   104,
     104,   105,   105,   106,   107,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   111,   111,   112,   112,   112,   113,
     113,   113,   113,   114,   115,   116,   116,   116,   117,   117,
     118,   119,   119,   120,   121,   122,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   124,   125,   125,   126,
     126,   127,   127,   128,   128,   128,   129,   129,   129,   130,
     130,   130,   130,   131,   131,   131,   131,   132,   132,   132,
     133,   133,   133,   133,   133,   133,   133,   134,   135,   135,
     135,   135,   135,   136,   137,   137,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   139,   139,   140,   140,   141,
     142,   143,   143,   143,   144,   144,   145,   145,   145,   146,
     147,   148,   149,   149,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   160,   161,   162,   163,
     163,   163,   163,   163,   164,   164,   165,   165,   166,   166,
     167,   168,   169,   169,   170,   171,   171,   172,   172,   173,
     174,   174,   175,   176,   176,   177,   177,   178,   179,   179,
     180,   181,   181,   182,   182,   183,   183,   184,   184,   185,
     186,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   191,   191,   191,   191,   192,   193
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     0,     6,     1,     1,     4,     2,     3,     3,
       2,     0,     4,     3,     0,     5,     3,     0,     5,     0,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     3,     1,     1,     2,     3,     1,     1,     1,
       1,     2,     3,     3,     6,     7,     0,     1,     4,     3,
       1,     2,     3,     4,     2,     3,     2,     4,     2,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     4,     4,
       6,     1,     3,     3,     1,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     2,     2,     2,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     2,     2,     3,     1,     1,
       1,     1,     1,     1,     5,     1,     2,     3,     9,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     2,
       2,     5,     1,     3,     1,     4,     1,     4,     1,     2,
       1,     2,     3,     3,     4,     1,     3,     5,     0,     2,
       6,     0,     1,     1,     1,     3,     2,     1,     3,     3,
       1,     1,     1,     0,     4,     0,     4,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "T_AMPERSAND", "T_ANY",
  "T_ASTERISK", "T_ATTRIBUTE", "T_BOOLEAN", "T_CASE", "T_CHAR",
  "T_CHARACTER_LITERAL", "T_CIRCUMFLEX", "T_COLON", "T_COMMA", "T_CONST",
  "T_CONTEXT", "T_DEFAULT", "T_DOUBLE", "T_ENUM", "T_EQUAL", "T_EXCEPTION",
  "T_FALSE", "T_FIXED", "T_FIXED_PT_LITERAL", "T_FLOAT",
  "T_FLOATING_PT_LITERAL", "T_GREATER_THAN_SIGN", "T_IDENTIFIER", "T_IN",
  "T_INOUT", "T_INTEGER_LITERAL", "T_INTERFACE", "T_LEFT_CURLY_BRACKET",
  "T_LEFT_PARANTHESIS", "T_LEFT_SQUARE_BRACKET", "T_LESS_THAN_SIGN",
  "T_LONG", "T_MINUS_SIGN", "T_MODULE", "T_OCTET", "T_ONEWAY", "T_OUT",
  "T_PERCENT_SIGN", "T_PLUS_SIGN", "T_RAISES", "T_READONLY",
  "T_RIGHT_CURLY_BRACKET", "T_RIGHT_PARANTHESIS", "T_RIGHT_SQUARE_BRACKET",
  "T_SCOPE", "T_SEMICOLON", "T_SEQUENCE", "T_SHIFTLEFT", "T_SHIFTRIGHT",
  "T_SHORT", "T_SOLIDUS", "T_STRING", "T_STRING_LITERAL", "T_PRAGMA",
  "T_STRUCT", "T_SWITCH", "T_TILDE", "T_TRUE", "T_OBJECT", "T_TYPEDEF",
  "T_UNION", "T_UNSIGNED", "T_VERTICAL_LINE", "T_VOID", "T_WCHAR",
  "T_WSTRING", "T_UNKNOWN", "T_ABSTRACT", "T_LOCAL", "T_VALUETYPE",
  "T_TRUNCATABLE", "T_SUPPORTS", "T_CUSTOM", "T_PUBLIC", "T_PRIVATE",
  "T_FACTORY", "T_NATIVE", "T_VALUEBASE", "$accept", "specification",
  "definitions", "definition", "module", "$@1", "interface",
  "interface_dcl", "forward_dcl", "interface_header", "$@2", "$@3", "$@4",
  "interface_body", "exports", "export", "interface_inheritance_spec",
  "interface_names", "scoped_names", "interface_name", "scoped_name",
  "value", "value_forward_dcl", "value_box_dcl", "value_abs_dcl",
  "value_body", "value_dcl", "value_elements", "value_header",
  "value_inheritance_spec", "value_inheritance_bases", "value_names",
  "value_name", "value_element", "state_member", "init_dcl",
  "init_param_decls", "init_param_decl", "init_param_attribute",
  "const_dcl", "const_type", "const_exp", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_const", "type_dcl", "type_spec",
  "simple_type_spec", "base_type_spec", "declarators", "declarator",
  "simple_declarator", "complex_declarator", "floating_pt_type",
  "integer_type", "signed_int", "signed_short_int", "signed_long_int",
  "signed_longlong_int", "unsigned_int", "unsigned_short_int",
  "unsigned_long_int", "unsigned_longlong_int", "char_type",
  "wide_char_type", "boolean_type", "octet_type", "any_type",
  "object_type", "struct_type", "member_list", "member", "union_type",
  "switch_type_spec", "switch_body", "case", "case_label", "element_spec",
  "enum_type", "enumerators", "enumerator", "string_type",
  "wide_string_type", "array_declarator", "fixed_array_sizes",
  "fixed_array_size", "attr_dcl", "simple_declarators", "except_dcl",
  "members", "op_dcl", "op_attribute", "op_type_spec", "parameter_dcls",
  "param_dcls", "param_dcl", "param_attribute", "raises_expr",
  "context_expr", "string_literals", "T_string_literal", "param_type_spec",
  "fixed_pt_const_type", "value_base_type", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   137,   137,   138,   143,   144,   149,   150,   151,   152,
     153,   154,   159,   159,   165,   166,   171,   177,   178,   179,
     184,   185,   185,   186,   187,   187,   188,   189,   189,   194,
     195,   199,   200,   205,   206,   207,   208,   209,   214,   218,
     219,   223,   224,   229,   234,   235,   236,   241,   242,   243,
     244,   249,   250,   255,   260,   262,   267,   268,   273,   275,
     279,   280,   285,   286,   287,   288,   293,   294,   295,   299,
     300,   301,   302,   306,   311,   316,   317,   318,   323,   324,
     329,   336,   337,   342,   347,   352,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   371,   376,   377,   382,
     383,   388,   389,   394,   395,   396,   401,   402,   403,   408,
     409,   410,   411,   417,   418,   419,   420,   425,   426,   427,
     433,   434,   435,   436,   437,   438,   439,   444,   450,   451,
     452,   453,   454,   459,   465,   467,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   500,   501,   506,   507,   512,
     517,   522,   523,   524,   529,   530,   535,   536,   537,   542,
     547,   552,   557,   558,   559,   564,   569,   574,   579,   584,
     589,   594,   599,   604,   609,   615,   616,   621,   626,   633,
     634,   635,   636,   637,   642,   643,   648,   649,   654,   655,
     660,   665,   670,   671,   676,   688,   689,   694,   695,   700,
     704,   705,   710,   715,   716,   720,   721,   726,   731,   732,
     737,   743,   744,   749,   750,   755,   756,   760,   761,   766,
     771,   772,   773,   778,   779,   784,   785,   789,   790,   794,
     795,   800,   801,   802,   803,   814,   819
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 4690 "yacc.tab.cc"

#line 822 "yacc.yy"


namespace yy {

// Report an error to the user.
void parser::error (const location& l, const std::string& msg)
{
	drv.parser_error (l, msg);
}

}
