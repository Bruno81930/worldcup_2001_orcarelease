//_____________________________________________________________________________
//
//	SFLS.C
//_____________________________________________________________________________

// 2do BUG: new definitions should be checked if they already exist. overwrite
//	instead of pushing_back in such a case.

#include "SFLS.h"

#include "../client.h"
#include "../Memory.h"

#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <ctype.h>
#include <stdio.h>

//_____________________________________________________________________________
//
//	SFLS_System
//_____________________________________________________________________________

// SFLS_System::SFLS_System

SFLS_System::SFLS_System(char* filename)
{
	this->advices = new SFLS_RuleBase(&(Mem->RuleSet));
	this->infos = new SFLS_RuleBase(&(Mem->InfoSet));

	this->read_file(filename);
}


// SFLS_System::read_file

void SFLS_System::read_file(char* filename)
{
	FILE* file;
	char ch;

	int n = 0;
	char* message = (char*)malloc(SFLS_STR_LEN * sizeof(char));

	int credit;
	char* credit_str = (char*)malloc(SFLS_STR_LEN * sizeof(char));
	
	bool first = true;
	
	file = fopen(filename, "r");

	if(!file)
	{
		cerr << "Error: cannot open SFL-Rulefile" << endl;
		exit(1);
	}


	while((ch = fgetc(file)) != EOF)
	{
		// nach Kommentarzeichen Text bis zum Zeilenende ignorieren
   		if(ch == SFLS_COMMENT_SIGN)
   		{
   			while(ch != '\n')
   				ch = fgetc(file);
		}
				
		// n blanks/newlines -> 1 space
		else if(ch == ' ' || ch == '\t' || ch == '\n')
		{
			while(ch == ' ' || ch == '\t'|| ch == '\n')
				ch = fgetc(file);
			 			
			ungetc(ch, file);
			
 			message[n++] = ' ';
  		}
   		   		
		else if (ch == SFLS_CREDIT_START)
		{
   			// ausser erstes Mal: vorherige message abschliessen und an Parser uebergeben
   			if(!first)
        	{
        		message[n] = '\0';
		    	this->handle_message(message, credit);
			}
			else
			{
				first = false;
			}
       	
       	    //  credit lesen
       		n = 0;     			
   			while(ch != SFLS_CREDIT_END)
   			{
  				ch = fgetc(file);
  				credit_str[n++] = ch;
  			}
  			credit_str[n-1] = '\0'; // SFLS_CREDIT_END ueberschreiben
			sscanf(credit_str, "%d", &credit);
       	
 			// neue message starten
  			n = 0;
			message = (char*)malloc(SFLS_STR_LEN * sizeof(char));
		}
		
		else
		{
			message[n++] = ch;
		}
	}

    // letzte message abschliessen und an Parser uebergeben
    message[n] = '\0';
    this->handle_message(message, credit);
        	 	
	fclose(file);
}


// SFLS_System::handle_message

