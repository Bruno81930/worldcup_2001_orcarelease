//_____________________________________________________________________________
//
//  offenseplaytree.h
//_____________________________________________________________________________

#ifndef _OFFENSEPLAYTREE_H
#define _OFFENSEPLAYTREE_H

#include "defaultplaytree.h"


/// preferred passline variables ==============================================

extern int num_pref_pass_pts;
extern Vector ppps[];
extern Vector ppps_left[];
extern Vector ppps_right[];

extern Unum mates[];
extern Unum oppos[];
extern Vector  pt[];

/// preferred passline variables ==============================================


void offensive_positioning_tree();
void play_with_ball_tree();

bool passpartner();

void direction();


bool bestMates(int pppIndex);

bool bestPt(int numMates);

Vector getPassPosForMate(Unum mate, Vector targetPos);

void orca_dribble();
void orca_pass_ball(Unum mate, Vector pos);
void orca_hold_ball();
void orca_stop_ball();

bool goal_kick();

float positionPassValue(Vector pos);
float troubleValue(Unum mate_unum, float radius);
float troubleValue(Vector pos, float radius);
float positionValue(Unum mate_unum);
float positionValue(Vector pos);

int opponentsInPassway(Unum mate_unum);
int numOpponentsInPassway(Unum mate_unum, float angle, float extra_radius_ratio);
int numOpponentsInPassway(Vector point, float angle, float extra_radius_ratio);
int numOpponentsInPassway(Vector mate_pos, Vector ball_pos, float angle, float extra_radius_ratio);
int numTeammatesInPassway(Vector target_pos, Vector start_pos, float angle, float extra_radius_ratio, Bool IncludeMe);
bool opponentInPassway(Unum unum, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio);
bool mateInPassway(Unum unum, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio);
bool playerInPassway(char side, Unum unum, Vector start_pos, Vector target_pos,
float pass_angle, float extra_radius_ratio);
    Vector getFreeRunPos(Vector home_pos, float range);


int teammatesInCone(float wid_dist_ratio, Vector end, Vector vert, Bool IncludeMe); 
int opponentsInCone(float wid_dist_ratio, Vector end, Vector vert);


void Sort(int length, int *elements, int *keys);
void Sort1(int length, Vector *elements, Unum *mates, float *keys);

void free_run();
void free_run(Vector home, float range);
Vector getFreeRunPos(Vector home_pos, float range);                       

//===============================================
// Unum bestPasspartner(Region region = NULL)
//===============================================
void orca_pass_ball(Unum mate);

Unum bestPasspartner(Region* region = NULL);

int pppRegionCheck(int* pppis, Region* region);
int pppOppCheck(int* pppis);
Unum pppBestMate(int* pppis);

bool pppMateEligibleForPass(Unum mate, Vector target_pos);
bool posInPassway(Vector pos, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio);

#endif
