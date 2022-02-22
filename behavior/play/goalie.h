//_____________________________________________________________________________
//
//  goalie.h
//_____________________________________________________________________________

#ifndef _GOALIE_H
#define _GOALIE_H

#include "defaultplaytree.h"
#include "offenseplaytree.h"


//_____________________________________________________________________________
//
//	Philipp (Goalie)
//_____________________________________________________________________________

void goalie_play_tree();
void goalie_play_on_tree();

void goalie_go_to_ball(AngleDeg);
void goalie_go_to_ball();
void goalie_go_to_point(Vector target_pos , float buffer, float power);
void adjust_neck();
void Nface(AngleDeg);
Vector FindGoaliePosition();

Bool MyCatchableArea();
Bool WaitToCatch();

void my_goalie_free_kick(); //leo
void my_goal_kick(); //leo
Bool isShot(); //leo
Bool BallHeadingTowardsGoal();
Bool ShouldIComeOutToOpponent(); //leo
Bool ShouldIGoToBall(); //leo

#endif


