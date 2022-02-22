
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
#define	HOME_REGION	275
#define	QUAD	276
#define	ARC	277
#define	PT	278
#define	REG	279
#define	BALL	280
#define	POS	281
#define	HOME	282
#define	BALLTO	283
#define	MARK	284
#define	MARK_LINE	285
#define	OFFLINE	286
#define	HTYPE	287
#define	BMOVE_TOKEN	288
#define	DO_DONT	289
#define	TEAM	290
#define	TRUE_KW	291
#define	FALSE_KW	292
#define	PPOS	293
#define	BPOS	294
#define	BOWNER	295
#define	AND	296
#define	OR	297
#define	NOT	298
#define	PLAY_MODE_KW	299
#define	PLAY_MODE	300
#define	CLEAR	301
#define	ACTION	302
#define	TIME	303
#define	GOALDIFF	304
#define	STAMINA	305
#define	EQUAL	306
#define	EQUALUNUM	307
#define	LESS	308
#define	GREATER	309
#define	STATE	310
#define	BALLVELOCITY	311
#define	BALLINTERCEPTABLE	312
#define	BALLCATCHABLE	313
#define	FORCE	314
#define	INTERCEPTBALL	315
#define	CATCHBALL	316
#define	LEVEL	317
#define	PASSMODE	318
#define	TEAMFUNCTION	319
#define	TEAMFUNCTION_TU	320
#define	VAR	321
#define	UNUMFUNCTION_T	322
#define	UNUMFUNCTION_TU	323
#define	UNUMFUNCTION_TTU	324
#define	YOURHOME	325
#define	YOURADJUSTEDHOME	326
#define	PLUSPOINT	327
#define	MULTPOINT	328
#define	DEFINEP	329
#define	ERROR	330

#line 27 "coach_lang.y"


#include "SFLShierarchy.h"
#include "coach_lang.h"
#include "garbage.h"

int yyerror(char*);
int yylex();

extern CM_Message* g_pMess;

extern GarbageCollector g_GC; //the garbage collector


#line 44 "coach_lang.y"
typedef union
{
	int keyword;
	int separator;

	int inum;
	double dnum;
	char* str;

	CM_Message* g_pMess;

	CM_MetaToken* pMetaTok;
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
	SFLS_UnumSet*   pUSet;
	BallMoveTokenSet*   pBMTSet;

	SFLS_Team* pTeam;
	SFLS_Function* pFunc;
	DoubleOrVariable* pDoV;
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



#define	YYFINAL		291
#define	YYFLAG		-32768
#define	YYNTBASE	77

#define YYTRANSLATE(x) ((unsigned)(x) <= 330 ? yytranslate[x] : 105)

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
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     8,    10,    15,    18,    20,    25,
    30,    33,    35,    41,    47,    53,    59,    65,    70,    75,
    80,    86,    90,    93,    95,   102,   109,   111,   114,   116,
   122,   127,   132,   138,   143,   148,   153,   158,   163,   168,
   174,   180,   185,   189,   191,   195,   198,   200,   204,   208,
   217,   222,   228,   233,   238,   243,   248,   255,   260,   265,
   272,   278,   284,   290,   296,   302,   308,   314,   320,   325,
   331,   337,   339,   342,   344,   346,   350,   358,   367,   372,
   374,   377,   379,   385,   389,   395,   401,   408,   413,   419,
   423,   425,   429,   435,   439,   441,   443,   446,   449,   450,
   452,   457,   463,   470,   472,   474,   476
};

