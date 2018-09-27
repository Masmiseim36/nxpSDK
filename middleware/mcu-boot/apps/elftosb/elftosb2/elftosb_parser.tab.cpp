/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_IDENT = 258,
     TOK_STRING_LITERAL = 259,
     TOK_INT_LITERAL = 260,
     TOK_SECTION_NAME = 261,
     TOK_SOURCE_NAME = 262,
     TOK_BLOB = 263,
     TOK_DOT_DOT = 264,
     TOK_AND = 265,
     TOK_OR = 266,
     TOK_GEQ = 267,
     TOK_LEQ = 268,
     TOK_EQ = 269,
     TOK_NEQ = 270,
     TOK_POWER = 271,
     TOK_LSHIFT = 272,
     TOK_RSHIFT = 273,
     TOK_INT_SIZE = 274,
     TOK_OPTIONS = 275,
     TOK_CONSTANTS = 276,
     TOK_SOURCES = 277,
     TOK_FILTERS = 278,
     TOK_SECTION = 279,
     TOK_EXTERN = 280,
     TOK_FROM = 281,
     TOK_RAW = 282,
     TOK_LOAD = 283,
     TOK_JUMP = 284,
     TOK_CALL = 285,
     TOK_MODE = 286,
     TOK_ERASE = 287,
     TOK_ALL = 288,
     TOK_IF = 289,
     TOK_ELSE = 290,
     TOK_DEFINED = 291,
     TOK_INFO = 292,
     TOK_WARNING = 293,
     TOK_ERROR = 294,
     TOK_SIZEOF = 295,
     TOK_DCD = 296,
     TOK_HAB = 297,
     TOK_IVT = 298,
     TOK_UNSECURE = 299,
     TOK_RESET = 300,
     TOK_JUMP_SP = 301,
     TOK_ENABLE = 302,
     TOK_KEYBLOB = 303,
     TOK_ENCRYPT = 304,
     TOK_KEYWRAP = 305,
     UNARY_OP = 306
   };
#endif
/* Tokens.  */
#define TOK_IDENT 258
#define TOK_STRING_LITERAL 259
#define TOK_INT_LITERAL 260
#define TOK_SECTION_NAME 261
#define TOK_SOURCE_NAME 262
#define TOK_BLOB 263
#define TOK_DOT_DOT 264
#define TOK_AND 265
#define TOK_OR 266
#define TOK_GEQ 267
#define TOK_LEQ 268
#define TOK_EQ 269
#define TOK_NEQ 270
#define TOK_POWER 271
#define TOK_LSHIFT 272
#define TOK_RSHIFT 273
#define TOK_INT_SIZE 274
#define TOK_OPTIONS 275
#define TOK_CONSTANTS 276
#define TOK_SOURCES 277
#define TOK_FILTERS 278
#define TOK_SECTION 279
#define TOK_EXTERN 280
#define TOK_FROM 281
#define TOK_RAW 282
#define TOK_LOAD 283
#define TOK_JUMP 284
#define TOK_CALL 285
#define TOK_MODE 286
#define TOK_ERASE 287
#define TOK_ALL 288
#define TOK_IF 289
#define TOK_ELSE 290
#define TOK_DEFINED 291
#define TOK_INFO 292
#define TOK_WARNING 293
#define TOK_ERROR 294
#define TOK_SIZEOF 295
#define TOK_DCD 296
#define TOK_HAB 297
#define TOK_IVT 298
#define TOK_UNSECURE 299
#define TOK_RESET 300
#define TOK_JUMP_SP 301
#define TOK_ENABLE 302
#define TOK_KEYBLOB 303
#define TOK_ENCRYPT 304
#define TOK_KEYWRAP 305
#define UNARY_OP 306




/* Copy the first part of user declarations.  */
#line 18 "elftosb_parser.y"

#include "ElftosbLexer.h"
#include "ElftosbAST.h"
#include "Logging.h"
#include "Blob.h"
#include "format_string.h"
#include "Value.h"
#include "ConversionController.h"

using namespace elftosb;

//! Our special location type.
#define YYLTYPE token_loc_t

// this indicates that we're using our own type. it should be unset automatically
// but that's not working for some reason with the .hpp file.
#if defined(YYLTYPE_IS_TRIVIAL)
	#undef YYLTYPE_IS_TRIVIAL
	#define YYLTYPE_IS_TRIVIAL 0
#endif

//! Default location action
#define YYLLOC_DEFAULT(Current, Rhs, N)	\
	do {		\
		if (N)	\
		{		\
			(Current).m_firstLine = YYRHSLOC(Rhs, 1).m_firstLine;	\
			(Current).m_lastLine = YYRHSLOC(Rhs, N).m_lastLine;		\
		}		\
		else	\
		{		\
			(Current).m_firstLine = (Current).m_lastLine = YYRHSLOC(Rhs, 0).m_lastLine;	\
		}		\
	} while (0)

//! Forward declaration of yylex().
static int yylex(YYSTYPE * lvalp, YYLTYPE * yylloc, ElftosbLexer * lexer);

