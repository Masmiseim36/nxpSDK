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
     TOK_LOADH = 306,
     TOK_LOADS = 307,
     TOK_VERSION_CHECK = 308,
     TOK_SEC = 309,
     TOK_NSEC = 310,
     TOK_KEYSTORE_TO_NV = 311,
     TOK_KEYSTORE_FROM_NV = 312,
     UNARY_OP = 313
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
#define TOK_LOADH 306
#define TOK_LOADS 307
#define TOK_VERSION_CHECK 308
#define TOK_SEC 309
#define TOK_NSEC 310
#define TOK_KEYSTORE_TO_NV 311
#define TOK_KEYSTORE_FROM_NV 312
#define UNARY_OP 313




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
#line 263 "elftosb_parser.tab.c"
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
#line 288 "elftosb_parser.tab.c"

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
#define YYLAST   646

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  83
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  169
/* YYNRULES -- Number of states.  */
#define YYNSTATES  312

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   313

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,     2,     2,    81,    23,     2,
       9,    10,    79,    77,    16,    78,    20,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    17,
      25,    15,    19,     2,    27,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    13,     2,    14,    76,     2,     2,     2,     2,     2,
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
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    82
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
     166,   168,   170,   172,   174,   176,   178,   180,   182,   186,
     190,   195,   197,   200,   201,   203,   205,   207,   209,   213,
     218,   220,   222,   224,   228,   230,   233,   236,   237,   239,
     241,   246,   248,   249,   253,   258,   263,   265,   267,   271,
     273,   275,   277,   279,   283,   286,   287,   291,   295,   299,
     301,   304,   305,   311,   319,   327,   330,   332,   336,   340,
     344,   347,   350,   353,   360,   365,   368,   369,   371,   375,
     377,   379,   381,   385,   389,   393,   397,   401,   405,   409,
     413,   416,   421,   425,   430,   432,   436,   439,   441,   443,
     445,   449,   453,   457,   461,   465,   469,   473,   477,   481,
     485,   489,   491,   495,   499,   504,   509,   514,   517,   520
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      84,     0,    -1,    85,   103,    -1,    86,    -1,    85,    86,
      -1,    87,    -1,    88,    -1,    96,    -1,    92,    -1,    38,
      11,    89,    12,    -1,    39,    11,    89,    12,    -1,    90,
      -1,    89,    90,    -1,    91,    17,    -1,    -1,     3,    15,
     144,    -1,    66,     9,   146,    10,    11,    93,    12,    -1,
      94,    -1,    93,    94,    -1,    95,    -1,    -1,     9,   123,
      10,    -1,    40,    11,    97,    12,    -1,    98,    -1,    97,
      98,    -1,    99,   100,    17,    -1,    -1,     3,    15,     4,
      -1,     3,    15,    43,     9,   146,    10,    -1,     9,   101,
      10,    -1,    -1,   102,    -1,   101,    16,   102,    -1,     3,
      15,   144,    -1,   104,    -1,   103,   104,    -1,    42,     9,
     146,   105,    10,   107,    -1,    17,   106,    -1,    -1,   101,
      -1,    -1,    31,   117,    17,    -1,    11,   108,    12,    -1,
     109,    -1,   108,   109,    -1,   112,    17,    -1,   133,    -1,
     134,    -1,   135,    -1,   141,    -1,    -1,   111,    -1,   110,
     111,    -1,   112,    17,    -1,   141,    -1,    -1,   115,    -1,
     113,    -1,   114,    -1,   124,    -1,   131,    -1,   136,    -1,
     140,    -1,   137,    -1,   125,    -1,   138,    -1,   139,    -1,
     127,    -1,    69,   117,   120,    -1,    70,   117,   120,    -1,
      46,   116,   117,   120,    -1,     3,    -1,    27,   146,    -1,
      -1,   146,    -1,     4,    -1,     7,    -1,   118,    -1,   118,
      44,     7,    -1,     7,    13,   118,    14,    -1,     8,    -1,
     122,    -1,   119,    -1,   118,    16,   119,    -1,     6,    -1,
      22,     6,    -1,    19,   121,    -1,    -1,    20,    -1,   143,
      -1,    61,     9,   123,    10,    -1,   101,    -1,    -1,   126,
     129,   130,    -1,    60,   126,   143,   130,    -1,    64,   146,
     129,   130,    -1,    48,    -1,    47,    -1,    71,   128,   146,
      -1,    72,    -1,    73,    -1,     7,    -1,   146,    -1,     9,
     146,    10,    -1,     9,    10,    -1,    -1,    50,   132,   143,
      -1,    50,   132,    51,    -1,    50,    62,    51,    -1,     3,
      -1,    27,   146,    -1,    -1,    44,     7,    11,   110,    12,
      -1,    67,     9,   146,    10,    11,   110,    12,    -1,    68,
       9,   146,    10,    11,   110,    12,    -1,    49,   146,    -1,
      63,    -1,    65,   132,   143,    -1,    74,   132,   143,    -1,
      75,   132,   143,    -1,    55,     4,    -1,    56,     4,    -1,
      57,     4,    -1,    52,   145,    11,   108,    12,   142,    -1,
      53,    11,   108,    12,    -1,    53,   141,    -1,    -1,   146,
      -1,   146,    21,   146,    -1,   145,    -1,     4,    -1,   146,
      -1,   145,    25,   145,    -1,   145,    19,   145,    -1,   145,
      30,   145,    -1,   145,    31,   145,    -1,   145,    32,   145,
      -1,   145,    33,   145,    -1,   145,    28,   145,    -1,   145,
      29,   145,    -1,    26,   145,    -1,     3,     9,     7,    10,
      -1,     9,   145,    10,    -1,    54,     9,     3,    10,    -1,
     148,    -1,     7,    18,     3,    -1,    18,     3,    -1,   150,
      -1,     3,    -1,   147,    -1,   148,    77,   148,    -1,   148,
      78,   148,    -1,   148,    79,   148,    -1,   148,    80,   148,
      -1,   148,    81,   148,    -1,   148,    34,   148,    -1,   148,
      23,   148,    -1,   148,    24,   148,    -1,   148,    76,   148,
      -1,   148,    35,   148,    -1,   148,    36,   148,    -1,   149,
      -1,   148,    20,    37,    -1,     9,   148,    10,    -1,    58,
       9,   147,    10,    -1,    58,     9,     3,    10,    -1,    58,
       9,     7,    10,    -1,    77,   148,    -1,    78,   148,    -1,
       5,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   184,   184,   194,   200,   208,   209,   210,   211,   214,
     220,   226,   232,   239,   240,   243,   250,   258,   264,   271,
     272,   275,   287,   293,   299,   307,   319,   322,   327,   335,
     336,   340,   346,   354,   361,   367,   374,   389,   394,   400,
     405,   411,   417,   425,   431,   439,   440,   441,   442,   443,
     444,   447,   453,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   480,   492,
     504,   524,   529,   533,   536,   540,   545,   550,   555,   560,
     565,   570,   575,   581,   589,   594,   601,   606,   612,   617,
     623,   635,   636,   639,   668,   705,   725,   726,   729,   750,
     751,   754,   759,   766,   767,   768,   771,   785,   800,   809,
     814,   818,   822,   829,   838,   847,   854,   861,   870,   877,
     886,   891,   896,   903,   914,   918,   925,   928,   933,   940,
     944,   951,   955,   962,   969,   976,   983,   990,   997,  1004,
    1011,  1016,  1021,  1026,  1033,  1036,  1041,  1049,  1053,  1058,
    1069,  1076,  1083,  1090,  1097,  1104,  1111,  1118,  1125,  1132,
    1139,  1146,  1150,  1155,  1160,  1165,  1170,  1177,  1181,  1188
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
  "\"enable\"", "\"keyblob\"", "\"encrypt\"", "\"keywrap\"", "\"loadh\"",
  "\"loads\"", "\"version_check\"", "\"sec\"", "\"nsec\"",
  "\"keystore_to_nv\"", "\"keystore_from_nv\"", "'^'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "UNARY_OP", "$accept", "command_file", "blocks_list",
  "pre_section_block", "options_block", "constants_block",
  "const_def_list", "const_def_list_elem", "const_def", "keyblob_block",
  "keyblob_def_list", "keyblob_def_list_elem", "keyblob_def",
  "sources_block", "source_def_list", "source_def_list_elem", "source_def",
  "source_attrs_opt", "source_attr_list", "source_attr_list_elem",
  "section_defs", "section_def", "section_options_opt",
  "source_attr_list_opt", "section_contents", "full_stmt_list",
  "full_stmt_list_elem", "basic_stmt_list", "basic_stmt_list_elem",
  "basic_stmt", "loadh_stmt", "loads_stmt", "load_stmt", "load_opt",
  "load_data", "section_list", "section_list_elem", "load_target_opt",
  "load_target", "ivt_def", "assignment_list_opt", "call_stmt",
  "jump_sp_stmt", "call_or_jump", "ver_check_stmt", "sec_or_nsec",
  "call_target", "call_arg_opt", "erase_stmt", "mem_opt", "from_stmt",
  "encrypt_stmt", "keywrap_stmt", "mode_stmt", "reset_stmt", "enable_stmt",
  "keystore_stmt", "message_stmt", "if_stmt", "else_opt",
  "address_or_range", "const_expr", "bool_expr", "int_const_expr",
  "symbol_ref", "expr", "unary_expr", "int_value", 0
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
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,    94,    43,    45,    42,
      47,    37,   313
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    83,    84,    85,    85,    86,    86,    86,    86,    87,
      88,    89,    89,    90,    90,    91,    92,    93,    93,    94,
      94,    95,    96,    97,    97,    98,    98,    99,    99,   100,
     100,   101,   101,   102,   103,   103,   104,   105,   105,   106,
     106,   107,   107,   108,   108,   109,   109,   109,   109,   109,
     109,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   113,   114,
     115,   116,   116,   116,   117,   117,   117,   117,   117,   117,
     117,   117,   118,   118,   119,   119,   120,   120,   121,   121,
     122,   123,   123,   124,   124,   125,   126,   126,   127,   128,
     128,   129,   129,   130,   130,   130,   131,   131,   131,   132,
     132,   132,   133,   134,   135,   136,   137,   138,   139,   139,
     140,   140,   140,   141,   142,   142,   142,   143,   143,   144,
     144,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   146,   147,   147,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   149,   149,   150
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
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       4,     1,     2,     0,     1,     1,     1,     1,     3,     4,
       1,     1,     1,     3,     1,     2,     2,     0,     1,     1,
       4,     1,     0,     3,     4,     4,     1,     1,     3,     1,
       1,     1,     1,     3,     2,     0,     3,     3,     3,     1,
       2,     0,     5,     7,     7,     2,     1,     3,     3,     3,
       2,     2,     2,     6,     4,     2,     0,     1,     3,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     4,     3,     4,     1,     3,     2,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     4,     4,     4,     2,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     3,     5,     6,
       8,     7,    14,    14,    26,     0,     1,     0,     4,     2,
      34,     0,     0,    11,     0,     0,     0,     0,    23,    30,
     148,   169,     0,     0,     0,     0,     0,     0,     0,   149,
     144,   161,   147,     0,    35,     0,     9,    12,    13,    10,
       0,    22,    24,     0,     0,     0,     0,   146,     0,   167,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,   148,   130,     0,     0,     0,
      15,   129,   131,    27,     0,     0,     0,    31,    25,   145,
     163,     0,     0,     0,    20,   162,   156,   157,   155,   159,
     160,   158,   150,   151,   152,   153,   154,    40,     0,     0,
       0,   144,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   165,   166,   164,    92,
       0,    17,    19,    39,    37,     0,     0,   142,     0,   133,
     132,   138,   139,   134,   135,   136,   137,     0,    33,    32,
      91,     0,    16,    18,    50,     0,    36,   141,   143,    28,
      21,     0,    73,    97,    96,     0,   111,     0,     0,     0,
       0,     0,   116,     0,   111,     0,     0,     0,     0,     0,
     111,   111,     0,    43,     0,    57,    58,    56,    59,    64,
       0,    67,    60,    46,    47,    48,    61,    63,    65,    66,
      62,    49,    75,    84,    76,    80,     0,     0,     0,    77,
      82,    81,    74,     0,    71,     0,     0,   115,   109,     0,
       0,     0,     0,   120,   121,   122,     0,     0,     0,     0,
       0,    87,    87,    99,   100,     0,     0,     0,    42,    44,
      45,   101,   105,   102,     0,    85,    92,    41,     0,     0,
      55,    72,    87,   110,   108,   107,   106,   127,    50,   105,
     105,   117,     0,     0,     0,    68,    69,    98,   118,   119,
       0,    93,     0,     0,    83,    78,     0,    51,     0,    54,
      70,     0,     0,    94,    95,     0,     0,    88,    86,    89,
     104,     0,    79,    90,   112,    52,    53,   128,   126,    55,
      55,   103,     0,   123,     0,     0,    50,   125,   113,   114,
       0,   124
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    22,    23,    24,    10,
     130,   131,   132,    11,    27,    28,    29,    54,   150,    87,
      19,    20,   108,   134,   156,   182,   183,   276,   277,   184,
     185,   186,   187,   216,   208,   209,   210,   265,   288,   211,
     151,   188,   189,   190,   191,   235,   242,   271,   192,   221,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   303,
     256,    80,    81,    82,    39,    40,    41,    42
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -240
static const yytype_int16 yypact[] =
{
     169,    15,    62,   114,    13,    33,   117,  -240,  -240,  -240,
    -240,  -240,   144,   144,   158,   113,  -240,   160,  -240,   125,
    -240,   164,    22,  -240,   172,   161,   177,   220,  -240,   201,
    -240,  -240,   213,   113,   231,   234,   113,   113,   238,  -240,
     118,  -240,  -240,   113,  -240,     9,  -240,  -240,  -240,  -240,
      53,  -240,  -240,   248,   237,   254,   206,  -240,   142,  -240,
    -240,   247,   222,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   250,   256,  -240,    46,    46,   259,
    -240,   472,  -240,  -240,   260,   255,    92,  -240,  -240,  -240,
    -240,   261,    11,   262,   266,  -240,   183,   454,    55,     4,
       4,   459,    30,    30,   257,   257,   257,   248,   270,   274,
     575,   206,  -240,   292,    46,    46,    46,    46,    46,    46,
      46,    46,   113,     9,  -240,   248,  -240,  -240,  -240,   248,
     139,  -240,  -240,   281,  -240,    25,   288,  -240,   293,  -240,
    -240,   613,   613,  -240,  -240,  -240,  -240,   295,  -240,  -240,
     281,   296,  -240,  -240,   497,     1,  -240,  -240,  -240,  -240,
    -240,   303,    44,  -240,  -240,   113,    27,    46,   298,   312,
     313,   189,  -240,   113,   100,   311,   319,     1,     1,   155,
     100,   100,   244,  -240,   314,  -240,  -240,  -240,  -240,  -240,
     175,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,
    -240,  -240,  -240,  -240,   168,  -240,   324,   326,   321,    36,
    -240,  -240,  -240,   325,  -240,   113,     1,  -240,  -240,   113,
     299,    88,   598,  -240,  -240,  -240,   113,   175,   113,   113,
     113,   320,   320,  -240,  -240,   113,   113,   113,  -240,  -240,
    -240,   213,   334,  -240,    99,  -240,   248,  -240,    99,   342,
     527,  -240,   320,  -240,  -240,  -240,  -240,   340,   497,   334,
     334,  -240,   345,   353,   110,  -240,  -240,  -240,  -240,  -240,
     167,  -240,    98,   354,  -240,  -240,   341,  -240,   351,  -240,
    -240,   113,   277,  -240,  -240,   358,   360,  -240,  -240,  -240,
    -240,   362,  -240,  -240,  -240,  -240,  -240,  -240,   323,   527,
     527,  -240,     6,  -240,   371,   401,   497,  -240,  -240,  -240,
     310,  -240
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -240,  -240,  -240,   376,  -240,  -240,   373,   180,  -240,  -240,
    -240,   264,  -240,  -240,  -240,   365,  -240,  -240,   -38,   275,
    -240,   380,  -240,  -240,  -240,  -238,  -181,   -61,  -228,  -239,
    -240,  -240,  -240,  -240,  -146,   151,   154,  -158,  -240,  -240,
     157,  -240,  -240,   236,  -240,  -240,   181,   -13,  -240,   -37,
    -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,   -26,  -240,
    -138,   286,   -75,   -15,   356,   447,  -240,  -240
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      38,   239,   110,   112,    30,   202,    31,   203,   204,   205,
      33,   278,    75,    76,    31,    86,    32,   306,    77,    34,
     282,   127,    15,   206,    62,    21,    12,    34,    74,    55,
     218,   231,   232,    16,    46,    78,   154,   278,    65,   139,
     140,   141,   142,   143,   144,   145,   146,   214,   295,    75,
      62,    31,   248,    32,   219,    77,   155,    83,   167,    35,
     278,   278,   207,    79,    34,   278,   278,    35,   310,   133,
     252,   215,    78,    13,   266,    62,   295,   295,    36,    37,
     249,    69,    70,    71,    72,    73,    36,    37,   259,   220,
     261,    30,   222,    31,   280,    32,    84,    33,   268,   269,
      79,   239,   124,   218,    35,   203,    34,   147,   125,    71,
      72,    73,   292,    30,   248,    31,    30,    32,    31,    33,
      32,   206,    33,    36,    37,    14,   289,   219,    34,   239,
     287,    34,    69,    70,    71,    72,    73,   228,    62,   255,
     212,    63,    64,   236,   237,    91,    35,    21,   129,    92,
     217,   152,    65,    66,    67,     1,     2,     3,   227,    17,
      34,    26,   212,   212,    21,    36,    37,    17,    35,    43,
      30,    35,    31,    49,    32,   243,    33,   290,    30,    45,
      31,   244,   241,     4,    33,    34,    55,    36,    37,    48,
      36,    37,    50,    34,    68,    69,    70,    71,    72,    73,
     251,   212,    47,    62,   253,    47,   257,     1,     2,     3,
      53,   257,   243,   257,   262,   263,    90,    65,    66,    67,
     267,   257,   257,    26,   279,    35,    62,   233,   234,    63,
      64,    55,    51,    35,    57,     4,   163,   164,   304,   305,
      65,    66,    67,    58,    36,    37,   283,   284,    61,   257,
     279,    85,    36,    37,    88,   291,   238,    89,    94,    95,
      69,    70,    71,    72,    73,   109,   297,   107,   113,   122,
     123,   126,   128,   279,   279,   129,   307,    62,   279,   279,
     135,   136,    68,    69,    70,    71,    72,    73,   161,   298,
     162,   163,   164,   165,   166,   138,   167,   125,   157,   168,
     169,   170,   223,   158,   171,   159,   160,   172,   173,   174,
     213,   175,   176,   177,   178,   179,   224,   225,   180,   181,
     229,   161,   311,   162,   163,   164,   165,   166,   230,   167,
     245,   240,   168,   169,   170,   246,   250,   171,   247,   264,
     172,   173,   174,   270,   175,   176,   177,   178,   179,   275,
     254,   180,   181,   294,   161,   285,   162,   163,   164,   165,
     166,   281,   167,   286,   293,   168,   169,   170,   296,   299,
     171,   300,   301,   172,   173,   174,   302,   175,   176,   177,
     178,   179,    18,   308,   180,   181,    25,   162,   163,   164,
     165,   166,    52,   167,   153,   272,   168,   169,   170,    44,
     149,   171,   274,   273,   172,   173,   174,   226,   260,   148,
     177,   178,   179,   309,    93,   180,   181,   162,   163,   164,
     165,   166,     0,   167,     0,     0,   168,   169,   170,     0,
       0,   171,     0,     0,   172,   173,   174,     0,     0,     0,
     177,   178,   179,     0,     0,   180,   181,   162,   163,   164,
     165,   166,     0,   167,     0,     0,   168,   169,   170,     0,
       0,   171,     0,     0,   172,   173,   174,     0,     0,     0,
     177,   178,   179,     0,    62,   180,   181,    63,     0,    62,
      56,     0,    63,    59,    60,     0,     0,     0,    65,    66,
      67,   114,     0,    65,    66,    67,     0,   115,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,     0,     0,     0,   111,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,    73,    69,    70,    71,    72,
      73,   161,     0,   162,   163,   164,   165,   166,     0,   167,
       0,     0,   168,   169,   170,     0,     0,   171,     0,     0,
     172,   173,   174,     0,   175,   176,   177,   178,   179,     0,
       0,   180,   181,   162,   163,   164,   165,   166,     0,   167,
       0,     0,   168,   169,   170,   137,     0,   171,     0,     0,
     172,   173,   174,     0,   114,     0,   177,   178,   179,     0,
     115,   180,   181,   116,   117,   118,   119,   120,   121,   258,
       0,     0,     0,     0,     0,     0,     0,   114,     0,     0,
       0,     0,     0,   115,     0,     0,   116,   117,   118,   119,
     120,   121,   114,     0,     0,     0,     0,     0,   115,     0,
       0,     0,     0,   118,   119,   120,   121
};

