/*
*/
#ifndef SFLS_HIERARCHY_H
#define SFLS_HIERARCHY_H

#define SFLS_OUR 1
#define SFLS_OPP 0
#define SFLS_BOTH 2


#include "../types.h"
#include "coach_lang.h"
#include "coach_lang_comp.h"

// ############## Types ################################

enum SFLS_Level{
	low,
	mid,
	high
};

enum SFLS_PassMode{
	safe,
	dangerous
};

enum SFLS_UnumFunctionType{
		  Variable,
		  FastestPlayerToBall,
		  ClosestPlayerToBall,
		  FastestPlayerToPlayer,
		  ClosestPlayerToPlayer,
		  BestPassPartner,
		  BestDeckPartner
};

enum SFLS_TeamFunctionType{
		Constant,
        TeamOfFastestPlayerToBall,
        TeamOfClosestPlayerToBall,
        TeamOfFastestPlayerToPlayer,
        TeamOfClosestPlayerToPlayer
};


// ################### Variables ####################
// Note: Unum-Variables are covered by SFLS_UnumFunctionType.
// All other variables are treated as double, and theoretically all double and int-values can also be variables.
// So we cover int and double values with the folllowing structure. The structure either contains
// a normal double, or a variable. In the latter case the double has the value VARIABLE_MARKER
#define VARIABLE_MARKER -9999

class DoubleOrVariable{
public:
	double value;		// constant value or VARIABLE_MARKER if a variable is used
	int variablename;	// Variables are upper case letters, so variablename ranges from 1-26

	DoubleOrVariable(double value);		// if it is constant
	DoubleOrVariable(int variablename);	// if it is a variable
	bool isVariable(void) {return (value==VARIABLE_MARKER);}
};




// ################## SFLS_UnumSet and its extensions (Unum-Variables and Functions like FastestPlayerToBall)

class SFLS_UnumSet;


// encapsulates constant values like "opp", "our", and "both", but also "TeamOfFastestEtc.PP"
class SFLS_Team{
public:
	SFLS_Team(short side);	// used for type==Constant
	SFLS_Team(SFLS_TeamFunctionType type, short side = -1);
	SFLS_Team(SFLS_TeamFunctionType type, SFLS_Team * team, SFLS_UnumSet *set);
	SFLS_TeamFunctionType getType(){return type;}
	short getSide(){return side;}
	SFLS_UnumSet* getUnumSet(){return unumset;}
	void Print(ostream &);
private:
	short side;
	SFLS_TeamFunctionType type;
	SFLS_UnumSet *unumset;
	SFLS_Team * team;
};



class SFLS_Function{
public:
	// Note: Different types need different paramters
	// obsolete due to change in grammar: SFLS_Function(SFLS_UnumFunctionType type);
	SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team);
	SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team,
				SFLS_UnumSet *players);
	SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team,
				SFLS_Team *team_target, SFLS_UnumSet* players);
	SFLS_Function(int variablename);
	SFLS_UnumFunctionType getType() { return type; }
	SFLS_Team* getTeam(){return team;}
	SFLS_Team* getTeamtarget(){return team_target;}
	SFLS_UnumSet *getUnumSet(){return players;}
	int getVariableName(){return variablename;}
	void removeUnumFromVariable(CM_Token * rule,Unum unum);
	void Print(ostream &);
private:
	SFLS_UnumFunctionType type;
	SFLS_Team* team;
	SFLS_Team* team_target;
	SFLS_UnumSet *players;
	int variablename; // only used if type==Variable
};

class SFLS_UnumSet : public UnumSet{
public:
	SFLS_UnumSet(void);
  	void addSFLSFunction(SFLS_Function* function);
	short getOneByOne(int last,CM_Condition * condition, CM_Token *rule, bool positive);
	short getOneByOneOfVariables(int last, CM_Token *rule);
	bool isNumMember(Unum unum, CM_Token* rule, CM_Condition * condition, bool positive)
	{ return isNumMember(unum, rule, condition, true, positive); }
	bool isNumMember(Unum unum, CM_Token* rule, CM_Condition * condition, bool IncVar, bool positive);
	void removeUnumFromVariable(CM_Token * rule, Unum unum, int variablename);
	vector<SFLS_Function *> * getSFLS_Functions()
	{ return (&SFLS_Functions); }
	void Print(ostream& out);
	short getLastTimeEvaluated(void)
  { return lastEvaluationTime; }
	void setLastTimeEvaluated(short atime)
  { lastEvaluationTime=atime; }
	int containsVariable(void);
private:
	short lastEvaluationTime;
	vector<SFLS_Function *> SFLS_Functions;
	UnumSet * alreadyEvaluatedFunctions;
};






