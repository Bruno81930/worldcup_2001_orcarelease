//_____________________________________________________________________________
//
//	MemFormation.C
//
//	Sean Buttinger, 2001
//_____________________________________________________________________________

#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream.h>
#include <ctype.h>
#include <stdio.h>

#include "MemFormation.h"


PlayerPosition::PlayerPosition()
{
    Home.x = 0.0;
    Home.y = 0.0;
	HomeRange = 0.0;
	MaxRange = 0.0;
}


PlayerPosition::PlayerPosition(Vector Home, float HomeRange, float MaxRange)
{
    this->SetPlayerPosition(Home, HomeRange, MaxRange);
}


void PlayerPosition::SetPlayerPosition(Vector Home, float HomeRange, float MaxRange)
{
    this->Home = Home;
    this->HomeRange = HomeRange;
    this->MaxRange = MaxRange;
}


void PlayerPosition::SetHome(Vector Home)
{
    this->Home = Home;
}


void PlayerPosition::SetHomeRange(float HomeRange)
{
    this->HomeRange = HomeRange;
}


void PlayerPosition::SetMaxRange(float MaxRange)
{
    this->MaxRange = MaxRange;
}


bool PlayerPosition::PointInHomeRange(const Vector &point)
{
	if(Home.dist(point) <= HomeRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool PlayerPosition::PointInMaxRange(const Vector &point)
{
	if(Home.dist(point) <= MaxRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void PlayerPosition::Print()
{

  MAKELOG((1, DBG_FORM, "current formation: \n home x: %g\n home y: %g\n home range: %g\n max range: %g\n", Home.x, Home.y, HomeRange, MaxRange));

}


//_________________________________________________________________________________


TeamFormation::TeamFormation()
{
    ;
}


/*
TeamFormation::TeamFormation(char filename[], char formation_name[])
{
    FILE *file;

    file = fopen(filename, "r");

    if(!ReadFormation(file, formation_name))
    {
        //this = NULL; //???????
    }

    fclose(file);
}
*/


bool TeamFormation::ReadFormation(FILE* file, char formation_name[])
{
    char line[BUF_LEN];
    char name[BUF_LEN];

    int i = 0;

    bool formation_name_found = false;

    Vector home;
    float home_range;
    float max_range;

    // read lines until line starts with ':'
    while(!formation_name_found && read_next_line(file, line))
	{
        if(line[0] == ':')
        {
            sscanf(line, ":%s ", name);

            if(strcmp(name, formation_name) == 0
            || strcmp("next", formation_name) == 0)
            {
                strcpy(this->name, name);
                formation_name_found = true;
            }
        }
    }

    if(formation_name_found)
    {
        for(i = 0; i < 11 && read_next_line(file, line); ++i)
        {
            sscanf(line, "%f %f %f %f",
                &home.x, &home.y, &home_range, &max_range);

		   	position[i].SetPlayerPosition(home, home_range, max_range);
        }
    }

    return (i == 11 ? true : false);
}


bool TeamFormation::ReadNextFormation(FILE* file)
{
    return ReadFormation(file, "next");
}


bool TeamFormation::read_next_line(FILE* file, char* line)
{
    char read_line[BUF_LEN];
    bool ready = false;
    int i;

    // read one line per loop cycle, exit loop if non-empty and non-comment line read
    while(!ready && fgets(read_line, BUF_LEN, file) != NULL)
    {
        // ignore leading whitespace
        for(i=0; isspace(read_line[i]) && read_line[i] != '\n'; ++i)
        {
            ;
        }

        // ignore comment lines and empty lines
        if(read_line[i] == '#'
        || read_line[i] == '\n')
        {
            ;
        }
        // copy first read non-empty and non-comment line without leading whitespace
        // into 2nd argument, leave loop
        else
        {
            strcpy(line, read_line+i);
            ready = true;
        }
    }

    // return success or failure (in case there was no next line)
    return ready;
}

//_________________________________________________________________________________


TeamFormationArray::TeamFormationArray(char filename[])
{
    FILE* file;
    int i;

    TeamFormation* dummy = new TeamFormation();

    // count formation records in file

    num_formations = 0;

    file = fopen(filename, "r");

    while(dummy->ReadNextFormation(file))
    {
        ++num_formations;
    }

	delete(dummy);

    fclose(file);

    // read formations from file

    formation = new TeamFormation*[num_formations];

    file = fopen(filename, "r");

    for(i = 0; i < num_formations; ++i)
    {
        formation[i] = new TeamFormation();

        formation[i]->ReadNextFormation(file);
    }

    fclose(file);
}


TeamFormation* TeamFormationArray::GetFormation(char name[])
{
    int i;

    for(i = 0; i < num_formations && strcmp(formation[i]->name, name) != 0; ++i)
    {
        ;
    }

    if(i == num_formations)
    {
        return 0;
    }
    else
    {
        return formation[i];
    }
}

//<COMMUNICATION>

TeamFormation* TeamFormationArray::GetFormation(int id)
{
  	if (id >= 0 && id < num_formations)
	{
		return formation[id];
	}
  	else
	{
		return NULL;
	}
}

int TeamFormationArray::GetFormationID(char name[])
{
  	int i;

	for(i = 0; i < num_formations && strcmp(formation[i]->name, name) != 0; ++i)
	{
		;
	}

	if(i == num_formations)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

//</COMMUNICATION>


//_________________________________________________________________________________


 void FormationInfo::Initialize()
{
    char outstring[100];
    string ostr = ("");
    
	formations = new TeamFormationArray("formation.conf");

	currentAdjustedPosition = new PlayerPosition();
 	sprintf(outstring, "\n %d formations read: \n", formations->num_formations);
	ostr+=outstring;

    if(formations != 0 && formations->num_formations > 0)
    {
        for(int i = 0; i < formations->num_formations; ++i)
        {
	  sprintf(outstring, " %i : %s \n", i, formations->formation[i]->name);
	  ostr+=outstring;
        }

        MAKELOG((1, DBG_FORM,"%s", ostr.c_str() ));
    }
    else
    {
        cerr << "ERROR: No formation read." << endl << endl;

		exit(0);
    }

    currentPositionNumber = MyNumber - 1;

    setCurrentFormation("kick_off");
}


/*
	alte Version von FormationInfo::setCurrentFormation(char name[])
	neue Version: s.u.

void FormationInfo::setCurrentFormation(char name[])
{
	TeamFormation* formation = formations->GetFormation(name);

	if(formation == 0)
	{
		cout << "ERROR: formation " << name << " doesn't exist" << endl << endl;
	}
	else
	{
		currentFormation = formation;

		MAKELOG((1, DBG_FORM, "current formation: %s \n", currentFormation->name));

		//cout << "current formation: " << endl;
		//cout << currentFormation->name << endl << endl;

		setCurrentPosition(currentPositionNumber);
	}
}
*/


void FormationInfo::setCurrentPosition(int number)
{
    currentPositionNumber = number;

    currentPosition = &currentFormation->position[currentPositionNumber];

    // currentPosition->Print();
}



void FormationInfo::adjustCurrentPosition()
{
	adjustCurrentHome();
	adjustCurrentHomeRange();
	adjustCurrentMaxRange();
}



void FormationInfo::adjustCurrentHome()
{
/*
  currentAdjustedPosition->Home.x = currentPosition->Home.x
  // + [-1;+1] * homerange
  + (BallX() / (SP_pitch_length/2))  * 40.0
  // - [+1;0] * homerange/2
  //- (1 - ((currentPosition->Home.x+(SP_pitch_length/2)) / SP_pitch_length)) * (currentPosition->HomeRange / 2);
  + (BallX() / (SP_pitch_length/2))
  * (1 - ((currentPosition->Home.x + (SP_pitch_length/2) ) / SP_pitch_length))
  * 5.0
  + (BallX() / (SP_pitch_length/2))
  * ((currentPosition->Home.x + (SP_pitch_length/2) ) / SP_pitch_length)
  * 15.0
  + (BallX() / (SP_pitch_length/2))
  * (1 - (fabs(currentPosition->Home.x) / (SP_pitch_length/2) ))
  * 35.0;
  ////if(fabs(currentAdjustedPosition->Home.x) > SP_pitch_length / 9)
  ////{
  currentAdjustedPosition->Home.x *= (1 - fabs(currentAdjustedPosition->Home.x / (SP_pitch_length * 3)));
  ////} 
  currentAdjustedPosition->Home.y = currentPosition->Home.y;
*/
    
    if(BallX() > 0)
    {
	currentAdjustedPosition->Home.x = currentPosition->Home.x * .8
	    + ( BallX() / (SP_pitch_length/2) ) * 29
	    + (BallX() / (SP_pitch_length/2))
	    * (1 - (fabs(currentPosition->Home.x) / (SP_pitch_length/2) ))
	    * 10.0;
    }
    else
    {
	currentAdjustedPosition->Home.x = currentPosition->Home.x * .8
	    + ( BallX() / (SP_pitch_length/2) ) * 24;
    }
      
    currentAdjustedPosition->Home.y = currentPosition->Home.y;
}


void FormationInfo::adjustCurrentHomeRange()
{
	if(OP_override_home_range)
	{
		currentAdjustedPosition->HomeRange = OP_home_range;
	}
	else
	{
		currentAdjustedPosition->HomeRange = currentPosition->HomeRange;
	}
}


void FormationInfo::adjustCurrentMaxRange()
{
	if(OP_override_max_range)
	{
		currentAdjustedPosition->MaxRange = OP_max_range;
	}
	else
	{
		currentAdjustedPosition->MaxRange = currentPosition->MaxRange;
	}
}


//<COMMUNICATION>

void FormationInfo::setCurrentFormation(char name[])
{
  	int formationID = formations->GetFormationID(name);

  	if(formationID == -1)
  	{
		cout << "ERROR: formation " << name << " doesn't exist" << endl << endl;
	}
  	else
  	{
		SetCurrentFormation(formationID);
  	}
}

bool FormationInfo::isCurrentFormation(char name[])
{
	if (currentFormation == NULL) return false;
	return currentFormationID == formations->GetFormationID(name);
}



void FormationInfo::HearFormation(int formationID, Time formationTime, Unum sender, Time time)
{
	/* The stupid approach, probably some more checks neccessary */
	if (formationTime > currentFormationTime)
    {
		SetCurrentFormation(formationID);
    }
}


void FormationInfo::SetCurrentFormation(int id)
{
  	TeamFormation* formation = formations->GetFormation(id);

 	if (formation == NULL)
  	{
  		return;
	}

	if (!isCurrentFormation(formation->name))
	{
	  currentFormation = formation;
	  currentFormationID = id;
	  currentFormationTime = CurrentTime;
	  
	  MAKELOG((1, DBG_FORM, "current formation: %s", currentFormation->name));
	  
	  // cout << "current formation: " << endl;
	  // cout << currentFormation->name << endl << endl;

	  setCurrentPosition(currentPositionNumber);
	}
}


int FormationInfo::GetCurrentFormationID()
{
    return currentFormationID;
}


Time FormationInfo::GetCurrentFormationTime()
{
    return currentFormationTime;
}

//</COMMUNICATION>


//_____________________________________________________________________________
//
//	check what's going on in my homerange and maxrange
//_____________________________________________________________________________


int FormationInfo::numOpponentsInMyHomeRange()
{
	return numOpponentsInCircle(currentAdjustedPosition->Home, currentAdjustedPosition->HomeRange);
}

int FormationInfo::numOpponentsInMyMaxRange()
{
	return numOpponentsInCircle(currentAdjustedPosition->Home, currentAdjustedPosition->MaxRange);
}


int FormationInfo::numOpponentsInCircle(const Vector &center, float radius)
{

	int num = 0;

	for(int o = 1; o <= 11; ++o)
	{
		if(OpponentPositionValid(o)
		&& o != Unum_Unknown
		&& OpponentAbsolutePosition(o).dist(center) < radius)
		{
			++num;
		}
	}

	return num;
}



int FormationInfo::opponentsInMyHomeRange(Unum opps[])
{
	return opponentsInCircle(opps, currentAdjustedPosition->Home, currentAdjustedPosition->HomeRange);
}


int FormationInfo::opponentsInMyMaxRange(Unum opps[])
{
	return opponentsInCircle(opps, currentAdjustedPosition->Home, currentAdjustedPosition->MaxRange);
}


int FormationInfo::opponentsInCircle(Unum opps[], const Vector &center, float radius)
{
	int num = 0;

	for(int o = 1; o <= 11; ++o)
	{
		if(OpponentPositionValid(o)
		&& o != Unum_Unknown
		&& OpponentAbsolutePosition(o).dist(center) < radius)
		{
			opps[num++] = o;
		}
	}

	return num;
}



int FormationInfo::numTeammatesInMyHomeRange()
{
	return numTeammatesInCircle(currentAdjustedPosition->Home, currentAdjustedPosition->HomeRange);
}


int FormationInfo::numTeammatesInMyMaxRange()
{
	return numTeammatesInCircle(currentAdjustedPosition->Home, currentAdjustedPosition->MaxRange);
}


int FormationInfo::numTeammatesInCircle(const Vector &center, float radius)
{
	int num = 0;

	for(int m = 1; m <= 11; ++m)
	{
		if(m != MyNumber
		&& TeammatePositionValid(m)
		&& m != Unum_Unknown
		&& TeammateAbsolutePosition(m).dist(center) < radius)
		{
			++num;
		}
	}

	return num;
}



int FormationInfo::teammatesInMyHomeRange(Unum mates[])
{
	return teammatesInCircle(mates, currentAdjustedPosition->Home, currentAdjustedPosition->HomeRange);
}


int FormationInfo::teammatesInMyMaxRange(Unum mates[])
{
	return teammatesInCircle(mates, currentAdjustedPosition->Home, currentAdjustedPosition->MaxRange);
}


int FormationInfo::teammatesInCircle(Unum mates[], const Vector &center, float radius)
{
	int num = 0;

	for(int m = 1; m <= 11; ++m)
	{
		if(m != MyNumber
		&& TeammatePositionValid(m)
		&& m != Unum_Unknown
		&& TeammateAbsolutePosition(m).dist(center) < radius)
		{
			mates[num++] = m;
		}
	}

	return num;
}



bool FormationInfo::ballInMyHomeRange()
{
	return currentAdjustedPosition->PointInHomeRange(BallAbsolutePosition());
}


bool FormationInfo::ballInMyMaxRange()
{
	return currentAdjustedPosition->PointInMaxRange(BallAbsolutePosition());
}



bool FormationInfo::amInMyHomeRange()
{
	return currentAdjustedPosition->PointInHomeRange(MyPos());
}


bool FormationInfo::amInMyMaxRange()
{
	return currentAdjustedPosition->PointInMaxRange(MyPos());
}



bool FormationInfo::pointInMyHomeRange(const Vector &point)
{
	return currentAdjustedPosition->PointInHomeRange(point);
}


bool FormationInfo::pointInMyMaxRange(const Vector &point)
{
	return currentAdjustedPosition->PointInMaxRange(point);
}

