/*
 *Header:
 *File: typedefs.h 
 *Author: Noda Itsuki
 *Date: 1995/02/24
 *EndHeader:
 */

/*
 *Copyright:

    Copyright (C) 1996-2000 Electrotechnical Laboratory. 
    	Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
    Copyright (C) 2000 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Daniel Polani, Itsuki Noda

    This file is a part of SoccerServer.

    SoccerServer is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */
#ifndef _TYPES_H_
#define _TYPES_H_

#include "param.h"

/*
 *ModifyHistory
 *
 *EndModifyHistory
 */

typedef double Value ;
typedef int ID ;
typedef int TheNumber ;
typedef char* Name ;
typedef int Logical ;

#define NULLCHAR	'\000'

/*
We're not always picking up a later version of Linux, so we'll
just use an unsigned int
  #ifdef Linux_2_2
#define SOCKLEN_T socklen_t
#else
#define SOCKLEN_T int
#endif
*/
//#define SOCKLEN_T unsigned int
#define SOCKLEN_T size_t

/*
class Angle : public double {} ;
*/
typedef double Angle ;

#define TRUE 1 
#define FALSE 0 
#define	True 1
#define False 0

#define	DISABLE			0x0000
#define	STAND			0x0001
#define	KICK			0x0002
#define KICK_FAULT		0x0004
#define	GOALIE			0x0008
#define CATCH			0x0010
#define CATCH_FAULT		0x0020
#define BALL_TO_PLAYER		0x0040
#define PLAYER_TO_BALL		0x0080
#define DISCARD			0x0100
// [I.Noda:00/05/13] added for 3D viewer/commentator/small league
#define LOST			0x0200

#define BALL_COLLIDE		0x0400 // player collided with the ball
#define	PLAYER_COLLIDE		0x0800 // player collided with another player

typedef int	Side ;
#define LEFT	1
#define NEUTRAL	0
#define RIGHT 	-1

#define PLAYMODE_STRINGS {"",\
			"before_kick_off",\
			"time_over",\
			"play_on",\
			"kick_off_l",\
			"kick_off_r",\
			"kick_in_l",\
			"kick_in_r",\
			"free_kick_l",\
			"free_kick_r",\
			"corner_kick_l",\
			"corner_kick_r",\
			"goal_kick_l",\
			"goal_kick_r",\
			"goal_l",\
			"goal_r",\
			"drop_ball",\
			"offside_l",\
			"offside_r",\
			"penalty_kick_l",\
			"penalty_kick_r",\
			"first_half_over",\
			"pause",\
			"human_judge",\
			"foul_charge_l",\
			"foul_charge_r",\
			"foul_push_l",\
			"foul_push_r",\
			"foul_multiple_attack_l",\
			"foul_multiple_attack_r",\
			"foul_ballout_l",\
			"foul_ballout_r",\
		}

#define PLAYMODE_STRINGS_SHORT {"",\
			"bko",\
			"to",\
			"play",\
			"kol",\
			"kor",\
			"kil",\
			"kir",\
			"fkl",\
			"fkr",\
			"ckl",\
			"ckr",\
			"gkl",\
			"gkr",\
			"gl",\
			"gr",\
			"db",\
			"ol",\
			"or",\
			"ps",\
			"hj",\
			"flcl",\
			"flcr",\
			"flpl",\
			"flpur",\
			"flml",\
			"flmr",\
			"flbl",\
			"flbr",\
		}

