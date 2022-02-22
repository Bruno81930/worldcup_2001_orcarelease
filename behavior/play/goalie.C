 //_____________________________________________________________________________
 //
 //  goalie.C
 //_____________________________________________________________________________

 #include "goalie.h"


//<PHILIPP>

 //_____________________________________________________________________________
 //
 //	Goalie
 //_____________________________________________________________________________


void goalie_play_tree()
{
  static bool first_call = true;
  static int last_coach_time = 0;
  static bool moved = false;


  if (first_call && Mem->Initialized)
    {
      move(Mem->currentPosition->Home.x, Mem->currentPosition->Home.y);

      first_call = false;

      Mem->setCurrentFormation("4-4-2");
    }

  else if (Mem)
    {
      if(Mem->currentCoachMessage)
	{
	  // send time is saved in time_recv !
	  int send_time = Mem->currentCoachMessage->getTimeRecv();

	  // new coach message
	  if(last_coach_time != send_time)
	    {
	      cout << "COACH MESSAGE sent at :" << send_time << endl;
	      cout << "Current Time:          " << Mem->CurrentTime.t << endl;

	      last_coach_time = send_time;

	      CoachMessageType type = Mem->currentCoachMessage->getType();

	      cout << "COACH MESSAGE TYPE: ";

	      switch(type)
		{
		case CMT_Meta:      cout << "META" << endl;;
		  break;

		case CMT_Freeform:  cout << "FREEFORM" << endl;
		  break;

		case CMT_Advice:    cout << "ADVICE" << endl;
		  break;

		case CMT_Info:      cout << "INFO" << endl;
		  break;

		case CMT_Define:    cout << "DEFINE" << endl;
		  break;

		default:            cout << "unknown" << endl;
		  break;
		}
	    }
	}

      switch(Mem->PlayMode)
	{
	  // play_on mode

	case PM_Before_Kick_Off:
	case PM_My_Kick_Off:
	case PM_Their_Kick_Off:
	  if(!moved) {
	    move(Mem->currentPosition->Home);
	    moved=true;
	  }
	  else if (Mem->BallPositionValid()) {
	    face_neck_to_ball();
	  }
	  else scan_field_with_body();
	  break;




	  // offense modes

       
	
	case PM_My_Free_Kick:
	
	  if(Mem->OwnPenaltyArea.IsWithin(Mem->BallAbsolutePosition())) 
	    {
	      my_kick_tree();
	      //cerr << "kicken" << endl;
	    }
	  else
	    { 
	      orca_go_to_point(FindGoaliePosition(),0.1,100.0);
	      //cerr << "findgoalie"  << endl;
	    }
	  break;

	case PM_My_Goalie_Free_Kick:


	  // after goalie caught the ball

	  my_goalie_free_kick();
	  break;

	case PM_My_Goal_Kick:

	  // after ball out of bounds own baseline
	  my_goal_kick();
	  break;
	  // defense modes

	case PM_Their_Kick_In:
	case PM_Their_Corner_Kick:
	case PM_Their_Free_Kick:
	case PM_My_Kick_In:
        case PM_My_Corner_Kick:
	case PM_My_Offside_Kick:
	  orca_go_to_point(FindGoaliePosition(),0.1,100.0);
	  break;

	case PM_Their_Goal_Kick:
	case PM_Their_Offside_Kick:
	case PM_Their_Goalie_Free_Kick:

	  //agn cout << "DEFENSE\n";
	  goalie_play_on_tree();
	  break;


	  // any other mode
	case PM_Play_On:

	  //agn cout << "PLAY_ON\n";
	  moved=false;
	  goalie_play_on_tree();
	  break;



	default:

	  break;
	}
    }
}



// invoked during play_on
// decides about: catch ball
//                kick ball
//                go out to ball if I am fastest to ball
//                reason about shot
//                find a good position
//                or scan field if w/o information


