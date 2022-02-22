/* -*- Mode: C++ -*- */

/* parse.C
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

#include "MemOption.h"
#include "client.h"
#include "utils.h"
#include "communication/communicate.h"
#include "communication/message.h"
#include "communication/baseConvert.h"

using namespace communication;

void Parse_Sight(Time time, char *SightInfo);
void Parse_Sense(Time time, char *SenseInfo);
void Parse_Sound(Time time, char *SoundInfo);
void Parse_Referee_Sound(char *RefereeSound);
void Parse_Trainer_Sound(char *msg);
void Parse_My_Coach_Sound(Time time, char *msg);
void Parse_Their_Coach_Sound(Time time, char *msg);
void Parse_Orca_Player_Sound(Time time, char *msg);

/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

void Parse(char *SensoryInfo)
{
  //<SEAN>
  //cout << "Parse: RECEIVED " << SensoryInfo << "\n";
  //</SEAN>

  SenseType sense_type;
  int     time;
  int playertype;
  bool no_sensory_info = false;
  // check if message is a player_type - msg:
  if(strncmp("(player_type", const_cast<char *>(SensoryInfo), 12)==0){
    playertype=(int)SensoryInfo[13]-(int)'0';
    
    Mem->HeteroPlayerTypes[playertype]=new Player_type(const_cast<char *>(SensoryInfo));
    no_sensory_info=true;
    return;
  }
  // check if message is a server_param - msg:
  if(strncmp("(server_param", const_cast<char *>(SensoryInfo), 13)==0){
    Mem->ParseServerParam(SensoryInfo);
    // cout << "parsing server_param :)\n";
    no_sensory_info=true;
    return;
  }

  // check if message is a player_param - msg:
  if(strncmp("(player_param", const_cast<char *>(SensoryInfo), 13)==0){
     Mem->ParsePlayerParam(SensoryInfo);
     // cout << "parsing player_param :)\n";
     no_sensory_info=true;
     return;
  }



  if(no_sensory_info == false){
    switch ( SensoryInfo[3] ){
    case 'e': sense_type = See_Msg;   break; /* see   */
    case 'n': sense_type = Sense_Msg; break; /* sense */
    case 'a': sense_type = Hear_Msg;  break; /* hear  */
    default:  my_error("Sent an illegal message"); cerr << SensoryInfo << endl; return;
    }

    time = get_int(&SensoryInfo);         /* %d    */

    Time tm = Mem->update_time(time);

    switch ( sense_type ){
    case See_Msg:
      if ( !Mem->LastActionOpTime ) break;   /* Don't parse until I've started counting time   */
      if ( tm == Mem->LastSightTime ) break; /* Don't parse a second sight from the same cycle */
      if ( Mem->NewSight == TRUE ){
        Mem->ClearSeenInfo();
        Mem->LogAction2(190,"Sight from last cycle lying around -- clearing it");
      }
      Parse_Sight(tm, SensoryInfo);
      Mem->LastSightInterval = tm - Mem->LastSightTime;
      Mem->LastSightTime = tm;
      Mem->NewSight = TRUE;
      break;
    case Sense_Msg:
      Parse_Sense(tm, SensoryInfo);
      Mem->LastSenseTime = tm;
      break;
    case Hear_Msg:
      if ( !Mem->LastActionOpTime ) break; /* Don't parse until I've started counting time */
      Parse_Sound(tm, SensoryInfo);
      Mem->LastSoundTime = tm;
      break;
    }

    Mem->LastSenseType = sense_type;
    }
}




/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

