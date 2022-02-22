#include "SFLShierarchy.h"
#include "../client.h"

// Note: SFLS_UnumSet::containsVariable() returns only the first variable
// 2do: as soon as we cope with TEAM and UNUMSET in BestDeckPartner and BestPassPartner,
//	containsVariable() has to find recursive variables, too, like in
//	{(BestDeckPartner our {X})}

// DoubleOrVariable>>
DoubleOrVariable::DoubleOrVariable(double value){
	this->value=value;
	this->variablename=-1;	// doesn't matter actually
}
DoubleOrVariable::DoubleOrVariable(int variablename){
	this->value=VARIABLE_MARKER;
	this->variablename=variablename;
}


// SFLS_Team::
//SFLS_Team::SFLS_Team(){}
SFLS_Team::SFLS_Team(short side){  // used for type==Constant
        this->type=Constant,
        this->side=side;
}
SFLS_Team::SFLS_Team(SFLS_TeamFunctionType type, short side){
        this->type=type,
        this->side=side;
}
SFLS_Team::SFLS_Team(SFLS_TeamFunctionType type, SFLS_Team* team, SFLS_UnumSet* set){
	this->type=type;
	this->team=team;
	this->unumset=set;
}
void SFLS_Team::Print(ostream &out){
	switch(type){
		case Constant :
		 	out << (side==SFLS_OUR?" our ":" opp ") ; break;
		case TeamOfFastestPlayerToBall:
			out << " (TeamOfFastestPlayerToBall) " ; break;
		case TeamOfClosestPlayerToBall:
			out << " (TeamOfClosestPlayerToBall) " ; break;
		case TeamOfFastestPlayerToPlayer:
			out << " (TeamOfFastestPlayerToPlayer " ;
			team->Print(out);
			unumset->Print(out);
			out << ") ";
			break;
		case TeamOfClosestPlayerToPlayer:
			out << " (TeamOfClosestPlayerToPlayer " ;
			team->Print(out);
			unumset->Print(out);
			out << ") ";
			break;
	}
}


// SFLS_Function::
SFLS_Function::SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team){
	this->type=type;
	this->team=team;
}
SFLS_Function::SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team,
                SFLS_UnumSet *players){
	this->type=type;
	this->team=team;
	this->players=players;

}
SFLS_Function::SFLS_Function(SFLS_UnumFunctionType type, SFLS_Team *team,
                SFLS_Team *team_target, SFLS_UnumSet* players){
	this->type=type;
	this->team=team;
	this->players=players;
	this->team_target=team_target;
}
SFLS_Function::SFLS_Function(int variablename){
        type=Variable;
        this->variablename=variablename;
}
void SFLS_Function::Print(ostream & out){
	switch(type){
		case Variable		:
			out << ((char)(variablename+64)) << " "; break;
		case FastestPlayerToBall	:
			out << " (FastestPlayerToBall ";
			team->Print(out);
			out << ") "; break;
		case ClosestPlayerToBall	:
			out << " (ClosestPlayerToBall ";
			team->Print(out);
			out << ") "; break;
		case FastestPlayerToPlayer	:
			out << " (FastestPlayerToPlayer ";
			team->Print(out); out << " ";
			players->Print(out); out << ") ";
			break;
		case ClosestPlayerToPlayer	:
			out << " (ClosestPlayerToPlayer ";
			team->Print(out); out << " ";
			players->Print(out); out << ") ";
			break;
		case BestPassPartner		:
			out << " (BestPassPartner ";
			team->Print(out); out << " ";
			players->Print(out); out << ") ";
			break;
		case BestDeckPartner		:
			out << " (BestDeckPartner ";
			team->Print(out); out << " ";
			players->Print(out); out << ") ";
			break;
	}
}


// SFLS_DirCommand::
SFLS_DirCommand::SFLS_DirCommand( bool positive, SFLS_Team *team, SFLS_UnumSet* players , CM_Action* pAct, bool force):CM_DirCommand(){
              
        setSense(positive);
        setTeam(team);
        setPlayerSet(players);
        setAction(pAct);
        this->force=force;
}
void SFLS_DirCommand::Print(ostream &out){
	if(isForce())
		out << "(force ";
	else if(isPositive())
		out << "(do ";
	else
		out << "(dont ";
		
	team->Print(out);
	players->Print(out);
	getAction()->Print(out);
}