static const short yyrhs[] = {    78,
     0,    84,     0,    85,     0,    86,     0,    81,     0,     6,
    10,    79,     7,     0,    79,    80,     0,    80,     0,     6,
    19,   103,     7,     0,     6,    14,    82,     7,     0,    82,
    83,     0,    83,     0,     6,    15,     5,    94,     7,     0,
     6,    16,     5,    89,     7,     0,     6,    17,     5,    96,
     7,     0,     6,    18,     5,    91,     7,     0,     6,    75,
     5,    98,     7,     0,     6,    11,     5,     7,     0,     6,
    12,    88,     7,     0,     6,    13,    88,     7,     0,     6,
     3,    94,    90,     7,     0,     6,    47,     7,     0,    88,
    87,     0,    87,     0,     6,    35,    99,   100,    91,     7,
     0,     6,    60,    99,   100,    91,     7,     0,     5,     0,
    90,    89,     0,    89,     0,     6,    27,    96,   103,     7,
     0,     6,    27,    96,     7,     0,     6,    28,    96,     7,
     0,     6,    29,    96,    92,     7,     0,     6,    29,   100,
     7,     0,     6,    30,   100,     7,     0,     6,    31,   100,
     7,     0,     6,    31,    96,     7,     0,     6,    32,    96,
     7,     0,     6,    33,     3,     7,     0,     6,    31,    96,
   100,     7,     0,     6,    56,     5,     5,     7,     0,     6,
    61,   103,     7,     0,     6,    62,     7,     0,     5,     0,
     8,    93,     9,     0,    93,    34,     0,    34,     0,     6,
    37,     7,     0,     6,    38,     7,     0,     6,    39,    99,
   100,     3,     3,    96,     7,     0,     6,    40,    96,     7,
     0,     6,    41,    99,   100,     7,     0,     6,    45,    46,
     7,     0,     6,    42,    95,     7,     0,     6,    43,    95,
     7,     0,     6,    44,    94,     7,     0,     6,    48,    99,
   100,    91,     7,     0,     6,    49,   104,     7,     0,     6,
    50,   104,     7,     0,     6,    51,    99,   100,    63,     7,
     0,     6,    52,   104,   104,     7,     0,     6,    54,   104,
   104,     7,     0,     6,    55,   104,   104,     7,     0,     6,
    53,     3,     3,     7,     0,     6,    53,     3,   102,     7,
     0,     6,    53,   102,     3,     7,     0,     6,    53,   102,
   102,     7,     0,     6,    56,     5,     5,     7,     0,     6,
    57,   104,     7,     0,     6,    58,    99,   100,     7,     0,
     6,    59,    99,   100,     7,     0,     5,     0,    95,    94,
     0,    94,     0,    98,     0,     6,    20,     7,     0,     6,
    22,    98,    98,    98,    98,     7,     0,     6,    23,    98,
   103,   103,   103,   103,     7,     0,     6,    25,    97,     7,
     0,     5,     0,    97,    96,     0,    96,     0,     6,    24,
   103,   103,     7,     0,     6,    21,     7,     0,     6,    73,
    98,    98,     7,     0,     6,    74,    98,    98,     7,     0,
     6,    24,   103,   103,    98,     7,     0,     6,    24,    26,
     7,     0,     6,    24,    99,   100,     7,     0,     6,     5,
     7,     0,    36,     0,     6,    65,     7,     0,     6,    66,
    99,   100,     7,     0,     8,   101,     9,     0,     3,     0,
   102,     0,   101,     3,     0,   101,   102,     0,     0,    67,
     0,     6,    68,    99,     7,     0,     6,    69,    99,   100,
     7,     0,     6,    70,    99,    99,   100,     7,     0,     3,
     0,     4,     0,    67,     0,   103,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   237,   246,   255,   264,   273,   290,   300,   306,   316,   329,
   339,   345,   355,   363,   371,   379,   388,   405,   420,   435,
   450,   458,   466,   472,   487,   496,   505,   515,   521,   536,
   543,   550,   556,   563,   571,   579,   587,   593,   599,   604,
   609,   616,   621,   626,   636,   643,   648,   661,   666,   671,
   680,   686,   694,   699,   706,   713,   719,   724,   729,   734,
   739,   744,   749,   754,   759,   764,   769,   774,   781,   786,
   791,   796,   806,   812,   827,   831,   836,   845,   851,   858,
   868,   874,   884,   889,   894,   898,   902,   908,   913,   918,
   931,   936,   941,   957,   961,   966,   974,   979,   985,   998,
  1003,  1008,  1013,  1026,  1030,  1042,  1047
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","INT","REAL",
"STR","\"(\"","\")\"","\"{\"","\"}\"","META","FREEFORM","INFO","ADVICE","DEFINE",
"DEFINEC","DEFINED","DEFINER","DEFINEA","VERSION","NULL_REGION","HOME_REGION",
"QUAD","ARC","PT","REG","BALL","POS","HOME","BALLTO","MARK","MARK_LINE","OFFLINE",
"HTYPE","BMOVE_TOKEN","DO_DONT","TEAM","TRUE_KW","FALSE_KW","PPOS","BPOS","BOWNER",
"AND","OR","NOT","PLAY_MODE_KW","PLAY_MODE","CLEAR","ACTION","TIME","GOALDIFF",
"STAMINA","EQUAL","EQUALUNUM","LESS","GREATER","STATE","BALLVELOCITY","BALLINTERCEPTABLE",
"BALLCATCHABLE","FORCE","INTERCEPTBALL","CATCHBALL","LEVEL","PASSMODE","TEAMFUNCTION",
"TEAMFUNCTION_TU","VAR","UNUMFUNCTION_T","UNUMFUNCTION_TU","UNUMFUNCTION_TTU",
"YOURHOME","YOURADJUSTEDHOME","PLUSPOINT","MULTPOINT","DEFINEP","ERROR","message",
"meta_mess","meta_token_list","meta_token","define_mess","define_token_list",
"define_token","freeform_mess","info_mess","advice_mess","token","token_list",
"directive","directive_list","action","bmove_token_set","bmove_token_list","condition",
"condition_list","region","region_list","point","team","unum_set","unum_list",
"unum_function","num","value", NULL
};
#endif

static const short yyr1[] = {     0,
    77,    77,    77,    77,    77,    78,    79,    79,    80,    81,
    82,    82,    83,    83,    83,    83,    83,    84,    85,    86,
    87,    87,    88,    88,    89,    89,    89,    90,    90,    91,
    91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
    91,    91,    91,    91,    92,    93,    93,    94,    94,    94,
    94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
    94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
    94,    94,    95,    95,    96,    96,    96,    96,    96,    96,
    97,    97,    98,    98,    98,    98,    98,    98,    98,    98,
    99,    99,    99,   100,   100,   100,   101,   101,   101,   102,
   102,   102,   102,   103,   103,   104,   104
};

static const short yyr2[] = {     0,
     1,     1,     1,     1,     1,     4,     2,     1,     4,     4,
     2,     1,     5,     5,     5,     5,     5,     4,     4,     4,
     5,     3,     2,     1,     6,     6,     1,     2,     1,     5,
     4,     4,     5,     4,     4,     4,     4,     4,     4,     5,
     5,     4,     3,     1,     3,     2,     1,     3,     3,     8,
     4,     5,     4,     4,     4,     4,     6,     4,     4,     6,
     5,     5,     5,     5,     5,     5,     5,     5,     4,     5,
     5,     1,     2,     1,     1,     3,     7,     8,     4,     1,
     2,     1,     5,     3,     5,     5,     6,     4,     5,     3,
     1,     3,     5,     3,     1,     1,     2,     2,     0,     1,
     4,     5,     6,     1,     1,     1,     1
};

