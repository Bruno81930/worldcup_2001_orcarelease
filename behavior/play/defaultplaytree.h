//_____________________________________________________________________________
//
//  defaultplaytree.h
//_____________________________________________________________________________

#ifndef _DEFAULTPLAYTREE_H
#define _DEFAULTPLAYTREE_H

#include "client.h"
#include "Memory.h"
#include "test.h"
#include "geometry.h"
#include "dribble.h"
#include "behave.h"
#include "goalie.h"
#include "offenseplaytree.h"
#include "defenseplaytree.h"

#define MY_GOAL_VECTOR Vector(-Mem->SP_pitch_length/2, 0)
#define THEIR_GOAL_VECTOR Vector(Mem->SP_pitch_length/2, 0)


//_____________________________________________________________________________
//
//	Playtree
//_____________________________________________________________________________

void orca_play_tree();
void goal_kick_positioning_tree();
void kick_off_tree();
void their_kick_tree();
void my_kick_tree();
void play_on_tree();
void orca_go_to_point(Vector point, float buffer = 0.0, float dash_power = 100);
void orca_get_ball();



//_____________________________________________________________________________
//
//	Offensive
//_____________________________________________________________________________

//_____________________________________________________________________________
//
//	Defensive
//_____________________________________________________________________________

//_____________________________________________________________________________
//
//	Coach Message
//_____________________________________________________________________________

void handleCoachMessage();

#endif



