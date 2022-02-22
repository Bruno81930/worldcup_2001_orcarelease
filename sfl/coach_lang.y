
/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
        Patrick Riley, Tom Howard, Itsuki Noda, Mikhail Prokopenko, Jan Wendler

    This file is part of SoccerServer.

    SoccerServer is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
*/

%{

#include "SFLShierarchy.h"
#include "coach_lang.h"
#include "garbage.h"

int yyerror(char*);
int yylex();

extern CM_Message* g_pMess;

extern GarbageCollector g_GC; //the garbage collector

%}

/* BISON DECLARATIONS */

%union
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
}

/* Tokens */

%token <inum> INT
%token <dnum> REAL
%token <str>  STR

%token <separator> LP "("
%token <separator> RP ")"
%token <separator> LB "{"
%token <separator> RB "}"

%token <keyword> META
%token <keyword> FREEFORM
%token <keyword> INFO
%token <keyword> ADVICE

%token <keyword> DEFINE
%token <keyword> DEFINEC
%token <keyword> DEFINED
%token <keyword> DEFINER
%token <keyword> DEFINEA

/* meta stuff */
%token <keyword> VERSION

/* region stuff */
%token <keyword> NULL_REGION
/* TIMO: added HOME */
%token <keyword> HOME_REGION
%token <keyword> QUAD
%token <keyword> ARC
%token <keyword> PT
%token <keyword> REG
%token <keyword> BALL

/* action stuff */
%token <keyword> POS
%token <keyword> HOME
%token <keyword> BALLTO
%token <keyword> MARK
%token <keyword> MARK_LINE
%token <keyword> OFFLINE
%token <keyword> HTYPE
%token <inum>    BMOVE_TOKEN

/* directive stuff */
%token <inum>    DO_DONT
%token <inum>    TEAM

/* condition stuff */
%token <keyword> TRUE_KW
%token <keyword> FALSE_KW
%token <keyword> PPOS
%token <keyword> BPOS
%token <keyword> BOWNER
%token <keyword> AND
%token <keyword> OR
%token <keyword> NOT
%token <keyword> PLAY_MODE_KW
%token <inum>    PLAY_MODE

%token <keyword> CLEAR


/* SFLS */
%token <keyword> ACTION
%token <keyword> TIME
%token <keyword> GOALDIFF
%token <keyword> STAMINA
%token <keyword> EQUAL
%token <keyword> EQUALUNUM
%token <keyword> LESS
%token <keyword> GREATER
%token <keyword> STATE
%token <keyword> BALLVELOCITY
%token <keyword> BALLINTERCEPTABLE
%token <keyword> BALLCATCHABLE
%token <keyword> FORCE
%token <keyword> INTERCEPTBALL
%token <keyword> CATCHBALL
%token <inum>    LEVEL
%token <inum>    PASSMODE
%token <inum>    TEAMFUNCTION
%token <inum>    TEAMFUNCTION_TU
%token <inum>    VAR
%token <inum>    UNUMFUNCTION_T
%token <inum>    UNUMFUNCTION_TU
%token <inum>    UNUMFUNCTION_TTU
%token <keyword> YOURHOME
%token <keyword> YOURADJUSTEDHOME
%token <keyword> PLUSPOINT
%token <keyword> MULTPOINT
%token <keyword> DEFINEP
/* SFLS end */

/* the ERROR token is an error from lex and is never used below */
%token <inum>    ERROR



%type <g_pMess> message;

%type <g_pMess> meta_mess;
%type <pMetaTok> meta_token;
%type <pvpMetaTok> meta_token_list;

%type <g_pMess> define_mess;
%type <pDefTok> define_token;
%type <pvpDefTok> define_token_list;

%type <g_pMess> freeform_mess;

%type <g_pMess> info_mess;

%type <g_pMess> advice_mess;

%type <pReg> region;
%type <pRegPt> point;
%type <pvpReg> region_list;

%type <dnum> num;
%type <pUSet> unum_set;
%type <pUSet> unum_list;

%type <pBMTSet> bmove_token_set;
%type <pBMTSet> bmove_token_list;

%type <pAct> action;

%type <pDir> directive;
%type <pvpDir> directive_list;

%type <pCond> condition;
%type <pvpCond> condition_list;

%type <pTok> token;
%type <pvpTok> token_list;

%type <pTeam> team;
%type <pFunc> unum_function;
%type <pDoV> value;