static const short yydefact[] = {     0,
     0,     1,     5,     2,     3,     4,     0,     0,     0,     0,
     0,     0,     0,     8,     0,     0,    24,     0,     0,     0,
     0,    12,     0,     6,     7,    18,     0,     0,    19,    23,
    20,     0,     0,     0,     0,     0,    10,    11,   104,   105,
     0,    72,     0,     0,    22,     0,     0,     0,     0,     0,
     9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    27,     0,    29,     0,     0,     0,    80,     0,
     0,    75,    44,     0,     0,     0,     0,    48,    49,     0,
    91,     0,     0,     0,    74,     0,     0,     0,     0,     0,
   106,   107,     0,     0,     0,     0,     0,     0,   100,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    21,    28,
    13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    16,    17,     0,     0,    95,    99,     0,    96,
    51,     0,    54,    73,    55,    56,    53,     0,    58,    59,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    69,     0,     0,     0,     0,    90,    76,    84,
     0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    43,    92,     0,     0,     0,    52,     0,     0,    61,
    64,    65,     0,     0,     0,    66,    67,    62,    63,    68,
    70,    71,     0,     0,     0,     0,    88,     0,     0,    79,
    81,     0,     0,    31,     0,    32,     0,     0,    34,    35,
    37,     0,    36,    38,    39,     0,    42,     0,    97,    94,
    98,     0,    57,    60,   101,     0,     0,     0,     0,     0,
     0,    89,    83,     0,    85,    86,    30,    47,     0,    33,
    40,    41,    93,     0,   102,     0,    25,    26,     0,     0,
    87,    45,    46,    50,   103,    77,     0,    78,     0,     0,
     0
};

static const short yydefgoto[] = {   289,
     2,    13,    14,     3,    21,    22,     4,     5,     6,    17,
    18,    75,    76,    85,   238,   269,    95,    96,    81,   187,
    82,    92,   149,   205,   150,   102,   103
};

static const short yypact[] = {     4,
   202,-32768,-32768,-32768,-32768,-32768,    11,    16,    35,    35,
    53,    28,    26,-32768,    20,     5,-32768,    56,    93,   133,
   154,-32768,    21,-32768,-32768,-32768,   163,    60,-32768,-32768,
-32768,    64,    75,    81,   111,   129,-32768,-32768,-32768,-32768,
   131,-32768,   210,   199,-32768,   163,   199,   204,   212,   130,
-32768,   137,   147,     0,   204,     0,   163,   163,   163,   101,
     0,    41,    41,     0,    41,    58,    41,    41,   166,    41,
     0,     0,-32768,    -9,-32768,   105,   167,   171,-32768,    99,
   176,-32768,-32768,   168,   182,    18,   216,-32768,-32768,   156,
-32768,   127,   226,   127,-32768,   151,   159,   227,   228,   127,
-32768,-32768,   229,   230,   127,    41,   139,   118,-32768,   140,
    41,    41,   214,   232,   127,   127,     0,     0,-32768,-32768,
-32768,-32768,   233,   234,   235,   130,   130,   149,   204,   130,
   130,-32768,   204,   204,   123,   127,   123,   204,   240,   239,
    21,   238,-32768,-32768,   249,     0,-32768,-32768,   243,-32768,
-32768,   263,-32768,-32768,-32768,-32768,-32768,   212,-32768,-32768,
   208,   265,   266,   267,     0,     0,     0,   268,   269,   270,
   271,   272,-32768,   273,   274,   127,   127,-32768,-32768,-32768,
   130,    21,   275,   127,    21,-32768,   186,   130,   130,    67,
   276,    33,   277,   279,   280,     8,   281,   282,   283,   286,
   285,-32768,-32768,   127,    31,   290,-32768,   287,   288,-32768,
-32768,-32768,   289,   127,     0,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   212,   212,   130,    21,-32768,   291,   221,-32768,
-32768,   292,   293,-32768,   294,-32768,   250,   295,-32768,-32768,
-32768,   296,-32768,-32768,-32768,   297,-32768,   298,-32768,-32768,
-32768,   204,-32768,-32768,-32768,   299,   127,   300,   301,   130,
    21,-32768,-32768,   302,-32768,-32768,-32768,-32768,    59,-32768,
-32768,-32768,-32768,   303,-32768,   304,-32768,-32768,   305,    21,
-32768,-32768,-32768,-32768,-32768,-32768,   306,-32768,   314,   315,
-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,   284,-32768,-32768,   307,-32768,-32768,-32768,   213,
   308,   -16,-32768,  -128,-32768,-32768,   -24,   258,   -50,-32768,
   -49,   -52,   -87,-32768,   -64,   -23,   114
};


#define	YYLAST		328


