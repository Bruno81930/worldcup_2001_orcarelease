//_____________________________________________________________________________
//
//  defenseplaytree.C
//_____________________________________________________________________________

#include "defenseplaytree.h"

//________________________________________________________________
//
//  defensive_positioning_tree() : Verhalten in der Defensive 
//________________________________________________________________


void defensive_positioning_tree()
{

    int playerThatIShouldMark = coachMessage();

    if (playerThatIShouldMark != Unum_Unknown && Mem->OpponentPositionValid(playerThatIShouldMark) && playerThatIShouldMark != -1) {
		coverOpponent(playerThatIShouldMark);
    }

    else {

   //	Vector homePos = Mem->currentAdjustedPosition->Home;  

	Vector ballPos = Mem->BallAbsolutePosition();

	if (Mem->BallPositionValid()) {
	  
          if(Mem->FastestTeammateToBall() == Mem->MyNumber)
          {
	     orca_get_ball();
          }
          else if (Mem->currentPosition->Home.x < -24.0)     
	  {
	     lookForBallFromHomePos();
          }
	  else if (Mem->NumTeammatesCloserTo(ballPos) < 2)
	  {
	     orca_get_ball();
	  }
	  else if (!cover()) 
	  {
	     lookForBallFromHomePos();
	  }
 	}
		/*	if (Mem->FastestTeammateToBall() == Mem->MyNumber)
			{ 
				if(Mem->MyInterceptionAble()) {
		  
					orca_go_to_point(Mem->MyInterceptionPoint(), Mem->SP_kickable_area * 0.7, 100);
		  
					if (Mem->BallKickable()) {
						if (Mem->BallSpeed()>0.3) stop_ball(); 
						else { hold_ball(); }
					}
					else {
						orca_get_ball();
					}
				}
				else if (Mem->NumTeammatesCloserTo(ballPos) < 2) {
					orca_get_ball();
				}
				else if ((Mem->currentPosition->Home.x < -24.0)
						 || !cover())
				{
					lookForBallFromHomePos();
				}
			}
			else if ((Mem->currentPosition->Home.x < -24.0)
					 || !cover())
			{
				lookForBallFromHomePos();
			}
		}*/
	else {
	  if ((Mem->currentPosition->Home.x < -24.0) || !cover())
	  {
	    lookForBallFromHomePos();
	    //orca_go_to_point(homePos, 1.0, 70);
	  }
	}
    }
}




//____________________________________________________________________
//
//  coachMessage() : liefert einen zu deckenden Gegner vom Coach 
//____________________________________________________________________

int coachMessage()
{ 
       
    int playerThatIShouldMark = -1;
    
    vector <CM_Directive*> directives;
    /*
    directives = Mem->getDirectivesForPlayer(Mem->MyNumber);

    for (unsigned int dir = 0; dir < directives.size(); dir++) {

		CM_Action* action = ((CM_DirCommand*)directives[dir])->getAction();

		if (((CM_DirCommand*)directives[dir])->isPositive() && action->getType() == CMA_Mark) {

			playerThatIShouldMark = -1; // obsolete ((CM_ActMark*)action)->getNum();
		}
    }
    */
    return playerThatIShouldMark;

}

bool cover()
{

    Unum myOpponent = getOpponentToCover();

    if (myOpponent != 0)
    {
		coverOpponent(myOpponent);
		return true;
    }
    else return false;

}


//________________________________________________________
//
//  getOpponentToCover() : besten Gegner ermitteln
//________________________________________________________

