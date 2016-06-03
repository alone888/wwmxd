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

#ifndef YY_CITI_TOKENS_CITI_H_INCLUDED
# define YY_CITI_TOKENS_CITI_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int citi_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    InvalidCharacter = 258,
    Float = 259,
    Eol = 260,
    DATA = 261,
    VAR = 262,
    NAME = 263,
    Begin = 264,
    End = 265,
    Version = 266,
    Identifier = 267,
    Integer = 268,
    CITIFILE = 269,
    VarType = 270,
    SegListBegin = 271,
    VarListBegin = 272,
    SegListEnd = 273,
    VarListEnd = 274,
    COMMENT = 275,
    CONSTANT = 276,
    SEG = 277
  };
#endif
/* Tokens.  */
#define InvalidCharacter 258
#define Float 259
#define Eol 260
#define DATA 261
#define VAR 262
#define NAME 263
#define Begin 264
#define End 265
#define Version 266
#define Identifier 267
#define Integer 268
#define CITIFILE 269
#define VarType 270
#define SegListBegin 271
#define VarListBegin 272
#define SegListEnd 273
#define VarListEnd 274
#define COMMENT 275
#define CONSTANT 276
#define SEG 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 75 "parse_citi.y" /* yacc.c:1915  */

  char * ident;
  double f;
  int d;
  qucs::vector * v;
  struct citi_package_t * p;
  struct citi_header_t * h;

#line 107 "tokens_citi.h" /* yacc.c:1915  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE citi_lval;

int citi_parse (void);

#endif /* !YY_CITI_TOKENS_CITI_H_INCLUDED  */