static const short yytable[] = {    41,
    87,   110,    44,    94,    93,    90,   152,    27,   100,     1,
   147,   105,   158,   108,   241,   148,    12,   161,   115,   116,
    15,    77,   123,    39,    40,   117,    26,   174,   175,   208,
    78,    12,    24,   249,    98,    91,   108,   123,   125,   250,
    16,   128,   164,    39,    40,   169,    23,   194,   195,   197,
   118,    28,   124,   125,   126,   127,   128,   129,    20,   120,
   107,    16,    29,   108,   176,   177,    45,   282,    46,    39,
    40,   154,   154,   234,   109,   184,   181,   182,   186,    47,
   188,   189,   190,   191,   193,    48,   196,   198,   223,   224,
   130,   131,   283,   204,   258,   259,   228,   109,    16,    31,
   165,   166,   167,   123,   185,   130,   131,   101,   242,    73,
    74,   119,   213,   214,   215,    49,   248,   201,   124,   125,
   126,   127,   128,   129,   109,   147,   256,    79,   192,   147,
   148,   225,   108,    50,   148,    86,   231,    51,   232,   233,
   251,   163,   168,    88,   108,   108,    99,    32,    33,    34,
    35,    39,    40,    89,    90,    42,    43,   153,   226,    20,
    37,   229,   257,    42,    43,   155,   235,    42,    43,   276,
   113,   130,   131,   121,   183,   260,   104,   122,   106,   264,
   111,   112,   132,   114,    91,   165,   166,   167,   143,   109,
    79,    80,   230,   109,   133,   134,   135,   136,   137,   138,
   139,   274,   261,    73,    74,   109,   109,    36,    79,    80,
   279,     7,     8,     9,    10,    11,    83,    84,   172,   162,
   145,   146,   144,   140,   170,   171,    86,   263,   141,   142,
    30,    30,   151,   156,   157,   159,   160,   280,   173,   178,
   179,   180,   199,   200,   202,   206,    52,    53,    54,    55,
    56,    57,    58,    59,    60,   203,   287,    61,    62,    63,
    64,    65,    66,    67,    68,    69,    70,    71,    72,   207,
   209,   210,   211,   212,   216,   217,   218,   219,   220,   221,
   222,   227,   236,   268,   237,   239,   240,   243,   244,   245,
   246,   247,   252,   253,   254,   255,    25,   262,   265,   266,
   267,   270,   271,   272,   273,   275,   277,   278,   281,   284,
   285,   286,   288,   290,   291,    97,     0,    19,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    38
};

static const short yycheck[] = {    23,
    50,    66,    27,    56,    55,     6,    94,     3,    61,     6,
     3,    64,   100,     6,     7,     8,     6,   105,    71,    72,
     5,    46,     5,     3,     4,    35,     7,   115,   116,   158,
    47,     6,     7,     3,    59,    36,     6,     5,    21,     9,
     6,    24,   107,     3,     4,   110,    19,   135,   136,   137,
    60,    47,    20,    21,    22,    23,    24,    25,     6,    76,
     3,     6,     7,     6,   117,   118,     7,     9,     5,     3,
     4,    96,    97,     7,    67,   128,   126,   127,   129,     5,
   130,   131,   133,   134,   135,     5,   137,   138,   176,   177,
    73,    74,    34,   146,   223,   224,   184,    67,     6,     7,
    68,    69,    70,     5,   128,    73,    74,    67,   196,     5,
     6,     7,   165,   166,   167,     5,   204,   141,    20,    21,
    22,    23,    24,    25,    67,     3,   214,     5,     6,     3,
     8,   181,     6,     5,     8,     6,   187,     7,   188,   189,
   205,     3,     3,     7,     6,     6,    46,    15,    16,    17,
    18,     3,     4,     7,     6,     5,     6,     7,   182,     6,
     7,   185,   215,     5,     6,     7,   190,     5,     6,   257,
     5,    73,    74,     7,    26,   225,    63,     7,    65,   229,
    67,    68,     7,    70,    36,    68,    69,    70,     7,    67,
     5,     6,     7,    67,    27,    28,    29,    30,    31,    32,
    33,   252,   226,     5,     6,    67,    67,    75,     5,     6,
   260,    10,    11,    12,    13,    14,     5,     6,     5,   106,
    65,    66,     7,    56,   111,   112,     6,     7,    61,    62,
    18,    19,     7,     7,     7,     7,     7,   261,     7,     7,
     7,     7,     3,     5,     7,     3,    37,    38,    39,    40,
    41,    42,    43,    44,    45,     7,   280,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,     7,
    63,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,    34,     8,     7,     7,     7,     7,     7,
     5,     7,     3,     7,     7,     7,    13,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     0,     0,    58,    -1,    10,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21
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
#line 238 "coach_lang.y"
{
		yyval.g_pMess = yyvsp[0].g_pMess;
		g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = yyvsp[0].g_pMess;
	;
    break;}
case 2:
#line 247 "coach_lang.y"
{
		yyval.g_pMess = yyvsp[0].g_pMess;
		g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = yyvsp[0].g_pMess;
	;
    break;}
case 3:
#line 256 "coach_lang.y"
{
		yyval.g_pMess = yyvsp[0].g_pMess;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = yyvsp[0].g_pMess;
	;
    break;}
case 4:
#line 265 "coach_lang.y"
{
		yyval.g_pMess = yyvsp[0].g_pMess;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = yyvsp[0].g_pMess;
	;
    break;}
case 5:
#line 274 "coach_lang.y"
{
		yyval.g_pMess = yyvsp[0].g_pMess;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = yyvsp[0].g_pMess;
	;
    break;}
case 6:
#line 291 "coach_lang.y"
{
		yyval.g_pMess = new CM_MetaMessage(*yyvsp[-1].pvpMetaTok);
		//delete $3;
		//g_GC.remove($3, GCE_pvpMetaTok);
		//g_GC.add($$, GCE_pMess);
	;
    break;}
case 7:
#line 301 "coach_lang.y"
{
		yyvsp[-1].pvpMetaTok->push_back(yyvsp[0].pMetaTok);
		yyval.pvpMetaTok = yyvsp[-1].pvpMetaTok;
		//g_GC.remove($2, GCE_pMetaTok);
	;
    break;}
case 8:
#line 307 "coach_lang.y"
{
		yyval.pvpMetaTok = new vector<CM_MetaToken*>;
		yyval.pvpMetaTok->push_back(yyvsp[0].pMetaTok);
		//g_GC.add($$, GCE_pvpMetaTok);
		//g_GC.remove($1, GCE_pMetaTok);
	;
    break;}
case 9:
#line 317 "coach_lang.y"
{
		yyval.pMetaTok = new CM_MetaToken_Version(yyvsp[-1].dnum);
		//g_GC.add($$, GCE_pMetaTok);
	;
    break;}
case 10:
#line 330 "coach_lang.y"
{
		yyval.g_pMess = new CM_DefineMessage(*yyvsp[-1].pvpDefTok);
		//delete $3;
		//g_GC.remove($3, GCE_pvpDefTok);
		//g_GC.add($$, GCE_pMess);
	;
    break;}
case 11:
#line 340 "coach_lang.y"
{
		yyvsp[-1].pvpDefTok->push_back(yyvsp[0].pDefTok);
		yyval.pvpDefTok = yyvsp[-1].pvpDefTok;
		//g_GC.remove($2, GCE_pDefTok);
	;
    break;}
case 12:
#line 346 "coach_lang.y"
{
		yyval.pvpDefTok = new vector<CM_DefineToken*>;
		yyval.pvpDefTok->push_back(yyvsp[0].pDefTok);
		//g_GC.add($$, GCE_pvpDefTok);
		//g_GC.remove($1, GCE_pDefTok);
	;
    break;}
case 13:
#line 356 "coach_lang.y"
{
		yyval.pDefTok = new CM_DefTokCondition(strip_quotes(yyvsp[-2].str), yyvsp[-1].pCond);
 		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pCond);
	;
    break;}