// ############## Directive #############
class SFLS_DirCommand : public CM_DirCommand{
public:
  SFLS_DirCommand( bool positive , SFLS_Team *team,
		SFLS_UnumSet* players,
		CM_Action* pAct , bool force );
  bool isForce() {return force;}
  CM_DirectiveType getType() { return SFLSD_Command; }
  void setTeam(SFLS_Team *team) {this->team=team;}
  SFLS_Team *getTeam(){return team;}
  SFLS_UnumSet* getPlayerSet() { return players; }
  void setPlayerSet(SFLS_UnumSet* players) { this->players = players; }
  void Print(ostream &);
private:
  SFLS_Team *team;
  bool force;
  SFLS_UnumSet *players;
};


// ################# Conditions #########################


class SFLS_CondAction : public CM_Condition{
public:
	SFLS_CondAction(SFLS_Team *team, SFLS_UnumSet* players,
				CM_Action* action);
	~SFLS_CondAction();
	void Print(ostream& out){}
	void PrintPretty(ostream& out, string line_header){}
	CM_ConditionType getType(){ return SFLSC_Action; }
	bool eval() { return true; }  //HOOK: need to add real evaluation here

	/* class specific stuff */
	bool isOurSide();
	bool isTheirSide();
	CM_Action* getAction() { return action; }
	SFLS_UnumSet* getPlayerSet(){ return players; }

	void setAction(CM_Action* a) { if (action) delete action; action = a; }
	void forgetAction() { action = NULL; }
	void setTeam(SFLS_Team *team)  { this->team = team; }
	SFLS_Team *getTeam(void){return team;}
	void setPlayerSet(SFLS_UnumSet* players) { this->players = players;}
	void setAddPlayer(int i) { players->addNum(i); }
	void setRemovePlayer(int i) { players->removeNum(i); }
	void setClearAllPlayer() { players->clear(); }

private:
	SFLS_Team *team;
	SFLS_UnumSet *players;
	CM_Action * action;
};



class SFLS_CondTime : public CM_Condition{
public:
	SFLS_CondTime(DoubleOrVariable *time);
	void Print(ostream& out)
  {}
	void PrintPretty(ostream& out, string line_header)
  {}
  CM_ConditionType getType()
    { return SFLSC_Time; }
	void setTime(DoubleOrVariable *t)
  { 	time = t; }
	DoubleOrVariable* getTime()
  { 	return time; }
	bool eval(void)
{return false;}
private:
	DoubleOrVariable *time;
};



class SFLS_CondGoalDiff : public CM_Condition{
public:
	SFLS_CondGoalDiff(DoubleOrVariable *goaldiff);
	void Print(ostream& out){}
	void PrintPretty(ostream& out, string line_header){}
	CM_ConditionType getType() { return SFLSC_GoalDiff; }
	void setGoalDiff(DoubleOrVariable *diff){goal_diff=diff;}
	DoubleOrVariable* getGoalDiff(void)
{ return goal_diff; }
	bool eval(void) { return false; }
private:
	DoubleOrVariable* goal_diff;
};


class SFLS_CondStamina : public CM_Condition{
public:
	SFLS_CondStamina(SFLS_Team *team, SFLS_UnumSet* players,
				SFLS_Level level=high);
	void Print(ostream& out){}
	void PrintPretty(ostream& out, string line_header){}
	CM_ConditionType getType() { return SFLSC_Stamina; }
	bool eval() { return true; }  //HOOK: need to add real evaluation here

	/* class specific stuff */
	bool isOurSide();
	bool isTheirSide();
	SFLS_Level getLevel() { return level; }
	SFLS_UnumSet* getPlayerSet()  { return players; }

