/* -*- Mode: C++ -*- */

/* Memory.h
 * CMUnited99 (soccer client for Robocup99)
 * Peter Stone <pstone@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Peter Stone
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy
 * distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 * For more information, please see http://www.cs.cmu.edu/~robosoccer/
 */


#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "MemFormation.h"
#include "geometry.h"

#include "sfl/clangparser.h"
#include "sfl/SFLShierarchy.h"
#include "sfl/effector.h"

#include "sfl/SFLS.h"

bool classNameStartsWith(Region * region, char *description);
bool PrettyclassNameStartsWith(Region * point, char *description);



class SFLS_UnumVariableSet{ // Contains Unum-Variables in vectors for name and value
public:
	SFLS_UnumVariableSet(){};
	SFLS_UnumVariableSet(CM_Token* rule){this->rule=rule;}
	// ~SFLS_UnumVariableSet(); // 2do do we need a destructor for the vectors?
	CM_Token* getRule() {return rule;}
	void addVar(int aname, UnumSet unumset){
		names.push_back(aname); unumsets.push_back(&unumset);
	}
	void addUnumToVar(int aname, Unum unum);
	void removeUnumFromVar(int aname, Unum unum);
	int getIndexOfName(int aname);
	void reset(void);
// Too lazy to make these private:
	CM_Token * rule;	// pointer to the rule
	vector<int> names;	// vector of the names ('A' to 'Z')
	vector<UnumSet*> unumsets;
};


class Memory : public FormationInfo
{
    public:

        // constructor
        Memory();
        void Initialize(); // depends on the size of the teams

    	// PreferredPassPositions
    	PointSet* pppoints;
    	//bool checkWithin(Vector pos, Region * region);

    	// SFLS
	SFLS_System* sfls;


	// SFLS-rules are read into this vector:
	vector<CM_TokRule *> RuleSet;
	// SFLS-infos are read into this vector:
	vector<CM_TokRule *> InfoSet;
	// all rules/tokens have their own set of variables:
	vector<SFLS_UnumVariableSet *> UnumVariableSets;
	// vectors for states of SFLS:
	// Name and value of a StateVariable are in corresponding indices of the vector.
	vector<string> SFLS_StateNames;
	vector<string> SFLS_StateValues;


        // coach language data
        CM_Message* currentCoachMessage;

	vector<CM_Token *> CoachInfo;
	vector<CM_Token *> CoachAdvice;
	
	// vectors for defined coachlanguage-components:
	vector<CM_Condition *> definedConditions;
	vector<CM_Directive *> definedDirectives;
	vector<Region *> definedRegions;
	vector<CM_Action *> definedActions;
	vector<RegPoint *> definedPoints;

	vector<char *> ConditionNames;
	vector<char *> DirectiveNames;
	vector<char *> RegionNames;
	vector<char *> ActionNames;
	vector<char *> PointNames;


        // coach language methods
        void setCoachTokens(void);
	//vector<CM_Token *> getActiveAdvices(void);
	bool eval(CM_Condition * token, CM_Token * rule);
	bool eval(CM_Condition * token, CM_Token * rule, bool positive);
	//vector<CM_Directive*> getDirectivesForPlayer(int playernumber);

	void setCurrentCoachMessage(char message[], int send_time);
	Unum evalFunction(SFLS_Function * function, CM_Condition * condition,
			CM_Token *rule, UnumSet *alreadyIn, bool positive);
	SFLSEffector *effector;
	// 2do No doubt, the following might be solved in a more elegant way.
	void setEffectorMoved(bool moved);
	bool getEffectorMoved(void);
	bool noMoveMode(void);
//protected:

	//void updateCoachVariables(void);
	//void setCoachPullOffsidePosition(CM_Action * action);
	bool checkWithin(Vector pos, Region * region, CM_Condition *cond, CM_Token *tok,
					bool positive);
	void resetTokenVector(vector<CM_Token*> &vec);

	bool evalAnd(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalOr(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalNot(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalPlayerPosition(CM_Condition * token, CM_Token *rule, bool SFLS, bool positive);
	bool evalBallPosition(CM_Condition * token,CM_Token * rule, bool positive);
	bool evalBallOwner(CM_Condition * token, CM_Token *rule, bool SFLS, bool positive);
	bool evalPlayMode(CM_Condition * token);
	bool evalNamed(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalAction(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalBallVelocity(CM_Condition * token, CM_Token *rule);
	bool evalGoalDiff(CM_Condition * token, CM_Token *rule);
	bool evalTime(CM_Condition * token, CM_Token *rule);
	bool evalState(CM_Condition * token);
	bool evalStamina(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalBallInterceptable(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalBallCatchable(CM_Condition * token, CM_Token *rule, bool positive);
	bool evalEqual(CM_Condition * token, bool positive);
	bool evalEqualUnum(CM_Condition * token, CM_Token * rule, bool positive);
	bool evalLess(CM_Condition * token, bool positive);
	bool evalGreater(CM_Condition * token, bool positive);
	short evalTeam(SFLS_Team *team, CM_Token *rule);
	bool evalConditionForJustOneUnum(short unum, CM_Condition * condition);
	void preserveTruthValue(Unum unum, SFLS_UnumSet *SFLSplayerSet, CM_Token * rule,
			CM_Condition * token);
	bool evalComparationOfDoubleVariables(DoubleOrVariable *x, DoubleOrVariable *y,
		short mode, bool positive);
	void introduceVariableWithValue(int variablename, double recentvalue);
	// Double Variables are only kept during evaluation of conditions:
	vector<int> DoubleVariableNames;
	vector<double>  DoubleVariableValues;
	bool handleDoubleVariable(int variablename,double recentvalue);
	int variableSetIndexOfRule(CM_Token * rule);
	void parseSFLSRule(char message[]);
	// active rules:
	vector<CM_Token *> activeRules;
	void removeIrrelevantRulesFromVector(vector<CM_Token *> *rulevector);
	RegPointSimple* evalRegPointBall(void);
	RegPointSimple* evalRegPointPlayer(Region *pointPlayer,
				CM_Condition *condition, CM_Token * token, bool pos);
	RegPointSimple* evaluatePoint(RegPoint *point,CM_Condition *condition,
		CM_Token *rule, bool positive);
	bool doesDirectiveReferToMe(CM_Directive * directive, CM_TokRule* rule);
	Region * specifyRegion(Region * region, CM_Condition * condition, CM_Token *token,
			bool positive);
	SFLS_UnumSet * specifySFLSUnumSet(SFLS_UnumSet * unumset, CM_Condition *condition,
				CM_Token * rule, bool positive);
	CM_Action * specifyAction(CM_Action * action,CM_Condition *condition,
			CM_Token * rule, bool positive);
	//</timo>

};

/* extern Memory *const Mem; */   /* it's in client.h */

#endif