typedef enum _PlayMode {
	PM_Null,
	PM_BeforeKickOff,
	PM_TimeOver,
	PM_PlayOn,
	PM_KickOff_Left,
	PM_KickOff_Right,
	PM_KickIn_Left,
	PM_KickIn_Right,
	PM_FreeKick_Left,
	PM_FreeKick_Right,
	PM_CornerKick_Left,
	PM_CornerKick_Right,
	PM_GoalKick_Left,
	PM_GoalKick_Right,
	PM_AfterGoal_Left,
	PM_AfterGoal_Right,
	PM_Drop_Ball,
	PM_OffSide_Left,
	PM_OffSide_Right,
	// [I.Noda:00/05/13] added for 3D viewer/commentator/small league
	PM_PK_Left,
	PM_PK_Right,
	PM_FirstHalfOver,
	PM_Pause,
	PM_Human,
	PM_Foul_Charge_Left,
	PM_Foul_Charge_Right,
	PM_Foul_Push_Left,
	PM_Foul_Push_Right,
	PM_Foul_MultipleAttacker_Left,
	PM_Foul_MultipleAttacker_Right,
	PM_Foul_BallOut_Left,
	PM_Foul_BallOut_Right,
	PM_MAX 
} PlayMode ;

typedef enum _PlayerTypes {
  PT_Null,
  PT_default,
  PT_Hetro_A,
  PT_Hetro_B,
  PT_Hetro_C,
  PT_Hetro_D,
  PT_Hetro_E,
  PT_Hetro_F
} PlayerTypes;

typedef enum _MPObjectType {
	MPO_Ball,
	MPO_Player,
} MPObjectType ;

#define Zero 0.0e0

#define	COLOR_NAME_MAX	64

typedef struct {
	short	enable ;
	short	side ;
	short	unum ;
	short	angle ;
	short	x ;
	short	y ;
} pos_t ;

typedef struct {
	char	name[16] ;
	short	score ;
} team_t ;

typedef struct {
	char	pmode ;
	team_t	team[2] ;
	pos_t	pos[MAX_PLAYER * 2 + 1] ;
	short	time ;
} showinfo_t ;

typedef	struct {
	short	board ;
	char	message[2048] ;
} msginfo_t ;

typedef struct {
	short	x ;
	short	y ;
	char	color[COLOR_NAME_MAX] ;
} pointinfo_t ;

typedef struct {
	short	x ;
	short	y ;
	short	r ;
	char	color[COLOR_NAME_MAX] ;
} circleinfo_t ;
	
typedef struct {
	short	x1 ;
	short	y1 ;
	short	x2 ;
	short	y2 ;
	char	color[COLOR_NAME_MAX] ;
} lineinfo_t ;

typedef struct {
	short mode ;
	union {
		pointinfo_t		pinfo ;
		circleinfo_t	cinfo ;
		lineinfo_t		linfo ;
	} object ;
} drawinfo_t ;

typedef struct {
	short	mode ;
	union {
		showinfo_t	show ;
		msginfo_t	msg ;
		drawinfo_t	draw ;
	} body ;
} dispinfo_t ;

typedef struct displist {
	unsigned long	shost ;
        int				sport ;
        int 		version ;
	struct displist *next ;
} displist_t ;

typedef struct {
	char	optname[32] ;
	void	*vptr ;
	int		vsize ;
} option_t ;

#define	V_INT		1
#define	V_DOUBLE	2
#define	V_BOOL		3
#define	V_STRING	4
#define	V_ONOFF		5
#define V_NONE		6


typedef struct {
  long x;
  long y;
  long deltax;
  long deltay;
} ball_t;

typedef struct {
  short mode;
  short type;
  long x;
  long y;
  long deltax;
  long deltay;
  long body_angle;
  long head_angle;
  long view_width;
  short view_quality;
  long stamina;
  long effort;
  long recovery;
  short kick_count;
  short dash_count;
  short turn_count;
  short say_count;
  short tneck_count;
  short catch_count;
  short move_count;
  short chg_view_count;
} player_t;

typedef struct {
  char		pmode ;
  team_t	team[2] ;
  ball_t	ball;
  player_t	pos[MAX_PLAYER * 2] ;
  short		time ;
} showinfo_t2 ;  

typedef struct {
  ball_t	ball;
  player_t	pos[MAX_PLAYER * 2] ;
  short		time ;
} short_showinfo_t2 ;  