// Forward declaration of error handling function.
static void yyerror(YYLTYPE * yylloc, ElftosbLexer * lexer, CommandFileASTNode ** resultAST, const char * error);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 62 "elftosb_parser.y"
{
	int m_num;
	elftosb::SizedIntegerValue * m_int;
	Blob * m_blob;
	std::string * m_str;
	elftosb::ASTNode * m_ast;	// must use full name here because this is put into *.tab.hpp
}
/* Line 187 of yacc.c.  */
#line 249 "elftosb_parser.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 274 "elftosb_parser.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   540

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  158
/* YYNRULES -- Number of states.  */
#define YYNSTATES  291

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,     2,     2,    74,    23,     2,
       9,    10,    72,    70,    16,    71,    20,    73,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    17,
      25,    15,    19,     2,    27,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    13,     2,    14,    69,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    11,    24,    12,    22,     2,     2,     2,
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
       5,     6,     7,     8,    21,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     8,    11,    13,    15,    17,    19,
      24,    29,    31,    34,    37,    38,    42,    50,    52,    55,
      57,    58,    62,    67,    69,    72,    76,    77,    81,    88,
      92,    93,    95,    99,   103,   105,   108,   115,   118,   119,
     121,   122,   126,   130,   132,   135,   138,   140,   142,   144,
     146,   147,   149,   152,   155,   157,   158,   160,   162,   164,
     166,   168,   170,   172,   174,   179,   181,   184,   185,   187,
     189,   191,   193,   197,   202,   204,   206,   208,   212,   214,
     217,   220,   221,   223,   225,   230,   232,   233,   237,   242,
     247,   249,   251,   253,   255,   259,   262,   263,   267,   271,
     275,   277,   280,   281,   287,   295,   303,   306,   308,   312,
     315,   318,   321,   328,   333,   336,   337,   339,   343,   345,
     347,   349,   353,   357,   361,   365,   369,   373,   377,   381,
     384,   389,   393,   398,   400,   404,   407,   409,   411,   413,
     417,   421,   425,   429,   433,   437,   441,   445,   449,   453,
     457,   459,   463,   467,   472,   477,   482,   485,   488
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      77,     0,    -1,    78,    96,    -1,    79,    -1,    78,    79,
      -1,    80,    -1,    81,    -1,    89,    -1,    85,    -1,    38,
      11,    82,    12,    -1,    39,    11,    82,    12,    -1,    83,
      -1,    82,    83,    -1,    84,    17,    -1,    -1,     3,    15,
     132,    -1,    66,     9,   134,    10,    11,    86,    12,    -1,
      87,    -1,    86,    87,    -1,    88,    -1,    -1,     9,   114,
      10,    -1,    40,    11,    90,    12,    -1,    91,    -1,    90,
      91,    -1,    92,    93,    17,    -1,    -1,     3,    15,     4,
      -1,     3,    15,    43,     9,   134,    10,    -1,     9,    94,
      10,    -1,    -1,    95,    -1,    94,    16,    95,    -1,     3,
      15,   132,    -1,    97,    -1,    96,    97,    -1,    42,     9,
     134,    98,    10,   100,    -1,    17,    99,    -1,    -1,    94,
      -1,    -1,    31,   108,    17,    -1,    11,   101,    12,    -1,
     102,    -1,   101,   102,    -1,   105,    17,    -1,   122,    -1,
     123,    -1,   124,    -1,   129,    -1,    -1,   104,    -1,   103,
     104,    -1,   105,    17,    -1,   129,    -1,    -1,   106,    -1,
     115,    -1,   120,    -1,   125,    -1,   128,    -1,   126,    -1,
     116,    -1,   127,    -1,    46,   107,   108,   111,    -1,     3,
      -1,    27,   134,    -1,    -1,   134,    -1,     4,    -1,     7,
      -1,   109,    -1,   109,    44,     7,    -1,     7,    13,   109,
      14,    -1,     8,    -1,   113,    -1,   110,    -1,   109,    16,
     110,    -1,     6,    -1,    22,     6,    -1,    19,   112,    -1,
      -1,    20,    -1,   131,    -1,    61,     9,   114,    10,    -1,
      94,    -1,    -1,   117,   118,   119,    -1,    60,   117,   131,
     119,    -1,    64,   134,   118,   119,    -1,    48,    -1,    47,
      -1,     7,    -1,   134,    -1,     9,   134,    10,    -1,     9,
      10,    -1,    -1,    50,   121,   131,    -1,    50,   121,    51,
      -1,    50,    62,    51,    -1,     3,    -1,    27,   134,    -1,
      -1,    44,     7,    11,   103,    12,    -1,    67,     9,   134,
      10,    11,   103,    12,    -1,    68,     9,   134,    10,    11,
     103,    12,    -1,    49,   134,    -1,    63,    -1,    65,   121,
     131,    -1,    55,     4,    -1,    56,     4,    -1,    57,     4,
      -1,    52,   133,    11,   101,    12,   130,    -1,    53,    11,
     101,    12,    -1,    53,   129,    -1,    -1,   134,    -1,   134,
      21,   134,    -1,   133,    -1,     4,    -1,   134,    -1,   133,
      25,   133,    -1,   133,    19,   133,    -1,   133,    30,   133,
      -1,   133,    31,   133,    -1,   133,    32,   133,    -1,   133,
      33,   133,    -1,   133,    28,   133,    -1,   133,    29,   133,
      -1,    26,   133,    -1,     3,     9,     7,    10,    -1,     9,
     133,    10,    -1,    54,     9,     3,    10,    -1,   136,    -1,
       7,    18,     3,    -1,    18,     3,    -1,   138,    -1,     3,
      -1,   135,    -1,   136,    70,   136,    -1,   136,    71,   136,
      -1,   136,    72,   136,    -1,   136,    73,   136,    -1,   136,
      74,   136,    -1,   136,    34,   136,    -1,   136,    23,   136,
      -1,   136,    24,   136,    -1,   136,    69,   136,    -1,   136,
      35,   136,    -1,   136,    36,   136,    -1,   137,    -1,   136,
      20,    37,    -1,     9,   136,    10,    -1,    58,     9,   135,
      10,    -1,    58,     9,     3,    10,    -1,    58,     9,     7,
      10,    -1,    70,   136,    -1,    71,   136,    -1,     5,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   178,   178,   188,   194,   202,   203,   204,   205,   208,
     214,   220,   226,   233,   234,   237,   244,   252,   258,   265,
     266,   269,   281,   287,   293,   301,   313,   316,   321,   329,
     330,   334,   340,   348,   355,   361,   368,   383,   388,   394,
     399,   405,   411,   419,   425,   433,   434,   435,   436,   437,
     438,   441,   447,   455,   456,   457,   460,   461,   462,   463,
     464,   465,   466,   467,   470,   489,   494,   498,   501,   505,
     510,   515,   520,   525,   530,   535,   540,   546,   554,   559,
     566,   571,   577,   582,   588,   600,   601,   604,   633,   670,
     690,   691,   694,   699,   706,   707,   708,   711,   725,   740,
     749,   754,   758,   762,   769,   778,   787,   794,   801,   810,
     815,   820,   827,   838,   842,   849,   852,   857,   864,   868,
     875,   879,   886,   893,   900,   907,   914,   921,   928,   935,
     940,   945,   950,   957,   960,   965,   973,   977,   982,   993,
    1000,  1007,  1014,  1021,  1028,  1035,  1042,  1049,  1056,  1063,
    1070,  1074,  1079,  1084,  1089,  1094,  1101,  1105,  1112
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"identifier\"", "\"string\"",
  "\"integer\"", "\"section name\"", "\"source name\"",
  "\"binary object\"", "'('", "')'", "'{'", "'}'", "'['", "']'", "'='",
  "','", "';'", "':'", "'>'", "'.'", "\"..\"", "'~'", "'&'", "'|'", "'<'",
  "'!'", "'@'", "\"&&\"", "\"||\"", "\">=\"", "\"<=\"", "\"==\"", "\"!=\"",
  "\"**\"", "\"<<\"", "\">>\"", "\"integer size\"", "\"options\"",
  "\"constants\"", "\"sources\"", "\"filters\"", "\"section\"",
  "\"extern\"", "\"from\"", "\"raw\"", "\"load\"", "\"jump\"", "\"call\"",
  "\"mode\"", "\"erase\"", "\"all\"", "\"if\"", "\"else\"", "\"defined\"",
  "\"info\"", "\"warning\"", "\"error\"", "\"sizeof\"", "\"dcd\"",
  "\"hab\"", "\"ivt\"", "\"unsecure\"", "\"reset\"", "\"jump_sp\"",
  "\"enable\"", "\"keyblob\"", "\"encrypt\"", "\"keywrap\"", "'^'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "UNARY_OP", "$accept", "command_file",
  "blocks_list", "pre_section_block", "options_block", "constants_block",
  "const_def_list", "const_def_list_elem", "const_def", "keyblob_block",
  "keyblob_def_list", "keyblob_def_list_elem", "keyblob_def",
  "sources_block", "source_def_list", "source_def_list_elem", "source_def",
  "source_attrs_opt", "source_attr_list", "source_attr_list_elem",
  "section_defs", "section_def", "section_options_opt",
  "source_attr_list_opt", "section_contents", "full_stmt_list",
  "full_stmt_list_elem", "basic_stmt_list", "basic_stmt_list_elem",
  "basic_stmt", "load_stmt", "load_opt", "load_data", "section_list",
  "section_list_elem", "load_target_opt", "load_target", "ivt_def",
  "assignment_list_opt", "call_stmt", "jump_sp_stmt", "call_or_jump",
  "call_target", "call_arg_opt", "erase_stmt", "mem_opt", "from_stmt",
  "encrypt_stmt", "keywrap_stmt", "mode_stmt", "reset_stmt", "enable_stmt",
  "message_stmt", "if_stmt", "else_opt", "address_or_range", "const_expr",
  "bool_expr", "int_const_expr", "symbol_ref", "expr", "unary_expr",
  "int_value", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,    40,
      41,   123,   125,    91,    93,    61,    44,    59,    58,    62,
      46,   264,   126,    38,   124,    60,    33,    64,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,    94,
      43,    45,    42,    47,    37,   306
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    78,    78,    79,    79,    79,    79,    80,
      81,    82,    82,    83,    83,    84,    85,    86,    86,    87,
      87,    88,    89,    90,    90,    91,    91,    92,    92,    93,
      93,    94,    94,    95,    96,    96,    97,    98,    98,    99,
      99,   100,   100,   101,   101,   102,   102,   102,   102,   102,
     102,   103,   103,   104,   104,   104,   105,   105,   105,   105,
     105,   105,   105,   105,   106,   107,   107,   107,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   109,   110,   110,
     111,   111,   112,   112,   113,   114,   114,   115,   115,   116,
     117,   117,   118,   118,   119,   119,   119,   120,   120,   120,
     121,   121,   121,   122,   123,   124,   125,   126,   127,   128,
     128,   128,   129,   130,   130,   130,   131,   131,   132,   132,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   134,   135,   135,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   137,   137,   138
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     1,     1,     1,     1,     4,
       4,     1,     2,     2,     0,     3,     7,     1,     2,     1,
       0,     3,     4,     1,     2,     3,     0,     3,     6,     3,
       0,     1,     3,     3,     1,     2,     6,     2,     0,     1,
       0,     3,     3,     1,     2,     2,     1,     1,     1,     1,
       0,     1,     2,     2,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     2,     0,     1,     1,
       1,     1,     3,     4,     1,     1,     1,     3,     1,     2,
       2,     0,     1,     1,     4,     1,     0,     3,     4,     4,
       1,     1,     1,     1,     3,     2,     0,     3,     3,     3,
       1,     2,     0,     5,     7,     7,     2,     1,     3,     2,
       2,     2,     6,     4,     2,     0,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       4,     3,     4,     1,     3,     2,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     4,     4,     4,     2,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     3,     5,     6,
       8,     7,    14,    14,    26,     0,     1,     0,     4,     2,
      34,     0,     0,    11,     0,     0,     0,     0,    23,    30,
     137,   158,     0,     0,     0,     0,     0,     0,     0,   138,
     133,   150,   136,     0,    35,     0,     9,    12,    13,    10,
       0,    22,    24,     0,     0,     0,     0,   135,     0,   156,
     157,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,   137,   119,     0,     0,     0,
      15,   118,   120,    27,     0,     0,     0,    31,    25,   134,
     152,     0,     0,     0,    20,   151,   145,   146,   144,   148,
     149,   147,   139,   140,   141,   142,   143,    40,     0,     0,
       0,   133,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   154,   155,   153,    86,
       0,    17,    19,    39,    37,     0,     0,   131,     0,   122,
     121,   127,   128,   123,   124,   125,   126,     0,    33,    32,
      85,     0,    16,    18,    50,     0,    36,   130,   132,    28,
      21,     0,    67,    91,    90,     0,   102,     0,     0,     0,
       0,     0,   107,     0,   102,     0,     0,     0,    43,     0,
      56,    57,    62,     0,    58,    46,    47,    48,    59,    61,
      63,    60,    49,    69,    78,    70,    74,     0,     0,     0,
      71,    76,    75,    68,     0,    65,     0,     0,   106,   100,
       0,     0,     0,     0,   109,   110,   111,     0,     0,     0,
       0,     0,    42,    44,    45,    92,    96,    93,     0,    79,
      86,    41,     0,     0,    55,    66,    81,   101,    99,    98,
      97,   116,    50,    96,    96,   108,     0,     0,     0,    87,
       0,     0,    77,    72,     0,    51,     0,    54,     0,    64,
       0,     0,    88,    89,     0,     0,    95,     0,    73,    84,
     103,    52,    53,    82,    80,    83,   117,   115,    55,    55,
      94,     0,   112,     0,     0,    50,   114,   104,   105,     0,
     113
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    22,    23,    24,    10,
     130,   131,   132,    11,    27,    28,    29,    54,   150,    87,
      19,    20,   108,   134,   156,   177,   178,   254,   255,   179,
     180,   207,   199,   200,   201,   259,   274,   202,   151,   181,
     182,   183,   226,   249,   184,   212,   185,   186,   187,   188,
     189,   190,   191,   192,   282,   240,    80,    81,    82,    39,
      40,    41,    42
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -226
static const yytype_int16 yypact[] =
{
      99,    19,    75,    83,    17,   115,   124,  -226,  -226,  -226,
    -226,  -226,   108,   108,   126,   117,  -226,   123,  -226,    92,
    -226,   133,   113,  -226,   153,   116,   144,   186,  -226,   177,
    -226,  -226,   181,   117,   204,   200,   117,   117,   219,  -226,
     107,  -226,  -226,   117,  -226,     9,  -226,  -226,  -226,  -226,
      54,  -226,  -226,   229,   224,   239,   191,  -226,   164,  -226,
    -226,   233,   217,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   250,   264,  -226,    29,    29,   274,
    -226,   507,  -226,  -226,   279,   241,   136,  -226,  -226,  -226,
    -226,   282,    72,   288,   291,  -226,   318,   149,     4,   176,
     176,   310,    81,    81,   281,   281,   281,   229,   295,   299,
     422,   191,  -226,   305,    29,    29,    29,    29,    29,    29,
      29,    29,   117,     9,  -226,   229,  -226,  -226,  -226,   229,
      39,  -226,  -226,   293,  -226,     0,   303,  -226,   307,  -226,
    -226,   205,   205,  -226,  -226,  -226,  -226,   315,  -226,  -226,
     293,   316,  -226,  -226,   430,     1,  -226,  -226,  -226,  -226,
    -226,   324,    30,  -226,  -226,   117,    22,    29,   330,   337,
     338,   146,  -226,   117,    61,   334,   339,   222,  -226,   332,
    -226,  -226,  -226,   142,  -226,  -226,  -226,  -226,  -226,  -226,
    -226,  -226,  -226,  -226,  -226,    96,  -226,   341,   342,   333,
      37,  -226,  -226,  -226,   344,  -226,   117,     1,  -226,  -226,
     117,   313,    47,   477,  -226,  -226,  -226,   117,   142,   117,
     117,   117,  -226,  -226,  -226,   181,   347,  -226,    15,  -226,
     229,  -226,    15,   358,   465,  -226,   351,  -226,  -226,  -226,
    -226,   348,   430,   347,   347,  -226,   362,   363,    86,  -226,
      52,   367,  -226,  -226,   311,  -226,   361,  -226,   103,  -226,
     117,   247,  -226,  -226,   368,   375,  -226,   377,  -226,  -226,
    -226,  -226,  -226,  -226,  -226,  -226,  -226,   340,   465,   465,
    -226,    18,  -226,   350,   373,   430,  -226,  -226,  -226,   272,
    -226
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -226,  -226,  -226,   388,  -226,  -226,   382,   111,  -226,  -226,
    -226,   271,  -226,  -226,  -226,   376,  -226,  -226,   -38,   283,
    -226,   385,  -226,  -226,  -226,  -225,  -176,   -62,  -181,    -3,
    -226,  -226,   202,   183,   180,  -226,  -226,  -226,   187,  -226,
    -226,   245,   206,    -4,  -226,   244,  -226,  -226,  -226,  -226,
    -226,  -226,  -226,   -26,  -226,  -197,   304,   -75,   -15,   381,
     398,  -226,  -226
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      38,   223,   110,   112,    30,   193,    31,   194,   195,   196,
      33,   154,    75,    76,    31,    86,    32,   261,    77,    34,
     243,   194,   245,   197,    62,   209,    15,    34,    74,   285,
      12,   155,    75,   205,    31,    78,    32,   197,    77,   139,
     140,   141,   142,   143,   144,   145,   146,    34,   129,   210,
      30,   152,    31,   232,    32,    78,    33,   206,    83,    35,
     289,   275,   198,    79,   209,    34,   268,    35,   232,   133,
     167,    36,    37,   271,    69,    70,    71,    72,    73,    36,
      37,   233,   127,    79,   211,   223,    13,    35,   210,    30,
      55,    31,   213,    32,    14,    33,   266,    84,   239,    36,
      37,    62,   271,   271,    34,    35,    30,   147,    31,   228,
      32,    21,    33,   223,    55,    16,    21,    36,    37,    21,
      30,    34,    31,   273,    32,    46,    33,    62,    49,    26,
      63,    64,    43,    47,    17,    34,    47,     1,     2,     3,
     203,    65,    66,    67,    35,    30,   124,    31,    45,   225,
     208,    33,   125,    71,    72,    73,    36,    37,   218,    50,
      34,    35,     1,     2,     3,     4,    17,    91,   227,    62,
      48,    92,    63,    36,    37,    35,    68,    69,    70,    71,
      72,    73,    34,    65,    66,    67,    53,    36,    37,    26,
       4,   235,   203,   163,   164,   237,    62,   241,    51,    55,
      35,    90,   241,   227,   241,   246,   247,    57,   257,    58,
      65,    62,    36,    37,    63,    64,   283,   284,    68,    69,
      70,    71,    72,    73,   114,    65,    66,    67,   257,    61,
     115,   256,    85,   267,   222,   118,   119,   120,   121,   262,
     263,    88,    89,   241,    94,   276,    69,    70,    71,    72,
      73,   256,   257,   257,    95,   286,   123,   257,   257,   277,
      68,    69,    70,    71,    72,    73,   161,   107,   162,   163,
     164,   165,   166,   109,   167,   256,   256,   168,   169,   170,
     256,   256,   171,   113,   290,   172,   173,   174,   122,   175,
     176,   161,   126,   162,   163,   164,   165,   166,   128,   167,
     129,    62,   168,   169,   170,   135,   136,   171,   138,   125,
     172,   173,   174,   157,   175,   176,   161,   158,   162,   163,
     164,   165,   166,   270,   167,   159,   160,   168,   169,   170,
      62,   204,   171,    63,   214,   172,   173,   174,    62,   175,
     176,   215,   216,   220,    65,    66,    67,   229,   221,   224,
     231,   230,    65,    66,    67,   234,   248,   162,   163,   164,
     165,   166,   287,   167,   238,   253,   168,   169,   170,   260,
     258,   171,   264,   265,   172,   173,   174,   269,   272,   278,
      69,    70,    71,    72,    73,   288,   279,   280,    69,    70,
      71,    72,    73,   281,    18,    25,   162,   163,   164,   165,
     166,   153,   167,    52,    44,   168,   169,   170,   149,   236,
     171,   250,   252,   172,   173,   174,   217,   251,   219,   162,
     163,   164,   165,   166,   244,   167,     0,   148,   168,   169,
     170,    56,   137,   171,    59,    60,   172,   173,   174,    93,
       0,   114,     0,     0,     0,     0,     0,   115,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,     0,   161,   111,   162,   163,   164,   165,
     166,     0,   167,     0,     0,   168,   169,   170,   242,     0,
     171,     0,     0,   172,   173,   174,   114,   175,   176,     0,
       0,     0,   115,     0,     0,   116,   117,   118,   119,   120,
     121,   162,   163,   164,   165,   166,     0,   167,     0,     0,
     168,   169,   170,     0,     0,   171,   114,     0,   172,   173,
     174,     0,   115,     0,     0,   116,   117,   118,   119,   120,
     121
};