%start message

%%

/* Grammar RULES */

/*
	message
*/

message
	: meta_mess
	{
		$$ = $1;
		g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = $1;
	}
	| freeform_mess
	{
		$$ = $1;
		g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = $1;
	}
	| info_mess
	{
		$$ = $1;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = $1;
	}
	| advice_mess
	{
		$$ = $1;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = $1;
	}
	| define_mess
	{
		$$ = $1;
		//g_GC.remove(g_pMess, GCE_pMess);
	/*
		if (g_pMess) delete g_pMess;
	*/
		g_pMess = $1;
	}
;

/*
	meta_mess
	(message)
*/

meta_mess
	: LP META meta_token_list RP
	{
		$$ = new CM_MetaMessage(*$3);
		//delete $3;
		//g_GC.remove($3, GCE_pvpMetaTok);
		//g_GC.add($$, GCE_pMess);
	}
;

meta_token_list
	: meta_token_list meta_token
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pMetaTok);
	}
	| meta_token
	{
		$$ = new vector<CM_MetaToken*>;
		$$->push_back($1);
		//g_GC.add($$, GCE_pvpMetaTok);
		//g_GC.remove($1, GCE_pMetaTok);
	}
;

meta_token
	: LP VERSION num RP
	{
		$$ = new CM_MetaToken_Version($3);
		//g_GC.add($$, GCE_pMetaTok);
	}
;

/*
	define_mess
	(message)
*/

define_mess
	: LP DEFINE define_token_list RP
	{
		$$ = new CM_DefineMessage(*$3);
		//delete $3;
		//g_GC.remove($3, GCE_pvpDefTok);
		//g_GC.add($$, GCE_pMess);
	}
;

define_token_list
	: define_token_list define_token
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pDefTok);
	}
	| define_token
	{
		$$ = new vector<CM_DefineToken*>;
		$$->push_back($1);
		//g_GC.add($$, GCE_pvpDefTok);
		//g_GC.remove($1, GCE_pDefTok);
	}
;

define_token
	: LP DEFINEC STR condition RP
	{
		$$ = new CM_DefTokCondition(strip_quotes($3), $4);
 		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pCond);
	}
	| LP DEFINED STR directive RP
	{
		$$ = new CM_DefTokDirective(strip_quotes($3), $4);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pDir);
	}
	| LP DEFINER STR region RP
	{
		$$ = new CM_DefTokRegion(strip_quotes($3), $4);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pReg);
	}
	| LP DEFINEA STR action RP
	{
		$$ = new CM_DefTokAction(strip_quotes($3), $4);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pAct);
	}
	// thesis: changed DEFINEP to semantically "define a point"
	| LP DEFINEP STR point RP
	{
		/* SFLS */
		$$ = new SFLS_DefTokPoint(strip_quotes($3), $4);
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pDefTok);
		//g_GC.remove($4, GCE_pAct);
	}
;

/*
	freeform_mess
	(message)
*/

freeform_mess
	: LP FREEFORM STR RP
	{
		$$ = new CM_FreeformMessage(strip_quotes($3));
		//delete $3;
		//g_GC.remove($3, GCE_pChar);
		//g_GC.add($$, GCE_pMess);
	}
;

/*
	info_mess
	(message)
*/

info_mess
	: LP INFO token_list RP
	{
		$$ = new CM_InfoMessage(*$3);
		//delete $3;
		//g_GC.remove($3, GCE_pvpTok);
		//g_GC.add($$, GCE_pMess);
	}
;

/*
	advice_mess
	(message)
*/

advice_mess
	: LP ADVICE token_list RP
	{
		$$ = new CM_AdviceMessage(*$3);
		//delete $3;
		//g_GC.remove($3, GCE_pvpTok);
		//g_GC.add($$, GCE_pMess);
	}
;

/*
	token_list
	(info_mess, advice_mess)
*/

token
	: LP INT condition directive_list RP
	{
		$$ = new CM_TokRule($2, $3, *$4);
		//delete $4;
		//g_GC.add($$, GCE_pTok);
		//g_GC.remove($3, GCE_pCond);
		//g_GC.remove($4, GCE_pvpDir);
	}
	| LP CLEAR RP
	{
		$$ = new CM_TokClear();
		//g_GC.add($$, GCE_pTok);
	}
;

