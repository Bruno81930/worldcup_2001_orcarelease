
/*  A Bison parser, made from coach_lang.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	INT	257
#define	REAL	258
#define	STR	259
#define	LP	260
#define	RP	261
#define	LB	262
#define	RB	263
#define	META	264
#define	FREEFORM	265
#define	INFO	266
#define	ADVICE	267
#define	DEFINE	268
#define	DEFINEC	269
#define	DEFINED	270
#define	DEFINER	271
#define	DEFINEA	272
#define	VERSION	273
#define	NULL_REGION	274
#define	QUAD	275
#define	ARC	276
#define	PT	277
#define	REG	278
#define	BALL	279
#define	POS	280
#define	HOME	281
#define	BALLTO	282
#define	MARK	283
#define	MARK_LINE	284
#define	OFFLINE	285
#define	HTYPE	286
#define	BMOVE_TOKEN	287
#define	DO_DONT	288
#define	TEAM	289
#define	TRUE_KW	290
#define	FALSE_KW	291
#define	PPOS	292
#define	BPOS	293
#define	BOWNER	294
#define	AND	295
#define	OR	296
#define	NOT	297
#define	PLAY_MODE_KW	298
#define	PLAY_MODE	299
#define	CLEAR	300
#define	ERROR	301

#line 25 "coach_lang.y"

#include "coach_lang.h"
#include "garbage.h"
int yyerror(char*);
int yylex();
extern CM_Message* g_pMess;
//extern CM_Directive* pTesting;
//extern CM_Condition* pTesting;
//extern CM_Token* pTesting;
extern GarbageCollector g_GC; //the garbage collector

#line 38 "coach_lang.y"
typedef union {
  int keyword;
  int separator;
  int inum;
  double dnum;
  char* str;

  CM_Message*     g_pMess;

  CM_MetaToken*   pMetaTok;
  vector<CM_MetaToken*>* pvpMetaTok;

  CM_DefineToken* pDefTok;
  vector<CM_DefineToken*>* pvpDefTok;

  CM_Action* pAct;

  CM_Directive* pDir;
  vector<CM_Directive*>* pvpDir;

  CM_Condition* pCond;
  vector<CM_Condition*>* pvpCond;

  CM_Token* pTok;
  vector<CM_Token*>* pvpTok;

  Region*   pReg;
  RegPoint* pRegPt;
  vector<Region*>* pvpReg;
  UnumSet*   pUSet;
  BallMoveTokenSet*   pBMTSet;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		175
#define	YYFLAG		-32768
#define	YYNTBASE	48

#define YYTRANSLATE(x) ((unsigned)(x) <= 301 ? yytranslate[x] : 73)

static const char yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    10,    15,    18,    20,    25,
    30,    33,    35,    41,    47,    53,    59,    64,    69,    74,
    80,    84,    87,    89,    94,    99,   105,   110,   115,   120,
   125,   130,   135,   137,   144,   146,   149,   151,   155,   159,
   168,   173,   179,   184,   189,   194,   199,   201,   204,   206,
   208,   212,   220,   229,   234,   236,   239,   241,   247,   254,
   259,   265,   267,   269,   273,   276,   277,   281,   284
};

static const short yyrhs[] = {    49,
     0,    55,     0,    56,     0,    57,     0,    52,     0,     6,
    10,    50,     7,     0,    50,    51,     0,    51,     0,     6,
    19,    68,     7,     0,     6,    14,    53,     7,     0,    53,
    54,     0,    54,     0,     6,    15,     5,    63,     7,     0,
     6,    16,     5,    61,     7,     0,     6,    17,     5,    65,
     7,     0,     6,    18,     5,    60,     7,     0,     6,    11,
     5,     7,     0,     6,    12,    59,     7,     0,     6,    13,
    59,     7,     0,     6,     3,    63,    62,     7,     0,     6,
    46,     7,     0,    59,    58,     0,    58,     0,     6,    26,
    65,     7,     0,     6,    27,    65,     7,     0,     6,    28,
    65,    71,     7,     0,     6,    28,    69,     7,     0,     6,
    29,    69,     7,     0,     6,    30,    69,     7,     0,     6,
    30,    65,     7,     0,     6,    31,    65,     7,     0,     6,
    32,     3,     7,     0,     5,     0,     6,    34,    35,    69,
    60,     7,     0,     5,     0,    62,    61,     0,    61,     0,
     6,    36,     7,     0,     6,    37,     7,     0,     6,    38,
    35,    69,     3,     3,    65,     7,     0,     6,    39,    65,
     7,     0,     6,    40,    35,    69,     7,     0,     6,    44,
    45,     7,     0,     6,    41,    64,     7,     0,     6,    42,
    64,     7,     0,     6,    43,    63,     7,     0,     5,     0,
    64,    63,     0,    63,     0,    67,     0,     6,    20,     7,
     0,     6,    21,    67,    67,    67,    67,     7,     0,     6,
    22,    67,    68,    68,    68,    68,     7,     0,     6,    24,
    66,     7,     0,     5,     0,    66,    65,     0,    65,     0,
     6,    23,    68,    68,     7,     0,     6,    23,    68,    68,
    67,     7,     0,     6,    23,    25,     7,     0,     6,    23,
    35,     3,     7,     0,     3,     0,     4,     0,     8,    70,
     9,     0,    70,     3,     0,     0,     8,    72,     9,     0,
    72,    33,     0,    33,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   166,   168,   170,   172,   174,   178,   182,   184,   187,   190,
   194,   196,   199,   203,   207,   211,   217,   222,   226,   231,
   234,   235,   237,   242,   244,   246,   249,   252,   255,   258,
   260,   262,   264,   267,   270,   273,   274,   278,   280,   282,
   285,   288,   291,   294,   297,   300,   302,   305,   306,   310,
   311,   313,   316,   319,   322,   325,   326,   328,   329,   331,
   332,   334,   335,   337,   339,   340,   343,   345,   346
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","INT","REAL",
"STR","\"(\"","\")\"","\"{\"","\"}\"","META","FREEFORM","INFO","ADVICE","DEFINE",
"DEFINEC","DEFINED","DEFINER","DEFINEA","VERSION","NULL_REGION","QUAD","ARC",
"PT","REG","BALL","POS","HOME","BALLTO","MARK","MARK_LINE","OFFLINE","HTYPE",
"BMOVE_TOKEN","DO_DONT","TEAM","TRUE_KW","FALSE_KW","PPOS","BPOS","BOWNER","AND",
"OR","NOT","PLAY_MODE_KW","PLAY_MODE","CLEAR","ERROR","message","meta_mess",
"meta_token_list","meta_token","define_mess","define_token_list","define_token",
"freeform_mess","info_mess","advice_mess","token","token_list","action","directive",
"directive_list","condition","condition_list","region","region_list","point",
"num","unum_set","unum_list","bmove_token_set","bmove_token_list", NULL
};
#endif

static const short yyr1[] = {     0,
    48,    48,    48,    48,    48,    49,    50,    50,    51,    52,
    53,    53,    54,    54,    54,    54,    55,    56,    57,    58,
    58,    59,    59,    60,    60,    60,    60,    60,    60,    60,
    60,    60,    60,    61,    61,    62,    62,    63,    63,    63,
    63,    63,    63,    63,    63,    63,    63,    64,    64,    65,
    65,    65,    65,    65,    65,    66,    66,    67,    67,    67,
    67,    68,    68,    69,    70,    70,    71,    72,    72
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     1,     4,     2,     1,     4,     4,
     2,     1,     5,     5,     5,     5,     4,     4,     4,     5,
     3,     2,     1,     4,     4,     5,     4,     4,     4,     4,
     4,     4,     1,     6,     1,     2,     1,     3,     3,     8,
     4,     5,     4,     4,     4,     4,     1,     2,     1,     1,
     3,     7,     8,     4,     1,     2,     1,     5,     6,     4,
     5,     1,     1,     3,     2,     0,     3,     2,     1
};

static const short yydefact[] = {     0,
     0,     1,     5,     2,     3,     4,     0,     0,     0,     0,
     0,     0,     0,     8,     0,     0,    23,     0,     0,     0,
     0,    12,     0,     6,     7,    17,     0,     0,    18,    22,
    19,     0,     0,     0,     0,    10,    11,    62,    63,     0,
    47,     0,     0,    21,     0,     0,     0,     0,     9,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
    37,     0,     0,     0,    55,     0,     0,    50,    33,     0,
     0,    38,    39,     0,     0,     0,    49,     0,     0,     0,
     0,     0,    20,    36,    13,    14,     0,     0,     0,     0,
     0,    15,     0,     0,     0,     0,     0,     0,     0,    16,
    66,     0,    41,     0,    44,    48,    45,    46,    43,     0,
    51,     0,     0,     0,     0,     0,     0,    57,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    42,     0,     0,     0,    60,     0,     0,    54,    56,    24,
    25,     0,     0,    27,    28,    30,    29,    31,    32,    65,
    64,     0,     0,     0,     0,    61,    58,     0,    69,     0,
    26,     0,    34,     0,     0,    59,    67,    68,    40,    52,
     0,    53,     0,     0,     0
};

static const short yydefgoto[] = {   173,
     2,    13,    14,     3,    21,    22,     4,     5,     6,    17,
    18,    71,    61,    62,    77,    78,    67,   119,    68,    40,
   102,   129,   143,   160
};

static const short yypact[] = {     1,
    48,-32768,-32768,-32768,-32768,-32768,    21,    26,    31,    31,
    33,    49,    44,-32768,   108,     3,-32768,    69,    91,    -6,
    97,-32768,    51,-32768,-32768,-32768,   100,   111,-32768,-32768,
-32768,    84,   114,   115,   116,-32768,-32768,-32768,-32768,   117,
-32768,    42,   102,-32768,   100,   102,   104,   106,-32768,   118,
   119,    87,   104,    88,   100,   100,   100,    83,-32768,    93,
-32768,    58,   122,   123,-32768,    50,   124,-32768,-32768,   -12,
   125,-32768,-32768,   126,   128,   126,-32768,    86,    89,   129,
   130,   103,-32768,-32768,-32768,-32768,   132,   127,   127,    18,
   104,-32768,   104,   104,    82,   126,    82,   104,   137,-32768,
-32768,   138,-32768,   135,-32768,-32768,-32768,-32768,-32768,   126,
-32768,   120,   127,    51,   139,   141,    51,-32768,    95,   140,
   142,   143,   145,   146,   147,   148,   149,   150,    27,   155,
-32768,   106,   127,    51,-32768,   152,   107,-32768,-32768,-32768,
-32768,   112,   153,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   104,   154,   127,    51,-32768,-32768,   156,-32768,    -8,
-32768,   157,-32768,   158,    51,-32768,-32768,-32768,-32768,-32768,
   159,-32768,   162,   167,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,   160,-32768,-32768,   151,-32768,-32768,-32768,    98,
   161,    16,   -38,-32768,   -22,    94,   -53,-32768,   -85,   -88,
   -63,-32768,-32768,-32768
};


#define	YYLAST		173


static const short yytable[] = {    75,
   167,   117,   113,   114,    43,    27,     1,    64,    32,    33,
    34,    35,   104,    93,    94,    95,    96,    97,    98,    99,
    38,    39,    63,    84,   168,   134,    12,   133,   137,   150,
    15,   123,   124,   126,    80,   151,    16,   118,    20,   120,
   121,   122,   115,   125,   127,   155,   132,   154,    28,    12,
    24,   158,   116,    38,    39,   106,   106,     7,     8,     9,
    10,    11,    59,    60,    83,   139,   165,    23,   164,    87,
    88,    89,    90,    91,    16,    29,   171,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    65,    66,    45,   101,
    41,    42,   105,    41,    42,   107,    16,    31,   162,    65,
    66,   138,    20,    36,    41,    42,    59,    60,    65,    66,
    69,    70,   112,   157,    26,    30,    30,    44,    46,    47,
    48,    74,    76,    49,    72,    73,    82,    81,    85,    86,
    92,   100,   112,   101,   103,   108,   109,   110,   111,   128,
   130,   131,    90,   136,   159,   135,   140,   153,   141,    79,
   142,   144,   145,   146,   147,   148,   149,   152,   156,   161,
   163,   174,   166,   169,   170,   172,   175,     0,     0,     0,
    19,    37,    25
};

static const short yycheck[] = {    53,
     9,    90,    88,    89,    27,     3,     6,    46,    15,    16,
    17,    18,    76,    26,    27,    28,    29,    30,    31,    32,
     3,     4,    45,    62,    33,   114,     6,   113,   117,     3,
     5,    95,    96,    97,    57,     9,     6,    91,     6,    93,
    94,    95,    25,    97,    98,   134,   110,   133,    46,     6,
     7,   137,    35,     3,     4,    78,    79,    10,    11,    12,
    13,    14,     5,     6,     7,   119,   155,    19,   154,    20,
    21,    22,    23,    24,     6,     7,   165,    36,    37,    38,
    39,    40,    41,    42,    43,    44,     5,     6,     5,     8,
     5,     6,     7,     5,     6,     7,     6,     7,   152,     5,
     6,     7,     6,     7,     5,     6,     5,     6,     5,     6,
     5,     6,     6,     7,     7,    18,    19,     7,     5,     5,
     5,    35,    35,     7,     7,     7,    34,    45,     7,     7,
     7,     7,     6,     8,     7,     7,     7,    35,     7,     3,
     3,     7,    23,     3,    33,     7,     7,   132,     7,    56,
     8,     7,     7,     7,     7,     7,     7,     3,     7,     7,
     7,     0,     7,     7,     7,     7,     0,    -1,    -1,    -1,
    10,    21,    13
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "//usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "//usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 166 "coach_lang.y"
{ yyval.g_pMess = yyvsp[0].g_pMess; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = yyvsp[0].g_pMess; ;
    break;}
case 2:
#line 168 "coach_lang.y"
{ yyval.g_pMess = yyvsp[0].g_pMess; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = yyvsp[0].g_pMess; ;
    break;}
case 3:
#line 170 "coach_lang.y"
{ yyval.g_pMess = yyvsp[0].g_pMess; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = yyvsp[0].g_pMess; ;
    break;}
case 4:
#line 172 "coach_lang.y"
{ yyval.g_pMess = yyvsp[0].g_pMess; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = yyvsp[0].g_pMess; ;
    break;}
case 5:
#line 174 "coach_lang.y"
{ yyval.g_pMess = yyvsp[0].g_pMess; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = yyvsp[0].g_pMess; ;
    break;}
case 6:
#line 179 "coach_lang.y"
{ yyval.g_pMess = new CM_MetaMessage(*yyvsp[-1].pvpMetaTok); delete yyvsp[-1].pvpMetaTok; g_GC.remove(yyvsp[-1].pvpMetaTok, GCE_pvpMetaTok); 
          g_GC.add(yyval.g_pMess, GCE_pMess); ;
    break;}
case 7:
#line 183 "coach_lang.y"
{ yyvsp[-1].pvpMetaTok->push_back(yyvsp[0].pMetaTok); yyval.pvpMetaTok = yyvsp[-1].pvpMetaTok; g_GC.remove(yyvsp[0].pMetaTok, GCE_pMetaTok); ;
    break;}
case 8:
#line 184 "coach_lang.y"
{ yyval.pvpMetaTok = new vector<CM_MetaToken*>;yyval.pvpMetaTok->push_back(yyvsp[0].pMetaTok); 
                       g_GC.add(yyval.pvpMetaTok, GCE_pvpMetaTok); g_GC.remove(yyvsp[0].pMetaTok, GCE_pMetaTok); ;
    break;}
case 9:
#line 187 "coach_lang.y"
{ yyval.pMetaTok = new CM_MetaToken_Version(yyvsp[-1].dnum); g_GC.add(yyval.pMetaTok, GCE_pMetaTok); ;
    break;}
case 10:
#line 191 "coach_lang.y"
{ yyval.g_pMess = new CM_DefineMessage(*yyvsp[-1].pvpDefTok); delete yyvsp[-1].pvpDefTok; g_GC.remove(yyvsp[-1].pvpDefTok, GCE_pvpDefTok); 
          g_GC.add(yyval.g_pMess, GCE_pMess); ;
    break;}
case 11:
#line 195 "coach_lang.y"
{ yyvsp[-1].pvpDefTok->push_back(yyvsp[0].pDefTok); yyval.pvpDefTok = yyvsp[-1].pvpDefTok; g_GC.remove(yyvsp[0].pDefTok, GCE_pDefTok); ;
    break;}
case 12:
#line 196 "coach_lang.y"
{ yyval.pvpDefTok = new vector<CM_DefineToken*>;yyval.pvpDefTok->push_back(yyvsp[0].pDefTok); 
                         g_GC.add(yyval.pvpDefTok, GCE_pvpDefTok); g_GC.remove(yyvsp[0].pDefTok, GCE_pDefTok); ;
    break;}
case 13:
#line 200 "coach_lang.y"
{ yyval.pDefTok = new CM_DefTokCondition(strip_quotes(yyvsp[-2].str), yyvsp[-1].pCond);
                  delete yyvsp[-2].str; g_GC.remove(yyvsp[-2].str, GCE_pChar);
                  g_GC.add(yyval.pDefTok, GCE_pDefTok); g_GC.remove(yyvsp[-1].pCond, GCE_pCond); ;
    break;}
case 14:
#line 204 "coach_lang.y"
{ yyval.pDefTok = new CM_DefTokDirective(strip_quotes(yyvsp[-2].str), yyvsp[-1].pDir); 
                  delete yyvsp[-2].str; g_GC.remove(yyvsp[-2].str, GCE_pChar);
                  g_GC.add(yyval.pDefTok, GCE_pDefTok); g_GC.remove(yyvsp[-1].pDir, GCE_pDir); ;
    break;}
case 15:
#line 208 "coach_lang.y"
{ yyval.pDefTok = new CM_DefTokRegion(strip_quotes(yyvsp[-2].str), yyvsp[-1].pReg); 
                  delete yyvsp[-2].str; g_GC.remove(yyvsp[-2].str, GCE_pChar);
                  g_GC.add(yyval.pDefTok, GCE_pDefTok); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 16:
#line 212 "coach_lang.y"
{ yyval.pDefTok = new CM_DefTokAction(strip_quotes(yyvsp[-2].str), yyvsp[-1].pAct); 
                  delete yyvsp[-2].str; g_GC.remove(yyvsp[-2].str, GCE_pChar);
                  g_GC.add(yyval.pDefTok, GCE_pDefTok); g_GC.remove(yyvsp[-1].pAct, GCE_pAct); ;
    break;}
case 17:
#line 217 "coach_lang.y"
{ yyval.g_pMess = new CM_FreeformMessage(strip_quotes(yyvsp[-1].str));
                                    delete yyvsp[-1].str; g_GC.remove(yyvsp[-1].str, GCE_pChar);
                                    g_GC.add(yyval.g_pMess, GCE_pMess);
                                  ;
    break;}
case 18:
#line 223 "coach_lang.y"
{ yyval.g_pMess = new CM_InfoMessage(*yyvsp[-1].pvpTok); delete yyvsp[-1].pvpTok;
        g_GC.remove(yyvsp[-1].pvpTok, GCE_pvpTok); g_GC.add(yyval.g_pMess, GCE_pMess); ;
    break;}
case 19:
#line 227 "coach_lang.y"
{ yyval.g_pMess = new CM_AdviceMessage(*yyvsp[-1].pvpTok); delete yyvsp[-1].pvpTok;
        g_GC.remove(yyvsp[-1].pvpTok, GCE_pvpTok); g_GC.add(yyval.g_pMess, GCE_pMess); ;
    break;}
case 20:
#line 232 "coach_lang.y"
{ yyval.pTok = new CM_TokRule(yyvsp[-3].inum, yyvsp[-2].pCond, *yyvsp[-1].pvpDir); delete yyvsp[-1].pvpDir; 
          g_GC.add(yyval.pTok, GCE_pTok); g_GC.remove(yyvsp[-2].pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pvpDir, GCE_pvpDir); ;
    break;}
case 21:
#line 234 "coach_lang.y"
{ yyval.pTok = new CM_TokClear(); g_GC.add(yyval.pTok, GCE_pTok); ;
    break;}
case 22:
#line 236 "coach_lang.y"
{ yyvsp[-1].pvpTok->push_back(yyvsp[0].pTok); yyval.pvpTok = yyvsp[-1].pvpTok; g_GC.remove(yyvsp[0].pTok, GCE_pTok); ;
    break;}
case 23:
#line 238 "coach_lang.y"
{ yyval.pvpTok = new vector<CM_Token*>; yyval.pvpTok->push_back(yyvsp[0].pTok); 
              g_GC.add(yyval.pvpTok, GCE_pvpTok); g_GC.remove(yyvsp[0].pTok, GCE_pTok); ;
    break;}
case 24:
#line 243 "coach_lang.y"
{ yyval.pAct = new CM_ActPosition(yyvsp[-1].pReg); g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 25:
#line 245 "coach_lang.y"
{ yyval.pAct = new CM_ActHome(yyvsp[-1].pReg); g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 26:
#line 247 "coach_lang.y"
{ yyval.pAct = new CM_ActBallToRegion(yyvsp[-2].pReg, *yyvsp[-1].pBMTSet); 
          g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-2].pReg, GCE_pReg); g_GC.remove(yyvsp[-1].pBMTSet, GCE_pBMTSet); ;
    break;}
case 27:
#line 250 "coach_lang.y"
{ yyval.pAct = new CM_ActBallToPlayer(*yyvsp[-1].pUSet); delete yyvsp[-1].pUSet; 
          g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pUSet, GCE_pUSet); ;
    break;}
case 28:
#line 253 "coach_lang.y"
{ yyval.pAct = new CM_ActMark(*yyvsp[-1].pUSet); delete yyvsp[-1].pUSet;
 	  g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pUSet, GCE_pUSet); ;
    break;}
case 29:
#line 256 "coach_lang.y"
{ yyval.pAct = new CM_ActMarkLinePlayer(*yyvsp[-1].pUSet); delete yyvsp[-1].pUSet;
	  g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pUSet, GCE_pUSet); ;
    break;}
case 30:
#line 259 "coach_lang.y"
{ yyval.pAct = new CM_ActMarkLineRegion(yyvsp[-1].pReg); g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 31:
#line 261 "coach_lang.y"
{ yyval.pAct = new CM_ActOffsidesLine(yyvsp[-1].pReg); g_GC.add(yyval.pAct, GCE_pAct); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 32:
#line 263 "coach_lang.y"
{ yyval.pAct = new CM_ActHetType(yyvsp[-1].inum); g_GC.add(yyval.pAct, GCE_pAct); ;
    break;}
case 33:
#line 264 "coach_lang.y"
{ yyval.pAct = new CM_ActNamed(strip_quotes(yyvsp[0].str)); g_GC.add(yyval.pAct, GCE_pAct); 
                delete yyvsp[0].str; g_GC.remove(yyvsp[0].str, GCE_pChar); ;
    break;}
case 34:
#line 268 "coach_lang.y"
{ yyval.pDir = new CM_DirCommand((bool)yyvsp[-4].inum, (bool)yyvsp[-3].inum, *yyvsp[-2].pUSet, yyvsp[-1].pAct); delete yyvsp[-2].pUSet; 
          g_GC.add(yyval.pDir, GCE_pDir); g_GC.remove(yyvsp[-2].pUSet, GCE_pUSet); g_GC.remove(yyvsp[-1].pAct, GCE_pAct); ;
    break;}
case 35:
#line 270 "coach_lang.y"
{ yyval.pDir = new CM_DirNamed(strip_quotes(yyvsp[0].str)); g_GC.add(yyval.pDir, GCE_pDir); 
                  delete yyvsp[0].str; g_GC.remove(yyvsp[0].str, GCE_pChar); ;
    break;}
case 36:
#line 273 "coach_lang.y"
{ yyvsp[-1].pvpDir->push_back(yyvsp[0].pDir); yyval.pvpDir = yyvsp[-1].pvpDir; g_GC.remove(yyvsp[0].pDir, GCE_pDir); ;
    break;}
case 37:
#line 274 "coach_lang.y"
{ yyval.pvpDir = new vector<CM_Directive*>; yyval.pvpDir->push_back(yyvsp[0].pDir); 
                            g_GC.add(yyval.pvpDir, GCE_pvpDir); g_GC.remove(yyvsp[0].pDir, GCE_pDir); ;
    break;}
case 38:
#line 279 "coach_lang.y"
{ yyval.pCond = new CM_CondTrue(); g_GC.add(yyval.pCond, GCE_pCond); ;
    break;}
case 39:
#line 281 "coach_lang.y"
{ yyval.pCond = new CM_CondFalse(); g_GC.add(yyval.pCond, GCE_pCond); ;
    break;}
case 40:
#line 283 "coach_lang.y"
{ yyval.pCond = new CM_CondPlayerPosition((bool)yyvsp[-5].inum,*yyvsp[-4].pUSet,yyvsp[-3].inum,yyvsp[-2].inum,yyvsp[-1].pReg); delete yyvsp[-4].pUSet; 
                  g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-4].pUSet, GCE_pUSet); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 41:
#line 286 "coach_lang.y"
{ yyval.pCond = new CM_CondBallPosition(yyvsp[-1].pReg); 
                  g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pReg, GCE_pReg); ;
    break;}
case 42:
#line 289 "coach_lang.y"
{ yyval.pCond = new CM_CondBallOwner((bool)yyvsp[-2].inum,*yyvsp[-1].pUSet); delete yyvsp[-1].pUSet; 
                  g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pUSet, GCE_pUSet); ;
    break;}
case 43:
#line 292 "coach_lang.y"
{ yyval.pCond = new CM_CondPlayMode((CM_PlayModeType)yyvsp[-1].inum);
                  g_GC.add(yyval.pCond, GCE_pCond); ;
    break;}
case 44:
#line 295 "coach_lang.y"
{ yyval.pCond = new CM_CondAnd(*yyvsp[-1].pvpCond); delete yyvsp[-1].pvpCond; 
                  g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pvpCond, GCE_pvpCond); ;
    break;}
case 45:
#line 298 "coach_lang.y"
{ yyval.pCond = new CM_CondOr(*yyvsp[-1].pvpCond); delete yyvsp[-1].pvpCond; 
                  g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pvpCond, GCE_pvpCond); ;
    break;}
case 46:
#line 301 "coach_lang.y"
{ yyval.pCond = new CM_CondNot(yyvsp[-1].pCond); g_GC.add(yyval.pCond, GCE_pCond); g_GC.remove(yyvsp[-1].pCond, GCE_pCond); ;
    break;}
case 47:
#line 302 "coach_lang.y"
{ yyval.pCond = new CM_CondNamed(strip_quotes(yyvsp[0].str)); g_GC.add(yyval.pCond, GCE_pCond); 
                  delete yyvsp[0].str; g_GC.remove(yyvsp[0].str, GCE_pChar); ;
    break;}
case 48:
#line 305 "coach_lang.y"
{ yyvsp[-1].pvpCond->push_back(yyvsp[0].pCond); yyval.pvpCond = yyvsp[-1].pvpCond; g_GC.remove(yyvsp[0].pCond, GCE_pCond);;
    break;}
case 49:
#line 306 "coach_lang.y"
{ yyval.pvpCond = new vector<CM_Condition*>; yyval.pvpCond->push_back(yyvsp[0].pCond); 
                            g_GC.remove(yyvsp[0].pCond, GCE_pCond); g_GC.add(yyval.pvpCond, GCE_pvpCond);;
    break;}
case 50:
#line 310 "coach_lang.y"
{ yyval.pReg = yyvsp[0].pRegPt; ;
    break;}
case 51:
#line 312 "coach_lang.y"
{ yyval.pReg = new RegNull; g_GC.add(yyval.pReg, GCE_pReg); ;
    break;}
case 52:
#line 314 "coach_lang.y"
{ yyval.pReg = new RegQuad(yyvsp[-4].pRegPt, yyvsp[-3].pRegPt, yyvsp[-2].pRegPt, yyvsp[-1].pRegPt); g_GC.add(yyval.pReg, GCE_pReg);
           g_GC.remove(yyvsp[-4].pRegPt, GCE_pReg); g_GC.remove(yyvsp[-3].pRegPt, GCE_pReg); g_GC.remove(yyvsp[-2].pRegPt, GCE_pReg); g_GC.remove(yyvsp[-1].pRegPt, GCE_pReg); ;
    break;}
case 53:
#line 317 "coach_lang.y"
{ yyval.pReg = new RegArc(yyvsp[-5].pRegPt, yyvsp[-4].dnum, yyvsp[-3].dnum, yyvsp[-2].dnum, yyvsp[-1].dnum); 
           g_GC.add(yyval.pReg, GCE_pReg); g_GC.remove(yyvsp[-5].pRegPt, GCE_pReg); ;
    break;}
case 54:
#line 320 "coach_lang.y"
{ yyval.pReg = new RegUnion(*yyvsp[-1].pvpReg); g_GC.add(yyval.pReg, GCE_pReg);
           delete yyvsp[-1].pvpReg; g_GC.remove(yyvsp[-1].pvpReg, GCE_pvpReg); ;
    break;}
case 55:
#line 322 "coach_lang.y"
{ yyval.pReg = new RegNamed(strip_quotes(yyvsp[0].str)); g_GC.add(yyval.pReg, GCE_pReg); 
              delete yyvsp[0].str; g_GC.remove(yyvsp[0].str, GCE_pChar); ;
    break;}
case 56:
#line 325 "coach_lang.y"
{ yyvsp[-1].pvpReg->push_back(yyvsp[0].pReg); yyval.pvpReg = yyvsp[-1].pvpReg; g_GC.remove(yyvsp[0].pReg, GCE_pReg); ;
    break;}
case 57:
#line 326 "coach_lang.y"
{ yyval.pvpReg = new vector<Region*>;yyval.pvpReg->push_back(yyvsp[0].pReg); 
                   g_GC.remove(yyvsp[0].pReg, GCE_pReg); g_GC.add(yyval.pvpReg, GCE_pvpReg); ;
    break;}
case 58:
#line 328 "coach_lang.y"
{ yyval.pRegPt = new RegPointSimple(yyvsp[-2].dnum, yyvsp[-1].dnum); g_GC.add(yyval.pRegPt, GCE_pReg);;
    break;}
case 59:
#line 329 "coach_lang.y"
{ yyval.pRegPt = new RegPointRelative(yyvsp[-3].dnum, yyvsp[-2].dnum, yyvsp[-1].pRegPt);
                                g_GC.remove(yyvsp[-1].pRegPt, GCE_pReg); g_GC.add(yyval.pRegPt, GCE_pReg);;
    break;}
case 60:
#line 331 "coach_lang.y"
{ yyval.pRegPt = new RegPointBall(); g_GC.add(yyval.pRegPt, GCE_pReg);;
    break;}
case 61:
#line 332 "coach_lang.y"
{ yyval.pRegPt = new RegPointPlayer((bool)yyvsp[-2].inum, yyvsp[-1].inum); g_GC.add(yyval.pRegPt, GCE_pReg);;
    break;}
case 62:
#line 334 "coach_lang.y"
{ yyval.dnum = (double)yyvsp[0].inum; ;
    break;}
case 63:
#line 335 "coach_lang.y"
{ yyval.dnum = yyvsp[0].dnum; ;
    break;}
case 64:
#line 337 "coach_lang.y"
{ yyval.pUSet = yyvsp[-1].pUSet; ;
    break;}
case 65:
#line 339 "coach_lang.y"
{ yyval.pUSet = yyvsp[-1].pUSet; yyval.pUSet->addNum(yyvsp[0].inum); ;
    break;}
case 66:
#line 340 "coach_lang.y"
{ yyval.pUSet = new UnumSet(); g_GC.add(yyval.pUSet, GCE_pUSet);;
    break;}
case 67:
#line 343 "coach_lang.y"
{ yyval.pBMTSet = yyvsp[-1].pBMTSet; ;
    break;}
case 68:
#line 345 "coach_lang.y"
{ yyval.pBMTSet = yyvsp[-1].pBMTSet; yyval.pBMTSet->addToken((BallMoveToken)yyvsp[0].inum); ;
    break;}
case 69:
#line 346 "coach_lang.y"
{ yyval.pBMTSet = new BallMoveTokenSet((BallMoveToken)yyvsp[0].inum); 
                                g_GC.add(yyval.pBMTSet, GCE_pBMTSet); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "//usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 350 "coach_lang.y"

/* ADDITIONAL C CODE */
int yyerror (char* s)
{
  fprintf (stderr, "%s: next token %d\n", s, yychar);
  g_pMess = NULL;
  //pTesting = NULL;
  return 0;
}