void Parse_Sense(Time time, char *SenseInfo)
{
  get_word(&SenseInfo);
  SenseInfo += 10;       /* "view_mode " */

  switch ( SenseInfo[0] ){
  case 'h': Mem->ViewQuality = VQ_High; break;  /* high */
  case 'l': Mem->ViewQuality = VQ_Low;  break;  /* low  */
  default:  my_error("Unknown view quality");
  }

  Mem->LastViewWidth = Mem->ViewWidth;
  Mem->ViewWidthTime = time;
  get_next_word(&SenseInfo);  
  switch ( SenseInfo[1] ){
  case 'o': Mem->ViewWidth = VW_Normal; break;  /* normal */
  case 'a': Mem->ViewWidth = VW_Narrow; break;  /* narrow */
  case 'i': Mem->ViewWidth = VW_Wide;   break;  /* wide   */
  default:  my_error("Unknown view quality");
  }
   
  float stamina = get_float(&SenseInfo);
  float effort  = get_float(&SenseInfo);
  float speed   = get_float(&SenseInfo);
  float speed_angle   = get_float(&SenseInfo);  // Server 6.x feature 
  float head_angle = get_float(&SenseInfo);
  //cout << "speed "<<speed<<" speed angle "<<speed_angle<<" head_angle "<<head_angle<<endl; //Server 6.x  

  int kicks  =   get_int(&SenseInfo);
  int dashes =   get_int(&SenseInfo);
  int turns  =   get_int(&SenseInfo);
  int says   =   get_int(&SenseInfo);
  int turn_necks   =   get_int(&SenseInfo);

  Mem->SetMySensedInfo(stamina,effort,speed,speed_angle,head_angle,kicks,dashes,turns,says,turn_necks,time);
}


/****************************************************************************************/

#define NOCHNGINFO -500
#define NOFACEINFO -500

