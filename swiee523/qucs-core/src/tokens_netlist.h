/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_NETLIST_TOKENS_NETLIST_H_INCLUDED
# define YY_NETLIST_TOKENS_NETLIST_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int netlist_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    InvalidCharacter = 258,
    Identifier = 259,
    Assign = 260,
    ScaleOrUnit = 261,
    Eol = 262,
    Eqn = 263,
    DefSub = 264,
    EndSub = 265,
    REAL = 266,
    IMAG = 267,
    COMPLEX = 268,
    Character = 269,
    STRING = 270,
    Or = 271,
    And = 272,
    NotEqual = 273,
    Equal = 274,
    Less = 275,
    Greater = 276,
    LessOrEqual = 277,
    GreaterOrEqual = 278,
    Not = 279,
    NEG = 280,
    POS = 281
  };
#endif
/* Tokens.  */
#define InvalidCharacter 258
#define Identifier 259
#define Assign 260
#define ScaleOrUnit 261
#define Eol 262
#define Eqn 263
#define DefSub 264
#define EndSub 265
#define REAL 266
#define IMAG 267
#define COMPLEX 268
#define Character 269
#define STRING 270
#define Or 271
#define And 272
#define NotEqual 273
#define Equal 274
#define Less 275
#define Greater 276
#define LessOrEqual 277
#define GreaterOrEqual 278
#define Not 279
#define NEG 280
#define POS 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 78 "parse_netlist.y" /* yacc.c:1915  */

  char * ident;
  char * str;
  double d;
  char chr;
  struct definition_t * definition;
  struct definition_t * subcircuit;
  struct node_t * node;
  struct pair_t * pair;
  struct value_t * value;
  struct {
    double r;
    double i;
  } c;
  qucs::eqn::node * eqn;
  qucs::eqn::constant * con;
  qucs::eqn::reference * ref;
  qucs::eqn::application * app;
  qucs::eqn::assignment * assign;

#line 127 "tokens_netlist.h" /* yacc.c:1915  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE netlist_lval;

int netlist_parse (void);

#endif /* !YY_NETLIST_TOKENS_NETLIST_H_INCLUDED  */