void goalie_play_on_tree()
{  
   
  if (Mem->MyConf() && Mem->BallPositionValid()) 
    {
      if(Mem->PlayerDistanceToBall(Mem->MySide,Mem->MyNumber) < 5)
	{
	  if(Mem->ViewWidth !=  VW_Narrow)  
	    {
	      change_view(VW_Narrow);
	    }
	}
      else if(Mem->ViewWidth != VW_Normal)
	{
	  change_view(VW_Normal);
	}



    if (Mem->BallCatchable())  
      {     
      // if ball is catchable, we try to catch it if it is in our penalty area
      //if(WaitToCatch()){
      //	 adjust_neck();		
      //} 
      // else we either pass or kick it away
      if (!Mem->PlayedPass() && (Mem->OwnPenaltyArea.IsWithin(Mem->BallAbsolutePosition()))) {
	goalie_catch(Mem->BallAngleFromBody());    
      }

      else if (Mem->BallKickable() && passpartner()) {
	orca_pass_ball(mates[0], pt[0]);
      }
      
      else if (Mem->BallKickable()) {
	kick_ball(THEIR_GOAL_VECTOR, KM_HardestKick, Mem->SP_ball_speed_max, TURN_NONE);  //kick(100, Mem->RM_Their_Goal);             
	//kick(100, Mem->RM_Their_Goal, KM_hardest_kick);             //better to sideline???
      }  
    }
    
    else if(Mem->OwnPenaltyArea.IsWithin(Mem->BallPredictedPosition(5)) && Mem->InOwnPenaltyArea() && Mem->MyNumber == Mem->FastestTeammateToBall() && Mem->FastestOpponentToBall() &&    // if goalie is fastest to ball -> run
	    Mem->TeammateInterceptionNumberCycles(Mem->MyNumber) < 
	    Mem->OpponentInterceptionNumberCycles(Mem->FastestOpponentToBall()) && Mem->PlayMode==PM_Play_On ) 
      {
	
	goalie_go_to_ball(Mem->MarkerAngleFromBody(Mem->RM_Their_Goal));
      }

    else if(MyCatchableArea() && Mem->MyInterceptionAble()&& Mem->PlayMode==PM_Play_On) {
    
      goalie_go_to_ball(Mem->MarkerAngleFromBody(Mem->RM_Their_Goal));
    }
    
    else if(Mem->BallVelocityValid() && isShot() && Mem->MyInterceptionAble()&& Mem->PlayMode==PM_Play_On)  
      {              // if it looks like a shot -> intercept
		  
	goalie_go_to_point(Mem->MyInterceptionPoint(),0.1,100.0);                 
      
      }
    
    else 
      {
      
	goalie_go_to_point(FindGoaliePosition(),0.1,100.0);                 // find a good position
	adjust_neck();
      }
    }
  else scan_field_with_body();                                           // I have no clue about my surroundings -> scan field
}





// goes to ball and turns neck in proper direction
// call goalie_go_to_ball(Angle)

void goalie_go_to_ball()
{
  if (Mem->MyConf()) 
    {
      goalie_go_to_ball(Mem->MarkerAngleFromBody(Mem->RM_Their_Goal));
      adjust_neck();
    }
  else
    {
      turn(90);
    }
}

// go to ball with an angle to kick away ball (change to test_go_to_ball to invoke)

void goalie_go_to_ball(AngleDeg kick_angle)
{
  if ( Mem->MyConf() && Mem->BallPositionValid() )
    {
      if (Mem->MyInterceptionAble())
	{
	  orca_go_to_point(Mem->MyInterceptionPoint(),0);
	  adjust_neck();
	}
      else
	test_face_ball();  // always face ball if it's too far away
    }
  else
    turn(90);          // to scan
}

void goalie_go_to_point(Vector target_pos , float buffer, float power)
{
  float target_angle  = Mem->AngleToFromBody(target_pos);
  float target_dist = Mem->DistanceTo(target_pos);

  if(Mem->BallPositionValid())
    {
      float ball_angle = Mem->AngleToFromBody(Mem->BallAbsolutePosition());
      float abs_diff_angle = 0.0;
		
      if(fabs(ball_angle - target_angle) > 120.0)
	{
	  if((abs_diff_angle = (180 - fabs(target_angle))) > Mem->CP_max_go_to_point_angle_err)
	    {
				////cerr << "GOALIE: (";
				////cerr << Mem->CurrentTime.t << ")";
				////cerr << "\ttarget = " << target_angle;
				////cerr << "\tball_angle = " << ball_angle;
				////cerr << "\tabs_diff_angle = " << abs_diff_angle;
	      turn((target_angle > 0) ? (target_angle - 180) : (target_angle + 180));
	    }
	  else if(target_dist > buffer)
	    {
	      dash(Mem->CorrectDashPowerForStamina(-30.0));
	    }
	}
      else
	{
	  orca_go_to_point(target_pos, buffer, power);
	}
    }
  else
    {
      orca_go_to_point(target_pos, buffer, power);
    }


}