typedef struct {
    short id;
    long player_speed_max;
    long stamina_inc_max;
    long player_decay;
    long inertia_moment;
    long dash_power_rate;
    long player_size;
    long kickable_margin;
    long kick_rand;
    long extra_stamina;
    long effort_max;
    long effort_min;

  // spare variables which are to be used for paramenter added in the future    
    long sparelong1;
    long sparelong2;
    long sparelong3;
    long sparelong4;
    long sparelong5;
    long sparelong6;
    long sparelong7;
    long sparelong8;
    long sparelong9;
    long sparelong10;
} player_type_t;

typedef struct
{
    long gwidth ;					/* goal width */
    long inertia_moment ;			/* intertia moment for turn */
    long psize ;					/* player size */
    long pdecay ;					/* player decay */
    long prand ;					/* player rand */
    long pweight ;					/* player weight */
    long pspeed_max ;				/* player speed max */
    // th 6.3.00
    long paccel_max ;				/* player acceleration max */
    //
    long stamina_max ;				/* player stamina max */
    long stamina_inc ;				/* player stamina inc */
    long recover_init ;			/* player recovery init */
    long recover_dthr ;			/* player recovery decriment threshold */
    long recover_min ;				/* player recovery min */
    long recover_dec ;				/* player recovery decriment */
    long effort_init ;				/* player dash effort init */
    long effort_dthr ;				/* player dash effort decriment threshold */
    long effort_min ;				/* player dash effrot min */
    long effort_dec ;				/* player dash effort decriment */
    long effort_ithr ;				/* player dash effort incriment threshold */
    long effort_inc ;				/* player dash effort incriment */
    // pfr 8/14/00: for RC2000 evaluation
    long kick_rand;                                /* noise added directly to kicks */
    short team_actuator_noise;                        /* flag whether to use team specific actuator noise */
    long prand_factor_l;                           /* factor to multiple prand for left team */
    long prand_factor_r;                           /* factor to multiple prand for right team */
    long kick_rand_factor_l;                       /* factor to multiple kick_rand for left team */
    long kick_rand_factor_r;                       /* factor to multiple kick_rand for right team */

    long bsize ;					/* ball size */
    long bdecay ;					/* ball decay */
    long brand ;					/* ball rand */
    long bweight ;					/* ball weight */
    long bspeed_max ;				/* ball speed max */
    // th 6.3.00
    long baccel_max;				/* ball acceleration max */
    //
    long dprate ;					/* dash power rate */
    long kprate ;					/* kick power rate */
    long kmargin ;					/* kickable margin */
    long ctlradius ;				/* control radius */
    long ctlradius_width ;			/* (control radius) - (plyaer size) */
    long maxp ;					/* max power */
    long minp ;					/* min power */
    long maxm ;					/* max moment */
    long minm ;					/* min moment */
    long maxnm ;					/* max neck moment */
    long minnm ;					/* min neck moment */
    long maxn ;					/* max neck angle */
    long minn ;					/* min neck angle */
    long visangle ;				/* visible angle */
    long visdist ;					/* visible distance */
    long windir ;					/* wind direction */
    long winforce ;				/* wind force */
    long winang ;					/* wind angle for rand */
    long winrand ;					/* wind force for force */
    long kickable_area ;			/* kickable_area */
    long catch_area_l ;			/* goalie catchable area length */
    long catch_area_w ;			/* goalie catchable area width */
    long catch_prob ;				/* goalie catchable possibility */
    short   goalie_max_moves;                 /* goalie max moves after a catch */	
    long ckmargin ;				/* corner kick margin */
    long offside_area ;			/* offside active area size */
    short win_no ;					/* wind factor is none */
    short win_random ;				/* wind factor is random */
    short say_cnt_max ;				/* max count of coach SAY */
    short SayCoachMsgSize ;				/* max length of coach SAY */
    short clang_win_size;
    short clang_define_win;
    short clang_meta_win;
    short clang_advice_win;
    short clang_info_win;
    short clang_mess_delay;
    short clang_mess_per_cycle;
    short half_time ;					/* half time */
    short sim_st ;					/* simulator step interval msec */
    short send_st ;					/* udp send step interval msec */
    short recv_st ;					/* udp recv step interval msec */
    short sb_step ;					/* sense_body interval step msec */
    short lcm_st ;		                        /* lcm of all the above steps msec */
    short SayMsgSize ;				/* string size of say message */
    short hear_max ;					/* player hear_capacity_max */
    short hear_inc ;					/* player hear_capacity_inc */
    short hear_decay ;				/* player hear_capacity_decay */
    short cban_cycle ;				/* goalie catch ban cycle */
    short slow_down_factor ;                          /* factor to slow down simulator and send intervals */
    short useoffside ;				/* flag for using off side rule */
    short kickoffoffside ;			/* flag for permit kick off offside */
    long offside_kick_margin ;		/* offside kick margin */
    long audio_dist ;				/* audio cut off distance */
    long dist_qstep ;				/* quantize step of distance */
    long land_qstep ;				/* quantize step of distance for landmark */
    long dir_qstep ;				/* quantize step of direction */
    long dist_qstep_l ;			/* team right quantize step of distance */
    long dist_qstep_r ;			/* team left quantize step of distance */
    long land_qstep_l ;			/* team right quantize step of distance for landmark */
    long land_qstep_r ;			/* team left quantize step of distance for landmark */
    long dir_qstep_l ;				/* team left quantize step of direction */
    long dir_qstep_r ;				/* team right quantize step of direction */
    short CoachMode ;				/* coach mode */
    short CwRMode ;					/* coach with referee mode */
    short old_hear ;					/* old format for hear command (coach) */
    short sv_st ;					/* online coach's look interval step */


  // spare variables which are to be used for paramenter added in the future
  long sparelong1;
  long sparelong2;
  long sparelong3;
  long sparelong4;
  long sparelong5;
  long sparelong6;
  long sparelong7;
  long sparelong8;
  long sparelong9;
  long sparelong10;

  short start_goal_l;
  short start_goal_r;
  short fullstate_l;
  short fullstate_r;
  //short spareshort1;
  //short spareshort2;
  //short spareshort3;
  //short spareshort4;
  short spareshort5;
  short spareshort6;
  short spareshort7;
  short spareshort8;
  short spareshort9;
  short spareshort10;
  
} server_params_t;

