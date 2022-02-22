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
	######################
		O R C A 
	######################
	
	- Enum-Typen extended
	- removed some "private"-lines
	- 2do alle coach_lang_comp.h-Klassen, die nur UnumSet benutezn, ableiten!
*/



/* This file contains miscellaneous components for the coach language */

#ifndef _H_COACH_LANG_COMP
#define _H_COACH_LANG_COMP

#include <stream.h>
#include <vector.h>

#include "region.h"


/******** type definitions **********/
//It is important that BTO_None is first and BTO_All is last
enum BallMoveToken
{ BTO_None, BTO_Pass, BTO_Dribble, BTO_Clear, BTO_Score, BTO_All };
extern char* BallMoveToken_strings[];
const int BallMoveToken_max = 4;

enum CM_PlayModeType
{
  CMPM_None,
  CMPM_BeforeKickOff,
  CMPM_TimeOver,
  CMPM_PlayOn,
  CMPM_KickOff_Our,
  CMPM_KickOff_Opp,
  CMPM_KickIn_Our,
  CMPM_KickIn_Opp,
  CMPM_FreeKick_Our,
  CMPM_FreeKick_Opp,
  CMPM_CornerKick_Our,
  CMPM_CornerKick_Opp,
  CMPM_GoalKick_Our,
  CMPM_GoalKick_Opp,
  CMPM_GoalieCatch_Our,
  CMPM_GoalieCatch_Opp,
  CMPM_AfterGoal_Our,
  CMPM_AfterGoal_Opp
};
extern char* CM_PlayModeType_strings[];

enum CM_MetaTokenType {
  CMMT_Version
};

enum CM_ActionType {
  CMA_Position,
  CMA_Home,
  CMA_BallToRegion,
  CMA_BallToPlayer,
  CMA_Mark,
  CMA_MarkLinePlayer,
  CMA_MarkLineRegion,
  CMA_OffsidesLine,
  CMA_HetType,
  CMA_Named,
  // TIMO: added the following
  SFLSA_Position,
  SFLSA_MarkLineRegionPlayer,
  SFLSA_State,
  SFLSA_InterceptBall,
  SFLSA_ActMarkLinePlayer,
  SFLSA_Mark,
  SFLSA_BallToPlayer,
  SFLSA_CatchBall
};

enum CM_DirectiveType {
  CMD_Command,
  CMD_Named,
  SFLSD_Command
};

enum CM_ConditionType {
  CMC_True,
  CMC_False,
  CMC_PlayerPosition,
  CMC_BallPosition,
  CMC_BallOwner,
  CMC_PlayMode,
  CMC_And,
  CMC_Or,
  CMC_Not,
  CMC_Named,
  // TIMO: added the following
  SFLSC_Action,
  SFLSC_Time,
  SFLSC_GoalDiff,
  SFLSC_Stamina,
  SFLSC_Equal,
  SFLSC_EqualUnum,
  SFLSC_Greater,
  SFLSC_Less,
  SFLSC_State,
  SFLSC_BallVelocity,
  SFLSC_BallInterceptable,
  SFLSC_BallOwner,
  SFLSC_PlayerPosition,
  SFLSC_BallCatchable
};

enum CM_TokenType {
  CMT_Clear,
  CMT_Rule
};

enum CM_DefineTokenType {
  CMDT_Condition,
  CMDT_Directive,
  CMDT_Region,
  CMDT_Action,
  // TIMO: added the following
  SFLSDT_Plan
};


/******** Misc Function **********************/
/* This modifies the string in place! and returns a pointer to the new string start */
char* strip_quotes(char* str);

/******** Utility Classes ********************/
//adding the number 0 to the set turns on all the bits
class UnumSet {
public:
  UnumSet(int team_size = 11) { this->team_size = team_size; clear(); }

  void clear() { entries = 0; }
  void addNum(int i);
  void removeNum(int i);
  bool isNumMember(int i);

  void Print(ostream& out);
  friend ostream& operator <<(ostream & os, UnumSet& s)
    { s.Print(os); return os; } 
private:
  int team_size;
  unsigned entries;
} ;

class BallMoveTokenSet {
public:
  BallMoveTokenSet() { clear(); }
  BallMoveTokenSet(BallMoveToken init_entry) { clear(); addToken(init_entry); }

  void clear() { entries = 0; }
  void addToken(BallMoveToken t);
  void removeToken(BallMoveToken t);
  bool isTokenMember(BallMoveToken t);

