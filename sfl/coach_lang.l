/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Itsuki Noda,	Mikhail Prokopenko, Jan Wendler

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
 ###### modified by ORCA ######
*/

%{

#include "coach_lang.h"
#include "SFLShierarchy.h"
#include "coach_lang.tab.h"
#include "garbage.h"

extern GarbageCollector g_GC; //the garbage collector

%}

/* Definitions */

%option nomain
%option noyywrap

%%

\( { return LP;}
\) { return RP;}
\{ { return LB;}
\} { return RB;}

meta { return META;}
freeform { return FREEFORM;}
info { return INFO; }
advice { return ADVICE; }
define  { return DEFINE; }
definec { return DEFINEC; }
defined { return DEFINED; }
definer { return DEFINER; }
definea { return DEFINEA; }

ver { return VERSION;}

null { return NULL_REGION;}
homepos { return HOME_REGION;}
quad { return QUAD;}
arc { return ARC;}
pt { return PT;}
reg { return REG;}
ball { return BALL;}

pos { return POS;}
home { return HOME;}
bto { return BALLTO;}
mark { return MARK;}
markl { return MARK_LINE;}
oline { return OFFLINE;}
htype { return HTYPE;}
p { yylval.inum = (int)BTO_Pass; return BMOVE_TOKEN; }
d { yylval.inum = (int)BTO_Dribble; return BMOVE_TOKEN; }
c { yylval.inum = (int)BTO_Clear; return BMOVE_TOKEN; }
s { yylval.inum = (int)BTO_Score; return BMOVE_TOKEN; }

do   { yylval.inum = 1; return DO_DONT; }
dont { yylval.inum = 0; return DO_DONT; }

our  { yylval.inum = 1; return TEAM; }
opp  { yylval.inum = 0; return TEAM; }

clear { return CLEAR; }

true   { return TRUE_KW; }
false  { return FALSE_KW; }
ppos   { return PPOS; }
bpos   { return BPOS; }
bowner { return BOWNER; }
and    { return AND; }
or     { return OR; }
not    { return NOT; }
playm  { return PLAY_MODE_KW; }
bko       { yylval.inum = (int)CMPM_BeforeKickOff; return PLAY_MODE; }
time_over { yylval.inum = (int)CMPM_TimeOver; return PLAY_MODE; }
play_on   { yylval.inum = (int)CMPM_PlayOn; return PLAY_MODE; }
ko_our    { yylval.inum = (int)CMPM_KickOff_Our; return PLAY_MODE; }
ko_opp    { yylval.inum = (int)CMPM_KickOff_Opp; return PLAY_MODE; }
ki_our    { yylval.inum = (int)CMPM_KickIn_Our; return PLAY_MODE; }
ki_opp    { yylval.inum = (int)CMPM_KickIn_Opp; return PLAY_MODE; }
fk_our    { yylval.inum = (int)CMPM_FreeKick_Our; return PLAY_MODE; }
fk_opp    { yylval.inum = (int)CMPM_FreeKick_Opp; return PLAY_MODE; }
ck_our    { yylval.inum = (int)CMPM_CornerKick_Our; return PLAY_MODE; }
ck_opp    { yylval.inum = (int)CMPM_CornerKick_Opp; return PLAY_MODE; }
gk_our    { yylval.inum = (int)CMPM_GoalKick_Our; return PLAY_MODE; }
gk_opp    { yylval.inum = (int)CMPM_GoalKick_Opp; return PLAY_MODE; }
gc_our    { yylval.inum = (int)CMPM_GoalieCatch_Our; return PLAY_MODE; }
gc_opp    { yylval.inum = (int)CMPM_GoalieCatch_Opp; return PLAY_MODE; }
ag_our    { yylval.inum = (int)CMPM_AfterGoal_Our; return PLAY_MODE; }
ag_opp    { yylval.inum = (int)CMPM_AfterGoal_Opp; return PLAY_MODE; }




action		{ return ACTION; }
time		{ return TIME; }
goalDiff	{ return GOALDIFF; }
stamina		{ return STAMINA; }
equnum		{ return EQUALUNUM; }
eq		{ return EQUAL; }
lt		{ return LESS; }
gt		{ return GREATER; }
state		{ return STATE; }
ballVelocity		{ return BALLVELOCITY; }
ballInterceptable	{ return BALLINTERCEPTABLE; }
ballCatchable		{ return BALLCATCHABLE; }

force		{ return FORCE; }
interceptball	{ return INTERCEPTBALL; }
catchball	{ return CATCHBALL; }

low	{ yylval.inum = (int)low; return LEVEL; }
mid	{ yylval.inum = (int)mid; return LEVEL; }
high	{ yylval.inum = (int)high; return LEVEL; }

safe		{ yylval.inum = (int)safe; return PASSMODE; }
dangerous	{ yylval.inum = (int)dangerous; return PASSMODE; }

both           			{ yylval.inum = 2; return TEAM; }
teamOfFastestPlayerToBall	{ yylval.inum = (int)TeamOfFastestPlayerToBall; return TEAMFUNCTION; }
teamOfClosestPlayerToBall	{ yylval.inum = (int)TeamOfClosestPlayerToBall; return TEAMFUNCTION; }
teamOfFastestPlayerToPlayer	{ yylval.inum = (int)TeamOfFastestPlayerToPlayer; return TEAMFUNCTION_TU; }
teamOfClosestPlayerToPlayer	{ yylval.inum = (int)TeamOfClosestPlayerToPlayer; return TEAMFUNCTION_TU; }

[A-Z]	{ yylval.inum = (int)yytext[0] - (int)'A'; return VAR; }

fastestPlayerToBall	{ yylval.inum = (int)FastestPlayerToBall; return UNUMFUNCTION_T; }
closestPlayerToBall	{ yylval.inum = (int)ClosestPlayerToBall; return UNUMFUNCTION_T; }
fastestPlayerToPlayer	{ yylval.inum = (int)FastestPlayerToPlayer; return UNUMFUNCTION_TTU; }
closestPlayerToPlayer	{ yylval.inum = (int)ClosestPlayerToPlayer; return UNUMFUNCTION_TTU; }
bestPassPartner		{ yylval.inum = (int)BestPassPartner; return UNUMFUNCTION_TU; }
bestDeckPartner		{ yylval.inum = (int)BestDeckPartner; return UNUMFUNCTION_TU; }

yourhome		{ return YOURHOME; }
youradjustedhome	{ return YOURADJUSTEDHOME; }
plus			{ return PLUSPOINT; }
mult			{ return MULTPOINT; }

definep			{ return DEFINEP; }

#.* ;



[ \t\n]+ ;
[\-0-9]+ { yylval.inum = atoi(yytext); return INT; }
[\+\-0-9\.\e\E]+ { yylval.dnum = atof(yytext); return REAL; }
\"[0-9A-Za-z\(\)\.\+\-\*\/\?\<\>\_ ]{1,256}\" {yylval.str = new char[strlen(yytext)+1];
                                               strcpy(yylval.str,yytext);
                                               g_GC.add(yylval.str, GCE_pChar);
                                               return STR; }
. { return ERROR; }

%%

int yyparse();

/* Extra C stuff */

extern "C"
{
	int parse_coach_lang_string(char* s)
	{
		YY_BUFFER_STATE yybuffstateCurr;

		yybuffstateCurr = yy_scan_string(s);

		int ret = yyparse();

		yy_delete_buffer(yybuffstateCurr);

		return ret;
	}
}