/* adjusts the neck to ball*/
void adjust_neck()
{
  face_only_neck_to_ball();
}

/* Actually turns the neck*/
void Nface(AngleDeg target_ang)
{
  AngleDeg t_ang = target_ang;

  AngleDeg ang = t_ang - Mem->MyAng();
  NormalizeAngleDeg(&ang);

  if ( fabs(ang) > 1 && fabs(t_ang) <= 90 )
    turn_neck(ang);
}


/* This function returns the "best" Position between goal and Ball
   Ball and Goal. */
Vector FindGoaliePosition()
{
  float goalieMaxX = -1.0 * Mem->SP_pitch_length / 2.0;
  goalieMaxX += 5.0;

  Line BallLine;
  Line FiveMeterLine;

  Vector center = MY_GOAL_VECTOR;
  float  radius = Mem->SP_goal_width / 2.0;

  Vector goaliePosition=Mem->currentPosition->Home;

  if (Mem->BallPositionValid())
    {
      if (isShot())
	{
	  Vector point_on_goal_line = 
	    LineFromTwoPoints(Mem->MarkerPosition(Mem->RM_My_Goal_T),
			      Mem->MarkerPosition(Mem->RM_My_Goal_B)).intersection(
										   LineFromRay(Mem->BallAbsolutePosition(),
											       Mem->BallAbsoluteVelocity()));

	  BallLine.LineFromTwoPoints(Mem->BallAbsolutePosition(), point_on_goal_line);
	}
      else
	{
	  BallLine.LineFromTwoPoints(Mem->BallAbsolutePosition(), center);
	}

		
      FiveMeterLine.LineFromTwoPoints(Vector(goalieMaxX, 0), Vector(goalieMaxX, 1));
		
      if(Mem->BallX() < Mem->MarkerPosition(Mem->RM_Flag_B50).x)
	{
	  goaliePosition = center + Polar2Vector(radius, BallLine.angle()); 
	}
      else
	{
	  goaliePosition = FiveMeterLine.intersection(BallLine);
	  if (goaliePosition.dist(center) > radius)
	    {
	      goaliePosition = center + Polar2Vector(radius, BallLine.angle()); 
	    }
	} 
    }

  return (goaliePosition);
}

/* This function returns the "best" Position between goal and Ball
Ball and Goal. */
/*
  Vector FindGoaliePosition()
  {
  Vector goaliePosition =  Mem->MarkerPosition(Mem->RM_My_Goal);    
  Vector positioningCenter =  Mem->MarkerPosition(Mem->RM_My_Goal) - Vector(10.0, 0.0);
  
  if(Mem->BallPositionValid())
  {
  Vector positioningVector = Mem->BallAbsolutePosition() -
  positioningCenter ;
  
  positioningVector = Polar2Vector(15.0, positioningVector.dir() /
  2.0);
  
  goaliePosition = positioningCenter + positioningVector;
  }
  return (goaliePosition);
  }
*/

// The Function trys to find out, wether it is a clever idea to leave the goal to try to catch the ball


Bool MyCatchableArea()
{
  if(Mem->OwnPenaltyArea.IsWithin(Mem->BallAbsolutePosition()) && Mem->BallDistance() < 4.0 && Mem->MyInterceptionAble())
    return TRUE;
  return FALSE;
}


Bool WaitToCatch()
{
  if(Mem->OwnPenaltyArea.IsWithin(Mem->BallAbsolutePosition()) && Mem->BallCatchable()){
    float MyBallX = Mem->BallX() + Mem->BallAbsoluteVelocity().x;
    float MyBallY = Mem->BallY() + Mem->BallAbsoluteVelocity().y;
		
    return Mem->DistanceTo(Vector(MyBallX,MyBallY)) <= Mem->SP_catch_area_l ? TRUE : FALSE; 

  }
  return FALSE; 
}

//</PHILIPP>

//*******************************************************************************************************


//<LEO>


