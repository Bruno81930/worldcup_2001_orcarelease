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
#include "coach_lang.h"
#include "garbage.h"
int yyerror(char*);
int yylex();
extern CM_Message* g_pMess;
//extern CM_Directive* pTesting;
//extern CM_Condition* pTesting;
//extern CM_Token* pTesting;
extern GarbageCollector g_GC; //the garbage collector
%}
     
/* BISON DECLARATIONS */
%union {
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
}

%token <inum> INT
%token <dnum> REAL
%token <str>     STR
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
%token <inum> BMOVE_TOKEN
/* directive stuff */
%token <inum> DO_DONT
%token <inum> TEAM
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

/* to be deleted */

/* the ERROR token is an error from lex and is never used below */
%token <inum>    ERROR

%type <g_pMess> message;
%type <g_pMess> meta_mess;
%type <g_pMess> freeform_mess;
%type <g_pMess> info_mess;
%type <g_pMess> advice_mess;
%type <g_pMess> define_mess;
%type <pMetaTok> meta_token;
%type <pvpMetaTok> meta_token_list;
%type <pDefTok> define_token;
%type <pvpDefTok> define_token_list;
/* region stuff */
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

/*%type <pTok> temp;*/

%start message

%%
/*
temp : token { $$ = $1; pTesting = $1; }
;
*/
/* Grammar RULES */
message : meta_mess     { $$ = $1; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = $1; }
        | freeform_mess { $$ = $1; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = $1; }
        | info_mess     { $$ = $1; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = $1; }
        | advice_mess   { $$ = $1; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = $1; }
        | define_mess   { $$ = $1; g_GC.remove(g_pMess, GCE_pMess);
                          if (g_pMess) delete g_pMess; g_pMess = $1; }
;
/* meta messages */
meta_mess : LP META meta_token_list RP 
        { $$ = new CM_MetaMessage(*$3); delete $3; g_GC.remove($3, GCE_pvpMetaTok); 
          g_GC.add($$, GCE_pMess); }
;
meta_token_list : meta_token_list meta_token 
                { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pMetaTok); }
        | meta_token { $$ = new vector<CM_MetaToken*>;$$->push_back($1); 
                       g_GC.add($$, GCE_pvpMetaTok); g_GC.remove($1, GCE_pMetaTok); }
;
meta_token : LP VERSION num RP { $$ = new CM_MetaToken_Version($3); g_GC.add($$, GCE_pMetaTok); } 
;
/* define messages */
define_mess : LP DEFINE define_token_list RP 
        { $$ = new CM_DefineMessage(*$3); delete $3; g_GC.remove($3, GCE_pvpDefTok); 
          g_GC.add($$, GCE_pMess); }
;
define_token_list : define_token_list define_token 
                { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pDefTok); }
        | define_token { $$ = new vector<CM_DefineToken*>;$$->push_back($1); 
                         g_GC.add($$, GCE_pvpDefTok); g_GC.remove($1, GCE_pDefTok); }
;
define_token: LP DEFINEC STR condition RP 
                { $$ = new CM_DefTokCondition(strip_quotes($3), $4);
                  delete $3; g_GC.remove($3, GCE_pChar);
                  g_GC.add($$, GCE_pDefTok); g_GC.remove($4, GCE_pCond); }
            | LP DEFINED STR directive RP
                { $$ = new CM_DefTokDirective(strip_quotes($3), $4); 
                  delete $3; g_GC.remove($3, GCE_pChar);
                  g_GC.add($$, GCE_pDefTok); g_GC.remove($4, GCE_pDir); }
            | LP DEFINER STR region RP
                { $$ = new CM_DefTokRegion(strip_quotes($3), $4); 
                  delete $3; g_GC.remove($3, GCE_pChar);
                  g_GC.add($$, GCE_pDefTok); g_GC.remove($4, GCE_pReg); }
            | LP DEFINEA STR action RP
                { $$ = new CM_DefTokAction(strip_quotes($3), $4); 
                  delete $3; g_GC.remove($3, GCE_pChar);
                  g_GC.add($$, GCE_pDefTok); g_GC.remove($4, GCE_pAct); }