void Parse_Sight(Time time, char *SightInfo)
{
  float dist, ang;
  float dirChng; 
  float distChng;
  ObjType object_type;  
  char player_side;
  Unum player_number;
  float facedir;
  float neckdir;
  MarkerType marker;
  SideLine  line;
  Vqual view_qual;
  MarkerType closestMarker = No_Marker;
  Bool processThisMarker;
  float closestMarkerDist;
  /* float motionInfoDist = 1000; */

  while (*SightInfo != ')'){

    dirChng = NOCHNGINFO;
    facedir = NOFACEINFO;
    neckdir = NOFACEINFO;
    player_number = player_side = 0;

    get_word(&SightInfo);            /* " ((" */

    if ( *SightInfo=='g' ){
      object_type = OBJ_Marker;
      //SightInfo+=5;          /* "goal " */
      SightInfo+=2; // 6.x         /* "goal " */
      if ( *SightInfo=='r' )         marker = Goal_R; else
      if ( *SightInfo=='l' )         marker = Goal_L; else
      my_error("goal ?");
    } else
    if ( *SightInfo=='G' ){
      object_type = OBJ_Marker_Behind;  marker = Mem->ClosestGoal();  
    } else
    if ( *SightInfo=='f' ){
      object_type = OBJ_Marker;
      //SightInfo+=5;          /* "flag " */
       SightInfo+=2; // 6.x         /* "goal " */
      if ( *SightInfo=='r' ){
	SightInfo+=2;
	if ( *SightInfo=='0' )       marker = Flag_R0;   else
	if ( *SightInfo=='b' ){
	  SightInfo+=1;
	  if ( *SightInfo==')' )     marker = Flag_RB; 
	  else{
	    SightInfo+=1;
	    if ( *SightInfo=='1' )   marker = Flag_RB10; else
	    if ( *SightInfo=='2' )   marker = Flag_RB20; else
	    if ( *SightInfo=='3' )   marker = Flag_RB30; else
	    my_error("flag r b ?");
	  }  
	} else
	if ( *SightInfo=='t' ){
	  SightInfo+=1;
	  if ( *SightInfo==')' )     marker = Flag_RT; 
	  else{
	    SightInfo+=1;
	    if ( *SightInfo=='1' )   marker = Flag_RT10; else
	    if ( *SightInfo=='2' )   marker = Flag_RT20; else
	    if ( *SightInfo=='3' )   marker = Flag_RT30; else
	    my_error("flag r t ?");
	  }  
	} else
	my_error("flag r ?");
      } else
      if ( *SightInfo=='l' ){
	SightInfo+=2;
	if ( *SightInfo=='0' )       marker = Flag_L0;   else
	if ( *SightInfo=='b' ){
	  SightInfo+=1;
	  if ( *SightInfo==')' )     marker = Flag_LB; 
	  else{
	    SightInfo+=1;
	    if ( *SightInfo=='1' )   marker = Flag_LB10; else
	    if ( *SightInfo=='2' )   marker = Flag_LB20; else
	    if ( *SightInfo=='3' )   marker = Flag_LB30; else
	    my_error("flag l b ?");
	  }  
	} else
	if ( *SightInfo=='t' ){
	  SightInfo+=1;
	  if ( *SightInfo==')' )     marker = Flag_LT; 
	  else{
	    SightInfo+=1;
	    if ( *SightInfo=='1' )   marker = Flag_LT10; else
	    if ( *SightInfo=='2' )   marker = Flag_LT20; else
	    if ( *SightInfo=='3' )   marker = Flag_LT30; else
	    my_error("flag l t ?");
	  }  
	} else
	my_error("flag l ?");
      } else
      if ( *SightInfo=='t' ){
	SightInfo+=2;
	if ( *SightInfo=='0' )       marker = Flag_T0;   else
	if ( *SightInfo=='l' ){
	  SightInfo+=2;
	  if ( *SightInfo=='1' )     marker = Flag_TL10; else
	  if ( *SightInfo=='2' )     marker = Flag_TL20; else
	  if ( *SightInfo=='3' )     marker = Flag_TL30; else
	  if ( *SightInfo=='4' )     marker = Flag_TL40; else
	  if ( *SightInfo=='5' )     marker = Flag_TL50; else
	  my_error("flag t l ?");
	} else
	if ( *SightInfo=='r' ){
	  SightInfo+=2;
	  if ( *SightInfo=='1' )     marker = Flag_TR10; else
	  if ( *SightInfo=='2' )     marker = Flag_TR20; else
	  if ( *SightInfo=='3' )     marker = Flag_TR30; else
	  if ( *SightInfo=='4' )     marker = Flag_TR40; else
	  if ( *SightInfo=='5' )     marker = Flag_TR50; else
	  my_error("flag t r ?");
	} else
	my_error("flag t ?");
      } else
      if ( *SightInfo=='b' ){
	SightInfo+=2;
	if ( *SightInfo=='0' )       marker = Flag_B0;   else
	if ( *SightInfo=='l' ){
	  SightInfo+=2;
	  if ( *SightInfo=='1' )     marker = Flag_BL10; else
	  if ( *SightInfo=='2' )     marker = Flag_BL20; else
	  if ( *SightInfo=='3' )     marker = Flag_BL30; else
	  if ( *SightInfo=='4' )     marker = Flag_BL40; else
	  if ( *SightInfo=='5' )     marker = Flag_BL50; else
	  my_error("flag b l ?");
	} else
	if ( *SightInfo=='r' ){
	  SightInfo+=2;
	  if ( *SightInfo=='1' )     marker = Flag_BR10; else
	  if ( *SightInfo=='2' )     marker = Flag_BR20; else
	  if ( *SightInfo=='3' )     marker = Flag_BR30; else
	  if ( *SightInfo=='4' )     marker = Flag_BR40; else
	  if ( *SightInfo=='5' )     marker = Flag_BR50; else
	  my_error("flag b r ?");
	} else
	my_error("flag b ?");
      } else
      if ( *SightInfo=='c' ){
	SightInfo+=1;
	if ( *SightInfo==')' )       marker = Flag_C;
	else{
	  SightInfo+=1;
	  if ( *SightInfo=='b' )     marker = Flag_CB; else
	  if ( *SightInfo=='t' )     marker = Flag_CT; else
          my_error("flag c ?");
	}
      } else
      if ( *SightInfo=='p' ){
	SightInfo+=2;
	if ( *SightInfo=='r' ){
	  SightInfo+=2;
	  if ( *SightInfo=='t')      marker = Flag_PRT; else
	  if ( *SightInfo=='c')      marker = Flag_PRC; else
	  if ( *SightInfo=='b')      marker = Flag_PRB; else	    
	  my_error("flag p r ?");
	} else
	if ( *SightInfo=='l' ){
	  SightInfo+=2;
	  if ( *SightInfo=='t')      marker = Flag_PLT; else
	  if ( *SightInfo=='c')      marker = Flag_PLC; else
	  if ( *SightInfo=='b')      marker = Flag_PLB; else	    
	  my_error("flag p l ?");
	} else
	my_error("flag p ?");
      } else
      if ( *SightInfo=='g' ){
	SightInfo+=2;
	if ( *SightInfo=='l' ){
	  SightInfo+=2;
	  if ( *SightInfo=='t' )     marker = Flag_GLT; else
	  if ( *SightInfo=='b' )     marker = Flag_GLB; else
	  my_error("flag g l ?");
	} else
	if ( *SightInfo=='r' ){
	  SightInfo+=2;
	  if ( *SightInfo=='t' )     marker = Flag_GRT; else
	  if ( *SightInfo=='b' )     marker = Flag_GRB; else
	  my_error("flag g r ?");
	} else
	my_error("flag g ?");
      } else
      my_error("flag ?");
    } else
    if ( *SightInfo=='F' ){
      object_type = OBJ_Marker_Behind;
                                     marker = Mem->ClosestFlagTo(); /* could be No_Marker */
    } else
    if ( *SightInfo=='l' ){
      object_type = OBJ_Line;
      //SightInfo+=5;          /* "line " */
      SightInfo+=2;          /* "line " */  
      if ( *SightInfo=='r' )         line   = SL_Right;  else
      if ( *SightInfo=='l' )         line   = SL_Left;   else
      if ( *SightInfo=='t' )         line   = SL_Top;    else
      if ( *SightInfo=='b' )         line   = SL_Bottom; else
      my_error("line ?");
    } else
    if ( *SightInfo=='p' || *SightInfo=='P' ){
      object_type = OBJ_Player;
      //SightInfo+=6;                          /* "player" */
      SightInfo+=1;
      if ( *SightInfo == ' ' ){              /* there's a team */ 
	SightInfo++;
       SightInfo++; // 6.x : skip "
	if ( !strncmp(SightInfo,Mem->MyTeamName,Mem->MyTeamNameLen) )
          //cout <<"I identified a team member "<<endl;
	  player_side = Mem->MySide;
	else{
	  if ( Mem->TheirTeamName[0] == '\n' ){
	    int a=0;
	    while ( isalpha(*SightInfo) ) Mem->TheirTeamName[a++]=*SightInfo++;
	  }
	  player_side = Mem->TheirSide;
	}
	while ( *SightInfo != ' ' && *SightInfo != ')' ) SightInfo++; /* advance past team name */
	if ( *SightInfo== ' ' ){             /* there's a number */
	  player_number = get_int(&SightInfo);
	}
       //  BEGIN 6.x: check if player is the goalie
        while ( *SightInfo != ' ' && *SightInfo != ')' ) SightInfo++; /* advance past player number */
        if ( *SightInfo== ' ' ){             /* this might be the goalie! */
          SightInfo++;
          if ( !strncmp(SightInfo,"goalie",6) ){
            if(player_side == Mem->MySide){
              //cout <<"I identified our goalie: player number "<< player_number<<endl;
              //Mem->OurGoalieNumber = player_number;
            }
            else{
              //cout <<"I identified opponent goalie: player number "<< player_number<<endl;
              //Mem->TheirGoalieNumber = player_number;
            }				////}
            SightInfo+=6;
          }
        }
        //  END 6.x: check if player is the goalie     
       }
    } else
    if ( *SightInfo=='b' || *SightInfo=='B' )
      object_type = OBJ_Ball;
    else
      my_error("unknown object");

    advance_to(')',&SightInfo);              /* advance to end of object */

    /************************************/

    ang = get_float(&SightInfo);

    if ( *SightInfo != ')' ) {                  /* 'high' quality     */
      view_qual = VQ_High;
      dist = ang;
      ang = get_float(&SightInfo);
    }
    else {
      printf("%s",SightInfo-30);
      view_qual = VQ_Low;
    }

    if ( view_qual != Mem->ViewQuality ) my_error("View quality %d correct?",view_qual);

    if ( *SightInfo != ')' ){
      distChng = get_float(&SightInfo);
      dirChng  = get_float(&SightInfo);
    }

    if ( *SightInfo != ')' ){
      if (object_type != OBJ_Player) my_error("Only players should have facedir");
      facedir = get_float(&SightInfo);
      neckdir = get_float(&SightInfo);
    }

    if ( *SightInfo != ')' ) my_error("Should be done with object info here");
    SightInfo++;                                /* ")" */

    /************************************/

    switch (object_type){
    case OBJ_Marker:
    case OBJ_Marker_Behind:
      /* Want to save 2 closest for triangulation  */
      /* don't want marker_behind unless necessary */

      /* If it was a Marker_Behind and we don't know which one */
      if ( marker == No_Marker ){
	if ( object_type != OBJ_Marker_Behind ) my_error("Should know the marker");
	break;
      }

      processThisMarker = FALSE;
      if ( view_qual == VQ_Low ){               /* Low quality   */
        /* DON'T BOTHER PROCESSING ANY??? I don't think it helps ... */
	/* COULD process 2---then triangulate */
	/*if ( closestMarkerDist > 0 ){ */
	/*  closestMarkerDist = 0;  */                 /* Only process 1*/
	/*  processThisMarker = TRUE; */
	/*}*/
      }
      else{                                          /* high quality  */
	if ( closestMarker == No_Marker || dist < closestMarkerDist){
	  closestMarker = marker;
	  closestMarkerDist = dist;
	  processThisMarker = TRUE;
	  Mem->ClosestMarker = marker;
	}
	/* Don't bother with marker motion info -- get it from sense_body and my angle 
	if ( dirChng != NOCHNGINFO && dist < motionInfoDist ){
	  motionInfoDist = dist;
	  processThisMarker = TRUE;
	  Mem->ClosestMotionMarker = marker;
	}
	*/
      }
      if ( processThisMarker ){
	if ( view_qual == VQ_Low )              /* low quality   */
	  Mem->SeeMarker(marker, ang, time);
	else /* if (dirChng == NOCHNGINFO) */                  /* high quality  */
	  Mem->SeeMarker(marker, dist, ang, time);   /* No motion info*/
	/* else
	  Mem->SeeMarker(marker, dist, ang, distChng, dirChng, time); */
      }
      break;
    case OBJ_Line:
      if ( *SightInfo != ')' )
	/* There's another line coming.  Assuming lines happen
	   last in the visual string and the closer line comes first */
	; 
      else if ( view_qual == VQ_Low )           /* low quality   */
	Mem->SeeLine(line, ang, time);
      else                                           /* high quality  */
	Mem->SeeLine(line, dist, ang, time);
      break;
    case OBJ_Ball:
      if ( view_qual == VQ_Low )                /* low quality   */
	Mem->SeeBall(ang, time);
      else if ( dirChng == NOCHNGINFO )                   /* high quality  */
	Mem->SeeBall(dist, ang, time);
      else                                           /* know direction*/
	Mem->SeeBall(dist, ang, distChng, dirChng, time);         
      break;
    case OBJ_Player:
      if ( !player_side ){                      /* Too far for team or num */
	if ( view_qual == VQ_Low )                /* low quality   */
	  Mem->SeePlayer(ang, time);
	else if ( dirChng == NOCHNGINFO )                   /* high quality  */
	  Mem->SeePlayer(dist, ang, time);
	else                                           /* know direction*/
	  my_error("Shouldn't know dirChng when the player's far");
      }  

      else{
	if ( !player_number ){                  /* Too far for number     */
	  if ( view_qual == VQ_Low )                /* low quality   */
	    Mem->SeePlayer(player_side, ang, time);
	  else if ( dirChng == NOCHNGINFO )                   /* high quality  */
	    Mem->SeePlayer(player_side, dist, ang, time);
	  else                                           /* know direction*/
	    my_error("Shouldn't know dirChng when the team member's far");
	}

	else{                                   /* Know side AND number   */
	  if ( view_qual == VQ_Low )                /* low quality   */
	    Mem->SeePlayer(player_side, player_number, ang, time);
	  else if ( dirChng == NOCHNGINFO ){                  /* high quality  */
	    printf("%s\n",SightInfo-30);
	    my_error("Should know dirChng when know number");
	    Mem->SeePlayer(player_side, player_number, dist, ang, time);
	  }
	  else                                           /* know direction*/
	    Mem->SeePlayer(player_side, player_number, dist, ang, distChng, dirChng, facedir, neckdir, time);
	}
      }
    }
  }
}