//should be improved as follows: 
//if we find no passpartner, we should move again to the other corner of the penaltyarea,
//instead of kicking
void my_goalie_free_kick()
{


  static float g_wait=Mem->CP_goalie_wait_cyc;    // to give teammates time to rest & reposition 
  static bool moved = false;
  static bool turned = false;


  if (Mem->BallPositionValid()) {

    if (!moved) 
      {
	move(-45,12);                  // move to a good spot
	moved=true;     
      }

    else if (!turned)
      {
	turn(-Mem->MyBodyAng());
	turned=true;
      }

    else if (g_wait > 0) {
      g_wait -= 1.0;
    }

    else if (Mem->BallKickable()) {

      if (passpartner()) {
	
	orca_pass_ball(mates[0], pt[0]);
      }
      
      else if (Mem->TeammatePositionValid(Mem->ClosestTeammate())){

	// kick didn't work at all!!!!!   (maybe with KM_Hard???)
        
	orca_pass_ball(Mem->ClosestTeammate(), Mem->TeammateAbsolutePosition(Mem->ClosestTeammate()));
	//kick_ball(THEIR_GOAL_VECTOR, KM_HardestKick, TURN_NONE);             
      }
      else {                            // moving to a new pos 
	if (Mem->MyY() == 12) {
          move(-45,-12);
	  return;             // not a NICE RETURN!!!! 
	}
	else scan_field_with_body();
      }
      moved=false;                                  // resetting values
      
      turned=false;

      g_wait=Mem->CP_goalie_wait_cyc;
    }
    else {
      get_ball();
    }    
  }
  else
    {
      scan_field_with_body();
    }
}


// if opposing team missed the goal


void my_goal_kick()
{
  static float g_wait=Mem->CP_goalie_wait_cyc;    // to give teammates time to rest & reposition 
  static bool turned = false;


  if(Mem->BallPositionValid()) 
    {    

      if(!Mem->BallKickable())
	{
	  orca_go_to_point(Mem->MyInterceptionPoint(),0,40);	
	}

      else if (!turned)
	{
	  turn(-Mem->MyBodyAng());
	  turned=true;
	}

      else if (g_wait > 0) {
	g_wait -= 1.0;
      }
      
      else 
	{
	  if (Mem->BallKickable() && passpartner()) 
	    {
	
	      orca_pass_ball(mates[0], pt[0]);
	    }
      
	  else if (Mem->BallKickable()) {
	    orca_pass_ball(Mem->ClosestTeammate(), Mem->TeammateAbsolutePosition(Mem->ClosestTeammate()));
	    //kick_ball(THEIR_GOAL_VECTOR, KM_HardestKick, Mem->SP_ball_speed_max, TURN_NONE);  
	  }
	  else 
	    {
	      get_ball();      
	    }

	  turned=false;
      
	  g_wait=Mem->CP_goalie_wait_cyc;
	}    
    }
  else
    {
      scan_field_with_body();
    }
}








Bool BallHeadingTowardsGoal()
{

  Vector ball_vel = Mem->BallAbsoluteVelocity();
  Vector ball_pos = Mem->BallAbsolutePosition();
  float ball_vel_dir = ball_vel.dir();


  if(ball_vel.x >= 0) return FALSE;

  float top_angle = (Mem->MarkerPosition(Mem->RM_My_Goal_T) - ball_pos ).dir();
  float bottom_angle = (Mem->MarkerPosition(Mem->RM_My_Goal_B) - ball_pos).dir();

  top_angle =    (top_angle < 0 ? (180 + top_angle) : (top_angle) - 180);
  bottom_angle = (bottom_angle < 0 ? (180 + bottom_angle) : (bottom_angle) - 180);
  ball_vel_dir = (ball_vel_dir < 0 ? (180 + ball_vel_dir) : (ball_vel_dir) - 180);
	
  //	cerr << "GOALIE: (";
  //cerr << Mem->CurrentTime.t << ")";
  //cerr << "\ttop = " << top_angle << "\tbottom = " << bottom_angle;
  //cerr << "\tball_dir = " << ball_vel_dir;
  //cerr << "\tball_vel(" << ball_vel.x << "," << ball_vel.y << ") ";
  //cerr << endl;  

  if(ball_vel_dir > top_angle+2.0) return FALSE;
  if(ball_vel_dir < bottom_angle-2.0) return FALSE;

  //cerr << "GOALIE: (";
  //cerr << Mem->CurrentTime.t << ")";
  //cerr << "\tIS SHOT!!!";
  //cerr << endl;  
  return TRUE;

}