	SFLS_Team *getTeam(void){ return team;}
	void setLevel(SFLS_Level l) { level = l; }
	void setTeam(SFLS_Team *team) { this->team = team; }
	void setPlayerSet(SFLS_UnumSet* players) { this->players = players; }
	void setAddPlayer(int i) { players->addNum(i); }
	void setRemovePlayer(int i) { players->removeNum(i); }
	void setClearAllPlayer() { players->clear(); }
private:
	SFLS_Team *team;
	SFLS_UnumSet *players;
	SFLS_Level level;
};



class SFLS_CondBallCatchable : public CM_Condition{
public:
	SFLS_CondBallCatchable(SFLS_Team *team, SFLS_UnumSet* players);
	void Print(ostream& out){}
	void PrintPretty(ostream& out, string line_header){}
	CM_ConditionType getType() { return SFLSC_BallCatchable; }
	bool eval(){ return true; }  //HOOK: need to add real evaluation here

	/* class specific stuff */
	bool isOurSide();
	bool isTheirSide();
	SFLS_UnumSet* getPlayerSet() { return players; }

	void setTeam(SFLS_Team *team)  { this->team = team; }
	SFLS_Team* getTeam(void){return team;}
	void setPlayerSet(SFLS_UnumSet* players) { this->players = players; }
	void setAddPlayer(int i){ players->addNum(i); }
	void setRemovePlayer(int i)  { players->removeNum(i); }
	void setClearAllPlayer() { players->clear(); }
private:
	SFLS_Team *team;
	SFLS_UnumSet *players;
};



class SFLS_CondBallVelocity : public CM_Condition{
public:
	SFLS_CondBallVelocity(DoubleOrVariable *vel);
	void Print(ostream& out)
  {}
	void PrintPretty(ostream& out, string line_header)
  {}
	CM_ConditionType getType()
  { return SFLSC_BallVelocity; }
	void setBallVelocity(DoubleOrVariable *vel)
  { velocity=vel; }
	DoubleOrVariable* getBallVelocity(void)
  { 	return velocity; }
	bool eval(void)
  { return false; }
private:
	DoubleOrVariable* velocity;
};




class SFLS_CondBallInterceptable
  : public CM_Condition
{
public:
  SFLS_CondBallInterceptable(SFLS_Team *team, SFLS_UnumSet* players);
  ~SFLS_CondBallInterceptable();

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header)
  {}
  CM_ConditionType getType()
{ return SFLSC_BallInterceptable; }
  bool eval()
{ return true; } //HOOK: need to add real evaluation here

  /* class specific stuff */
  bool isOurSide();
  bool isTheirSide();
  SFLS_UnumSet* getPlayerSet()
{ return players; }
  SFLS_Team* getTeam(void)
{return team;}
  void setTeam(SFLS_Team *team)
{ this->team = team; }
  void setPlayerSet(SFLS_UnumSet* players)
{ this->players = players; }
  void setAddPlayer(int i) { players->addNum(i); }
  void setRemovePlayer(int i){ players->removeNum(i); }
  void setClearAllPlayer() { players->clear(); }
private:
  SFLS_Team *team;
  SFLS_UnumSet *players;
};



class SFLS_CondState : public CM_Condition{
public:
	SFLS_CondState(string name, string value);
	void Print(ostream& out)
  {}
	void PrintPretty(ostream& out, string line_header)
  {}
	CM_ConditionType getType()
{ return SFLSC_State; }
	void setName(string n)
  { 	name=n; }
	string getName()
  { 	return name; }
  	void setValue(string v)
  { 	value=v;}
	string getValue()
  { return value; }
	bool eval(void){return false;}
private:
	string name;
	string value;
};


class SFLS_CondEqual : public CM_Condition{
public:
	SFLS_CondEqual(DoubleOrVariable *x, DoubleOrVariable *y);
	DoubleOrVariable* getX()
{return x;}
	DoubleOrVariable* getY()
{return y;}
	CM_ConditionType getType()
{ return SFLSC_Equal; }
	bool eval(void)
{return false;}
	void Print(ostream &out)
  {}
	void PrintPretty(ostream &out, string header)
  {}
private:
	DoubleOrVariable *x;
	DoubleOrVariable *y;
};


