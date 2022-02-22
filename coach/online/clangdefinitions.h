
#include "coach_lang.h"
#include "coach_lang_comp.h"
#include "region.h"


class Definitions{

	// methods to get an defined object by its name:
	CM_Condition * getCondition(char * name);
	CM_Directive * getDirective(char * name);
	Region * getRegion(char * name);
	CM_Action * getAction(char * name);
	// methods to append new definition:
	void setCondition(char * name, CM_Condition * condition);
	void setDirective(char * name, CM_Directive * directive);
	void setRegion(char * name, Region * region);
	void setAction(char * name, CM_Action * action);


protected:
	// vectors for defined coachlanguage-components:
	vector<CM_Condition *> definedConditions;
	vector<CM_Directive *> definedDirectives;
	vector<Region *> definedRegions;
	vector<CM_Action *> definedActions;

	// vectors for their names:	
	vector<char *> ConditionNames;
	vector<char *> DirectiveNames;
	vector<char *> RegionNames;
	vector<char *> ActionNames;

};