  void Print(ostream& out);
  friend ostream& operator <<(ostream & os, BallMoveTokenSet& s)
    { s.Print(os); return os; } 
private:
  unsigned entries;
} ;

/******** MetaToken primitives ********************/
class CM_MetaToken 
{
public:
  CM_MetaToken() {}
  virtual ~CM_MetaToken() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_MetaToken& mt)
    { mt.Print(os); return os; } 

  virtual CM_MetaTokenType getType() = 0;
} ;

class CM_MetaToken_Version 
  : public CM_MetaToken 
{
public:
  CM_MetaToken_Version(float ver = 0.0) : CM_MetaToken() {this->ver = ver;}
  ~CM_MetaToken_Version() {}

  void Print(ostream& out) {out << "(ver " << ver << ")";}
  void PrintPretty(ostream& out, string line_header) 
    { out << line_header << "version: " << ver << endl; }
  CM_MetaTokenType getType() { return CMMT_Version; }

  void setVer(float argver) {ver = argver;}
  float getVer() { return ver; }
      
private:
  float ver;
} ;

/******** Action primitives ********************/
class CM_Action 
{
public:
  CM_Action() {}
  virtual ~CM_Action() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_Action& a)
    { a.Print(os); return os; } 

  virtual CM_ActionType getType() = 0;
};