token_list
	: token_list token
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pTok);
	}
	| token
	{
		$$ = new vector<CM_Token*>;
		$$->push_back($1);
		//g_GC.add($$, GCE_pvpTok);
		//g_GC.remove($1, GCE_pTok);
	}
;

/*
	directive_list
	(define_token, token)
*/

directive
	: LP DO_DONT team unum_set action RP
	{
		/* SFLS */
		$$ = new SFLS_DirCommand((bool)$2, $3, $4, $5, false);
		//delete $4;
		//g_GC.add($$, GCE_pDir);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($5, GCE_pAct);
	}
	| LP FORCE team unum_set action RP
	{
		/* SFLS */
		$$ = new SFLS_DirCommand((bool)$2, $3, $4, $5, true);
		//delete $4;
		//g_GC.add($$, GCE_pDir);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($5, GCE_pAct);
	}
 	| STR
	{
		$$ = new CM_DirNamed(strip_quotes($1));
		//g_GC.add($$, GCE_pDir);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	}
;

directive_list
	: directive_list directive
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pDir);
	}
	| directive
	{
		$$ = new vector<CM_Directive*>;
		$$->push_back($1);
		//g_GC.add($$, GCE_pvpDir);
		//g_GC.remove($1, GCE_pDir);
	}
;

/*
	action
	(directive, condition)
*/