// SFLS_CondAction::
SFLS_CondAction::SFLS_CondAction(SFLS_Team *team, SFLS_UnumSet* players,
                        CM_Action* action) : CM_Condition(){
        this->team=team;
        this->players=players;
        this->action=action;
}
SFLS_CondAction::~SFLS_CondAction(){if(action) delete action;}



// SFLS_CondTime::
SFLS_CondTime::SFLS_CondTime(DoubleOrVariable *time){
        this->time=time;
}

// SFLS_CondGoalDiff::
SFLS_CondGoalDiff::SFLS_CondGoalDiff(DoubleOrVariable *diff){
        this->goal_diff=diff;
}


// SFLS_CondStamina::
SFLS_CondStamina::SFLS_CondStamina(SFLS_Team *team, SFLS_UnumSet* players,
                        SFLS_Level level) : CM_Condition(){
        this->team=team;
        this->players=players;
        this->level=level;
}
//bool SFLS_CondStamina::isOurSide()  { return our_side; }
//bool SFLS_CondStamina::isTheirSide(){ return !our_side; }

//SFLS_CondBallCatchable::
SFLS_CondBallCatchable::SFLS_CondBallCatchable(SFLS_Team *team, SFLS_UnumSet* players)
         : CM_Condition(){
        this->team=team;
        this->players=players;
}
//bool SFLS_CondBallCatchable::isOurSide()  { return our_side; }
//bool SFLS_CondBallCatchable::isTheirSide(){ return !our_side; }


//SFLS_CondBallVelocity::
SFLS_CondBallVelocity::SFLS_CondBallVelocity(DoubleOrVariable *vel){
        this->velocity=vel;
}

// SFLS_CondBallInterceptable::
SFLS_CondBallInterceptable::SFLS_CondBallInterceptable(SFLS_Team *team, SFLS_UnumSet* players)
    : CM_Condition() {this->team=team; this->players=players; }
SFLS_CondBallInterceptable::~SFLS_CondBallInterceptable() {}
void SFLS_CondBallInterceptable::Print(ostream& out){
	out << "(ballInterceptable ";
	team->Print(out); out << " ";
	players->Print(out); 
	out << ")";
}

// SFLS_CondState::
SFLS_CondState::SFLS_CondState(string name, string value){
        this->name=name;
        this->value=value;
}

// SFLS_CondEqual::
SFLS_CondEqual::SFLS_CondEqual(DoubleOrVariable *x, DoubleOrVariable *y){
        this->x=x;
        this->y=y;
}


// SFLS_CondEqualUnum::
SFLS_CondEqualUnum::SFLS_CondEqualUnum(Unum x, Unum y){     // both constant
        this->x=x;
        this->y=y;
}
SFLS_CondEqualUnum::SFLS_CondEqualUnum(SFLS_Function *x, Unum y){ // x Variable, y constant
        this->x=-1;
        this->xvar=x;
        this->y=y;
}
SFLS_CondEqualUnum::SFLS_CondEqualUnum(Unum x, SFLS_Function* y){ // redundant to above method???
        this->x=x;
        this->y=-1;
        this->yvar=y;
}
SFLS_CondEqualUnum::SFLS_CondEqualUnum(SFLS_Function* x, SFLS_Function* y){ // both are variables
        this->x=-1;
        this->y=-1;
        this->xvar=x;
        this->yvar=y;
}
void SFLS_CondEqualUnum::Print(ostream & out){
	out << "(equnum ";
	if(x!=-1)
		out << x;
	else
		xvar->Print(out);
	if(y!=-1)
		out << " " << y;
	else
		yvar->Print(out);
	out << ")";
}

// SFLS_CondPlayerPosition::
SFLS_CondPlayerPosition::SFLS_CondPlayerPosition(SFLS_Team *team,
      SFLS_UnumSet* players,
      int min_match, int max_match,Region* pReg ):
      CM_CondPlayerPosition(){
	setTeam(team); setPlayerSet(players);
	setMinMatch(min_match); setMaxMatch(max_match); setRegion(pReg);}
