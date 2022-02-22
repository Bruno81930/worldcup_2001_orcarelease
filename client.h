#ifndef _CLIENT_H_
#define _CLIENT_H_

/* client.h
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


#include "Memory.h"
#include <string>
#include <deque>
extern Memory *Mem;


void turn(AngleDeg ang);
void dash(float power);
void say(char* msg);
void kick(float power, AngleDeg dir);
void goalie_catch(AngleDeg dir);
void move(float x, float y);
inline void move(Vector p) { move(p.x,p.y); }
void disconnect();



void turn_neck(AngleDeg ang);
void change_view(Vqual qual, Vwidth width);
inline void change_view(Vqual qual) { change_view(qual,Mem->ViewWidth); }
inline void change_view(Vwidth width) { change_view(Mem->ViewQuality,width); }

#endif