static const yytype_int16 yycheck[] =
{
      15,   182,    77,    78,     3,     4,     5,     6,     7,     8,
       9,   250,     3,     4,     5,    53,     7,    11,     9,    18,
     258,    10,     9,    22,    20,     3,    11,    18,    43,    18,
       3,   177,   178,     0,    12,    26,    11,   276,    34,   114,
     115,   116,   117,   118,   119,   120,   121,     3,   276,     3,
      20,     5,    16,     7,    27,     9,    31,     4,    52,    58,
     299,   300,    61,    54,    18,   304,   305,    58,   306,   107,
     216,    27,    26,    11,   232,    20,   304,   305,    77,    78,
      44,    77,    78,    79,    80,    81,    77,    78,   226,    62,
     228,     3,   167,     5,   252,     7,    43,     9,   236,   237,
      54,   282,    10,     3,    58,     6,    18,   122,    16,    79,
      80,    81,    14,     3,    16,     5,     3,     7,     5,     9,
       7,    22,     9,    77,    78,    11,   264,    27,    18,   310,
      20,    18,    77,    78,    79,    80,    81,   174,    20,    51,
     155,    23,    24,   180,   181,     3,    58,     3,     9,     7,
     165,    12,    34,    35,    36,    38,    39,    40,   173,    42,
      18,     3,   177,   178,     3,    77,    78,    42,    58,     9,
       3,    58,     5,    12,     7,   190,     9,    10,     3,    15,
       5,    13,     7,    66,     9,    18,    18,    77,    78,    17,
      77,    78,    15,    18,    76,    77,    78,    79,    80,    81,
     215,   216,    22,    20,   219,    25,   221,    38,    39,    40,
       9,   226,   227,   228,   229,   230,    10,    34,    35,    36,
     235,   236,   237,     3,   250,    58,    20,    72,    73,    23,
      24,    18,    12,    58,     3,    66,    47,    48,   299,   300,
      34,    35,    36,     9,    77,    78,   259,   260,    10,   264,
     276,     3,    77,    78,    17,   270,    12,     3,    11,    37,
      77,    78,    79,    80,    81,     9,   281,    17,     9,     9,
      15,    10,    10,   299,   300,     9,   302,    20,   304,   305,
      10,     7,    76,    77,    78,    79,    80,    81,    44,    12,
      46,    47,    48,    49,    50,     3,    52,    16,    10,    55,
      56,    57,     4,    10,    60,    10,    10,    63,    64,    65,
       7,    67,    68,    69,    70,    71,     4,     4,    74,    75,
       9,    44,    12,    46,    47,    48,    49,    50,     9,    52,
       6,    17,    55,    56,    57,     9,    11,    60,    17,    19,
      63,    64,    65,     9,    67,    68,    69,    70,    71,     7,
      51,    74,    75,    12,    44,    10,    46,    47,    48,    49,
      50,    21,    52,    10,    10,    55,    56,    57,    17,    11,
      60,    11,    10,    63,    64,    65,    53,    67,    68,    69,
      70,    71,     6,    12,    74,    75,    13,    46,    47,    48,
      49,    50,    27,    52,   130,   244,    55,    56,    57,    19,
     125,    60,   248,   246,    63,    64,    65,   171,   227,   123,
      69,    70,    71,    12,    58,    74,    75,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    71,    -1,    -1,    74,    75,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    71,    -1,    20,    74,    75,    23,    -1,    20,
      33,    -1,    23,    36,    37,    -1,    -1,    -1,    34,    35,
      36,    19,    -1,    34,    35,    36,    -1,    25,    -1,    -1,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      76,    77,    78,    79,    80,    81,    77,    78,    79,    80,
      81,    44,    -1,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    -1,
      -1,    74,    75,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    55,    56,    57,    10,    -1,    60,    -1,    -1,
      63,    64,    65,    -1,    19,    -1,    69,    70,    71,    -1,
      25,    74,    75,    28,    29,    30,    31,    32,    33,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    29,    30,    31,
      32,    33,    19,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    30,    31,    32,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    38,    39,    40,    66,    84,    85,    86,    87,    88,
      92,    96,    11,    11,    11,     9,     0,    42,    86,   103,
     104,     3,    89,    90,    91,    89,     3,    97,    98,    99,
       3,     5,     7,     9,    18,    58,    77,    78,   146,   147,
     148,   149,   150,     9,   104,    15,    12,    90,    17,    12,
      15,    12,    98,     9,   100,    18,   148,     3,     9,   148,
     148,    10,    20,    23,    24,    34,    35,    36,    76,    77,
      78,    79,    80,    81,   146,     3,     4,     9,    26,    54,
     144,   145,   146,     4,    43,     3,   101,   102,    17,     3,
      10,     3,     7,   147,    11,    37,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,    17,   105,     9,
     145,   148,   145,     9,    19,    25,    28,    29,    30,    31,
      32,    33,     9,    15,    10,    16,    10,    10,    10,     9,
      93,    94,    95,   101,   106,    10,     7,    10,     3,   145,
     145,   145,   145,   145,   145,   145,   145,   146,   144,   102,
     101,   123,    12,    94,    11,    31,   107,    10,    10,    10,
      10,    44,    46,    47,    48,    49,    50,    52,    55,    56,
      57,    60,    63,    64,    65,    67,    68,    69,    70,    71,
      74,    75,   108,   109,   112,   113,   114,   115,   124,   125,
     126,   127,   131,   133,   134,   135,   136,   137,   138,   139,
     140,   141,     4,     6,     7,     8,    22,    61,   117,   118,
     119,   122,   146,     7,     3,    27,   116,   146,     3,    27,
      62,   132,   145,     4,     4,     4,   126,   146,   132,     9,
       9,   117,   117,    72,    73,   128,   132,   132,    12,   109,
      17,     7,   129,   146,    13,     6,     9,    17,    16,    44,
      11,   146,   117,   146,    51,    51,   143,   146,    11,   143,
     129,   143,   146,   146,    19,   120,   120,   146,   143,   143,
       9,   130,   118,   123,   119,     7,   110,   111,   112,   141,
     120,    21,   108,   130,   130,    10,    10,    20,   121,   143,
      10,   146,    14,    10,    12,   111,    17,   146,    12,    11,
      11,    10,    53,   142,   110,   110,    11,   141,    12,    12,
     108,    12
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
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1577 "elftosb_parser.tab.c"
	break;
      case 4: /* "\"string\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1582 "elftosb_parser.tab.c"
	break;
      case 5: /* "\"integer\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_int); };
#line 1587 "elftosb_parser.tab.c"
	break;
      case 6: /* "\"section name\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1592 "elftosb_parser.tab.c"
	break;
      case 7: /* "\"source name\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_str); };
#line 1597 "elftosb_parser.tab.c"
	break;
      case 8: /* "\"binary object\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_blob); };
#line 1602 "elftosb_parser.tab.c"
	break;
      case 37: /* "\"integer size\"" */
#line 180 "elftosb_parser.y"
	{ delete (yyvaluep->m_int); };
#line 1607 "elftosb_parser.tab.c"
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
#line 185 "elftosb_parser.y"
    {
							CommandFileASTNode * commandFile = new CommandFileASTNode();
							commandFile->setBlocks(dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast)));
							commandFile->setSections(dynamic_cast<ListASTNode*>((yyvsp[(2) - (2)].m_ast)));
							commandFile->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							*resultAST = commandFile;
						;}
    break;

  case 3:
#line 195 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						;}
    break;

  case 4:
#line 201 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						;}
    break;

  case 5:
#line 208 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 6:
#line 209 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 7:
#line 210 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 8:
#line 211 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 9:
#line 215 "elftosb_parser.y"
    {
								(yyval.m_ast) = new OptionsBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
							;}
    break;

  case 10:
#line 221 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ConstantsBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
							;}
    break;

  case 11:
#line 227 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 12:
#line 233 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							;}
    break;

  case 13:
#line 239 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); ;}
    break;

  case 14:
#line 240 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 15:
#line 244 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AssignmentASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 16:
#line 251 "elftosb_parser.y"
    {
							KeyblobBlockASTNode * keyblob = new KeyblobBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(6) - (7)].m_ast)));
							keyblob->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
							(yyval.m_ast) = keyblob;
						;}
    break;

  case 17:
#line 259 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						;}
    break;

  case 18:
#line 265 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						;}
    break;

  case 19:
#line 271 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 20:
#line 272 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 21:
#line 276 "elftosb_parser.y"
    {
								KeyblobEntryASTNode * entry = new KeyblobEntryASTNode();
								if ((yyvsp[(2) - (3)].m_ast))
								{
									entry->setFieldAssignments(dynamic_cast<ListASTNode*>((yyvsp[(2) - (3)].m_ast)));
								}
								entry->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = entry;
							;}
    break;

  case 22:
#line 288 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SourcesBlockASTNode(dynamic_cast<ListASTNode *>((yyvsp[(3) - (4)].m_ast)));
						;}
    break;

  case 23:
#line 294 "elftosb_parser.y"
    {
							ListASTNode * list = new ListASTNode();
							list->appendNode((yyvsp[(1) - (1)].m_ast));
							(yyval.m_ast) = list;
						;}
    break;

  case 24:
#line 300 "elftosb_parser.y"
    {
							dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
							(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
						;}
    break;

  case 25:
#line 308 "elftosb_parser.y"
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
							;}
    break;

  case 26:
#line 319 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 27:
#line 323 "elftosb_parser.y"
    {
								(yyval.m_ast) = new PathSourceDefASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 28:
#line 328 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ExternSourceDefASTNode((yyvsp[(1) - (6)].m_str), dynamic_cast<ExprASTNode*>((yyvsp[(5) - (6)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (6)]), (yylsp[(6) - (6)]));
							;}
    break;

  case 29:
#line 335 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(2) - (3)].m_ast); ;}
    break;

  case 30:
#line 336 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 31:
#line 341 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 32:
#line 347 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast))->appendNode((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (3)].m_ast);
							;}
    break;

  case 33:
#line 355 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AssignmentASTNode((yyvsp[(1) - (3)].m_str), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 34:
#line 362 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 35:
#line 368 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							;}
    break;

  case 36:
#line 375 "elftosb_parser.y"
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
							;}
    break;

  case 37:
#line 390 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							;}
    break;

  case 38:
#line 394 "elftosb_parser.y"
    {
								(yyval.m_ast) = NULL;
							;}
    break;

  case 39:
#line 401 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 40:
#line 405 "elftosb_parser.y"
    {
								(yyval.m_ast) = NULL;
							;}
    break;

  case 41:
#line 412 "elftosb_parser.y"
    {
								DataSectionContentsASTNode * dataSection = new DataSectionContentsASTNode((yyvsp[(2) - (3)].m_ast));
								dataSection->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = dataSection;
							;}
    break;

  case 42:
#line 418 "elftosb_parser.y"
    {
								ListASTNode * listNode = dynamic_cast<ListASTNode*>((yyvsp[(2) - (3)].m_ast));
								(yyval.m_ast) = new BootableSectionContentsASTNode(listNode);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 43:
#line 426 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 44:
#line 432 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							;}
    break;

  case 45:
#line 439 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); ;}
    break;

  case 46:
#line 440 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 47:
#line 441 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 48:
#line 442 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 49:
#line 443 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 50:
#line 444 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 51:
#line 448 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 52:
#line 454 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (2)].m_ast))->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (2)].m_ast);
							;}
    break;

  case 53:
#line 461 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (2)].m_ast); ;}
    break;

  case 54:
#line 462 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 55:
#line 463 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 56:
#line 466 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 57:
#line 467 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 58:
#line 468 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 59:
#line 469 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 60:
#line 470 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 61:
#line 471 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 62:
#line 472 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 63:
#line 473 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 64:
#line 474 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 65:
#line 475 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 66:
#line 476 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 67:
#line 477 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 68:
#line 481 "elftosb_parser.y"
    {
								LoadStatementASTNode * stmt = new LoadStatementASTNode();
								stmt->setLoadType(LoadStatementASTNode::LoadType_t::loadHash);
								stmt->setData((yyvsp[(2) - (3)].m_ast));
								stmt->setTarget((yyvsp[(3) - (3)].m_ast));
								// set char locations for the statement
								stmt->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = stmt;
							;}
    break;

  case 69:
#line 493 "elftosb_parser.y"
    {
								LoadStatementASTNode * stmt = new LoadStatementASTNode();
								stmt->setLoadType(LoadStatementASTNode::LoadType_t::loadSecret);
								stmt->setData((yyvsp[(2) - (3)].m_ast));
								stmt->setTarget((yyvsp[(3) - (3)].m_ast));
								// set char locations for the statement
								stmt->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = stmt;
							;}
    break;

  case 70:
#line 505 "elftosb_parser.y"
    {
								LoadStatementASTNode * stmt = new LoadStatementASTNode();
								stmt->setLoadType(LoadStatementASTNode::LoadType_t::load);
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
							;}
    break;

  case 71:
#line 525 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 72:
#line 530 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							;}
    break;

  case 73:
#line 533 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 74:
#line 537 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 75:
#line 541 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 76:
#line 546 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SourceASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 77:
#line 551 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(1) - (1)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 78:
#line 556 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast)), (yyvsp[(3) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 79:
#line 561 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionMatchListASTNode(dynamic_cast<ListASTNode*>((yyvsp[(3) - (4)].m_ast)), (yyvsp[(1) - (4)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
							;}
    break;

  case 80:
#line 566 "elftosb_parser.y"
    {
								(yyval.m_ast) = new BlobConstASTNode((yyvsp[(1) - (1)].m_blob));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 81:
#line 571 "elftosb_parser.y"
    {
							;}
    break;

  case 82:
#line 576 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(1) - (1)].m_ast));
								(yyval.m_ast) = list;
							;}
    break;

  case 83:
#line 582 "elftosb_parser.y"
    {
								dynamic_cast<ListASTNode*>((yyvsp[(1) - (3)].m_ast))->appendNode((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = (yyvsp[(1) - (3)].m_ast);
							;}
    break;

  case 84:
#line 590 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionASTNode((yyvsp[(1) - (1)].m_str), SectionASTNode::kInclude);
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 85:
#line 595 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SectionASTNode((yyvsp[(2) - (2)].m_str), SectionASTNode::kExclude);
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 86:
#line 602 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							;}
    break;

  case 87:
#line 606 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NaturalLocationASTNode();
//								$$->setLocation();
							;}
    break;

  case 88:
#line 613 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NaturalLocationASTNode();
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 89:
#line 618 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 90:
#line 624 "elftosb_parser.y"
    {
								IVTConstASTNode * ivt = new IVTConstASTNode();
								if ((yyvsp[(3) - (4)].m_ast))
								{
									ivt->setFieldAssignments(dynamic_cast<ListASTNode*>((yyvsp[(3) - (4)].m_ast)));
								}
								ivt->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
								(yyval.m_ast) = ivt;
							;}
    break;

  case 91:
#line 635 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 92:
#line 636 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 93:
#line 640 "elftosb_parser.y"
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
							;}
    break;

  case 94:
#line 669 "elftosb_parser.y"
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
							;}
    break;

  case 95:
#line 706 "elftosb_parser.y"
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
						;}
    break;

  case 96:
#line 725 "elftosb_parser.y"
    { (yyval.m_num) = 1; ;}
    break;

  case 97:
#line 726 "elftosb_parser.y"
    { (yyval.m_num) = 2; ;}
    break;

  case 98:
#line 730 "elftosb_parser.y"
    {
							auto * stmt = new CheckVersionStatementASTNode();
							switch ((yyvsp[(2) - (3)].m_num))
							{
								case 1:
									stmt->setVersionType(CheckVersionStatementASTNode::CheckVersionType::SecureVersion);
									break;
								case 2:
									stmt->setVersionType(CheckVersionStatementASTNode::CheckVersionType::NonSecureVersion);
									break;
								default:
									yyerror(&yylloc, lexer, resultAST, "invalid sec_or_nsec value");
									YYABORT;
									break;
							}
							stmt->setVersion((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = stmt;
						;}
    break;

  case 99:
#line 750 "elftosb_parser.y"
    { (yyval.m_num) = 1; ;}
    break;

  case 100:
#line 751 "elftosb_parser.y"
    { (yyval.m_num) = 2; ;}
    break;

  case 101:
#line 755 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode(NULL, (yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 102:
#line 760 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (1)].m_ast), NULL);
								(yyval.m_ast)->setLocation((yyvsp[(1) - (1)].m_ast));
							;}
    break;

  case 103:
#line 766 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(2) - (3)].m_ast); ;}
    break;

  case 104:
#line 767 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 105:
#line 768 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 106:
#line 772 "elftosb_parser.y"
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
							;}
    break;

  case 107:
#line 786 "elftosb_parser.y"
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
							;}
    break;

  case 108:
#line 801 "elftosb_parser.y"
    {
								EraseStatementASTNode * eraseNode = new EraseStatementASTNode();
								eraseNode->setEraseAllUnsecure(true);
								eraseNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = eraseNode;
							;}
    break;

  case 109:
#line 810 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 110:
#line 815 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							;}
    break;

  case 111:
#line 818 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 112:
#line 823 "elftosb_parser.y"
    {
								(yyval.m_ast) = new FromStatementASTNode((yyvsp[(2) - (5)].m_str), dynamic_cast<ListASTNode*>((yyvsp[(4) - (5)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (5)]), (yylsp[(5) - (5)]));
							;}
    break;

  case 113:
#line 830 "elftosb_parser.y"
    {
								EncryptStatementASTNode * encryptNode = new EncryptStatementASTNode(dynamic_cast<ListASTNode*>((yyvsp[(6) - (7)].m_ast)));
								encryptNode->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
								encryptNode->setLocation((yylsp[(1) - (7)]), (yylsp[(7) - (7)]));
								(yyval.m_ast) = encryptNode;
							;}
    break;

  case 114:
#line 839 "elftosb_parser.y"
    {
								KeywrapStatementASTNode * keywrapNode = new KeywrapStatementASTNode(dynamic_cast<ListASTNode*>((yyvsp[(6) - (7)].m_ast)));
								keywrapNode->setKeyblobNumberExpr(dynamic_cast<ExprASTNode *>((yyvsp[(3) - (7)].m_ast)));
								keywrapNode->setLocation((yylsp[(1) - (7)]), (yylsp[(7) - (7)]));
								(yyval.m_ast) = keywrapNode;
							;}
    break;

  case 115:
#line 848 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ModeStatementASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 116:
#line 855 "elftosb_parser.y"
    {
								(yyval.m_ast) = new ResetStatementASTNode();
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]), (yylsp[(1) - (1)]));
							;}
    break;

  case 117:
#line 862 "elftosb_parser.y"
    {
								MemEnableStatementASTNode * enableNode = new MemEnableStatementASTNode((yyvsp[(3) - (3)].m_ast));
								enableNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								enableNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = enableNode;
							;}
    break;

  case 118:
#line 871 "elftosb_parser.y"
    {
								KeyStoreToNvStatementASTNode * enableNode = new KeyStoreToNvStatementASTNode((yyvsp[(3) - (3)].m_ast));
								enableNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								enableNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = enableNode;
							;}
    break;

  case 119:
#line 878 "elftosb_parser.y"
    {
								KeyStoreFromNvStatementASTNode * enableNode = new KeyStoreFromNvStatementASTNode((yyvsp[(3) - (3)].m_ast));
								enableNode->setMemOption((yyvsp[(2) - (3)].m_ast));
								enableNode->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
								(yyval.m_ast) = enableNode;
							;}
    break;

  case 120:
#line 887 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kInfo, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 121:
#line 892 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kWarning, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 122:
#line 897 "elftosb_parser.y"
    {
								(yyval.m_ast) = new MessageStatementASTNode(MessageStatementASTNode::kError, (yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 123:
#line 904 "elftosb_parser.y"
    {
								IfStatementASTNode * ifStmt = new IfStatementASTNode();
								ifStmt->setConditionExpr(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (6)].m_ast)));
								ifStmt->setIfStatements(dynamic_cast<ListASTNode*>((yyvsp[(4) - (6)].m_ast)));
								ifStmt->setElseStatements(dynamic_cast<ListASTNode*>((yyvsp[(6) - (6)].m_ast)));
								ifStmt->setLocation((yylsp[(1) - (6)]), (yylsp[(6) - (6)]));
								(yyval.m_ast) = ifStmt;
							;}
    break;

  case 124:
#line 915 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(3) - (4)].m_ast);
							;}
    break;

  case 125:
#line 919 "elftosb_parser.y"
    {
								ListASTNode * list = new ListASTNode();
								list->appendNode((yyvsp[(2) - (2)].m_ast));
								(yyval.m_ast) = list;
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 126:
#line 925 "elftosb_parser.y"
    { (yyval.m_ast) = NULL; ;}
    break;

  case 127:
#line 929 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (1)].m_ast), NULL);
								(yyval.m_ast)->setLocation((yyvsp[(1) - (1)].m_ast));
							;}
    break;

  case 128:
#line 934 "elftosb_parser.y"
    {
								(yyval.m_ast) = new AddressRangeASTNode((yyvsp[(1) - (3)].m_ast), (yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 129:
#line 941 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 130:
#line 945 "elftosb_parser.y"
    {
								(yyval.m_ast) = new StringConstASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 131:
#line 952 "elftosb_parser.y"
    {
							(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
						;}
    break;

  case 132:
#line 956 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kLessThan, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 133:
#line 963 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kGreaterThan, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 134:
#line 970 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kGreaterThanEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 135:
#line 977 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kLessThanEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 136:
#line 984 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 137:
#line 991 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kNotEqual, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 138:
#line 998 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBooleanAnd, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 139:
#line 1005 "elftosb_parser.y"
    {
							ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
							ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
							(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBooleanOr, right);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 140:
#line 1012 "elftosb_parser.y"
    {
							(yyval.m_ast) = new BooleanNotExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
							(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
						;}
    break;

  case 141:
#line 1017 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SourceFileFunctionASTNode((yyvsp[(1) - (4)].m_str), (yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						;}
    break;

  case 142:
#line 1022 "elftosb_parser.y"
    {
							(yyval.m_ast) = (yyvsp[(2) - (3)].m_ast);
							(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
						;}
    break;

  case 143:
#line 1027 "elftosb_parser.y"
    {
							(yyval.m_ast) = new DefinedOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						;}
    break;

  case 144:
#line 1033 "elftosb_parser.y"
    { (yyval.m_ast) = (yyvsp[(1) - (1)].m_ast); ;}
    break;

  case 145:
#line 1037 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode((yyvsp[(3) - (3)].m_str), (yyvsp[(1) - (3)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 146:
#line 1042 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolASTNode((yyvsp[(2) - (2)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 147:
#line 1050 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 148:
#line 1054 "elftosb_parser.y"
    {
								(yyval.m_ast) = new VariableExprASTNode((yyvsp[(1) - (1)].m_str));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 149:
#line 1059 "elftosb_parser.y"
    {
								(yyval.m_ast) = new SymbolRefExprASTNode(dynamic_cast<SymbolASTNode*>((yyvsp[(1) - (1)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;

  case 150:
#line 1070 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kAdd, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 151:
#line 1077 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kSubtract, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 152:
#line 1084 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kMultiply, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 153:
#line 1091 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kDivide, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 154:
#line 1098 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kModulus, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 155:
#line 1105 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kPower, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 156:
#line 1112 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseAnd, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 157:
#line 1119 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseOr, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 158:
#line 1126 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kBitwiseXor, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 159:
#line 1133 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kShiftLeft, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 160:
#line 1140 "elftosb_parser.y"
    {
								ExprASTNode * left = dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast));
								ExprASTNode * right = dynamic_cast<ExprASTNode*>((yyvsp[(3) - (3)].m_ast));
								(yyval.m_ast) = new BinaryOpExprASTNode(left, BinaryOpExprASTNode::kShiftRight, right);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 161:
#line 1147 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(1) - (1)].m_ast);
							;}
    break;

  case 162:
#line 1151 "elftosb_parser.y"
    {
								(yyval.m_ast) = new IntSizeExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(1) - (3)].m_ast)), (yyvsp[(3) - (3)].m_int)->getWordSize());
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 163:
#line 1156 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (3)].m_ast);
								(yyval.m_ast)->setLocation((yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
							;}
    break;

  case 164:
#line 1161 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode(dynamic_cast<SymbolASTNode*>((yyvsp[(3) - (4)].m_ast)));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						;}
    break;

  case 165:
#line 1166 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						;}
    break;

  case 166:
#line 1171 "elftosb_parser.y"
    {
							(yyval.m_ast) = new SizeofOperatorASTNode((yyvsp[(3) - (4)].m_str));
							(yyval.m_ast)->setLocation((yylsp[(1) - (4)]), (yylsp[(4) - (4)]));
						;}
    break;

  case 167:
#line 1178 "elftosb_parser.y"
    {
								(yyval.m_ast) = (yyvsp[(2) - (2)].m_ast);
							;}
    break;

  case 168:
#line 1182 "elftosb_parser.y"
    {
								(yyval.m_ast) = new NegativeExprASTNode(dynamic_cast<ExprASTNode*>((yyvsp[(2) - (2)].m_ast)));
								(yyval.m_ast)->setLocation((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
							;}
    break;

  case 169:
#line 1189 "elftosb_parser.y"
    {
								(yyval.m_ast) = new IntConstExprASTNode((yyvsp[(1) - (1)].m_int)->getValue(), (yyvsp[(1) - (1)].m_int)->getWordSize());
								(yyval.m_ast)->setLocation((yylsp[(1) - (1)]));
							;}
    break;


/* Line 1267 of yacc.c.  */
#line 3339 "elftosb_parser.tab.c"
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


#line 1195 "elftosb_parser.y"


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


