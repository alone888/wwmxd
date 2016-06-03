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

#ifndef YY_DATASET_TOKENS_DATASET_H_INCLUDED
# define YY_DATASET_TOKENS_DATASET_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int dataset_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    InvalidCharacter = 258,
    Identifier = 259,
    REAL = 260,
    IMAG = 261,
    COMPLEX = 262,
    Integer = 263,
    Eol = 264,
    IndepBegin = 265,
    DepBegin = 266,
    IndepEnd = 267,
    DepEnd = 268,
    Version = 269
  };
#endif
/* Tokens.  */
#define InvalidCharacter 258
#define Identifier 259
#define REAL 260
#define IMAG 261
#define COMPLEX 262
#define Integer 263
#define Eol 264
#define IndepBegin 265
#define DepBegin 266
#define IndepEnd 267
#define DepEnd 268
#define Version 269

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 67 "parse_dataset.y" /* yacc.c:1915  */

  char * ident;
  double f;
  struct {
    double r;
    double i;
  } c;
  long n;
  qucs::vector * v;
  qucs::dataset * data;
  qucs::strlist * list;

#line 95 "tokens_dataset.h" /* yacc.c:1915  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE dataset_lval;

int dataset_parse (void);

#endif /* !YY_DATASET_TOKENS_DATASET_H_INCLUDED  */
