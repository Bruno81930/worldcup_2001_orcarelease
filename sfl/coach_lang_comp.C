/* -*- Mode: C -*- */

/*
 *Copyright:

    Copyright (C) 1996-2000 Electrotechnical Laboratory. 
    	Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
    Copyright (C) 2000 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Daniel Polani, Itsuki Noda

    This file is a part of SoccerServer.

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


/*
	##############  ORCA #############

*/


/* This file contains miscellaneous components for the coach language */

#include "coach_lang_comp.h"

/******** type definitions **********/
char* ActBallTo_strings[] =
{ "NONE", "p", "d", "a" };
char* BallMoveToken_strings[] =
{ "NONE", "p", "d", "c", "s", "ALL" };
char* CM_PlayModeType_strings[] = {
  "",
  "bko",
  "time_over",
  "play_on",
  "ko_our",
  "ko_opp",
  "ki_our",
  "ki_opp",
  "fk_our",
  "fk_opp",
  "ck_our",
  "ck_opp",
  "gk_our",
  "gk_opp",
  "gc_our",
  "gc_opp",
  "ag_our",
  "ag_opp" };
  

/******** Misc Function **********************/
char* strip_quotes(char* str)
{
  int len = strlen(str);
  if (len == 0) return str;
  if (str[len-1] == '"')
    str[len-1] = 0;
  return (str[0] == '"' ? str+1 : str);
}


/******** Utility Classes ********************/
void UnumSet::addNum(int i)
{
  if (i < 0 || i > team_size)
    return;
  if (i==0) {
    entries = ~0;
  } else {
    entries |= (1 << (i-1));
  }
}

void UnumSet::removeNum(int i)
{
  if (i < 0 || i > team_size)
    return;
  if (i==0) {
    clear();
  } else {
    entries &= ~(1 << (i-1));
  }
}

bool UnumSet::isNumMember(int i)
{
  if (i < 0 || i > team_size)
    return false;
  if (i==0) {
    return (~(((~0) << team_size) | entries)) == 0;
  } else {
    return !!(entries & (1 << (i-1)));
  }
}

void UnumSet::Print(ostream& out)
{
  char buf[2]; //this is used to handle spacing
  buf[0] = 0;
  buf[1] = 0;
  out << "{";
  if (isNumMember(0)) {
    out << "0";
  } else {
    for (int i=1; i<=team_size; i++)
      if (entries & (1 << (i-1))) {
	out << buf << i;
	buf[0] = ' ';
      }
  }
  out << "}";
  //out << " " << entries;
}

void BallMoveTokenSet::addToken(BallMoveToken t)
{
  int i = (int)t;
  if (i < 1 || i > BallMoveToken_max)
    return;
  if (i==(int)BTO_All) {
    entries = ~0;
  } else {
    entries |= (1 << (i-1));
  }
}

void BallMoveTokenSet::removeToken(BallMoveToken t)
{
  int i = (int)t;
  if (i < 1 || i > BallMoveToken_max)
    return;
  if (i==BTO_All) {
    clear();
  } else {
    entries &= ~(1 << (i-1));
  }
}

bool BallMoveTokenSet::isTokenMember(BallMoveToken t)
{
  int i = (int)t;
  if (i < 1 || i > BallMoveToken_max)
    return false;
  if (i==BTO_All) {
    return (~(((~0) << ((int)BallMoveToken_max)) | entries)) == 0;
  } else {
    return !!(entries & (1 << (i-1)));
  }
}

void BallMoveTokenSet::Print(ostream& out)
{
  out << "{";
  for (int i=1; i<=BallMoveToken_max; i++)
    if (entries & (1 << (i-1))) out << BallMoveToken_strings[i];
  out << "}";
  //out << " " << entries;
}


/******** Action primitives ********************/

/******** Directive ********************/
CM_DirCommand::CM_DirCommand( bool positive, bool our_side,
			    const UnumSet& players, CM_Action* pAct)
  : CM_Directive()
{
  this->positive = positive;
  this->our_side = our_side;
  this->players = players;
  this->pAct = pAct;
}


void CM_DirCommand::Print(ostream& out)
{
  out << "("
      << (positive ? "do" : "dont") << " "
      << (our_side ? "our" : "opp") << " "
      << players << " "
      << *pAct << ")";
}