;
/* freeform messages */
freeform_mess: LP FREEFORM STR RP { $$ = new CM_FreeformMessage(strip_quotes($3));
                                    delete $3; g_GC.remove($3, GCE_pChar);
                                    g_GC.add($$, GCE_pMess);
                                  }
;
info_mess: LP INFO token_list RP 
        { $$ = new CM_InfoMessage(*$3); delete $3;
        g_GC.remove($3, GCE_pvpTok); g_GC.add($$, GCE_pMess); }
;
advice_mess: LP ADVICE token_list RP 
        { $$ = new CM_AdviceMessage(*$3); delete $3;
        g_GC.remove($3, GCE_pvpTok); g_GC.add($$, GCE_pMess); }
;

token: LP INT condition directive_list RP 
        { $$ = new CM_TokRule($2, $3, *$4); delete $4; 
          g_GC.add($$, GCE_pTok); g_GC.remove($3, GCE_pCond); g_GC.remove($4, GCE_pvpDir); }
     | LP CLEAR RP { $$ = new CM_TokClear(); g_GC.add($$, GCE_pTok); }
token_list: token_list token 
            { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pTok); }
          | token 
            { $$ = new vector<CM_Token*>; $$->push_back($1); 
              g_GC.add($$, GCE_pvpTok); g_GC.remove($1, GCE_pTok); }
;

action: LP POS region RP                 
        { $$ = new CM_ActPosition($3); g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pReg); }
      | LP HOME region RP                
        { $$ = new CM_ActHome($3); g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pReg); }
      | LP BALLTO region bmove_token_set RP 
        { $$ = new CM_ActBallToRegion($3, *$4); 
          g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pReg); g_GC.remove($4, GCE_pBMTSet); }
      | LP BALLTO unum_set RP            
        { $$ = new CM_ActBallToPlayer(*$3); delete $3; 
          g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pUSet); }
      | LP MARK unum_set RP                   
        { $$ = new CM_ActMark(*$3); delete $3;
 	  g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pUSet); }
      | LP MARK_LINE unum_set RP                   
        { $$ = new CM_ActMarkLinePlayer(*$3); delete $3;
	  g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pUSet); }
      | LP MARK_LINE region RP                   
        { $$ = new CM_ActMarkLineRegion($3); g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pReg); }
      | LP OFFLINE region RP             
        { $$ = new CM_ActOffsidesLine($3); g_GC.add($$, GCE_pAct); g_GC.remove($3, GCE_pReg); }
      | LP HTYPE INT RP                  
        { $$ = new CM_ActHetType($3); g_GC.add($$, GCE_pAct); }
      | STR   { $$ = new CM_ActNamed(strip_quotes($1)); g_GC.add($$, GCE_pAct); 
                delete $1; g_GC.remove($1, GCE_pChar); }

directive: LP DO_DONT TEAM unum_set action RP 
        { $$ = new CM_DirCommand((bool)$2, (bool)$3, *$4, $5); delete $4; 
          g_GC.add($$, GCE_pDir); g_GC.remove($4, GCE_pUSet); g_GC.remove($5, GCE_pAct); }
        | STR   { $$ = new CM_DirNamed(strip_quotes($1)); g_GC.add($$, GCE_pDir); 
                  delete $1; g_GC.remove($1, GCE_pChar); }
;
directive_list: directive_list directive { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pDir); }
              | directive { $$ = new vector<CM_Directive*>; $$->push_back($1); 
                            g_GC.add($$, GCE_pvpDir); g_GC.remove($1, GCE_pDir); }
;