case 14:
#line 364 "coach_lang.y"
{
		yyval.pDefTok = new CM_DefTokDirective(strip_quotes(yyvsp[-2].str), yyvsp[-1].pDir);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pDir);
	;
    break;}
case 15:
#line 372 "coach_lang.y"
{
		yyval.pDefTok = new CM_DefTokRegion(strip_quotes(yyvsp[-2].str), yyvsp[-1].pReg);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pReg);
	;
    break;}
case 16:
#line 380 "coach_lang.y"
{
		yyval.pDefTok = new CM_DefTokAction(strip_quotes(yyvsp[-2].str), yyvsp[-1].pAct);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pAct);
	;
    break;}
case 17:
#line 389 "coach_lang.y"
{
		/* SFLS */
		yyval.pDefTok = new SFLS_DefTokPoint(strip_quotes(yyvsp[-2].str), yyvsp[-1].pRegPt);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pAct);
	;
    break;}
case 18:
#line 406 "coach_lang.y"
{
		yyval.g_pMess = new CM_FreeformMessage(strip_quotes(yyvsp[-1].str));
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pMess);
	;
    break;}
case 19:
#line 421 "coach_lang.y"
{
		yyval.g_pMess = new CM_InfoMessage(*yyvsp[-1].pvpTok);
		//delete $3;
		//g_GC.remove($3, GCE_pvpTok);
		//g_GC.add($$, GCE_pMess);
	;
    break;}
case 20:
#line 436 "coach_lang.y"
{
		yyval.g_pMess = new CM_AdviceMessage(*yyvsp[-1].pvpTok);
		//delete $3;
		//g_GC.remove($3, GCE_pvpTok);
		//g_GC.add($$, GCE_pMess);
	;
    break;}
case 21:
#line 451 "coach_lang.y"
{
		yyval.pTok = new CM_TokRule(yyvsp[-3].inum, yyvsp[-2].pCond, *yyvsp[-1].pvpDir);
		//delete $4;
		//g_GC.add($$, GCE_pTok);
		//g_GC.remove($3, GCE_pCond);
		//g_GC.remove($4, GCE_pvpDir);
	;
    break;}
case 22:
#line 459 "coach_lang.y"
{
		yyval.pTok = new CM_TokClear();
		//g_GC.add($$, GCE_pTok);
	;
    break;}
case 23:
#line 467 "coach_lang.y"
{
		yyvsp[-1].pvpTok->push_back(yyvsp[0].pTok);
		yyval.pvpTok = yyvsp[-1].pvpTok;
		//g_GC.remove($2, GCE_pTok);
	;
    break;}
case 24:
#line 473 "coach_lang.y"
{
		yyval.pvpTok = new vector<CM_Token*>;
		yyval.pvpTok->push_back(yyvsp[0].pTok);
		//g_GC.add($$, GCE_pvpTok);
		//g_GC.remove($1, GCE_pTok);
	;
    break;}
case 25:
#line 488 "coach_lang.y"
{
		/* SFLS */
		yyval.pDir = new SFLS_DirCommand((bool)yyvsp[-4].inum, yyvsp[-3].pTeam, yyvsp[-2].pUSet, yyvsp[-1].pAct, false);
		//delete $4;
		//g_GC.add($$, GCE_pDir);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($5, GCE_pAct);
	;
    break;}
case 26:
#line 497 "coach_lang.y"
{
		/* SFLS */
		yyval.pDir = new SFLS_DirCommand((bool)yyvsp[-4].keyword, yyvsp[-3].pTeam, yyvsp[-2].pUSet, yyvsp[-1].pAct, true);
		//delete $4;
		//g_GC.add($$, GCE_pDir);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($5, GCE_pAct);
	;
    break;}