Unum getOpponentToCover()
{
    Unum oppos[Mem->SP_team_size];

    for (int i = 0; i < Mem->SP_team_size; i++) {
		oppos[i] = 0;
    }
	
    // numOpponents ist die Anzahl der Gegner in meiner MaxRange
    // rufe Funktion zur Gegnergewichtung auf
    int numOpponents = Mem->opponentsInMyMaxRange(oppos);

	if (numOpponents == 0) return 0;
    
    for (int i = 0; i < numOpponents; i++) {
		if (!opponentCheck(oppos[i]))
		{
			numOpponents--;
			for (int j = i; i < numOpponents; i++)
			{
				oppos[j] = oppos[j + 1];
			}
		}
    }

    Unum opponent[numOpponents];

    float oppWeights[numOpponents];
    
    for (int i = 0; i < numOpponents; i++) {
		opponent[i] = oppos[i];
		oppWeights[i] = opponentWeight(oppos[i]);
    }
    // Array sortieren, damit der Beste am Anfang steht
    BubbleSort(numOpponents, opponent, oppWeights);
    
    bool found = false;

    Unum myOpponent = 0;
    
    for (int i = 0; i < numOpponents; i++) {
	
		// wenn ausgewaehlte Gegner nicht schon gedeckt, dann gefunden
		if (opponentNotCoveredCheck(opponent[i])) {

			myOpponent = opponent[i];
			
			found = true;
	    
			break;
		}
    }


    // wenn kein passender Gegner in meiner MaxRange zum decken, dann versuche ich den dichtesten am eigenen Tor 
    if (found == false) {

		if (Mem->BallX() < 20) {

			myOpponent = Mem->ClosestOpponentTo(MY_GOAL_VECTOR);
		}

		else {

			myOpponent = Mem->ClosestOpponentTo(THEIR_GOAL_VECTOR);
		}
    
    	// wenn nicht schon gedeckt, dann gefunden
		if (opponentNotCoveredCheck(myOpponent)) {

			found = true;
	
		}
		
		// wenn noch kein passender Gegner gefunden, dann versuche ich den mir am dichtesten 
		else {
			
			myOpponent = Mem->ClosestOpponent();
	
			// wenn nicht schon gedeckt, dann gefunden
			if (opponentNotCoveredCheck(myOpponent)) {

				found = true;
				
			}
		}
    }
 
    // wenn gefunden und Position valid und Nummer bekannt, dann Gegner decken (coverOpponent) 
    if (found == true) {
		return myOpponent;
		//coverOpponent(myOpponent);
    }
    return 0;

}


float opponentWeight(Unum opp)
{
    
    float oppWeight = pow(Mem->OpponentDistanceTo(opp, Mem->currentAdjustedPosition->Home) * Mem->OP_home_dist_weight, Mem->OP_home_dist_pow); 
	
    oppWeight += pow(Mem->OpponentDistanceTo(opp, Mem->MyPos()) * Mem->OP_my_dist_weight, Mem->OP_my_dist_pow); 
    if(Mem->BallPositionValid())
    {    
		oppWeight += pow(Mem->OpponentDistanceTo(opp, Mem->BallAbsolutePosition()) * Mem->OP_ball_dist_weight, Mem->OP_ball_dist_pow);
    }
	
	
	if (Mem->BallX() < 0) {
		oppWeight += pow(Mem->OpponentDistanceTo(opp, MY_GOAL_VECTOR) * Mem->OP_goal_dist_weight, Mem->OP_goal_dist_pow);  // GOAL_DIST_WEIGHT = 2.0
	}
	else 
    {
		
		oppWeight += Mem->OpponentDistanceTo(opp, THEIR_GOAL_VECTOR) * Mem->OP_goal_dist_weight;  // GOAL_DIST_WEIGHT = 2.0
		
		if (Mem->BallPositionValid() && (Mem->BallX() < -20)) {
			
			if (opponentCheck(opp)) {
				
				// 1. Sonderfall: hoehere Gewichtung der Gegner, wenn der Ball 10m und sie 20m vorm eigenen Tor  
				if (Mem->OpponentX(opp) < -20) {
					
					oppWeight -= oppWeight / 8.0;
				}
				
				// 2. Sonderfall: hoehere Gewichtung der Gegner, wenn der Ball 10m vorm eigenen Tor und sie im eigenen Strafraum  
				if (Mem->InOwnPenaltyArea(opp)) {
					
					oppWeight -= oppWeight / 8.0;
				}
				
				// 3. Sonderfall: hoehere Gewichtung der Gegner, wenn der Ball 10m vorm eigenen Tor und  mehr Gegner als Mitspieler im Ballumkreis von 3.5m
				// COVER_TOLERANZ = 3.5
				int oppsInCircle  = Mem->numOpponentsInCircle(Mem->OpponentAbsolutePosition(opp), Mem->OP_cover_tolerance * 2.0);  
				
				int matesInCircle = Mem->numTeammatesInCircle(Mem->OpponentAbsolutePosition(opp), Mem->OP_cover_tolerance * 2.0);
				
				oppWeight -= oppWeight * (oppsInCircle - matesInCircle) / 11.0;
				
			}
		}
    }
	if(Mem->OP_cover_use_trouble)
	{
		float trouble = 0;
		
		Vector opp_pos = Mem->OpponentAbsolutePosition(opp);
		
		for(Unum mate_unum = 1; mate_unum <= Mem->SP_team_size; ++mate_unum)
		{
			if (!Mem->TeammatePositionValid(mate_unum))
			{
				continue;
			}
			else
			{
				float mate_opp_dist = Mem->TeammateDistanceTo(mate_unum, opp_pos);
				
				if(mate_opp_dist < Mem->OP_cover_tolerance * 3)
				{
					trouble += pow(Mem->OP_cover_tolerance * 3 - mate_opp_dist, 2);
				}
			}
		}
		oppWeight /= trouble + .001;
	}
	return oppWeight;
}

