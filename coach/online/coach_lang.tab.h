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


extern YYSTYPE yylval;