case 27:
#line 506 "coach_lang.y"
{
		yyval.pDir = new CM_DirNamed(strip_quotes(yyvsp[0].str));
		//g_GC.add($$, GCE_pDir);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	;
    break;}
case 28:
#line 516 "coach_lang.y"
{
		yyvsp[-1].pvpDir->push_back(yyvsp[0].pDir);
		yyval.pvpDir = yyvsp[-1].pvpDir;
		//g_GC.remove($2, GCE_pDir);
	;
    break;}
case 29:
#line 522 "coach_lang.y"
{
		yyval.pvpDir = new vector<CM_Directive*>;
		yyval.pvpDir->push_back(yyvsp[0].pDir);
		//g_GC.add($$, GCE_pvpDir);
		//g_GC.remove($1, GCE_pDir);
	;
    break;}
case 30:
#line 537 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActPosition(yyvsp[-2].pReg, yyvsp[-1].dnum);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 31:
#line 544 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new CM_ActPosition(yyvsp[-1].pReg);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 32:
#line 551 "coach_lang.y"
{
		yyval.pAct = new CM_ActHome(yyvsp[-1].pReg);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 33:
#line 557 "coach_lang.y"
{
		yyval.pAct = new CM_ActBallToRegion(yyvsp[-2].pReg, *yyvsp[-1].pBMTSet);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
		//g_GC.remove($4, GCE_pBMTSet);
	;
    break;}
case 34:
#line 564 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActBallToPlayer(yyvsp[-1].pUSet);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	;
    break;}
case 35:
#line 572 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActMark(yyvsp[-1].pUSet);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	;
    break;}
case 36:
#line 580 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActMarkLinePlayer(yyvsp[-1].pUSet);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	;
    break;}
case 37:
#line 588 "coach_lang.y"
{
		yyval.pAct = new CM_ActMarkLineRegion(yyvsp[-1].pReg);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 38:
#line 594 "coach_lang.y"
{
		yyval.pAct = new CM_ActOffsidesLine(yyvsp[-1].pReg);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 39:
#line 600 "coach_lang.y"
{
		yyval.pAct = new CM_ActHetType(yyvsp[-1].inum);
		//g_GC.add($$, GCE_pAct);
	;
    break;}
case 40:
#line 605 "coach_lang.y"
{
		/* SFLS Reihenfolge !!!*/
		yyval.pAct = new SFLS_ActMarkRegionPlayer(yyvsp[-1].pUSet, yyvsp[-2].pReg);
	;
    break;}
case 41:
#line 610 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActState(strip_quotes(yyvsp[-2].str), strip_quotes(yyvsp[-1].str));
		//delete $3;
		//delete $4;
	;
    break;}
case 42:
#line 617 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActInterceptBall(yyvsp[-1].dnum);
  	;
    break;}
case 43:
#line 622 "coach_lang.y"
{
		/* SFLS */
		yyval.pAct = new SFLS_ActCatchBall();
	;
    break;}
case 44:
#line 627 "coach_lang.y"
{
		yyval.pAct = new CM_ActNamed(strip_quotes(yyvsp[0].str));
		//g_GC.add($$, GCE_pAct);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	;
    break;}
case 45:
#line 637 "coach_lang.y"
{
		yyval.pBMTSet = yyvsp[-1].pBMTSet;
	;
    break;}
case 46:
#line 644 "coach_lang.y"
{
		yyval.pBMTSet = yyvsp[-1].pBMTSet;
		yyval.pBMTSet->addToken((BallMoveToken)yyvsp[0].inum);
	;
    break;}
case 47:
#line 649 "coach_lang.y"
{
		yyval.pBMTSet = new BallMoveTokenSet((BallMoveToken)yyvsp[0].inum);
		//g_GC.add($$, GCE_pBMTSet);
	;
    break;}
case 48:
#line 662 "coach_lang.y"
{
		yyval.pCond = new CM_CondTrue();
		//g_GC.add($$, GCE_pCond);
	;
    break;}
case 49:
#line 667 "coach_lang.y"
{
		yyval.pCond = new CM_CondFalse();
		//g_GC.add($$, GCE_pCond);
	;
    break;}
case 50:
#line 672 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondPlayerPosition(yyvsp[-5].pTeam, yyvsp[-4].pUSet, yyvsp[-3].inum, yyvsp[-2].inum, yyvsp[-1].pReg);
		//delete $4;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($7, GCE_pReg);
	;
    break;}
case 51:
#line 681 "coach_lang.y"
{
		yyval.pCond = new CM_CondBallPosition(yyvsp[-1].pReg);
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 52:
#line 687 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondBallOwner(yyvsp[-2].pTeam, yyvsp[-1].pUSet);
		//delete $4;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($4, GCE_pUSet);
	;
    break;}
case 53:
#line 695 "coach_lang.y"
{
		yyval.pCond = new CM_CondPlayMode((CM_PlayModeType)yyvsp[-1].inum);
		//g_GC.add($$, GCE_pCond);
	;
    break;}
case 54:
#line 700 "coach_lang.y"
{
		yyval.pCond = new CM_CondAnd(*yyvsp[-1].pvpCond);
		//delete $3;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pvpCond);
	;
    break;}
case 55:
#line 707 "coach_lang.y"
{
		yyval.pCond = new CM_CondOr(*yyvsp[-1].pvpCond);
		//delete $3;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pvpCond);
	;
    break;}
case 56:
#line 714 "coach_lang.y"
{
		yyval.pCond = new CM_CondNot(yyvsp[-1].pCond);
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pCond);
	;
    break;}