class SFLS_CondEqualUnum : public CM_Condition{
public:
	SFLS_CondEqualUnum(Unum x, Unum y);	// both constant MACHT DAS SINN???????
	SFLS_CondEqualUnum(SFLS_Function *x, Unum y); // x Variable, y constant
	SFLS_CondEqualUnum(Unum x, SFLS_Function* y); // redundant to above method???
	SFLS_CondEqualUnum(SFLS_Function* x, SFLS_Function* y); // both are variables
	bool isXConstant()
{return (x!=-1); }
	bool isYConstant()
{return (y!=-1); }
	Unum getConstantX()
{return x;}
	Unum getConstantY()
{return y;}
	SFLS_Function * getVariableX()
{return xvar;}
	SFLS_Function * getVariableY()
{return yvar;}
void Print(class ostream &);
	void PrintPretty(ostream & out, string header)
  {}
	bool eval()
  {return true;}
	CM_ConditionType getType()
  { return SFLSC_EqualUnum; }
private:
	Unum x;	// 0 to 11, or -1 if Variable
	Unum y; // 0 to 11, or -1 if Variable
	SFLS_Function *xvar;	// obsolete, if x is a constant
	SFLS_Function *yvar;	// obsolete, if y is a constant
};




class SFLS_CondLess : public CM_Condition{
public:
  SFLS_CondLess(DoubleOrVariable *x, DoubleOrVariable *y);
	DoubleOrVariable* getX()
  {return x;}
	DoubleOrVariable* getY()
  {return y;}
  CM_ConditionType getType()
    { return SFLSC_Less; }
  bool eval(void)
  {return false;}
	void Print(ostream &out)
  {}
	void PrintPretty(ostream &out, string header)
  {}
private:
	DoubleOrVariable *x;
	DoubleOrVariable *y;
};


class SFLS_CondGreater : public CM_Condition{
public:
  SFLS_CondGreater(DoubleOrVariable *x, DoubleOrVariable *y);

	DoubleOrVariable* getX()
  {return x;}
	DoubleOrVariable* getY()
  {return y;}
	CM_ConditionType getType()
  { return SFLSC_Less; }
	bool eval(void)
  { return false; }
	void Print(ostream &out)
  {}
	void PrintPretty(ostream &out, string header)
  {}
private:
    DoubleOrVariable *x;
    DoubleOrVariable *y;
};


class SFLS_CondPlayerPosition : public CM_CondPlayerPosition{
public:
  	SFLS_CondPlayerPosition(SFLS_Team *team,
		SFLS_UnumSet* players ,
  		int min_match, int max_match,Region* pReg );

	SFLS_UnumSet* getPlayerSet(){return players;}
	void setPlayerSet(SFLS_UnumSet* playerset){players=playerset;}
	CM_ConditionType getType() { return SFLSC_PlayerPosition; }
	void setTeam(SFLS_Team *team){this->team=team;}
	SFLS_Team* getTeam(void){return team;}
	void Print(ostream &out);
private:
	SFLS_Team *team;
	SFLS_UnumSet* players;
};


class SFLS_CondBallOwner : public CM_CondBallOwner{
public:
  	SFLS_CondBallOwner(SFLS_Team *team,
		SFLS_UnumSet* players);
	void setPlayerSet(SFLS_UnumSet* playerset){players=playerset;}
	SFLS_UnumSet* getPlayerSet(){return players;}
	CM_ConditionType getType(){ return SFLSC_BallOwner; }
	void setTeam(SFLS_Team *teeam) {team=teeam;}
	SFLS_Team* getTeam(void){return team;}
	void Print(ostream &);
private:
	SFLS_Team *team;
	SFLS_UnumSet* players;

};


/* And unchanged in coach_lang_comp.h
class CM_CondTrue
class CM_CondFalse
class CM_CondBallPosition
class CM_CondPlayMode
class CM_CondAnd
class CM_CondOr
class CM_CondNot
class CM_CondNamed

*/





// ################### Actions ###############


class SFLS_ActPosition : public CM_ActPosition{
public:
  SFLS_ActPosition(Region* r, int dashpower)
  { forgetRegion(); setRegion(r); this->dashpower=dashpower; }
  void setDashPower(int power)
{dashpower=power;}
  int getDashPower()
{return dashpower;}
	void Print(ostream& out) { out << "(pos " << *getRegion() << " " << dashpower << ")"; }
private:
  int dashpower;
};





class SFLS_ActState : public CM_Action{
public:
  SFLS_ActState(string name, string value)
  { this->name=name; this->value=value; }