/****************************************************************************************/

void Parse_Sound(Time time, char *SoundInfo)
{
  //<SEAN>
  //cout << "Parse_Sound: RECEIVED " << SoundInfo << "\n";
  //</SEAN>

  if ( SoundInfo[1] == 'r' ){   /* Referee or Coach message */
    SoundInfo += 9;             /* " referee " */
    if (strncmp(SoundInfo, "training", 8) == 0)
      Parse_Trainer_Sound(SoundInfo);
    else if ( islower(SoundInfo[0]) )
      Parse_Referee_Sound(SoundInfo);  
    else
      my_error("Referee sounds should start with lower case letters!");
    return;
  }
  else if ( SoundInfo[1] == 'o' ){  /* Online coach message */
    SoundInfo += 14;                /* online_coach_ */
    if ( SoundInfo[0] == Mem->MySide ){
      advance_to(' ',&SoundInfo);   /* advance to end of side */
      SoundInfo++;
      Parse_My_Coach_Sound(time,SoundInfo);
    }
    else if ( SoundInfo[0] == Mem->TheirSide ){
      advance_to(' ',&SoundInfo);   /* advance to end of side */
      SoundInfo++;
      Parse_Their_Coach_Sound(time,SoundInfo);
    }
    else 
      my_error("online_coach_?");
    return;
  }
  
  //cerr<<"COM_HEAR_MESSAGE: "<<Mem->MyNumber<<SoundInfo<<endl;

  /* Here you could do something with the heard message */
  Parse_Orca_Player_Sound(time, SoundInfo);

  //cerr<<"COM_TEST_AFTER_ORCA_PLAYER_SOUND"<<endl;

}