case 57:
#line 720 "coach_lang.y"
{
		/* SFLS */
     		yyval.pCond = new SFLS_CondAction(yyvsp[-3].pTeam, yyvsp[-2].pUSet, yyvsp[-1].pAct);
	;
    break;}
case 58:
#line 725 "coach_lang.y"
{
		/* SFLS */
        	yyval.pCond = new SFLS_CondTime(yyvsp[-1].pDoV);
  	;
    break;}
case 59:
#line 730 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondGoalDiff(yyvsp[-1].pDoV);
  	;
    break;}
case 60:
#line 735 "coach_lang.y"
{
		/* SFLS */
 		yyval.pCond = new SFLS_CondStamina(yyvsp[-3].pTeam, yyvsp[-2].pUSet, (SFLS_Level)yyvsp[-1].inum);
  	;
    break;}
case 61:
#line 740 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondEqual(yyvsp[-2].pDoV, yyvsp[-1].pDoV);
	;
    break;}
case 62:
#line 745 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondLess(yyvsp[-2].pDoV, yyvsp[-1].pDoV);
  	;
    break;}
case 63:
#line 750 "coach_lang.y"
{
		/* SFLS */
 		yyval.pCond = new SFLS_CondGreater(yyvsp[-2].pDoV, yyvsp[-1].pDoV);
  	;
    break;}
case 64:
#line 755 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondEqualUnum(yyvsp[-2].inum, yyvsp[-1].inum);
	;
    break;}
case 65:
#line 760 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondEqualUnum(yyvsp[-2].inum, yyvsp[-1].pFunc);
	;
    break;}
case 66:
#line 765 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondEqualUnum(yyvsp[-2].pFunc, yyvsp[-1].inum);
	;
    break;}
case 67:
#line 770 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondEqualUnum(yyvsp[-2].pFunc, yyvsp[-1].pFunc);
	;
    break;}
case 68:
#line 775 "coach_lang.y"
{
		/* SFLS */
  		yyval.pCond = new SFLS_CondState(strip_quotes(yyvsp[-2].str), strip_quotes(yyvsp[-1].str));
		//delete $3;
		//delete $4;
  	;
    break;}
case 69:
#line 782 "coach_lang.y"
{
		/* SFLS */
        	yyval.pCond = new SFLS_CondBallVelocity(yyvsp[-1].pDoV);
  	;
    break;}
case 70:
#line 787 "coach_lang.y"
{
		/* SFLS */
       	 	yyval.pCond = new SFLS_CondBallInterceptable(yyvsp[-2].pTeam, yyvsp[-1].pUSet);
  	;
    break;}
case 71:
#line 792 "coach_lang.y"
{
		/* SFLS */
		yyval.pCond = new SFLS_CondBallCatchable(yyvsp[-2].pTeam, yyvsp[-1].pUSet);
  	;
    break;}
case 72:
#line 797 "coach_lang.y"
{
		yyval.pCond = new CM_CondNamed(strip_quotes(yyvsp[0].str));
		//g_GC.add($$, GCE_pCond);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	;
    break;}
case 73:
#line 807 "coach_lang.y"
{
		yyvsp[-1].pvpCond->push_back(yyvsp[0].pCond);
		yyval.pvpCond = yyvsp[-1].pvpCond;
		//g_GC.remove($2, GCE_pCond);
	;
    break;}
case 74:
#line 813 "coach_lang.y"
{
		yyval.pvpCond = new vector<CM_Condition*>;
		yyval.pvpCond->push_back(yyvsp[0].pCond);
		//g_GC.remove($1, GCE_pCond);
		//g_GC.add($$, GCE_pvpCond);
	;
    break;}
case 75:
#line 828 "coach_lang.y"
{
		yyval.pReg = yyvsp[0].pRegPt;
	;
    break;}