class CM_ActPosition
  : public CM_Action
{
public:
  CM_ActPosition() : CM_Action() { pReg = NULL; }
  CM_ActPosition(Region* r) : CM_Action() { pReg=NULL; setRegion(r);}
  ~CM_ActPosition() {}

  void Print(ostream& out)
    { out << "(pos " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "position self at: " << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ActionType getType() { return CMA_Position; }

  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void    setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion() { pReg = NULL; }
private:
  Region* pReg;
} ;

class CM_ActHome
  : public CM_Action
{
public:
  CM_ActHome() : CM_Action() {pReg = NULL;}
  CM_ActHome(Region* r) : CM_Action() { pReg=NULL; setRegion(r); }
  ~CM_ActHome() {}

  void Print(ostream& out)
    { out << "(home " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "home position at: " << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ActionType getType() { return CMA_Home; }

  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void    setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion() { pReg = NULL; }
private:
  Region* pReg;
} ;


class CM_ActBallToRegion
  : public CM_Action
{
public:
  CM_ActBallToRegion() : CM_Action() { pReg = NULL; }
  CM_ActBallToRegion(Region* pReg, BallMoveTokenSet& bmtset) : CM_Action()
    { this->pReg=pReg; this->bmtset = bmtset; }
  ~CM_ActBallToRegion() { if (pReg) delete pReg;}

  void Print(ostream& out) { out << "(bto " << *pReg << " " << bmtset << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "ball to: use " << bmtset << " to go to:" << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ActionType getType() { return CMA_BallToRegion; }

  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void    setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion() { pReg = NULL; }

  const BallMoveTokenSet& getBMTSet() { return bmtset; }
  void setBMTSet(const BallMoveTokenSet& bmtset) { this->bmtset = bmtset; }
  void setAddBMT(BallMoveToken t) { bmtset.addToken(t); }
  void setRemoveBMT(BallMoveToken t) { bmtset.removeToken(t); }
  void setClearAllBMT() { bmtset.clear(); }
private:
  Region* pReg;
  BallMoveTokenSet bmtset;
} ;

class CM_ActBallToPlayer
  : public CM_Action
{
public:
  CM_ActBallToPlayer() : CM_Action() {}
  CM_ActBallToPlayer(UnumSet& players) : CM_Action()
    { this->players = players; }
  ~CM_ActBallToPlayer() {}

  void Print(ostream& out) { out << "(bto " << players << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "ball to players: " << players << endl; }
  CM_ActionType getType() { return CMA_BallToPlayer; }

  /* class specific stuff */
  const UnumSet& getPlayers() { return players; }
  void setPlayerSet(const UnumSet& players) { this->players = players; }
  void setAddPlayer(int i) { players.addNum(i); }
  void setRemovePlayer(int i) { players.removeNum(i); }
  void setClearAllPlayer() { players.clear(); }
private:
  UnumSet players;
} ;


class CM_ActMark
  : public CM_Action
{
public:
  CM_ActMark() : CM_Action() {}
  CM_ActMark(UnumSet& players) : CM_Action() {this->players=players;}
  ~CM_ActMark() {}

  void Print(ostream& out)
    { out << "(mark " << players << ")";}
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "mark opponent " << players << endl; }
  CM_ActionType getType() { return CMA_Mark; }

  /* class specific stuff */
 const UnumSet& getPlayers() { return players; }
 void setPlayerSet(const UnumSet& players) { this->players = players; }
 void setAddPlayer(int i) { players.addNum(i); }
 void setRemovePlayer(int i) { players.removeNum(i); }
 void setClearAllPlayer() { players.clear(); }
private:
  UnumSet players;
} ;

class CM_ActMarkLinePlayer
  : public CM_Action
{
public:
  CM_ActMarkLinePlayer(UnumSet& players) : CM_Action() {this->players = players;}
  ~CM_ActMarkLinePlayer() {}

  void Print(ostream& out)
    { out << "(markl " << players << ")";}
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "mark line to opponent " << players << endl; }
  CM_ActionType getType() { return CMA_MarkLinePlayer; }

  /* class specific stuff */
 const UnumSet& getPlayers() { return players; }
 void setPlayerSet(const UnumSet& players) { this->players = players; }
 void setAddPlayer(int i) { players.addNum(i); }
 void setRemovePlayer(int i) { players.removeNum(i); }
 void setClearAllPlayer() { players.clear(); }
private:
  UnumSet players;
} ;

class CM_ActMarkLineRegion
  : public CM_Action
{
public:
  CM_ActMarkLineRegion(Region* pReg = NULL) : CM_Action()
    { this->pReg = pReg; }
  ~CM_ActMarkLineRegion() { if (pReg) delete pReg; }

  void Print(ostream& out)
    { out << "(markl " << *pReg << ")";}
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "mark line to region:" << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ActionType getType() { return CMA_MarkLineRegion; }

  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void    setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion() { pReg = NULL; }

private:
  Region* pReg;
} ;

class CM_ActOffsidesLine
  : public CM_Action
{
public:
  CM_ActOffsidesLine() : CM_Action() {pReg = NULL;}
  CM_ActOffsidesLine(Region* r) : CM_Action() { pReg=NULL; setRegion(r); }
  ~CM_ActOffsidesLine() {}

  void Print(ostream& out)
    { out << "(oline " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "offsides line at: " << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ActionType getType() { return CMA_OffsidesLine; }

  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void    setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion() { pReg = NULL; }
private:
  Region* pReg;
} ;

class CM_ActHetType
  : public CM_Action
{
public:
  //CM_ActHetType() : CM_Action() {player_type = 0;}
  CM_ActHetType(int player_type = 0) : CM_Action() {setPlayerType(player_type);}
  ~CM_ActHetType() {}

  void Print(ostream& out)
    { out << "(htype " << player_type << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "heterogenous type: " << player_type << endl; }
  CM_ActionType getType() { return CMA_HetType; }

  /* class specific stuff */
  int getPlayerType() { return player_type; }
  void setPlayerType(int player_type) { this->player_type = player_type; }
private:
  int player_type;
} ;

class CM_ActNamed
  : public CM_Action
{
public:
  CM_ActNamed(char* name = NULL) : CM_Action() { setName(name); }
  ~CM_ActNamed() {}

  void Print(ostream& out) { out << "\"" << name << "\""; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "action named \"" << name << "\"" << endl; }
  CM_ActionType getType() { return CMA_Named; }

  /* class specific */
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }

private:
  char name[max_name_len+1];
} ;

/******** Directive ********************/
class CM_Directive
{
public:
  CM_Directive() {}
  virtual ~CM_Directive() {}
  
  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_Directive& d)
    { d.Print(os); return os; } 
  virtual CM_DirectiveType getType() = 0;
};