Bool isShot()
{
  if (!Mem->BallPositionValid()) return FALSE;
  if (!Mem->BallVelocityValid()) return FALSE;

  if( BallHeadingTowardsGoal() && Mem->BallSpeed() > 0.7 && Mem->BallX() < -25)
    {
      return TRUE;
    }

  return FALSE;

}

#define CP_goalie_cycle_int_buffer 10
#define CP_goalie_max_come_out_dist (Mem->SP_penalty_area_length + 4)
Bool ShouldIGoToBall()
{
  if (!Mem->BallPositionValid())
    my_error("ShouldIGoToBall: ball position not valid");
  if (Mem->PlayMode != PM_Play_On)
    return FALSE;
  //next two lines extedn end of cone SP_kickable_area past the ball absolute position
  Vector meToBall = Mem->BallAbsolutePosition() - Mem->MyPos();
  Vector coneEnd =  Mem->BallAbsolutePosition() +
    meToBall * (2 * Mem->SP_kickable_area / meToBall.mod());
  if (Mem->OwnPenaltyArea.IsWithin(Mem->BallAbsolutePosition()) &&
      Mem->NumPlayersInCone(Mem->CP_goalie_go_to_ball_cone_ratio, coneEnd) == 0 &&
      Mem->MyInterceptionAble() &&
      Mem->OwnPenaltyArea.IsWithin(Mem->MyInterceptionPoint()))
    /*	  (!Mem->BallVelocityValid() ||
	  (Mem->BallAbsoluteVelocity().x <= 0 &&
	  Mem->BallSpeed() >= Mem->CP_ball_moving_threshold)))  */
    return  TRUE;

  Vector desired_pos = FindGoaliePosition();
  if (Mem->MyInterceptionAble() &&
      Mem->OwnPenaltyArea.IsWithin(Mem->MyInterceptionPoint()) &&
      Mem->NumPlayersInCone(Mem->CP_goalie_go_to_ball_cone_ratio,
			    Mem->BallAbsolutePosition()) == 0 &&
      Mem->MyPos().dist(desired_pos) >
      Mem->MyPos().dist(Mem->MyInterceptionPoint()))
    return TRUE;

  if (Mem->FastestTeammateToBall() == Mem->MyNumber &&
      Mem->MyInterceptionAble() &&
      Mem->OwnPenaltyArea.IsWithin(Mem->MyInterceptionPoint()))
    return TRUE;

  Unum opp_fastest = Mem->FastestOpponentToBall();
  if (Mem->FastestTeammateToBall() == Mem->MyNumber &&
      Mem->MyInterceptionAble() &&
      Mem->MarkerPosition(Mem->RM_Their_Goal).dist(Mem->MyInterceptionPoint())
      < CP_goalie_max_come_out_dist &&
      (opp_fastest == Unum_Unknown ||
       Mem->MyInterceptionNumberCycles() -
       Mem->OpponentInterceptionNumberCycles(opp_fastest) > CP_goalie_cycle_int_buffer))
    return TRUE;

  return FALSE;
}

// see CMU 99


Bool ShouldIComeOutToOpponent()
{
  float buf = MinMax(0,
		     Mem->CP_goalie_breakaway_kickable_buffer *
		     ((Mem->BallDistance() - Mem->CP_goalie_no_buffer_dist) /
		      (Mem->CP_goalie_opponent_dist_to_block-Mem->CP_goalie_no_buffer_dist)),
		     Mem->CP_goalie_breakaway_kickable_buffer);
  Unum opp = Mem->OpponentWithBall(-buf);
  if (opp == Unum_Unknown) {
    Mem->LogAction2(140, "comeout: no opp with ball");
    return FALSE;
  }
  //if (!Mem->TheirBreakaway()) {
  //  Mem->LogAction2(140, "comeout: not a breakaway");
  //  return FALSE;
  //}
  return (Mem->MarkerPosition(Mem->RM_My_Goal).dist(Mem->OpponentAbsolutePosition(opp)) <
	  Mem->CP_goalie_opponent_dist_to_block)
    ? TRUE : FALSE;
}



//</LEO>





 





