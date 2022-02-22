//_____________________________________________________________________________
//
//  defenseplaytree.h
//_____________________________________________________________________________

#ifndef _DEFENSEPLAYTREE_H
#define _DEFENSEPLAYTREE_H

#include "defaultplaytree.h"


#define MY_DIST_WEIGHT 2.0
#define GOAL_DIST_WEIGHT 7.0
#define BALL_DIST_WEIGHT 3.0

#define COVER_TOLERANCE 5.0
//_____________________________________________________________________________
//
//	Defensive
//_____________________________________________________________________________


void defensive_positioning_tree();

int coachMessage();

Unum getOpponentToCover();
bool cover();

//int opponentWeight(int length, Unum oppos[]);
float opponentWeight(Unum opp);

void coverOpponent(Unum opponent);
void lookForBallFromHomePos();

bool opponentNotCoveredCheck(Unum opponent);
bool opponentCheck(Unum opponent);
bool alreadyCovered(Unum opponent);

void sortTeammatesByDistanceToPoint(int numTeammates, Vector point, Unum mates[]);


Vector getLookForBallPos(Vector homePos, float range);
Vector smartAdjustedHomePos();



//void sortOpponentsByDistanceToPoint(int numOpponents, Vector point, Unum oppos[]);
//int numTeammatesInOwnPenaltyArea();
//int numOpponentsInOwnPenaltyArea();


#endif