class CM_DirCommand
  : public CM_Directive
{
public:
  CM_DirCommand( bool positive = true, bool our_side = true,
		const UnumSet& players = UnumSet(), CM_Action* pAct = NULL);
		
  ~CM_DirCommand() {if (pAct) delete pAct;}

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_DirectiveType getType() { return CMD_Command; }

  bool isPositive() { return positive; }
  bool isNegative() { return !positive; }
  
  bool isOurSide()  { return our_side; }
  bool isTheirSide(){ return !our_side; }
  const UnumSet& getPlayerSet() { return players; }
  CM_Action* getAction() { return pAct; }

  void setSense(bool positive) { this->positive = positive; }
  void setOurSide(bool our_side) { this->our_side = our_side; }
  void setPlayerSet(const UnumSet& players) { this->players = players; }
  void setAddPlayer(int i) { players.addNum(i); }
  void setRemovePlayer(int i) { players.removeNum(i); }
  void setClearAllPlayer() { players.clear(); }
  void setAction(CM_Action* pAct) { if (this->pAct) delete this->pAct; this->pAct = pAct; }
  
private:
  bool positive; //true = do; false = dont
  bool our_side; //true = our; false = opp
  UnumSet players;
  CM_Action* pAct;
} ;

class CM_DirNamed
  : public CM_Directive
{
public:
  CM_DirNamed(char* name = NULL) : CM_Directive() { setName(name); }
  ~CM_DirNamed() {}

  void Print(ostream& out) { out << "\"" << name << "\""; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "directived named \"" << name << "\"" << endl; }
  CM_DirectiveType getType() { return CMD_Named; }

  /* class specific */
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }

private:
  char name[max_name_len+1];

};

/******** Conditions ********************/
class CM_Condition
{
public:
  CM_Condition() {}
  virtual ~CM_Condition() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_Condition& c)
    { c.Print(os); return os; }

  virtual CM_ConditionType getType() = 0;

  virtual bool eval() = 0;
};