//__________________________________________________
//
//  coverOpponent(..) : besten Gegner decken
//__________________________________________________

void coverOpponent(Unum myOpponent)
{

	float goalDist;

	Vector opponentPos = Mem->OpponentAbsolutePosition(myOpponent);

   
	// wenn die Distanze des Gegners zum eigenen Tor kleiner als 35m, dann stelle ich mich zwischen Tor und Gegner  
	if ((goalDist = opponentPos.dist(MY_GOAL_VECTOR)) < 30) {

		float ptdist = goalDist / 5.0;
		
		if(ptdist > Mem->OP_cover_tolerance) ptdist = Mem->OP_cover_tolerance; 
	
		Vector target = PointInBetween(opponentPos, MY_GOAL_VECTOR, ptdist);
			
		orca_go_to_point(target, 1.0, 100);
	
		face_only_neck_to_ball();
			
	}

	// wenn nicht, aber die BallPosition ist valid, dann stelle ich mich zwischen Ball und Gegner 
	else if (Mem->BallPositionValid()) {

		float ballDist =  Mem->BallAbsolutePosition().dist(opponentPos);
		
		float ptdist  = 3.0;
		
		float buffer  = ballDist / 10.0;
		
		if(ptdist > Mem->OP_cover_tolerance) ptdist = Mem->OP_cover_tolerance; 
		
		Vector target = PointInBetween(opponentPos, Mem->BallAbsolutePosition(), ptdist);
			
		orca_go_to_point(target, buffer, 100);
				
		face_only_neck_to_ball();
			
	}

	// wenn die BallPosition nicht bekannt ist, dann gehe ich zum Gegner     
	else {
		orca_go_to_point(opponentPos, Mem->OP_cover_tolerance / 2.0, 100);
	}
}




// richte mich von der aktiellen HomePosition zum Ball aus
void lookForBallFromHomePos()
{

	Vector homePos  = Mem->currentAdjustedPosition->Home;  

	float  homeRange = Mem->currentAdjustedPosition->HomeRange;

	Vector ballPos = Mem->BallAbsolutePosition();
 

	Vector next_pos = PointInBetween(homePos, ballPos, homeRange);
										
	orca_go_to_point(next_pos, 1.0, 70);
	
}

// ueberpruefen, ob der ausgewaehlte Gegner (opponent) schon gedeckt wird 
bool opponentNotCoveredCheck(Unum opponent)
{
	return (opponentCheck(opponent) && !alreadyCovered(opponent));
}



bool opponentCheck(Unum opponent)
{
	return ((opponent != Unum_Unknown) && Mem->OpponentPositionValid(opponent));
}




bool alreadyCovered(Unum opponent)
{

	return ((Mem->numTeammatesInCircle(Mem->OpponentAbsolutePosition(opponent), Mem->OP_cover_tolerance)
		     - Mem->numOpponentsInCircle(Mem->OpponentAbsolutePosition(opponent), Mem->OP_cover_tolerance * 1.5)
		     >= 0));
    
}


    

Vector smartAdjustedHomePos()
{
	Vector homePos = Mem->currentAdjustedPosition->Home;
	float homeRange = Mem->currentAdjustedPosition->HomeRange;


        if((Mem->PlayMode==PM_My_Goalie_Free_Kick) ||(Mem->PlayMode==PM_My_Goal_Kick))
	{
	    homePos = Mem->currentPosition->Home;
	}

	if (Mem->HeardPassValid() && Mem->GetHeardPasspartner() == Mem->MyNumber)
	{
	    homePos = Mem->GetHeardPassTargetVector();
	}

	if (Mem->TeamInPossession() == Mem->MySide) 
	{
		homePos = getFreeRunPos(homePos, homeRange);
	}
	else if (Mem->currentPosition->Home.x < -24.0)     
	{
		homePos = getLookForBallPos(homePos, homeRange);
	}

	return homePos;
}


Vector getLookForBallPos(Vector homePos, float range)
{
	Vector ballPos = Mem->BallAbsolutePosition();

	return PointInBetween(homePos, ballPos, range);
}