  ~SFLS_ActState()
  {}
  void Print(ostream& out)
  {}
  void PrintPretty(ostream& out, string line_header)
  {}
  CM_ActionType getType()
{ return SFLSA_State; }
  void setName(string n)
{name=n;}
  string getName()
{return name;}
  void setValue(string v)
{value=v;}
  string getValue()
{return value;}
  
private:
  string name;
  string value;
};


class SFLS_ActInterceptBall : public CM_Action{
public:
	SFLS_ActInterceptBall(double power)
  {         this->power=power; }

	void Print(ostream& out)
  { out << "(interceptball)" << endl; }
  
	void PrintPretty(ostream& out, string line_header)
  { out << "(interceptball)" << endl; }
	CM_ActionType getType()
  { return SFLSA_InterceptBall; }
	void setPower(double p)
  {power=p;}
	double getPower()
  {return power;}

private:
	double power;
};



class SFLS_ActCatchBall : public CM_Action{
public:
	SFLS_ActCatchBall(void)
  {}
	void Print(ostream& out)
  {}
	void PrintPretty(ostream& out, string line_header)
  {}
	CM_ActionType getType()
{ return SFLSA_CatchBall; }
};




class SFLS_ActMarkRegionPlayer : public CM_Action{
public:
  SFLS_ActMarkRegionPlayer()
  {}
  SFLS_ActMarkRegionPlayer(SFLS_UnumSet* players, Region * pReg)
  { this->players = players; this->pReg=pReg; }

  ~SFLS_ActMarkRegionPlayer()
    {if(pReg) delete pReg;}

  void Print(ostream& out)
  { out << "(markregion " << *players << " " << *pReg << ")"; }
  void PrintPretty(ostream& out, string line_header)
  { out << line_header << "mark line from player to region: " << players <<" " << pReg<< endl; }
  CM_ActionType getType()
  { return SFLSA_MarkLineRegionPlayer; }

  /* class specific stuff */
  SFLS_UnumSet* getPlayers()
    { return players; }
  void setPlayerSet(SFLS_UnumSet* players)
    { this->players = players; }
  void setAddPlayer(int i)
  { players->addNum(i); }
  void setRemovePlayer(int i)
    { players->removeNum(i); }
  void setClearAllPlayer()
    { players->clear(); }

  Region* getRegion()
    { return pReg; }
  void    setRegion(Region* r)
    { if (pReg) delete pReg; pReg = r; }
  void    forgetRegion()
    { pReg = NULL; }
private:
  SFLS_UnumSet *players;
  Region* pReg;
};


class SFLS_ActBallToPlayer : public CM_ActBallToPlayer{
public:
	SFLS_ActBallToPlayer(SFLS_UnumSet* players)
  {	
	  //cout << "SFLS Konstruktor: " ; players->Print(cout); cout << endl;
	  this->players=players;
  }
	void setPlayerSet (SFLS_UnumSet * playerset)
  {players=playerset;}
	SFLS_UnumSet* getPlayers()
  {return players;}
	CM_ActionType getType()
  { return SFLSA_BallToPlayer; }
	void Print(ostream &out)
  {	out << "(bto "; players->Print(out); out<< ")"; }
private:
	SFLS_UnumSet* players;
};

class SFLS_ActMark : public CM_ActMark{
public:
  	SFLS_ActMark(SFLS_UnumSet* players)
  {setPlayerSet(players);}
	void setPlayerSet (SFLS_UnumSet * playerset)
  {players=playerset;}
	SFLS_UnumSet* getPlayers()
  {return players;}
  	CM_ActionType getType()
  { return SFLSA_Mark; }
private:
	SFLS_UnumSet* players;
};

class SFLS_ActMarkLinePlayer : public CM_ActMark{
public:
	SFLS_ActMarkLinePlayer(SFLS_UnumSet* players)
  { setPlayerSet(players); }
	void setPlayerSet (SFLS_UnumSet * playerset)
  {players=playerset;}
	SFLS_UnumSet* getPlayers()
  {return players;}
	CM_ActionType getType()
  { return SFLSA_ActMarkLinePlayer; }
private:
	SFLS_UnumSet* players;
};

/* And unchanged in coach_lang_comp.h
class CM_ActHome
class CM_ActBallToRegion
class CM_ActMarkLineRegion



*/
// ################### DefineToken ###############