class CM_CondTrue
  : public CM_Condition
{
public:
  CM_CondTrue() : CM_Condition() {}
  ~CM_CondTrue() {}

  void Print(ostream& out) { out << "(true)"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "true" << endl; }
  CM_ConditionType getType() { return CMC_True; }
  bool eval() { return true; }
};
class CM_CondFalse
  : public CM_Condition
{
public:
  CM_CondFalse() : CM_Condition() {}
  ~CM_CondFalse() {}

  void Print(ostream& out) { out << "(false)"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "false" << endl; }
  CM_ConditionType getType() { return CMC_False; }
  bool eval() { return false; }
};
class CM_CondPlayerPosition
  : public CM_Condition
{
public:
  CM_CondPlayerPosition(bool our_side = false,const UnumSet& players =UnumSet(),
  int min_match=1, int max_match=11,Region* pReg = NULL): CM_Condition()
 	{this->our_side=our_side; this->players=players;
  	this->min_match = min_match; this->max_match = max_match;
  	this->pReg = pReg;}
  ~CM_CondPlayerPosition() {if (pReg) delete pReg;}

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_ConditionType getType() { return CMC_PlayerPosition; }
  bool eval() { return true; } //HOOK: need to add real evaluation here

  /* class specific stuff */
  bool isOurSide()  { return our_side; }
  bool isTheirSide(){ return !our_side; }
  int  getMinMatch() { return min_match; }
  int  getMaxMatch() { return max_match; }
  Region* getRegion() { return pReg; }
  const UnumSet& getPlayerSet() { return players; }
  void setMinMatch(int x) { min_match = x; }
  void setMaxMatch(int x) { max_match = x; }
  void setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void forgetRegion() { pReg = NULL; }
  void setOurSide(bool our_side) { this->our_side = our_side; }
  void setPlayerSet(const UnumSet& players) { this->players = players; }
  void setAddPlayer(int i) { players.addNum(i); }
  void setRemovePlayer(int i) { players.removeNum(i); }
  void setClearAllPlayer() { players.clear(); }
private:
  bool our_side;
  UnumSet players;
  int min_match;
  int max_match;
  Region* pReg;
};
class CM_CondBallPosition
  : public CM_Condition
{
public:
  CM_CondBallPosition(Region* pReg = NULL) : CM_Condition() {this->pReg = pReg;}
  ~CM_CondBallPosition() {if (pReg) delete pReg;}

  void Print(ostream& out) { out << "(bpos " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "is ball position at: " << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_ConditionType getType() { return CMC_BallPosition; }
  bool eval() { return true; } //HOOK: need to add real evaluation here
  
  /* class specific stuff */
  Region* getRegion() { return pReg; }
  void setRegion(Region* r) { if (pReg) delete pReg; pReg = r; }
  void forgetRegion() { pReg = NULL; }
private:
  Region* pReg;
};
class CM_CondBallOwner
  : public CM_Condition
{
public:
  CM_CondBallOwner(bool our_side = false, const UnumSet& players = UnumSet())
    : CM_Condition() {this->our_side=our_side; this->players=players; }
  ~CM_CondBallOwner() {}

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_ConditionType getType() { return CMC_BallOwner; }
  bool eval() { return true; } //HOOK: need to add real evaluation here

  /* class specific stuff */
  bool isOurSide()  { return our_side; }
  bool isTheirSide(){ return !our_side; }
  const UnumSet& getPlayerSet() { return players; }

  void setOurSide(bool our_side) { this->our_side = our_side; }
  void setPlayerSet(const UnumSet& players) { this->players = players; }
  void setAddPlayer(int i) { players.addNum(i); }
  void setRemovePlayer(int i) { players.removeNum(i); }
  void setClearAllPlayer() { players.clear(); }
private:
  bool our_side;
  UnumSet players;

private:
};
class CM_CondPlayMode
  : public CM_Condition
{
public:
  CM_CondPlayMode(CM_PlayModeType pm = CMPM_None) : CM_Condition() {this->pm= pm;}
  ~CM_CondPlayMode() {}

  void Print(ostream& out) { out << "(playm " << CM_PlayModeType_strings[pm] << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "is play mode '"
	  << CM_PlayModeType_strings[pm] << "'" << endl; }
  CM_ConditionType getType() { return CMC_PlayMode; }
  bool eval() { return true; } //HOOK: need to add real evaluation here
  
  /* class specific stuff */
  CM_PlayModeType getPlayMode() { return pm; }
  void setRegion(CM_PlayModeType) { this->pm = pm; }
private:
  CM_PlayModeType pm;
};
class CM_CondAnd
  : public CM_Condition
{
public:
  CM_CondAnd() : CM_Condition() {}
  CM_CondAnd(vector<CM_Condition*>& vCond) : CM_Condition() {this->vCond = vCond;}
  ~CM_CondAnd() {clearConditions();}

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_ConditionType getType() { return CMC_And; }
  bool eval();

  vector<CM_Condition*>& getConditions() { return vCond; }
  void setConditions(vector<CM_Condition*>& vCond) 
    { clearConditions(); this->vCond = vCond; }
  void clearConditions();

private:
  vector<CM_Condition*> vCond;
};
class CM_CondOr
  : public CM_Condition
{
public:
  CM_CondOr() : CM_Condition() {}
  CM_CondOr(vector<CM_Condition*>& vCond) : CM_Condition() {this->vCond = vCond;}
  ~CM_CondOr() {clearConditions();}

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_ConditionType getType() { return CMC_Or; }
  bool eval();

  vector<CM_Condition*>& getConditions() { return vCond; }
  void setConditions(vector<CM_Condition*>& vCond) 
    { clearConditions(); this->vCond = vCond; }
  void clearConditions();

private:
  vector<CM_Condition*> vCond;
  
private:
};
class CM_CondNot
  : public CM_Condition
{
public:
  CM_CondNot(CM_Condition* pCond = NULL) : CM_Condition() {this->pCond = pCond;}
  ~CM_CondNot() {if (pCond) delete pCond;}

  void Print(ostream& out) { if (pCond) out << "(not " << *pCond << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "not " << endl;
    pCond->PrintPretty(out, line_header + " +"); }
  CM_ConditionType getType() { return CMC_Not; }
  bool eval() { return !(pCond->eval()); }
  
  /* class specific stuff */
  CM_Condition* getCondition() { return pCond; }
  void setCondition(CM_Condition* pCond) { if (pCond) delete pCond; this->pCond = pCond; }
  void forgetCondition() { pCond = NULL; }
private:
  CM_Condition* pCond;
};
class CM_CondNamed
  : public CM_Condition
{
public:
  CM_CondNamed(char* name = NULL) : CM_Condition() { setName(name); }
  ~CM_CondNamed() {}

  void Print(ostream& out) { out << "\"" << name << "\""; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "condition named \"" << name << "\"" << endl; }
  CM_ConditionType getType() { return CMC_Named; }
  bool eval() { return true; } //HOOK: add real evlauation

  /* class specific */
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }

private:
  char name[max_name_len+1];

};