action
	: LP POS region num RP
	{
		/* SFLS */
		$$ = new SFLS_ActPosition($3, $4);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP POS region RP
	{
		/* SFLS */
		$$ = new CM_ActPosition($3);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP HOME region RP
	{
		$$ = new CM_ActHome($3);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP BALLTO region bmove_token_set RP
	{
		$$ = new CM_ActBallToRegion($3, *$4);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
		//g_GC.remove($4, GCE_pBMTSet);
	}
	| LP BALLTO unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_ActBallToPlayer($3);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	}
	| LP MARK unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_ActMark($3);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	}
	| LP MARK_LINE unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_ActMarkLinePlayer($3);
		//delete $3;
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pUSet);
	}
	| LP MARK_LINE region RP
	{
		$$ = new CM_ActMarkLineRegion($3);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP OFFLINE region RP
	{
		$$ = new CM_ActOffsidesLine($3);
		//g_GC.add($$, GCE_pAct);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP HTYPE INT RP
	{
		$$ = new CM_ActHetType($3);
		//g_GC.add($$, GCE_pAct);
	}
	| LP MARK_LINE region unum_set RP
	{
		/* SFLS Reihenfolge !!!*/
		$$ = new SFLS_ActMarkRegionPlayer($4, $3);
	}
	| LP STATE STR STR RP
	{
		/* SFLS */
		$$ = new SFLS_ActState(strip_quotes($3), strip_quotes($4));
		//delete $3;
		//delete $4;
	}
	| LP INTERCEPTBALL num RP
	{
		/* SFLS */
		$$ = new SFLS_ActInterceptBall($3);
  	}
	| LP CATCHBALL RP
	{
		/* SFLS */
		$$ = new SFLS_ActCatchBall();
	}
	| STR
	{
		$$ = new CM_ActNamed(strip_quotes($1));
		//g_GC.add($$, GCE_pAct);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	}
;

bmove_token_set
	: LB bmove_token_list RB
 	{
		$$ = $2;
	}
;

bmove_token_list
	: bmove_token_list BMOVE_TOKEN
	{
		$$ = $1;
		$$->addToken((BallMoveToken)$2);
	}
	| BMOVE_TOKEN
	{
		$$ = new BallMoveTokenSet((BallMoveToken)$1);
		//g_GC.add($$, GCE_pBMTSet);
	}
;

/*
	condition
	(define_token, token)
*/

condition
	: LP TRUE_KW RP
	{
		$$ = new CM_CondTrue();
		//g_GC.add($$, GCE_pCond);
	}
	| LP FALSE_KW RP
	{
		$$ = new CM_CondFalse();
		//g_GC.add($$, GCE_pCond);
	}
	| LP PPOS team unum_set INT INT region RP
	{
		/* SFLS */
		$$ = new SFLS_CondPlayerPosition($3, $4, $5, $6, $7);
		//delete $4;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($4, GCE_pUSet);
		//g_GC.remove($7, GCE_pReg);
	}
	| LP BPOS region RP
	{
		$$ = new CM_CondBallPosition($3);
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP BOWNER team unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_CondBallOwner($3, $4);
		//delete $4;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($4, GCE_pUSet);
	}
	| LP PLAY_MODE_KW PLAY_MODE RP
	{
		$$ = new CM_CondPlayMode((CM_PlayModeType)$3);
		//g_GC.add($$, GCE_pCond);
	}
	| LP AND condition_list RP
	{
		$$ = new CM_CondAnd(*$3);
		//delete $3;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pvpCond);
	}
	| LP OR condition_list RP
	{
		$$ = new CM_CondOr(*$3);
		//delete $3;
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pvpCond);
	}
	| LP NOT condition RP
	{
		$$ = new CM_CondNot($3);
		//g_GC.add($$, GCE_pCond);
		//g_GC.remove($3, GCE_pCond);
	}
 	| LP ACTION team unum_set action RP
	{
		/* SFLS */
     		$$ = new SFLS_CondAction($3, $4, $5);
	}
	| LP TIME value RP
	{
		/* SFLS */
        	$$ = new SFLS_CondTime($3);
  	}
	| LP GOALDIFF value RP
	{
		/* SFLS */
		$$ = new SFLS_CondGoalDiff($3);
  	}
	| LP STAMINA team unum_set LEVEL RP
	{
		/* SFLS */
 		$$ = new SFLS_CondStamina($3, $4, (SFLS_Level)$5);
  	}
	| LP EQUAL value value RP
	{
		/* SFLS */
		$$ = new SFLS_CondEqual($3, $4);
	}
	| LP LESS value value RP
	{
		/* SFLS */
		$$ = new SFLS_CondLess($3, $4);
  	}
	| LP GREATER value value RP
	{
		/* SFLS */
 		$$ = new SFLS_CondGreater($3, $4);
  	}
	| LP EQUALUNUM INT INT RP
	{
		/* SFLS */
		$$ = new SFLS_CondEqualUnum($3, $4);
	}
	| LP EQUALUNUM INT unum_function RP
	{
		/* SFLS */
		$$ = new SFLS_CondEqualUnum($3, $4);
	}
	| LP EQUALUNUM unum_function INT RP
	{
		/* SFLS */
		$$ = new SFLS_CondEqualUnum($3, $4);
	}
	| LP EQUALUNUM unum_function unum_function RP
	{
		/* SFLS */
		$$ = new SFLS_CondEqualUnum($3, $4);
	}
	| LP STATE STR STR RP
	{
		/* SFLS */
  		$$ = new SFLS_CondState(strip_quotes($3), strip_quotes($4));
		//delete $3;
		//delete $4;
  	}
	| LP BALLVELOCITY value RP
	{
		/* SFLS */
        	$$ = new SFLS_CondBallVelocity($3);
  	}
	| LP BALLINTERCEPTABLE team unum_set RP
	{
		/* SFLS */
       	 	$$ = new SFLS_CondBallInterceptable($3, $4);
  	}
	| LP BALLCATCHABLE team unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_CondBallCatchable($3, $4);
  	}
	| STR
	{
		$$ = new CM_CondNamed(strip_quotes($1));
		//g_GC.add($$, GCE_pCond);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	}
;

condition_list
	: condition_list condition
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pCond);
	}
	| condition
	{
		$$ = new vector<CM_Condition*>;
		$$->push_back($1);
		//g_GC.remove($1, GCE_pCond);
		//g_GC.add($$, GCE_pvpCond);
	}
;

/*
	region
	(define_token, action, condition)
*/

region
	: point
	{
		$$ = $1;
	}
	| LP NULL_REGION RP
	{
		$$ = new RegNull;
		//g_GC.add($$, GCE_pReg);
	}
	| LP QUAD point point point point RP
	{
		$$ = new RegQuad($3, $4, $5, $6);
		//g_GC.add($$, GCE_pReg);
		//g_GC.remove($3, GCE_pReg);
		//g_GC.remove($4, GCE_pReg);
		//g_GC.remove($5, GCE_pReg);
		//g_GC.remove($6, GCE_pReg);
	}
	| LP ARC point num num num num RP
	{
		$$ = new RegArc($3, $4, $5, $6, $7);
		//g_GC.add($$, GCE_pReg);
		//g_GC.remove($3, GCE_pReg);
	}
	| LP REG region_list RP
	{
		$$ = new RegUnion(*$3);
		//g_GC.add($$, GCE_pReg);
		//delete $3;
		//g_GC.remove($3, GCE_pvpReg);
	}
	| STR
	{
		$$ = new RegNamed(strip_quotes($1));
		//g_GC.add($$, GCE_pReg);
		//delete $1;
		//g_GC.remove($1, GCE_pChar);
	}