/* ??????????????????????? */
class SFLS_DefTokPlan
  : public CM_DefineToken
{
public:
  SFLS_DefTokPlan(char* name = NULL, vector<CM_Token*>* pPlan = NULL)
    : CM_DefineToken()
    { this->pPlan = pPlan; setName(name);}
  ~SFLS_DefTokPlan() { if (pPlan) delete pPlan; }

  void Print(ostream& out) { out << "(definep \"" << name << "\" " << "*pPlan" << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Plan \"" << name << "\"" << endl;
    // pPlan->PrintPretty(out, line_header + " "); 
	}
  CM_DefineTokenType getType() { return SFLSDT_Plan; }

// private: 
  vector<CM_Token*>* pPlan;
};






// REGIONS
// Note: Although SFLS_RegPointPlayer uses a SFLS_UnumSet, only the first
// entry will be used!
class SFLS_RegPointPlayer : public RegPointPlayer
{
public:
  SFLS_RegPointPlayer(SFLS_Team *team, SFLS_UnumSet *unumSet);
  	~SFLS_RegPointPlayer()
  {}

	SFLS_Team* getTeam()
  { 	return team; }
  void setTeam(SFLS_Team *team)
  { 	this->team=team; }

	SFLS_UnumSet* getPlayerSet(void)
  { 	return unumSet; }
	void setPlayerSet(SFLS_UnumSet *unumSet)
  { this->unumSet=unumSet; }
	void Print(ostream &out)
  {	out << "(pt player 2do 2do))"; }

	void PrintPretty(ostream &out)
  { out << "(SFLS_RegPointPlayer 2do 2do)"; }

private:
	SFLS_Team *team;
	SFLS_UnumSet *unumSet;
};




class SFLS_RegHome : public RegPoint{
public:
	void Print(ostream &out)
  { out << "(home) "; }
	void PrintPretty(ostream &out, string header)
  { out << "(home) "; }
	void PrintSimple(ostream &out)
  { out << "(home) "; }
};


// classes for point-arithmetic:
class SFLS_PlusPoint : public RegPoint{
public:
	SFLS_PlusPoint(RegPoint *a, RegPoint *b);
	void Print(ostream &out) { out << "(plus 2do 2do)"; }
	void PrintPretty(ostream &out, string header) { out << "(plus 2do 2do)"; }
	void PrintSimple(ostream &out) { out << "(plus 2do 2do)"; }
	RegPoint* getA() { return a;}
	RegPoint* getB() { return b;}
private:
	RegPoint *a;
	RegPoint *b;
};

class SFLS_MultPoint : public RegPoint{
public:
	SFLS_MultPoint(RegPoint *a, RegPoint *b);
	void Print(ostream &out) { out << "(mult 2do 2do)"; }
	void PrintPretty(ostream &out, string header) { out << "(mult 2do 2do)"; }
	void PrintSimple(ostream &out) { out << "(mult 2do 2do)"; }
	RegPoint* getA() { return a;}
	RegPoint* getB() { return b;}
private:
	RegPoint *a;
	RegPoint *b;
};

class SFLS_DefTokPoint
  : public CM_DefineToken
{
public:
  SFLS_DefTokPoint(char* name = NULL, RegPoint *pPoint = NULL)
    : CM_DefineToken()
    { this->pPoint = pPoint; setName(name);}
  ~SFLS_DefTokPoint() { if (pPoint) delete pPoint; }

  void Print(ostream& out) { out << "(definep \"" << name << "\" " << "*pPoint" << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Point \"" << name << "\"" << endl;
    // pPlan->PrintPretty(out, line_header + " "); 
	}
  CM_DefineTokenType getType() { return SFLSDT_Point; }
// too lazy to make private:
  RegPoint* pPoint;
};

class SFLS_PointNamed
  : public RegPoint
{
public:
  SFLS_PointNamed(char* name = NULL) : RegPoint() { setName(name); }
  ~SFLS_PointNamed() {}

  void Print(ostream& out) { out << "\'" << name << "\'"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "point named \"" << name << "\"" << endl; }
  void PrintSimple(ostream& out) { out << "\'" << name << "\'"; }
  bool eval() { return true; } //HOOK: add real evaluation

  /* class specific */
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }

private:
  char name[max_name_len+1];

};


#endif