/******** Tokens ********************/
class CM_Token {
public:
  CM_Token() {}
  virtual ~CM_Token() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_Token& t)
    { t.Print(os); return os; } 
  virtual CM_TokenType getType() = 0;
};
class CM_TokClear
  : public CM_Token
{
public:
  CM_TokClear() : CM_Token() {}
  ~CM_TokClear() {}

  void Print(ostream& out) { out << "(clear)"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "CLEAR token" << endl; }
  CM_TokenType getType() { return CMT_Clear; }
};
class CM_TokRule
  : public CM_Token
{
public:
  CM_TokRule(int ttl, CM_Condition* pCond = NULL) : CM_Token()
    { time_to_live = ttl; this->pCond = pCond; }
  CM_TokRule(int ttl, CM_Condition* pCond, vector<CM_Directive*>& vDir) : CM_Token()
    { time_to_live = ttl; this->pCond = pCond; this->vDir = vDir; }
  ~CM_TokRule() {if (pCond) delete pCond; clearDirectives(); }

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CM_TokenType getType() { return CMT_Rule; }

  /* class specific stuff */
  int getTimeToLive() { return time_to_live; }
  void setTimeToLive(int ttl) { time_to_live = ttl; }
  CM_Condition* getCondition() { return pCond; }
  void setCondition(CM_Condition* pCond) { if (pCond) delete pCond; this->pCond = pCond; }
  void forgetCondition() { pCond = NULL; }
  vector<CM_Directive*>& getDirectives() { return vDir; }
  void setDirectives(vector<CM_Directive*>& vDir) 
    { clearDirectives(); this->vDir = vDir; }
  void clearDirectives();
private:
  int time_to_live;
  CM_Condition* pCond;
  vector<CM_Directive*> vDir;
};

/******** DefineTokens ********************/

class CM_DefineToken {
public:
  CM_DefineToken() { name[0] = 0; }
  virtual ~CM_DefineToken() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_DefineToken& t)
    { t.Print(os); return os; }

  virtual CM_DefineTokenType getType() = 0;
  
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }
  
protected:
  char name[max_name_len+1];
};

class CM_DefTokCondition
  : public CM_DefineToken
{
public:
  CM_DefTokCondition(char* name = NULL, CM_Condition* pCond = NULL)
    : CM_DefineToken()
    { this->pCond = pCond; setName(name);}
  ~CM_DefTokCondition() { if (pCond) delete pCond; }

  void Print(ostream& out) { out << "(definec \"" << name << "\" " << *pCond << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Condition \"" << name << "\"" << endl;
    pCond->PrintPretty(out, line_header + " "); }
  CM_DefineTokenType getType() { return CMDT_Condition; }
  
// private: TIMO
  CM_Condition* pCond;
};

class CM_DefTokDirective
  : public CM_DefineToken
{
public:
  CM_DefTokDirective(char* name = NULL, CM_Directive* pDir = NULL)
    : CM_DefineToken()
    { this->pDir = pDir; setName(name);}
  ~CM_DefTokDirective() { if (pDir) delete pDir; }

  void Print(ostream& out) { out << "(defined \"" << name << "\" " << *pDir << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Directive \"" << name << "\"" << endl;
    pDir->PrintPretty(out, line_header + " "); }
  CM_DefineTokenType getType() { return CMDT_Directive; }

// private: TIMO
  CM_Directive* pDir;
};

class CM_DefTokRegion
  : public CM_DefineToken
{
public:
  CM_DefTokRegion(char* name = NULL, Region* pReg = NULL)
    : CM_DefineToken()
    { this->pReg = pReg; setName(name);}
  ~CM_DefTokRegion() { if (pReg) delete pReg; }

  void Print(ostream& out) { out << "(definer \"" << name << "\" " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Region \"" << name << "\"" << endl;
    pReg->PrintPretty(out, line_header + " "); }
  CM_DefineTokenType getType() { return CMDT_Region; }

// private: TIMO
  Region* pReg;
};

class CM_DefTokAction
  : public CM_DefineToken
{
public:
  CM_DefTokAction(char* name = NULL, CM_Action* pAct = NULL)
    : CM_DefineToken()
    { this->pAct = pAct; setName(name);}
  ~CM_DefTokAction() { if (pAct) delete pAct; }

  void Print(ostream& out) { out << "(definea \"" << name << "\" " << *pAct << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Action \"" << name << "\"" << endl;
    pAct->PrintPretty(out, line_header + " "); }
  CM_DefineTokenType getType() { return CMDT_Action; }

// private: TIMO 
  CM_Action* pAct;
};



#endif
