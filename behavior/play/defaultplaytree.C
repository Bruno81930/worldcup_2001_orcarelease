//_____________________________________________________________________________
//
//  defaultplaytree.C
//_____________________________________________________________________________

#include "defaultplaytree.h"

static int steps_since_kickoff = 0;

//_____________________________________________________________________________
//
//	Playtree
//_____________________________________________________________________________

void orca_play_tree()
{
    static bool moved = false;
    static bool kickoff_done = false;

    //	cout << Mem->CP_goalie_wait_cyc << endl;
    //cerr << Mem->OP_example_string << Mem->OP_example_int << endl;
	
    // Test fuer neue Debug Routine
    /**
     * Format:
     *
     * MAKELOG((0, // level, < means more important
     *	    1, // facility binary mask 
     *         " debug text ",
     *
     */

    // End Test
    if(Mem)
    {
	// react on newly arrived coach message

	if(Mem->currentCoachMessage)
	{
	    handleCoachMessage();
	}


	// playtree

	switch(Mem->PlayMode)
	{
	    // kickoff modes

	case PM_Before_Kick_Off:
	case PM_My_Kick_Off:
	case PM_Their_Kick_Off:

	    steps_since_kickoff = 0;

	    if(Mem->KickOffMode == KO_Mine)
	    {
		kickoff_done = false;

		if(!moved)
		{
		    Mem->setCurrentFormation("kick_off");  // "my_kick_off"

		    move(Mem->currentPosition->Home.x, Mem->currentPosition->Home.y);
		    moved = true;
		}
		else
		{
		    kick_off_tree();
		}
	    }
	    else
	    {
		kickoff_done = true;

		if(!moved)
		{
		    Mem->setCurrentFormation("start");  // "their_kick_off"

		    move(Mem->currentPosition->Home.x, Mem->currentPosition->Home.y);
		    moved = true;
		}
		else
		{
		    defensive_positioning_tree();
		}

	    }

	    break;

	    // non-kickoff modes
				
	default:
			
	    ++steps_since_kickoff;
			
	    // finish kickoff
			
	    if(!kickoff_done)
	    {
		if(Mem->BallAbsolutePosition().dist(Vector(0, 0)) > 3)
		{
		    kickoff_done = true;
		}
		else
		{
		    kick_off_tree();
		    return;
		}
	    }
			
			
	    // change formation after kickoff
			
	    if(moved)
	    {
		moved = false;
		Mem->setCurrentFormation("4-4-2");
	    }
			
			
	    /**agn
	     *    if player is offside do something about it!
	     *    try to find new position that is 'onside' and go there!
	     */
	    Mem->update_offside_lines();
	    /*			
			if((Mem->InOffsidePosition()) && (Mem->BallPossessor() != Mem->MyNumber) && 
			(Mem->FastestTeammateToBall() != Mem->MyNumber))
			{
			
			orca_go_to_point(Mem->PositionToOnsidePosition(Mem->MyPos()) , 1.0, 100);
			return;
			}
	    */
			
	    // adjust position
			
	    if(Mem->BallPositionValid())
	    {
		Mem->adjustCurrentPosition();
		////orca_go_to_point(Mem->currentAdjustedPosition->Home, 1.0);
		////return;
	    }
	    else
	    {
		test_face_ball();
		return;
				
	    }
				
	    // play_on mode

	    switch(Mem->PlayMode)
	    {

	    case PM_Play_On:

		if(!Mem->isCurrentFormation("4-4-2")) Mem->setCurrentFormation("4-4-2");
				
		if(! Mem->amInMyMaxRange()
		   && !(Mem->BallPositionValid()
			&& (Mem->BallKickable()
			    || Mem->ClosestTeammateToBall() == Mem->MyNumber
			    || Mem->FastestTeammateToBall() == Mem->MyNumber) ) )
		  {
		    orca_go_to_point(Mem->currentAdjustedPosition->Home, 1.0, 90);
		    return;
		  }
		
		play_on_tree();
		break;

				// offense modes

	    case PM_My_Kick_In:
	    case PM_My_Corner_Kick:
	    case PM_My_Offside_Kick:
	    case PM_My_Free_Kick:

		my_kick_tree();
		break;

	    case PM_My_Goalie_Free_Kick:
	    case PM_My_Goal_Kick:
						
		if(!Mem->isCurrentFormation("goalie-kick-off"))
		    Mem->setCurrentFormation("goalie-kick-off");
		goal_kick_positioning_tree();
		break;

				// defense modes

	    case PM_Their_Kick_In:
	    case PM_Their_Corner_Kick:
	    case PM_Their_Goal_Kick:
	    case PM_Their_Offside_Kick:
	    case PM_Their_Free_Kick:
	    case PM_Their_Goalie_Free_Kick:

		their_kick_tree();
		break;


				// any other mode

	    default:

		break;

	    }
	}
    }
}


//_____________________________________________________________________________
//
//	GoalKickOff
//_____________________________________________________________________________