void SFLS_System::handle_message(char* msg_str, int credit)
{
	CM_Message* message = parse_coach_message(msg_str);


		message->Print(cerr); cerr << endl;

	
	if(message != 0) switch(message->getType())
	{
		case CMT_Advice:
		{
			vector<CM_Token*> tokens = ((CM_AdviceMessage*)message)->getTokens();

			for(int i = 0; (unsigned)i < tokens.size(); i++)
			{
				if(tokens[i]->getType() == CMT_Rule) // ignore clear-tokens
				{
					this->advices->insert(new SFLS_Rule((CM_TokRule*)tokens[i], credit));
				}
			}

			break;
		}

		case CMT_Info:
		{
			vector<CM_Token*> tokens = ((CM_InfoMessage*)message)->getTokens();

			for(int i = 0; (unsigned)i < tokens.size(); i++)
			{
				if(tokens[i]->getType() == CMT_Rule) // ignore clear-tokens
				{
					this->infos->insert(new SFLS_Rule((CM_TokRule*)tokens[i], credit));
				}
			}

			break;
		}

		case CMT_Define:
		{
			vector<CM_DefineToken*> tokens = ((CM_DefineMessage*)message)->getTokens();

			for(int i = 0; (unsigned)i < tokens.size(); i++)
			{
				switch(tokens[i]->getType())
				{
					case CMDT_Condition:
					{
						Mem->definedConditions.push_back(((CM_DefTokCondition*)tokens[i])->pCond);
						Mem->ConditionNames.push_back(((CM_DefTokCondition*)tokens[i])->getName());
						break;
					}
					case CMDT_Directive:
					{
						Mem->definedDirectives.push_back(((CM_DefTokDirective*)tokens[i])->pDir);
						Mem->DirectiveNames.push_back(((CM_DefTokDirective*)tokens[i])->getName());
						break;
					}
					case CMDT_Region:
					{
						Mem->definedRegions.push_back(((CM_DefTokRegion*)tokens[i])->pReg);
						Mem->RegionNames.push_back(((CM_DefTokRegion*)tokens[i])->getName());
						break;
					}
					case CMDT_Action:
					{
						Mem->definedActions.push_back(((CM_DefTokAction*)tokens[i])->pAct);
						Mem->ActionNames.push_back(((CM_DefTokAction*)tokens[i])->getName());
						break;
					}
					case SFLSDT_Point:
					{
						Mem->definedPoints.push_back(((SFLS_DefTokPoint*)tokens[i])->pPoint);
						Mem->PointNames.push_back(((SFLS_DefTokPoint*)tokens[i])->getName());
						break;						
					}
					default:
					{
						break;
					}
				}
			}

			break;
		}

		case CMT_Freeform:
		case CMT_Meta:
		default:
		{
			break;
		}
	}
}


// SFLS_System::one_cycle

void SFLS_System::one_cycle()
{
	MAKELOG((30,DBG_OTHER,"\n\n(%d) SFLS-cycle START @ %d", Mem->MyNumber, Mem->CurrentTime.t));

	//this->advices->print();

	this->advices->match();

	this->advices->select();

	if(((SFLS_Rule*)this->advices->active_rule)!=NULL)
	{
		((SFLS_Rule*)this->advices->active_rule)->apply();
	}
	else cerr << "Player " << Mem->MyNumber << " executing NULL @ "<< Mem->CurrentTime.t <<"\n" << endl;
	
	MAKELOG((30,DBG_OTHER,"(%d) SFLS-cycle END @ %d\n\n", Mem->MyNumber, Mem->CurrentTime.t));
}



//_____________________________________________________________________________
//
//	SFLS_RuleBase
//_____________________________________________________________________________

// SFLS_RuleBase::SFLS_RuleBase

SFLS_RuleBase::SFLS_RuleBase(vector<CM_TokRule*>* rules)
{
	this->rules = rules;
	this->rules->clear();

	this->matching_rules = new vector<CM_TokRule*>();

	this->active_rule = 0;
}


// SFLS_RuleBase::insert

void SFLS_RuleBase::insert(SFLS_Rule* rule)
{
	this->rules->push_back(rule);
}


// SFLS_RuleBase::print

void SFLS_RuleBase::print()
{
	cout << "SIZE:" << this->rules->size() << endl;

	for(unsigned i = 0; i < this->rules->size(); ++i)
	{
		cout << "rule #" << i << ": ";

		((SFLS_Rule*)((*this->rules)[i]))->print();

		cout << endl;
	}
}


// SFLS_RuleBase::match