void SFLS_CondPlayerPosition::Print(ostream& out){
	out << "(ppos ";
	getTeam()->Print(out);
	out <<" ";
	getPlayerSet()->Print(out);
	out <<" ";
	getRegion()->Print(out);
	out << ")";
}


// SFLS_CondBallOwner::
SFLS_CondBallOwner::SFLS_CondBallOwner(SFLS_Team *team,
        SFLS_UnumSet* players ) : CM_CondBallOwner(){
        this->team=team; setPlayerSet(players);}
void SFLS_CondBallOwner::Print(ostream &out){
	out << "(bowner ";
	team->Print(out);
	players->Print(out);
	out << ") ";
}

//	if an UnumSet like {1 X} is used, X should not contain 1
//  {1 (FastestPlayerToBall our)}, if fastest==1, then only one unum is in set (coping with MIN and MAX
//		in player-position-condition)
SFLS_UnumSet::SFLS_UnumSet(){
	lastEvaluationTime=-1;
	alreadyEvaluatedFunctions=new UnumSet();
}

void SFLS_UnumSet::Print(ostream& out){
  out << "{";
  if (((UnumSet*)this)->isNumMember(0)) {
    out << "0";
  } else {
    for (int i=1; i<=team_size; i++)
      if (entries & (1 << (i-1))) {
	out << i << " ";
      }
  }
  if(SFLS_Functions.size()==0)
  	out <<"} ";
  else{
  	for(int i=0; i<(signed)SFLS_Functions.size(); i++){
    		out << " ";
    		SFLS_Functions[i]->Print(out);
    		out << "} ";
  	}
  }
}

void SFLS_UnumSet::addSFLSFunction(SFLS_Function* function){
	  lastEvaluationTime=Mem->CurrentTime.t;
          SFLS_Functions.push_back(function);
}


// if checkVariables is false, it will only be tested, if unum is in the UnumSet or Functions
bool SFLS_UnumSet::isNumMember(Unum unum, CM_Token* rule, CM_Condition * condition,
		bool checkVariables, bool positive){
	int functionResult;
	// first check explicitly mentioned Unums by checking the bits.
	// If unum is not in there, check the SFLS_Functions.
	if(((UnumSet *)this)->isNumMember(unum))
		return true;	// unum was mentioned explicitly
	// unum was not mentioned explicitly
	if(lastEvaluationTime==Mem->CurrentTime.t){
		//cout << "already evaluated" << endl;
		// we already evaluated this SFLS_UnumSet and its functions.
		if(alreadyEvaluatedFunctions->isNumMember(unum)) return true;
		// does a variable cover this unum?
		if(!checkVariables) return false;
		for(int i=0; i<(signed)SFLS_Functions.size(); i++){
		
			if(SFLS_Functions[i]->getType()!=Variable) continue;
			functionResult=getOneByOneOfVariables(0,rule);
			
			if(functionResult==unum) return true;
			
			while((functionResult=getOneByOneOfVariables(functionResult,rule))!=-1){
			
				if(functionResult==unum) return true;
			}			
		}
		return false;
	}
	
	alreadyEvaluatedFunctions->clear();	// reset
	
	// we need to evaluate all functions before we can check whether unum is in it.
	bool result=false;
	for(int i=0; i<(signed)SFLS_Functions.size(); i++){
		// evaluate Variable only once per cycle, since it will be stored in UnumVariableSets
		functionResult=Mem->evalFunction(SFLS_Functions[i], condition, rule,((UnumSet*)this),positive);

		if(SFLS_Functions[i]->getType()!=Variable){		
			alreadyEvaluatedFunctions->addNum(functionResult);
			if(functionResult==unum)
				result=true;
		}else{	// variable
			// variables return only one of the possible values in evalFunction(), so
   	    		// in case of variable getONEbyONEOfVariable() until it returns -1
		
			if(!checkVariables) continue;
			if(functionResult==unum) result=true;
		
			while((functionResult=getOneByOneOfVariables(functionResult,
			rule))!=-1){
				if(functionResult==unum){
					result=true;
					break;	// evaluate next function
				}
			}
		}
	}
	lastEvaluationTime=Mem->CurrentTime.t;
	return result;
}

