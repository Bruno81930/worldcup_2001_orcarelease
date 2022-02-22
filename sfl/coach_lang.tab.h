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


extern YYSTYPE yylval;
