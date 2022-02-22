
#include "clangdefinitions.h"


void Definitions::setCondition(char * name, CM_Condition * condition){
	ConditionNames.push_back(name);
	definedConditions.push_back(condition);
}
void Definitions::setDirective(char * name, CM_Directive * directive){
	DirectiveNames.push_back(name);
	definedDirectives.push_back(directive);
}
void Definitions::setRegion(char * name, Region * region){
	RegionNames.push_back(name);
	definedRegions.push_back(region);
}
void Definitions::setAction(char * name, CM_Action * action){
	ActionNames.push_back(name);
	definedActions.push_back(action);
}






CM_Condition * Definitions::getCondition(char * name){
	for(int i=0; i<ConditionNames.size(); i++){
		if(strcmp(name, ConditionNames[i])==0)
			return definedConditions[i];
	}
	return NULL;
}
CM_Directive * Definitions::getDirective(char * name){
	for(int i=0; i<DirectiveNames.size(); i++){
		if(strcmp(name, DirectiveNames[i])==0)
			return definedDirectives[i];
	}
	return NULL;
}
Region * Definitions::getRegion(char * name){
	for(int i=0; i<RegionNames.size(); i++){
		if(strcmp(name, RegionNames[i])==0)
			return definedRegions[i];
	}
	return NULL;
}
CM_Action * Definitions::getAction(char * name){
	for(int i=0; i<ActionNames.size(); i++){
		if(strcmp(name, ActionNames[i])==0)
			return definedActions[i];
	}
	return NULL;
}