// Note: This is NOT efficient yet, but might, when SFLS_Functions get cached.
// Return possible unums of SFLS_UnumSet one at a time, ordered from 0 to 11
// -1 if no more unums are in this SFLS_UnumSet.
short SFLS_UnumSet::getOneByOne(int last,CM_Condition * condition, CM_Token *rule, bool positive){
	if(isNumMember(0, rule, condition,positive)) return 0;	// all Unums are in this UnumSet, anyway
	
	// - check UnumSet and SFLS_Functions
	for(Unum i=last+1; i<12; i++){
		if(isNumMember(i,rule,condition,positive)) return i;	// bit is set
	}
	return -1;
};


// Use this if you don't want to evaluate Unums again, but have already everything in
// UnumVariableSets. This checks ALL Variables in UnumSet!
short SFLS_UnumSet::getOneByOneOfVariables(int last, CM_Token *rule){
	int setIndex=-1;
	// find VariableSet for this UnumSet:
	for(int set=0;set<(signed)Mem->UnumVariableSets.size(); set++){
		if(Mem->UnumVariableSets[set]->getRule()==rule) setIndex=set;
	}
	for(int i=last+1; i<12; i++){
		if(setIndex!=-1){	// there is a variable set for this UnumSet
			for(int var=0; var<(signed)SFLS_Functions.size(); var++){
			    if(SFLS_Functions[var]->getType()==Variable){
			    	int varname=SFLS_Functions[var]->getVariableName();
				for(int nam=0; nam<(signed)Mem->UnumVariableSets[setIndex]->names.size();
				    nam++){
					if(Mem->UnumVariableSets[setIndex]->names[nam]==
					 	varname){
						  if(Mem->UnumVariableSets[setIndex]->unumsets[nam]->
						  	isNumMember(i)) return i;
						}
				}
			    }
			}
		}
	}
	return -1;
};





void SFLS_UnumSet::removeUnumFromVariable(CM_Token * rule, Unum unum, int variablename){
	int setIndex=-1;
	// find VariableSet for this UnumSet:
	for(int set=0;set<(signed)Mem->UnumVariableSets.size(); set++){
		if(Mem->UnumVariableSets[set]->getRule()==rule) setIndex=set;
	}
	if(setIndex==-1) return;	// no VariableSet for this rule
	// which variablenames are in this playerset:
	for(int func=0; func<(signed)SFLS_Functions.size(); func++){
		if(SFLS_Functions[func]->getType()==Variable){// we have a variable in SFLSUnumSet
			if(variablename==-1 || variablename==SFLS_Functions[func]->getVariableName()){
				Mem->UnumVariableSets[setIndex]->removeUnumFromVar(
					SFLS_Functions[func]->getVariableName(), unum);
			}
		}
	}

};


// 2do this copes only with the first variable in the set!
int SFLS_UnumSet::containsVariable(){
	for(int i=0; i<(signed)SFLS_Functions.size(); i++){
		if(SFLS_Functions[i]->getType()==Variable){
			return (SFLS_Functions[i]->getVariableName());
		}
	}
	return -1;
}


SFLS_CondLess::SFLS_CondLess(DoubleOrVariable *x, DoubleOrVariable *y)
  { this->x=x; this->y=y; }


SFLS_CondGreater::SFLS_CondGreater(DoubleOrVariable *x, DoubleOrVariable *y)
  { this->x=x; this->y=y; }

SFLS_RegPointPlayer::SFLS_RegPointPlayer(SFLS_Team *team, SFLS_UnumSet *unumSet)
  : team(team), unumSet(unumSet)
{}

SFLS_PlusPoint::SFLS_PlusPoint(RegPoint *a, RegPoint *b){
	this->a=a;
	this->b=b;
}
SFLS_MultPoint::SFLS_MultPoint(RegPoint *a, RegPoint *b){
	this->a=a;
	this->b=b;
}