case 76:
#line 832 "coach_lang.y"
{
		yyval.pReg = new RegNull;
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 77:
#line 837 "coach_lang.y"
{
		yyval.pReg = new RegQuad(yyvsp[-4].pRegPt, yyvsp[-3].pRegPt, yyvsp[-2].pRegPt, yyvsp[-1].pRegPt);
		//g_GC.add($$, GCE_pReg);
		//g_GC.remove($3, GCE_pReg);
		//g_GC.remove($4, GCE_pReg);
		//g_GC.remove($5, GCE_pReg);
		//g_GC.remove($6, GCE_pReg);
	;
    break;}
case 78:
#line 846 "coach_lang.y"
{
		yyval.pReg = new RegArc(yyvsp[-5].pRegPt, yyvsp[-4].dnum, yyvsp[-3].dnum, yyvsp[-2].dnum, yyvsp[-1].dnum);
		//g_GC.add($$, GCE_pReg);
		//g_GC.remove($3, GCE_pReg);
	;
    break;}
case 79:
#line 852 "coach_lang.y"
{
		yyval.pReg = new RegUnion(*yyvsp[-1].pvpReg);
		//g_GC.add($$, GCE_pReg);
		//delete $3;
		//g_GC.remove($3, GCE_pvpReg);
	;
    break;}
case 80:
#line 859 "coach_lang.y"
{
		yyval.pReg = new RegNamed(strip_quotes(yyvsp[0].str));
		//g_GC.add($$, GCE_pReg);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	;
    break;}
case 81:
#line 869 "coach_lang.y"
{
		yyvsp[-1].pvpReg->push_back(yyvsp[0].pReg);
		yyval.pvpReg = yyvsp[-1].pvpReg;
		//g_GC.remove($2, GCE_pReg);
	;
    break;}
case 82:
#line 875 "coach_lang.y"
{
		yyval.pvpReg = new vector<Region*>;
		yyval.pvpReg->push_back(yyvsp[0].pReg);
		//g_GC.remove($1, GCE_pReg);
		//g_GC.add($$, GCE_pvpReg);
	;
    break;}
case 83:
#line 885 "coach_lang.y"
{
		yyval.pRegPt = new RegPointSimple(yyvsp[-2].dnum, yyvsp[-1].dnum);
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 84:
#line 890 "coach_lang.y"
{
		yyval.pRegPt = new SFLS_RegHome;
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 85:
#line 895 "coach_lang.y"
{
		yyval.pRegPt = new SFLS_PlusPoint(yyvsp[-2].pRegPt, yyvsp[-1].pRegPt);
	;
    break;}
case 86:
#line 899 "coach_lang.y"
{
		yyval.pRegPt = new SFLS_MultPoint(yyvsp[-2].pRegPt, yyvsp[-1].pRegPt);
	;
    break;}
case 87:
#line 903 "coach_lang.y"
{
		yyval.pRegPt = new RegPointRelative(yyvsp[-3].dnum, yyvsp[-2].dnum, yyvsp[-1].pRegPt);
		//g_GC.remove($5, GCE_pReg);
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 88:
#line 909 "coach_lang.y"
{
		yyval.pRegPt = new RegPointBall();
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 89:
#line 914 "coach_lang.y"
{
		yyval.pRegPt = new SFLS_RegPointPlayer(yyvsp[-2].pTeam, yyvsp[-1].pUSet);
		//g_GC.add($$, GCE_pReg);
	;
    break;}
case 90:
#line 919 "coach_lang.y"
{
		yyval.pRegPt = new SFLS_PointNamed(strip_quotes(yyvsp[-1].str));
	;
    break;}
case 91:
#line 932 "coach_lang.y"
{
		/* SFLS */
		yyval.pTeam = new SFLS_Team(Constant, yyvsp[0].inum);
  	;
    break;}
case 92:
#line 937 "coach_lang.y"
{
		/* SFLS */
		yyval.pTeam = new SFLS_Team(yyvsp[-1].inum);
 	;
    break;}
case 93:
#line 942 "coach_lang.y"
{
		/* SFLS */
		yyval.pTeam = new SFLS_Team((SFLS_TeamFunctionType)yyvsp[-3].inum, yyvsp[-2].pTeam, yyvsp[-1].pUSet);
 	;
    break;}
case 94:
#line 958 "coach_lang.y"
{
		yyval.pUSet = yyvsp[-1].pUSet;
	;
    break;}
case 95:
#line 962 "coach_lang.y"
{
		yyval.pUSet = new SFLS_UnumSet();
		yyval.pUSet->addNum(yyvsp[0].inum);
	;
    break;}
case 96:
#line 967 "coach_lang.y"
{
		yyval.pUSet = new SFLS_UnumSet();
		yyval.pUSet->addSFLSFunction(yyvsp[0].pFunc);
  	;
    break;}
case 97:
#line 975 "coach_lang.y"
{
		yyval.pUSet = yyvsp[-1].pUSet;
		yyval.pUSet->addNum(yyvsp[0].inum);
	;
    break;}
case 98:
#line 980 "coach_lang.y"
{
	 	/* SFLS */
		yyval.pUSet = yyvsp[-1].pUSet;
		yyval.pUSet->addSFLSFunction(yyvsp[0].pFunc);
	;
    break;}
case 99:
#line 986 "coach_lang.y"
{
		yyval.pUSet = new SFLS_UnumSet();
		//g_GC.add($$, GCE_pUSet);
	;
    break;}
case 100:
#line 999 "coach_lang.y"
{
		/* SFLS */
		yyval.pFunc = new SFLS_Function(yyvsp[0].inum);
	;
    break;}
case 101:
#line 1004 "coach_lang.y"
{
		/* SFLS */
		yyval.pFunc = new SFLS_Function((SFLS_UnumFunctionType)yyvsp[-2].inum, yyvsp[-1].pTeam);
  	;
    break;}
case 102:
#line 1009 "coach_lang.y"
{
		/* SFLS */
		yyval.pFunc = new SFLS_Function((SFLS_UnumFunctionType)yyvsp[-3].inum, yyvsp[-2].pTeam, yyvsp[-1].pUSet);
  	;
    break;}
case 103:
#line 1014 "coach_lang.y"
{
		/* SFLS */
		yyval.pFunc = new SFLS_Function((SFLS_UnumFunctionType)yyvsp[-4].inum, yyvsp[-3].pTeam, yyvsp[-2].pTeam, yyvsp[-1].pUSet);
  	;
    break;}
case 104:
#line 1027 "coach_lang.y"
{
		yyval.dnum = (double)yyvsp[0].inum;
	;
    break;}
case 105:
#line 1031 "coach_lang.y"
{
		yyval.dnum = yyvsp[0].dnum;
	;
    break;}
case 106:
#line 1043 "coach_lang.y"
{
		/* SFLS */
		yyval.pDoV = new DoubleOrVariable(yyvsp[0].inum);
	;
    break;}
case 107:
#line 1048 "coach_lang.y"
{
		/* SFLS */
		yyval.pDoV = new DoubleOrVariable(yyvsp[0].dnum);
  	;
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
#line 1054 "coach_lang.y"


/* ADDITIONAL C CODE */

int yyerror (char* s)
{
	fprintf (stderr, "%s: next token %d\n", s, yychar);

	g_pMess = NULL;

	return 0;
}