condition: LP TRUE_KW RP                      
                { $$ = new CM_CondTrue(); g_GC.add($$, GCE_pCond); }
         | LP FALSE_KW RP                     
                { $$ = new CM_CondFalse(); g_GC.add($$, GCE_pCond); }
         | LP PPOS TEAM unum_set INT INT region RP 
                { $$ = new CM_CondPlayerPosition((bool)$3,*$4,$5,$6,$7); delete $4; 
                  g_GC.add($$, GCE_pCond); g_GC.remove($4, GCE_pUSet); g_GC.remove($7, GCE_pReg); }
         | LP BPOS region RP               
                { $$ = new CM_CondBallPosition($3); 
                  g_GC.add($$, GCE_pCond); g_GC.remove($3, GCE_pReg); }
         | LP BOWNER TEAM unum_set RP      
                { $$ = new CM_CondBallOwner((bool)$3,*$4); delete $4; 
                  g_GC.add($$, GCE_pCond); g_GC.remove($4, GCE_pUSet); }
         | LP PLAY_MODE_KW PLAY_MODE RP
                { $$ = new CM_CondPlayMode((CM_PlayModeType)$3);
                  g_GC.add($$, GCE_pCond); }
         | LP AND condition_list RP        
                { $$ = new CM_CondAnd(*$3); delete $3; 
                  g_GC.add($$, GCE_pCond); g_GC.remove($3, GCE_pvpCond); }
         | LP OR condition_list RP         
                { $$ = new CM_CondOr(*$3); delete $3; 
                  g_GC.add($$, GCE_pCond); g_GC.remove($3, GCE_pvpCond); }
         | LP NOT condition RP             
                { $$ = new CM_CondNot($3); g_GC.add($$, GCE_pCond); g_GC.remove($3, GCE_pCond); }
         | STR  { $$ = new CM_CondNamed(strip_quotes($1)); g_GC.add($$, GCE_pCond); 
                  delete $1; g_GC.remove($1, GCE_pChar); }
;
condition_list: condition_list condition { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pCond);}
              | condition { $$ = new vector<CM_Condition*>; $$->push_back($1); 
                            g_GC.remove($1, GCE_pCond); g_GC.add($$, GCE_pvpCond);}
;

region: point { $$ = $1; }
      | LP NULL_REGION RP 
         { $$ = new RegNull; g_GC.add($$, GCE_pReg); }
      | LP QUAD point point point point RP 
         { $$ = new RegQuad($3, $4, $5, $6); g_GC.add($$, GCE_pReg);
           g_GC.remove($3, GCE_pReg); g_GC.remove($4, GCE_pReg); g_GC.remove($5, GCE_pReg); g_GC.remove($6, GCE_pReg); }
      | LP ARC point num num num num RP 
         { $$ = new RegArc($3, $4, $5, $6, $7); 
           g_GC.add($$, GCE_pReg); g_GC.remove($3, GCE_pReg); }
      | LP REG region_list RP 
         { $$ = new RegUnion(*$3); g_GC.add($$, GCE_pReg);
           delete $3; g_GC.remove($3, GCE_pvpReg); }
      | STR { $$ = new RegNamed(strip_quotes($1)); g_GC.add($$, GCE_pReg); 
              delete $1; g_GC.remove($1, GCE_pChar); }
;
region_list : region_list region { $1->push_back($2); $$ = $1; g_GC.remove($2, GCE_pReg); }
        | region { $$ = new vector<Region*>;$$->push_back($1); 
                   g_GC.remove($1, GCE_pReg); g_GC.add($$, GCE_pvpReg); }
point: LP PT num num RP { $$ = new RegPointSimple($3, $4); g_GC.add($$, GCE_pReg);}
     | LP PT num num point RP { $$ = new RegPointRelative($3, $4, $5);
                                g_GC.remove($5, GCE_pReg); g_GC.add($$, GCE_pReg);}
     | LP PT BALL RP { $$ = new RegPointBall(); g_GC.add($$, GCE_pReg);}
     | LP PT TEAM INT RP { $$ = new RegPointPlayer((bool)$3, $4); g_GC.add($$, GCE_pReg);}
;
num: INT { $$ = (double)$1; }
   | REAL { $$ = $1; }
;
unum_set: LB unum_list RB { $$ = $2; }
;
unum_list: unum_list INT { $$ = $1; $$->addNum($2); }
        | /*empty*/ { $$ = new UnumSet(); g_GC.add($$, GCE_pUSet);}
;

bmove_token_set: LB bmove_token_list RB { $$ = $2; }
;
bmove_token_list: bmove_token_list BMOVE_TOKEN { $$ = $1; $$->addToken((BallMoveToken)$2); }
                | BMOVE_TOKEN { $$ = new BallMoveTokenSet((BallMoveToken)$1); 
                                g_GC.add($$, GCE_pBMTSet); }
;
 
%%
/* ADDITIONAL C CODE */
int yyerror (char* s)
{
  fprintf (stderr, "%s: next token %d\n", s, yychar);
  g_pMess = NULL;
  //pTesting = NULL;
  return 0;
}