void CM_DirCommand::PrintPretty(ostream& out, string line_header)
{
  out << line_header 
      << (our_side ? "our team" : "opponent") << " "
      << players << " "
      << (positive ? "DO" : "DON'T") << ": " << endl;
  pAct->PrintPretty(out, line_header + " ");
}



/******** Conditions ********************/

void CM_CondPlayerPosition::Print(ostream& out)
{
  out << "(ppos"
      << " " << (our_side ? "our" : "opp")
      << " " << players 
      << " " << min_match
      << " " << max_match
      << " " << *pReg
      << ")";
}
void CM_CondPlayerPosition::PrintPretty(ostream& out, string line_header)
{
  out << line_header
      << "are " 
      << min_match << "-" << max_match << " "
      << (our_side ? "our team" : "opponent") << " "
      << players << " "
      << "in:" << endl;
  pReg->PrintPretty(out, line_header + " ");
}


void CM_CondBallOwner::Print(ostream& out)
{
  out << "(bowner"
      << " " << (our_side ? "our" : "opp") 
      << " " << players
      << ")";  
}
void CM_CondBallOwner::PrintPretty(ostream& out, string line_header)
{
  out << line_header
      << "is ball controlled by someone in " 
      << (our_side ? "our team" : "opponent") << " "
      << players << endl;
}


void CM_CondAnd::Print(ostream& out)
{
  vector<CM_Condition*>::iterator cond_iter;
  out << "(and";
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) 
    out << " " << **cond_iter;
  out << ")";
}

void CM_CondAnd::PrintPretty(ostream& out, string line_header)
{
  vector<CM_Condition*>::iterator cond_iter;
  out << line_header << "and" << endl;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) 
    (*cond_iter)->PrintPretty(out, line_header + " +");
}

bool CM_CondAnd::eval()
{
  vector<CM_Condition*>::iterator cond_iter;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) {
    if (!(*cond_iter)->eval())
      return false;
  }
  return true;
}

void CM_CondAnd::clearConditions()
{
  vector<CM_Condition*>::iterator cond_iter;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter)
    delete (*cond_iter);
  vCond.clear();
}


void CM_CondOr::Print(ostream& out)
{
  vector<CM_Condition*>::iterator cond_iter;
  out << "(or";
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) 
    out << " " << **cond_iter;
  out << ")";
}

void CM_CondOr::PrintPretty(ostream& out, string line_header)
{
  vector<CM_Condition*>::iterator cond_iter;
  out << line_header << "or" << endl;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) 
    (*cond_iter)->PrintPretty(out, line_header + " +");
}

bool CM_CondOr::eval()
{
  vector<CM_Condition*>::iterator cond_iter;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter) {
    if ((*cond_iter)->eval())
      return true;
  }
  return false;
}

void CM_CondOr::clearConditions()
{
  vector<CM_Condition*>::iterator cond_iter;
  for (cond_iter = vCond.begin(); cond_iter != vCond.end(); ++cond_iter)
    delete (*cond_iter);
  vCond.clear();
}


/******** Tokens ********************/
void CM_TokRule::Print(ostream& out)
{
  if (!pCond)
    return;
  
  out << "(" << time_to_live << " " << *pCond;

  vector<CM_Directive*>::iterator dir_iter;
  for (dir_iter = vDir.begin(); dir_iter != vDir.end(); ++dir_iter)
    out << " " << (**dir_iter);
  out << ")";
}
void CM_TokRule::PrintPretty(ostream& out, string line_header)
{
  if (!pCond)
    return;

  out << line_header << "for " << time_to_live << " cycles, if" << endl;
  pCond->PrintPretty(out, line_header + " +");

  out << line_header << "then" << endl;
  vector<CM_Directive*>::iterator dir_iter;
  for (dir_iter = vDir.begin(); dir_iter != vDir.end(); ++dir_iter)
    (*dir_iter)->PrintPretty(out, line_header + " *");
}


void CM_TokRule::clearDirectives()
{
  vector<CM_Directive*>::iterator dir_iter;
  for (dir_iter = vDir.begin(); dir_iter != vDir.end(); ++dir_iter)
    delete (*dir_iter);
  vDir.clear();
}