/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

void Parse_Referee_Sound(char *msg)
{
  //printf("Parsing ref sound: '%s\'\n", msg);
  msg[strlen(msg) - 2] = 0; /* cut off the newline and paren */
  Mem->LogAction3(200,"Referee message: %s",msg);

  switch( msg[0] ){
  case 'p': Mem->SetPlayMode(PM_Play_On); break;           /* play_on */
  case 'k': 
    if ( msg[5] == 'i' ){                                  /* kick_in */
      if ( msg[8] == Mem->MySide )
	Mem->SetPlayMode(PM_My_Kick_In);
      else if ( msg[8] == Mem->TheirSide )
	Mem->SetPlayMode(PM_Their_Kick_In);
      else 
	my_error("kick_in_?");
    }
    else if ( msg[5] == 'o' ){                            /* kick_off */
      if ( msg[9] == Mem->MySide )
	Mem->SetPlayMode(PM_My_Kick_Off);
      else if ( msg[9] == Mem->TheirSide )
	Mem->SetPlayMode(PM_Their_Kick_Off);
      else 
	my_error("kick_off_?");
    }
    else
      my_error("referee k..?");
    break;
  case 'g': 
    if ( msg[5] == 'k' ){                                 /* goal_kick */
      if ( msg[10] == Mem->MySide )
	Mem->SetPlayMode(PM_My_Goal_Kick);
      else if ( msg[10] == Mem->TheirSide )
	Mem->SetPlayMode(PM_Their_Goal_Kick);
      else 
	my_error("goal_kick_?");
    }
    else if ( msg[5] == 'e' ){                           /* goalie_catch_ball */
      if ( msg[18] == Mem->MySide )
	Mem->SetPlayMode(PM_My_Goalie_Free_Kick);
      else if ( msg[18] == Mem->TheirSide )
	Mem->SetPlayMode(PM_Their_Goalie_Free_Kick);
      else
	my_error("goalie_catch_ball_?");
    }
    else if ( msg[5] == Mem->MySide ){                    /* goal */
      Mem->MyScore++;
      //Mem->MyScore = get_int(&msg[7]);
      Mem->KickOffMode = KO_Theirs;
      Mem->SetPlayMode(PM_Before_Kick_Off);
    }
    else if ( msg[5] == Mem->TheirSide ){
      Mem->TheirScore++;
      //Mem->TheirScore = get_int(&msg[7]);
      Mem->KickOffMode = KO_Mine;
      Mem->SetPlayMode(PM_Before_Kick_Off);
    }
    else 
      my_error("referee g..?");
    break;
  case 'c':                                               /* corner_kick */
    if ( msg[12] == Mem->MySide )
      Mem->SetPlayMode(PM_My_Corner_Kick);
    else if ( msg[12] == Mem->TheirSide )
      Mem->SetPlayMode(PM_Their_Corner_Kick);
    else 
      my_error("corner_kick_?");
    break;
  case 'd': Mem->SetPlayMode(PM_Drop_Ball); break;        /* drop_ball */    
  case 'o':                                               /* offside */    
    if ( msg[8] == Mem->MySide )
      Mem->SetPlayMode(PM_Their_Offside_Kick);
    else if ( msg[8] == Mem->TheirSide )
      Mem->SetPlayMode(PM_My_Offside_Kick);
    else 
      my_error("offside_?");
    break;
  case 'f':
    if ( msg[5] == 'k' ){                                 /* free_kick */
      if ( msg[10] == Mem->MySide )
	Mem->SetPlayMode(PM_My_Free_Kick);
      else if ( msg[10] == Mem->TheirSide )
	Mem->SetPlayMode(PM_Their_Free_Kick);
      else 
	my_error("free_kick_?");
    }
    else if ( msg[5] == Mem->MySide )                     /* foul */
      ;
    else if ( msg[5] == Mem->TheirSide )
      ;
    else 
      my_error("referee f..?");
    break;
  case 'h':                                               /* half_time */
    Mem->SetPlayMode(PM_Half_Time);  /* play_mode to before_kick_off        */
    if ( Mem->MySide == 'l' )
      Mem->KickOffMode = KO_Theirs;
    else 
      Mem->KickOffMode = KO_Mine;
    break;             
  case 'b': Mem->SetPlayMode(PM_Before_Kick_Off); break;       /* before_kick_off */
  case 't': 
    if ( msg[5] == 'u' ){                             /* time_up */
      Mem->SetPlayMode(PM_Time_Up); 
    }
    else if ( msg[5] == 'o' )                             /* time_over */
      {
	break;
      }
    else if ( msg[5] == 'e' ){                            /* time_extended */
      Mem->SetPlayMode(PM_Extended_Time);
      if ( Mem->MySide == 'l' )
	Mem->KickOffMode = KO_Mine;
      else 
	Mem->KickOffMode = KO_Theirs;
    } else 
      my_error("referee t..?");
    break;
  default: my_error("Referee msg ????");
  }
}

