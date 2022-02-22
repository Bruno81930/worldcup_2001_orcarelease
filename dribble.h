/* -*- Mode: C++ -*- */

/* dribble.h
 * CMUnited99 (soccer client for Robocup99)
 * Peter Stone <pstone@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Peter Stone
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy and distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 * For more information, please see http://www.cs.cmu.edu/~robosoccer/
 */

#ifndef _DRIBBLE_H
#define _DRIBBLE_H

#include "Memory.h"
#include "kick.h"

void PatsTest_dribble();
void PatsTest_dribble2();

typedef enum DRIBBLERES {
  DR_None,
  DR_Error,
  DR_LostBall,
  DR_GotThere,
  DR_Going
} DribbleRes;

typedef enum DRIBBLEMODE {
  DM_None,
  DM_Strict, /* moves very little if ball is at wrong angle */
  DM_Lazy    /* will pick an intermediate point if switching sides, but will
		keep moving. Not as safe with players around */
} DribbleMode;


TurnKickCommand dribble_dash(Vector vEndPos, float max_pow,
			     AngleDeg drib_ang, DribbleMode mode);
TurnKickCommand dribble_kick(Vector vEndPos, float max_pow,
			     AngleDeg drib_ang, DribbleMode mode);
AngleDeg NormalizeDribbleAngle(AngleDeg ang);

/* drib_ang should be between 90 and -90
   we always dribble in front of us */
DribbleRes DribbleTo(Vector vEndPos, float max_dash_pow, float buffer,
		     AngleDeg drib_ang, DribbleMode mode,
		     Bool IsDodge=FALSE, Vector DodgePoint=0);

DribbleRes SmartDribbleTo(Vector vEndPos, float max_dash_pow, float buffer);
inline DribbleRes SmartDribbleTo(Vector vEndPos, float max_dash_pow)
{ return SmartDribbleTo(vEndPos, max_dash_pow, Mem->CP_at_point_buffer); }
inline DribbleRes SmartDribbleTo(Vector vEndPos)
{ return SmartDribbleTo(vEndPos, Mem->CP_dribble_dash_pow, Mem->CP_at_point_buffer); }

DribbleRes SimpleDribbleTo(Vector vEndPos, float max_dash_pow = 75, 
			  float buffer = 1.0);

#endif