static const yytype_int16 yycheck[] =
{
      15,   177,    77,    78,     3,     4,     5,     6,     7,     8,
       9,    11,     3,     4,     5,    53,     7,   242,     9,    18,
     217,     6,   219,    22,    20,     3,     9,    18,    43,    11,
      11,    31,     3,     3,     5,    26,     7,    22,     9,   114,
     115,   116,   117,   118,   119,   120,   121,    18,     9,    27,
       3,    12,     5,    16,     7,    26,     9,    27,     4,    58,
     285,   258,    61,    54,     3,    18,    14,    58,    16,   107,
      52,    70,    71,   254,    70,    71,    72,    73,    74,    70,
      71,    44,    10,    54,    62,   261,    11,    58,    27,     3,
      18,     5,   167,     7,    11,     9,    10,    43,    51,    70,
      71,    20,   283,   284,    18,    58,     3,   122,     5,    13,
       7,     3,     9,   289,    18,     0,     3,    70,    71,     3,
       3,    18,     5,    20,     7,    12,     9,    20,    12,     3,
      23,    24,     9,    22,    42,    18,    25,    38,    39,    40,
     155,    34,    35,    36,    58,     3,    10,     5,    15,     7,
     165,     9,    16,    72,    73,    74,    70,    71,   173,    15,
      18,    58,    38,    39,    40,    66,    42,     3,   183,    20,
      17,     7,    23,    70,    71,    58,    69,    70,    71,    72,
      73,    74,    18,    34,    35,    36,     9,    70,    71,     3,
      66,   206,   207,    47,    48,   210,    20,   212,    12,    18,
      58,    10,   217,   218,   219,   220,   221,     3,   234,     9,
      34,    20,    70,    71,    23,    24,   278,   279,    69,    70,
      71,    72,    73,    74,    19,    34,    35,    36,   254,    10,
      25,   234,     3,   248,    12,    30,    31,    32,    33,   243,
     244,    17,     3,   258,    11,   260,    70,    71,    72,    73,
      74,   254,   278,   279,    37,   281,    15,   283,   284,    12,
      69,    70,    71,    72,    73,    74,    44,    17,    46,    47,
      48,    49,    50,     9,    52,   278,   279,    55,    56,    57,
     283,   284,    60,     9,    12,    63,    64,    65,     9,    67,
      68,    44,    10,    46,    47,    48,    49,    50,    10,    52,
       9,    20,    55,    56,    57,    10,     7,    60,     3,    16,
      63,    64,    65,    10,    67,    68,    44,    10,    46,    47,
      48,    49,    50,    12,    52,    10,    10,    55,    56,    57,
      20,     7,    60,    23,     4,    63,    64,    65,    20,    67,
      68,     4,     4,     9,    34,    35,    36,     6,     9,    17,
      17,     9,    34,    35,    36,    11,     9,    46,    47,    48,
      49,    50,    12,    52,    51,     7,    55,    56,    57,    21,
      19,    60,    10,    10,    63,    64,    65,    10,    17,    11,
      70,    71,    72,    73,    74,    12,    11,    10,    70,    71,
      72,    73,    74,    53,     6,    13,    46,    47,    48,    49,
      50,   130,    52,    27,    19,    55,    56,    57,   125,   207,
      60,   228,   232,    63,    64,    65,   171,   230,   174,    46,
      47,    48,    49,    50,   218,    52,    -1,   123,    55,    56,
      57,    33,    10,    60,    36,    37,    63,    64,    65,    58,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    -1,    44,    77,    46,    47,    48,    49,
      50,    -1,    52,    -1,    -1,    55,    56,    57,    11,    -1,
      60,    -1,    -1,    63,    64,    65,    19,    67,    68,    -1,
      -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,    32,
      33,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      55,    56,    57,    -1,    -1,    60,    19,    -1,    63,    64,
      65,    -1,    25,    -1,    -1,    28,    29,    30,    31,    32,
      33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    38,    39,    40,    66,    77,    78,    79,    80,    81,
      85,    89,    11,    11,    11,     9,     0,    42,    79,    96,
      97,     3,    82,    83,    84,    82,     3,    90,    91,    92,
       3,     5,     7,     9,    18,    58,    70,    71,   134,   135,
     136,   137,   138,     9,    97,    15,    12,    83,    17,    12,
      15,    12,    91,     9,    93,    18,   136,     3,     9,   136,
     136,    10,    20,    23,    24,    34,    35,    36,    69,    70,
      71,    72,    73,    74,   134,     3,     4,     9,    26,    54,
     132,   133,   134,     4,    43,     3,    94,    95,    17,     3,
      10,     3,     7,   135,    11,    37,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,    17,    98,     9,
     133,   136,   133,     9,    19,    25,    28,    29,    30,    31,
      32,    33,     9,    15,    10,    16,    10,    10,    10,     9,
      86,    87,    88,    94,    99,    10,     7,    10,     3,   133,
     133,   133,   133,   133,   133,   133,   133,   134,   132,    95,
      94,   114,    12,    87,    11,    31,   100,    10,    10,    10,
      10,    44,    46,    47,    48,    49,    50,    52,    55,    56,
      57,    60,    63,    64,    65,    67,    68,   101,   102,   105,
     106,   115,   116,   117,   120,   122,   123,   124,   125,   126,
     127,   128,   129,     4,     6,     7,     8,    22,    61,   108,
     109,   110,   113,   134,     7,     3,    27,   107,   134,     3,
      27,    62,   121,   133,     4,     4,     4,   117,   134,   121,
       9,     9,    12,   102,    17,     7,   118,   134,    13,     6,
       9,    17,    16,    44,    11,   134,   108,   134,    51,    51,
     131,   134,    11,   131,   118,   131,   134,   134,     9,   119,
     109,   114,   110,     7,   103,   104,   105,   129,    19,   111,
      21,   101,   119,   119,    10,    10,    10,   134,    14,    10,
      12,   104,    17,    20,   112,   131,   134,    12,    11,    11,
      10,    53,   130,   103,   103,    11,   129,    12,    12,   101,
      12
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, lexer, resultAST, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, lexer)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, lexer, resultAST); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ElftosbLexer * lexer, CommandFileASTNode ** resultAST)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, lexer, resultAST)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ElftosbLexer * lexer;
    CommandFileASTNode ** resultAST;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (lexer);
  YYUSE (resultAST);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ElftosbLexer * lexer, CommandFileASTNode ** resultAST)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, lexer, resultAST)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ElftosbLexer * lexer;
    CommandFileASTNode ** resultAST;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, lexer, resultAST);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ElftosbLexer * lexer, CommandFileASTNode ** resultAST)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, lexer, resultAST)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    ElftosbLexer * lexer;
    CommandFileASTNode ** resultAST;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , lexer, resultAST);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, lexer, resultAST); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ElftosbLexer * lexer, CommandFileASTNode ** resultAST)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, lexer, resultAST)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    ElftosbLexer * lexer;
    CommandFileASTNode ** resultAST;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (lexer);
  YYUSE (resultAST);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "\"identifier\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1524 "elftosb_parser.tab.cpp"
	break;
      case 4: /* "\"string\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1529 "elftosb_parser.tab.cpp"
	break;
      case 5: /* "\"integer\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_int); };
#line 1534 "elftosb_parser.tab.cpp"
	break;
      case 6: /* "\"section name\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1539 "elftosb_parser.tab.cpp"
	break;
      case 7: /* "\"source name\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1544 "elftosb_parser.tab.cpp"
	break;
      case 8: /* "\"binary object\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_blob); };
#line 1549 "elftosb_parser.tab.cpp"
	break;
      case 37: /* "\"integer size\"" */