void SFLS_RuleBase::match()
{
	for(unsigned i = 0; i < Mem->UnumVariableSets.size(); ++i)
	{
		Mem->UnumVariableSets[i]->reset();
	}

	this->matching_rules->clear();

	for(unsigned i = 0; i < this->rules->size(); ++i)
	{
		Mem->DoubleVariableNames.clear();
		Mem->DoubleVariableValues.clear();

		if((*(this->rules))[i]->getTimeToLive() < Mem->CurrentTime.t)
		{
			continue;
		}

		if(((SFLS_Rule*)((*(this->rules))[i]))->eval())
		{
			this->matching_rules->push_back((*(this->rules))[i]);


			// <hack by timo>
			// Stop matching as soon as one rule matched. This practically turns off
			// the selector. Used for master thesis. Remove at own will everything between <...> tags.
			// need to make sure that matching rule also refers to me!
			
			vector<CM_Directive*> directives; // the directives of the matching rule
			bool relevant=false;	// does this rule refer to the agent?
			
			CM_TokRule * matchedRule=(CM_TokRule*)((*(this->rules))[i]);
		        directives=matchedRule->getDirectives();

        		for(int dir=0; dir<(signed)directives.size(); dir++){
		    		relevant=Mem->doesDirectiveReferToMe(directives[dir],matchedRule);
		    		if(relevant) break;
        		}
			if(relevant)
			{
				i=this->rules->size()+1;
			}
			// </hack by timo>
		}
	}


	Mem->removeIrrelevantRulesFromVector((vector<CM_Token*>*)(this->matching_rules));

	for(unsigned i = 0; i < this->matching_rules->size(); ++i)
	{
		//cout << "Player " << Mem->MyNumber << ": RELEVANT MATCHING RULE: ";
		//((SFLS_Rule*)((*(this->matching_rules))[i]))->print();
		//cout << endl;
	}


}


// SFLS_RuleBase::select

void SFLS_RuleBase::select()
{
	int credit_max = -9999999;
	unsigned index;
	unsigned i;

	// <thesis>
	this->active_rule = NULL;
	// </thesis>

	for(i = 0; i < this->matching_rules->size(); ++i)
	{
		if (((SFLS_Rule*)((*this->matching_rules)[i]))->credit > credit_max)
		{
			this->active_rule = (*this->matching_rules)[i];

			credit_max = ((SFLS_Rule*)((*this->matching_rules)[i]))->credit;

			index = i;
		}
	}

	if(this->matching_rules->size() == 0)
	{
		cerr << "Player " << Mem->MyNumber << ": SELECTOR: NO RULE SELECTED @ " << Mem->CurrentTime.t
			 << ",  # of matching rules: " << this->matching_rules->size() << endl;
	}
	else
	{
		//cout<< "Player " << Mem->MyNumber << "SELECTED RULE: ";
		//((SFLS_Rule*)((*this->matching_rules)[index]))->print();
		//cout << endl;
  	}
}


//_____________________________________________________________________________
//
//	SFLS_Rule
//_____________________________________________________________________________


// SFLS_Rule::SFLS_Rule

SFLS_Rule::SFLS_Rule(CM_TokRule* rule, int credit) : CM_TokRule(rule->getTimeToLive(),
                                                                rule->getCondition(),
                                                                rule->getDirectives())
{
	this->credit = credit;
}


// SFLS_Rule::print

void SFLS_Rule::print()
{
	cout << *((CM_Token*)this) << " CREDIT: " << this->credit;
}

// SFLS_Rule::eval

bool SFLS_Rule::eval()
{
	return Mem->eval(this->getCondition(), (CM_Token*)this);
}


// SFLS_Rule::apply

void SFLS_Rule::apply()
{
	vector<CM_Directive*> directives = this->getDirectives();

	int index = -1;

	for(unsigned i = 0; i < directives.size(); ++i)
	{
		if(Mem->doesDirectiveReferToMe(directives[i], this))
		{
			index = i;

			break;
		}
	}

	if(index != -1)
	{
		CM_Action* action;

		action = Mem->specifyAction(((CM_DirCommand*)directives[index])->getAction(),
		 		 this->getCondition(), (CM_Token*)this, true);

		if(!action)
		{
			MAKELOG((30,DBG_OTHER,"WARNING: no action can be executed."));
			return;
		}

		cout << "Player " << Mem->MyNumber << ": EXECUTING ACTION: ";
		action->Print(cout);
		cout << " @ " << Mem->CurrentTime.t << "\n"<< endl;

		Mem->effector->doAction(action);
    }
	else
	{
		MAKELOG((30,DBG_OTHER,"WARNING: no directive refers to me (%d) @ %d", Mem->MyNumber, Mem->CurrentTime.t));
	}
}


