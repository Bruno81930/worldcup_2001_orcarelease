//_____________________________________________________________________________
//
//	SFLS.h
//_____________________________________________________________________________
//
//	SFLS_System
// 	SFLS_Rule
//	SFLS_RuleBase
//
//	in ../behavior/orcabehavior.c:
//
//		Mem->sfls->advices->match();
//		Mem->sfls->advices->select();
//		SFLS_Rule* active_rule = (SFLS_Rule*)Mem->sfls->advices->active_rule;
//		active_rule->apply();
//_____________________________________________________________________________


#ifndef _SFLS_H_
#define _SFLS_H_


#include "coach_lang.h"
#include "coach_lang_comp.h"
#include "SFLShierarchy.h"

#include <vector.h>


#define SFLS_MSG_LEN 	4096
#define SFLS_STR_LEN 	(SFLS_MSG_LEN + 128)

#define SFLS_CREDIT_START	'<'
#define SFLS_CREDIT_END		'>'
#define SFLS_COMMENT_SIGN	'#'


class SFLS_System;
class SFLS_RuleBase;
class SFLS_Rule;


//_____________________________________________________________________________
//
//	SFLS_System
//_____________________________________________________________________________


class SFLS_System
{
	public:

	// data
	SFLS_RuleBase* advices;
	SFLS_RuleBase* infos;

	// constructors
	SFLS_System(char* filename);

	// methods
	void read_file(char* filename);
	void handle_message(char* msg_str, int credit);
	void one_cycle();
};


//_____________________________________________________________________________
//
//	SFLS_RuleBase
//_____________________________________________________________________________


class SFLS_RuleBase
{
	public:

	// data
    vector<CM_TokRule*>* rules;
	vector<CM_TokRule*>* matching_rules;
	CM_TokRule* active_rule;

	// constructors
	SFLS_RuleBase(vector<CM_TokRule*>* rules);

	// methods
    void insert(SFLS_Rule* rule);
	void print();
    void match();
	void select();
};


//_____________________________________________________________________________
//
//	SFLS_Rule
//_____________________________________________________________________________


class SFLS_Rule : public CM_TokRule
{
	public:

	// data
	int ttl;
	int credit;

	// constructors
    SFLS_Rule(CM_TokRule* rule, int credit = 0);

	// methods
	void print();
	bool eval();
	void apply();
};


#endif