#line 174 "elftosb_parser.y"
	{ delete (yyvaluep->m_int); };
#line 1554 "elftosb_parser.tab.cpp"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (ElftosbLexer * lexer, CommandFileASTNode ** resultAST);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (ElftosbLexer * lexer, CommandFileASTNode ** resultAST)
#else
int
yyparse (lexer, resultAST)
    ElftosbLexer * lexer;
    CommandFileASTNode ** resultAST;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 179 "elftosb_parser.y"
    {
							CommandFileASTNode * commandFile = new CommandFileASTNode();
							commandFile->setBlocks(dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast)));
							commandFile->setSections(dynamic_cast<ListASTNode*>((yyvsp[(2) - (2)].m_ast)));
							commandFile->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							*resultAST = commandFile;
						}
    break;

  case 3:
#line 189 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						}
    break;

  case 4:
#line 195 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						}
    break;

  case 5:
#line 202 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 6:
#line 203 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 7:
#line 204 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 8:
#line 205 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 9:
#line 209 "elftosb_parser.y"
    {
								(yyval.m_ast) = new OptionsBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
							}
    break;

  case 10:
#line 215 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ConstantsBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
							}
    break;

  case 11:
#line 221 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 12:
#line 227 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							}
    break;

  case 13:
#line 233 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); }
    break;

  case 14:
#line 234 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 15:
#line 238 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AssignmentASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 16:
#line 245 "elftosb_parser.y"
    {
							KeyblobBlockASTNode * keyblob = new KeyblobBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(6) - (7)].m_ast)));
							keyblob->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
							(yyval.m_ast) = keyblob;
						}
    break;

  case 17:
#line 253 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						}
    break;

  case 18:
#line 259 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						}
    break;

  case 19:
#line 265 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 20:
#line 266 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 21:
#line 270 "elftosb_parser.y"
    {
								KeyblobEntryASTNode * entry = new KeyblobEntryASTNode();
								if ((yyvsp[(2) - (3)].m_ast))
								{
									entry->setFieldAssignments(dynamic_cast<ListASTNode*>((yyvsp[(2) - (3)].m_ast)));
								}
								entry->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = entry;
							}
    break;

  case 22:
#line 282 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SourcesBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
						}
    break;

  case 23:
#line 288 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						}
    break;

  case 24:
#line 294 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						}
    break;

  case 25:
#line 302 "elftosb_parser.y"
    {
								// tell the lexer that this is the name of a source file
								SourceDefASTNode * node = dynamic_cast<SourceDefASTNode*>((yyvsp[(1) - (3)].m_ast));
								if ((yyvsp[(2) - (3)].m_ast))
								{
									node->setAttributes(dynamic_cast<ListASTNode*>((yyvsp[(2) - (3)].m_ast)));
								}
								node->setLocation(node->getLocation(), (yylsp[(3) - (3)]));
								lexer->addSourceName(node->getName());
								(yyval.m_ast) = (yyvsp[(1) - (3)].m_ast);
							}
    break;

  case 26:
#line 313 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 27:
#line 317 "elftosb_parser.y"
    {
								(yyval.m_ast) = new PathSourceDefASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 28:
#line 322 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ExternSourceDefASTNode((yyvsp[(1) - (6)].m_str), dynamic_cast<ExprASTNode*>((yyvsp[(5) - (6)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (6)]), (yylsp[(6) - (6)]));
							}
    break;

  case 29:
#line 329 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(2) - (3)].m_ast); }
    break;

  case 30:
#line 330 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 31:
#line 335 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 32:
#line 341 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast))->appendNode((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (3)].m_ast);
							}
    break;

  case 33:
#line 349 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AssignmentASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 34:
#line 356 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 35:
#line 362 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							}
    break;

  case 36:
#line 369 "elftosb_parser.y"
    {
								SectionContentsASTNode * sectionNode = dynamic_cast<SectionContentsASTNode*>((yyvsp[(6) - (6)].m_ast));
								if (sectionNode)
								{
									ExprASTNode * exprNode = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (6)].m_ast));
									sectionNode->setSectionNumberExpr(exprNode);
									sectionNode->setOptions(dynamic_cast<ListASTNode*>((yyvsp[(4) - (6)].m_ast)));
									sectionNode->setLocation((yylsp[(1) - (6)]), sectionNode->getLocation());
								}
								(yyval.m_ast) = (yyvsp[(6) - (6)].m_ast);
							}
    break;

  case 37:
#line 384 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							}
    break;

  case 38:
#line 388 "elftosb_parser.y"
    {
								(yyval.m_ast) = NULL;
							}
    break;

  case 39:
#line 395 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 40:
#line 399 "elftosb_parser.y"
    {
								(yyval.m_ast) = NULL;
							}
    break;

  case 41:
#line 406 "elftosb_parser.y"
    {
								DataSectionContentsASTNode * dataSection = new DataSectionContentsASTNode((yyvsp[(2) - (3)].m_ast));
								dataSection->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = dataSection;
							}
    break;

  case 42:
#line 412 "elftosb_parser.y"
    {
								ListASTNode * listNode = dynamic_cast<ListASTNode*>((yyvsp[(2) - (3)].m_ast));
								(yyval.m_ast) = new BootableSectionContentsASTNode(listNode);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 43:
#line 420 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 44:
#line 426 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							}
    break;

  case 45:
#line 433 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); }
    break;

  case 46:
#line 434 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 47:
#line 435 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 48:
#line 436 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 49:
#line 437 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 50:
#line 438 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 51:
#line 442 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 52:
#line 448 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							}
    break;

  case 53:
#line 455 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); }
    break;

  case 54:
#line 456 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 55:
#line 457 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 56:
#line 460 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 57:
#line 461 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 58:
#line 462 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 59:
#line 463 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 60:
#line 464 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 61:
#line 465 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 62:
#line 466 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 63:
#line 467 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 64:
#line 471 "elftosb_parser.y"
    {
								LoadStatementASTNode * stmt = new LoadStatementASTNode();
								stmt->setLoadOption((yyvsp[(2) - (4)].m_ast));
								stmt->setData((yyvsp[(3) - (4)].m_ast));
								stmt->setTarget((yyvsp[(4) - (4)].m_ast));
								// set char locations for the statement
								if ((yyvsp[(4) - (4)].m_ast))
								{
									stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
								}
								else
								{
									stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(3) - (4)]));
								}
								(yyval.m_ast) = stmt;
							}
    break;

  case 65:
#line 490 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 66:
#line 495 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							}
    break;

  case 67:
#line 498 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 68:
#line 502 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 69:
#line 506 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 70:
#line 511 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SourceASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 71:
#line 516 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(1) - (1)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 72:
#line 521 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast)), (yyvsp[(3) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 73:
#line 526 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(3) - (4)].m_ast)), (yyvsp[(1) - (4)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
							}
    break;

  case 74:
#line 531 "elftosb_parser.y"
    {
								(yyval.m_ast) = new BlobConstASTNode((yyvsp[(1) - (1)].m_blob));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 75:
#line 536 "elftosb_parser.y"
    {
							}
    break;

  case 76:
#line 541 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							}
    break;

  case 77:
#line 547 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast))->appendNode((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (3)].m_ast);
							}
    break;

  case 78:
#line 555 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionASTNode((yyvsp[(1) - (1)].m_str), SectionASTNode::kInclude);
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 79:
#line 560 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionASTNode((yyvsp[(2) - (2)].m_str), SectionASTNode::kExclude);
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 80:
#line 567 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							}
    break;

  case 81:
#line 571 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NaturalLocationASTNode();
//								$$->setLocation();
							}
    break;

  case 82:
#line 578 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NaturalLocationASTNode();
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 83:
#line 583 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 84:
#line 589 "elftosb_parser.y"
    {
								IVTConstASTNode * ivt = new IVTConstASTNode();
								if ((yyvsp[(3) - (4)].m_ast))
								{
									ivt->setFieldAssignments(dynamic_cast<ListASTNode*>((yyvsp[(3) - (4)].m_ast)));
								}
								ivt->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
								(yyval.m_ast) = ivt;
							}
    break;

  case 85:
#line 600 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 86:
#line 601 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 87:
#line 605 "elftosb_parser.y"
    {
								CallStatementASTNode * stmt = new CallStatementASTNode();
								switch ((yyvsp[(1) - (3)].m_num))
								{
									case 1:
										stmt->setCallType(CallStatementASTNode::kCallType);
										break;
									case 2:
										stmt->setCallType(CallStatementASTNode::kJumpType);
										break;
									default:
										yyerror(&yylloc, lexer, resultAST, "invalid call_or_jump value");
										YYABORT;
										break;
								}
								stmt->setTarget((yyvsp[(2) - (3)].m_ast));
								stmt->setArgument((yyvsp[(3) - (3)].m_ast));
								stmt->setIsHAB(false);
								if ((yyvsp[(3) - (3)].m_ast))
								{
									stmt->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								}
								else
								{
									stmt->setLocation((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
								}
								(yyval.m_ast) = stmt;
							}
    break;

  case 88:
#line 634 "elftosb_parser.y"
    {
								if (!elftosb::g_enableHABSupport)
								{
									yyerror(&yylloc, lexer, resultAST, "HAB features not supported with the selected family");
									YYABORT;
								}
								
								CallStatementASTNode * stmt = new CallStatementASTNode();
								switch ((yyvsp[(2) - (4)].m_num))
								{
									case 1:
										stmt->setCallType(CallStatementASTNode::kCallType);
										break;
									case 2:
										stmt->setCallType(CallStatementASTNode::kJumpType);
										break;
									default:
										yyerror(&yylloc, lexer, resultAST, "invalid call_or_jump value");
										YYABORT;
										break;
								}
								stmt->setTarget((yyvsp[(3) - (4)].m_ast));
								stmt->setArgument((yyvsp[(4) - (4)].m_ast));
								stmt->setIsHAB(true);
								if ((yyvsp[(4) - (4)].m_ast))
								{
									stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
								}
								else
								{
									stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(3) - (4)]));
								}
								(yyval.m_ast) = stmt;
							}
    break;

  case 89:
#line 671 "elftosb_parser.y"
    {
							CallStatementASTNode * stmt = new CallStatementASTNode();
							stmt->setCallType(CallStatementASTNode::kJumpType);
							stmt->setStackPointer((yyvsp[(2) - (4)].m_ast));
							stmt->setTarget((yyvsp[(3) - (4)].m_ast));
							stmt->setArgument((yyvsp[(4) - (4)].m_ast));
							stmt->setIsHAB(false);
							if ((yyvsp[(4) - (4)].m_ast))
							{
								stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
							}
							else
							{
								stmt->setLocation((yylsp[(1) - (4)]), (yylsp[(3) - (4)]));
							}
							(yyval.m_ast) = stmt;
						}
    break;

  case 90:
#line 690 "elftosb_parser.y"
    { (yyval.m_num) = 1; }
    break;

  case 91:
#line 691 "elftosb_parser.y"
    { (yyval.m_num) = 2; }
    break;

  case 92:
#line 695 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode(NULL, (yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 93:
#line 700 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (1)].m_ast), NULL);
								(yyval.m_ast)->setLocation((yyvsp[(1) - (1)].m_ast));
							}
    break;

  case 94:
#line 706 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(2) - (3)].m_ast); }
    break;

  case 95:
#line 707 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 96:
#line 708 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 97:
#line 712 "elftosb_parser.y"
    {
								EraseStatementASTNode * eraseNode = new EraseStatementASTNode((yyvsp[(3) - (3)].m_ast));
								eraseNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								if ((yyvsp[(2) - (3)].m_ast))
								{
									eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								}
								else
								{
									eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
								}
								(yyval.m_ast) = eraseNode;
							}
    break;

  case 98:
#line 726 "elftosb_parser.y"
    {
								EraseStatementASTNode * eraseNode = new EraseStatementASTNode();
								eraseNode->setEraseAll(true);
								eraseNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								if ((yyvsp[(2) - (3)].m_ast))
								{
									eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								}
								else
								{
									eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
								}
								(yyval.m_ast) = eraseNode;
							}
    break;

  case 99:
#line 741 "elftosb_parser.y"
    {
								EraseStatementASTNode * eraseNode = new EraseStatementASTNode();
								eraseNode->setEraseAllUnsecure(true);
								eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = eraseNode;
							}
    break;

  case 100:
#line 750 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 101:
#line 755 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							}
    break;

  case 102:
#line 758 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 103:
#line 763 "elftosb_parser.y"
    {
								(yyval.m_ast) = new FromStatementASTNode((yyvsp[(2) - (5)].m_str), dynamic_cast<ListASTNode*>((yyvsp[(4) - (5)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (5)]), (yylsp[(5) - (5)]));
							}
    break;

  case 104:
#line 770 "elftosb_parser.y"
    {
								EncryptStatementASTNode * encryptNode = new EncryptStatementASTNode(dynamic_cast<ListASTNode*>((yyvsp[(6) - (7)].m_ast)));
								encryptNode->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
								encryptNode->setLocation((yylsp[(1) - (7)]), (yylsp[(7) - (7)]));
								(yyval.m_ast) = encryptNode;
							}
    break;

  case 105:
#line 779 "elftosb_parser.y"
    {
								KeywrapStatementASTNode * keywrapNode = new KeywrapStatementASTNode(dynamic_cast<ListASTNode*>((yyvsp[(6) - (7)].m_ast)));
								keywrapNode->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
								keywrapNode->setLocation((yylsp[(1) - (7)]), (yylsp[(7) - (7)]));
								(yyval.m_ast) = keywrapNode;
							}
    break;

  case 106:
#line 788 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ModeStatementASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 107:
#line 795 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ResetStatementASTNode();
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]), (yylsp[(1) - (1)]));
							}
    break;

  case 108:
#line 802 "elftosb_parser.y"
    {
								MemEnableStatementASTNode * enableNode = new MemEnableStatementASTNode((yyvsp[(3) - (3)].m_ast));
								enableNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								enableNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = enableNode;
							}
    break;

  case 109:
#line 811 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kInfo, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 110:
#line 816 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kWarning, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 111:
#line 821 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kError, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 112:
#line 828 "elftosb_parser.y"
    {
								IfStatementASTNode * ifStmt = new IfStatementASTNode();
								ifStmt->setConditionExpr(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (6)].m_ast)));
								ifStmt->setIfStatements(dynamic_cast<ListASTNode*>((yyvsp[(4) - (6)].m_ast)));
								ifStmt->setElseStatements(dynamic_cast<ListASTNode*>((yyvsp[(6) - (6)].m_ast)));
								ifStmt->setLocation((yylsp[(1) - (6)]), (yylsp[(6) - (6)]));
								(yyval.m_ast) = ifStmt;
							}
    break;

  case 113:
#line 839 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(3) - (4)].m_ast);
							}
    break;

  case 114:
#line 843 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = list;
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 115:
#line 849 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; }
    break;

  case 116:
#line 853 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (1)].m_ast), NULL);
								(yyval.m_ast)->setLocation((yyvsp[(1) - (1)].m_ast));
							}
    break;

  case 117:
#line 858 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (3)].m_ast), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 118:
#line 865 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 119:
#line 869 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 120:
#line 876 "elftosb_parser.y"
    {
							(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
						}
    break;

  case 121:
#line 880 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kLessThan, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 122:
#line 887 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kGreaterThan, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 123:
#line 894 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kGreaterThanEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 124:
#line 901 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kLessThanEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 125:
#line 908 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 126:
#line 915 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kNotEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 127:
#line 922 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBooleanAnd, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 128:
#line 929 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBooleanOr, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 129:
#line 936 "elftosb_parser.y"
    {
							(yyval.m_ast) = new BooleanNotExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
							(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
						}
    break;

  case 130:
#line 941 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SourceFileFunctionASTNode((yyvsp[(1) - (4)].m_str), (yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						}
    break;

  case 131:
#line 946 "elftosb_parser.y"
    {
							(yyval.m_ast) = (yyvsp[(2) - (3)].m_ast);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						}
    break;

  case 132:
#line 951 "elftosb_parser.y"
    {
							(yyval.m_ast) = new DefinedOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						}
    break;

  case 133:
#line 957 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); }
    break;

  case 134:
#line 961 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode((yyvsp[(3) - (3)].m_str), (yyvsp[(1) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 135:
#line 966 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode((yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 136:
#line 974 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 137:
#line 978 "elftosb_parser.y"
    {
								(yyval.m_ast) = new VariableExprASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 138:
#line 983 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolRefExprASTNode(dynamic_cast<SymbolASTNode*>((yyvsp[(1) - (1)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;

  case 139:
#line 994 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kAdd, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 140:
#line 1001 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kSubtract, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 141:
#line 1008 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kMultiply, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 142:
#line 1015 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kDivide, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 143:
#line 1022 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kModulus, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 144:
#line 1029 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kPower, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 145:
#line 1036 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseAnd, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 146:
#line 1043 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseOr, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 147:
#line 1050 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseXor, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 148:
#line 1057 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kShiftLeft, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 149:
#line 1064 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kShiftRight, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 150:
#line 1071 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							}
    break;

  case 151:
#line 1075 "elftosb_parser.y"
    {
								(yyval.m_ast) = new IntSizeExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast)), (yyvsp[(3) - (3)].m_int)->getWordSize());
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 152:
#line 1080 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (3)].m_ast);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							}
    break;

  case 153:
#line 1085 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode(dynamic_cast<SymbolASTNode*>((yyvsp[(3) - (4)].m_ast)));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						}
    break;

  case 154:
#line 1090 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						}
    break;

  case 155:
#line 1095 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						}
    break;

  case 156:
#line 1102 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							}
    break;

  case 157:
#line 1106 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NegativeExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							}
    break;

  case 158:
#line 1113 "elftosb_parser.y"
    {
								(yyval.m_ast) = new IntConstExprASTNode((yyvsp[(1) - (1)].m_int)->getValue(), (yyvsp[(1) - (1)].m_int)->getWordSize());
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							}
    break;


/* Line 1267 of yacc.c.  */
#line 3187 "elftosb_parser.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, lexer, resultAST, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, lexer, resultAST, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, lexer, resultAST, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, lexer, resultAST);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, lexer, resultAST);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, lexer, resultAST, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, lexer, resultAST);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, lexer, resultAST);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1119 "elftosb_parser.y"


/* code goes here */

static int yylex(YYSTYPE * lvalp, YYLTYPE * yylloc, ElftosbLexer * lexer)
{
	int token = lexer->yylex();
	*yylloc = lexer->getLocation();
	lexer->getSymbolValue(lvalp);
	return token;
}

static void yyerror(YYLTYPE * yylloc, ElftosbLexer * lexer, CommandFileASTNode ** resultAST, const char * error)
{
	throw syntax_error(format_string("line %d: %s\n", yylloc->m_firstLine, error));
}