/****************************************************************************************/

/* the trainer send a string that is essentially command line options */
void Parse_Trainer_Sound(char *msg)
{
  msg += 9; /* 'training ' */
  msg[strlen(msg) - 2] = 0; /* cut off the newline and paren */
  Mem->GetOptions(msg);
  Mem->GetBall()->forget(); /* set 0 confidence in ball pos */
  // cout << "Incorp trainer message" << endl;
  Mem->LogAction2(175, "Incorporated trainer sound");
}

/****************************************************************************************/

void Parse_My_Coach_Sound(Time time, char *msg)
{
  /* Here you can parse the coach message */

    //<SEAN>

    int send_time;

    // sscanf(msg, "\"%d ", &send_time);  <7.05
    sscanf(msg, "%d ", &send_time);  

    // cut off everything before the actual coach message starts
    while(msg[0] != '(')
    {
        ++msg;
    }

    // cut off last 3 characters ( ")\n )
    // msg[strlen(msg) - 3] = 0; < 7.05
    msg[strlen(msg) - 2] = 0;

    // parse coach message and set as current
    Mem->setCurrentCoachMessage(msg, send_time);

    //</SEAN>
}

/****************************************************************************************/

void Parse_Their_Coach_Sound(Time time, char *msg)
{
  /* Here you can parse their coach's messages */
}


//<COMMUNICATION>
void Parse_Orca_Player_Sound(Time time, char *msg)
{
    if (!strstr(msg, "self")) // don't hear own messages
    {
		msg = strchr(msg, '"') + 1;
		if(Mem->OP_communicate)
		{
			parse_player_message(msg);
			// cerr<<"COM_TEST_AFTER_PARSE_PLAYER_SOUND"<<endl;
		}
	}
}

//</COMMUNICATION>