typedef struct {
    short player_types;
    short subs_max;
    short pt_max;
    
    long player_speed_max_delta_min;
    long player_speed_max_delta_max;
    long stamina_inc_max_delta_factor;
    
    long player_decay_delta_min;
    long player_decay_delta_max;
    long inertia_moment_delta_factor;

    long dash_power_rate_delta_min;
    long dash_power_rate_delta_max;
    long player_size_delta_factor;

    long kickable_margin_delta_min;
    long kickable_margin_delta_max;
    long kick_rand_delta_factor;

    long extra_stamina_delta_min;
    long extra_stamina_delta_max;
    long effort_max_delta_factor;
    long effort_min_delta_factor;

    long sparelong1;
    long sparelong2;
    long sparelong3;
    long sparelong4;
    long sparelong5;
    long sparelong6;
    long sparelong7;
    long sparelong8;
    long sparelong9;
    long sparelong10;

  short spareshort1;
  short spareshort2;
  short spareshort3;
  short spareshort4;
  short spareshort5;
  short spareshort6;
  short spareshort7;
  short spareshort8;
  short spareshort9;
  short spareshort10;

} player_params_t;

typedef struct {
  short mode;
  union {
      showinfo_t2 		show;
      msginfo_t 		msg;
      player_type_t		ptinfo;
      server_params_t		sparams;
      player_params_t		pparams;
  } body;
} dispinfo_t2 ;


#endif