void goal_kick_positioning_tree()
{
  /////free_run(Vector(Mem->currentPosition->Home.x-15,Mem->currentPosition->Home.y), 3.0);
  free_run(Vector(Mem->currentPosition->Home.x-8.0,Mem->currentPosition->Home.y), 8.0);
}

//_____________________________________________________________________________
//
//	KickOff
//_____________________________________________________________________________


void kick_off_tree()
{
  if (Mem->ClosestTeammateToBall() == Mem->MyNumber)   {

    if (Mem->BallKickable()) {

      pass_ball(5);
    }
    else {

      orca_get_ball();
    }
  }
  else  {

    face_neck_and_body_to_ball();
  }
}



//_____________________________________________________________________________
//
//	Their Kick_In, Free_Kick, Corner_Kick, Offside_Kick
//_____________________________________________________________________________

void their_kick_tree() {

	if((Mem->currentPosition->Home.x < -24)
	   && (Mem->MyPos().x > 0))
	{
		orca_go_to_point(Vector(0, Mem->MyPos().y));
	}
	else defensive_positioning_tree();
}



//_____________________________________________________________________________
//
//	Kick_In, Free_Kick, Corner_Kick, Offside_Kick
//_____________________________________________________________________________


void my_kick_tree()
{
	 
  if (Mem->NumTeammatesCloserTo(Mem->BallAbsolutePosition()) < 1)   {         // closest player goes to ball
    
    if (!Mem->BallKickable())
    {
        orca_go_to_point(Mem->MyInterceptionPoint(),0,40);	
    }

//<SEATTLE 1>
    else if(goal_kick())
    {
    	return;
    }
//</SEATTLE 1>
    else if(passpartner())
    {
      orca_pass_ball(mates[0], pt[0]);
    }
    else if(Mem->TeammatePositionValid(Mem->ClosestTeammate()))
    {
      orca_pass_ball(Mem->ClosestTeammate(), Mem->TeammateAbsolutePosition(Mem->ClosestTeammate()));  
    }
    else 
    {
      scan_field_with_body();
    }
  }

  else if(Mem->NumTeammatesCloserTo(Mem->BallAbsolutePosition()) == 1) {   // secondclosest moves to position close to kicker
      
    if (Mem->BallY() < 0) {
      
      orca_go_to_point(Vector(Mem->BallX(),Mem->BallY()+5.0),1.0,40.0);
    }
    else {
      
      orca_go_to_point(Vector(Mem->BallX(),Mem->BallY()-5.0),1.0,40.0);
    }
  }
  else {                                             // any other player 
    if (Mem->MyNumber == 1) orca_go_to_point(FindGoaliePosition(),0.1,50.0);
    
    if (Mem->BallY() > 0) {
      
      free_run(Vector(Mem->currentAdjustedPosition->Home.x, Mem->currentAdjustedPosition->Home.y+10) ,5.0);
      }
    else {
      
      free_run(Vector(Mem->currentAdjustedPosition->Home.x, Mem->currentAdjustedPosition->Home.y-10) ,5.0);
    }
  }
}


//_____________________________________________________________________________
//
//	play_on : decide whether playing defensive or offensive
//
//  Is Mem->TeamInPossession() really a good criterion ???
//_____________________________________________________________________________


void play_on_tree()
{
  
  if (Mem->BallKickable()) {
    
    play_with_ball_tree();
  }
  
  else if (Mem->TeamInPossession() == Mem->MySide) {
    
    offensive_positioning_tree();
    
  }
  
  else defensive_positioning_tree();
}



void orca_go_to_point(Vector point, float buffer, float dash_power)
{
    if (! Mem->PlayMode == PM_Play_On) dash_power = 20;
    test_go_to_point(point, buffer, Mem->CorrectDashPowerForStamina(dash_power));
}



/*****************************************************************************************/


void orca_get_ball()
{
  get_ball();
}



//_____________________________________________________________________________
//
//	handle last coach message
//_____________________________________________________________________________

void handleCoachMessage()
{

    static int last_coach_time = 0;

    // send time is saved in time_recv
    int send_time = Mem->currentCoachMessage->getTimeRecv();

    // new coach message
    if(last_coach_time != send_time)
    {
	//		cout << "COACH MESSAGE sent at: " << send_time << endl;
	//		cout << "Current Time         : " << Mem->CurrentTime.t << endl;

	last_coach_time = send_time;

	CoachMessageType type = Mem->currentCoachMessage->getType();

	//		cout << "COACH MESSAGE TYPE   : ";

	/*		switch(type)
			{
			case CMT_Meta:      cout << "META";
			break;

			case CMT_Freeform:  cout << "FREEFORM";
			break;

			case CMT_Advice:    cout << "ADVICE";
			break;

			case CMT_Info:      cout << "INFO";
			break;

			case CMT_Define:    cout << "DEFINE";
			break;

			default:            cout << "unknown";
			break;
			}

			cout << endl << endl;*/
    }
}