;

region_list
	: region_list region
	{
		$1->push_back($2);
		$$ = $1;
		//g_GC.remove($2, GCE_pReg);
	}
	| region
	{
		$$ = new vector<Region*>;
		$$->push_back($1);
		//g_GC.remove($1, GCE_pReg);
		//g_GC.add($$, GCE_pvpReg);
	}
;

point
	: LP PT num num RP
	{
		$$ = new RegPointSimple($3, $4);
		//g_GC.add($$, GCE_pReg);
	}
	| LP HOME_REGION RP
	{
		$$ = new SFLS_RegHome;
		//g_GC.add($$, GCE_pReg);
	}
	| LP PLUSPOINT point point RP
	{
		$$ = new SFLS_PlusPoint($3, $4);
	}
	| LP MULTPOINT point point RP
	{
		$$ = new SFLS_MultPoint($3, $4);
	}
	| LP PT num num point RP
	{
		$$ = new RegPointRelative($3, $4, $5);
		//g_GC.remove($5, GCE_pReg);
		//g_GC.add($$, GCE_pReg);
	}
	| LP PT BALL RP
	{
		$$ = new RegPointBall();
		//g_GC.add($$, GCE_pReg);
	}
	| LP PT team unum_set RP
	{
		$$ = new SFLS_RegPointPlayer($3, $4);
		//g_GC.add($$, GCE_pReg);
	}
	| LP STR RP
	{
		$$ = new SFLS_PointNamed(strip_quotes($2));
	}

;

/*
	team
	(directive, condition, point)
*/

team
	: TEAM
	{
		/* SFLS */
		$$ = new SFLS_Team(Constant, $1);
  	}
	| LP TEAMFUNCTION RP
	{
		/* SFLS */
		$$ = new SFLS_Team($2);
 	}
	| LP TEAMFUNCTION_TU team unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_Team((SFLS_TeamFunctionType)$2, $3, $4);
 	}
;

/*
	unum_set
	(team, ...)

	wenn das unum_set nur eine unum enthaelt, koennen die Klammern
	weggelassen werden
*/

unum_set
	: LB unum_list RB
	{
		$$ = $2;
	}
	| INT
	{
		$$ = new SFLS_UnumSet();
		$$->addNum($1);
	}
	| unum_function
	{
		$$ = new SFLS_UnumSet();
		$$->addSFLSFunction($1);
  	}
;

unum_list
	: unum_list INT
	{
		$$ = $1;
		$$->addNum($2);
	}
	| unum_list unum_function
	{
	 	/* SFLS */
		$$ = $1;
		$$->addSFLSFunction($2);
	}
	| /*empty*/
	{
		$$ = new SFLS_UnumSet();
		//g_GC.add($$, GCE_pUSet);
	}
;

/*
	unum_function
	(condition, unum_set)
*/

unum_function
	: VAR
	{
		/* SFLS */
		$$ = new SFLS_Function($1);
	}
	| LP UNUMFUNCTION_T team RP
	{
		/* SFLS */
		$$ = new SFLS_Function((SFLS_UnumFunctionType)$2, $3);
  	}
	| LP UNUMFUNCTION_TU team unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_Function((SFLS_UnumFunctionType)$2, $3, $4);
  	}
	| LP UNUMFUNCTION_TTU team team unum_set RP
	{
		/* SFLS */
		$$ = new SFLS_Function((SFLS_UnumFunctionType)$2, $3, $4, $5);
  	}
;

/*
	num
	(meta_token, action, region, point, value)
*/

num
	: INT
	{
		$$ = (double)$1;
	}
	| REAL
	{
		$$ = $1;
	}
;

/*
	value
	(condition)
*/

value
	: VAR
	{
		/* SFLS */
		$$ = new DoubleOrVariable($1);
	}
	| num
	{
		/* SFLS */
		$$ = new DoubleOrVariable($1);
  	}
;

%%

/* ADDITIONAL C CODE */

int yyerror (char* s)
{
	fprintf (stderr, "%s: next token %d\n", s, yychar);

	g_pMess = NULL;

	return 0;
}
