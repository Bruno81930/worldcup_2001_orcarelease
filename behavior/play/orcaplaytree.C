#include "orcaplaytree.h"

/***********************************************************
 * This is our decision tree that is active while the client 
 * isrunning in play mode. 
 * Just copy this file to yournameplaytree.C and change the
 * orca_play_tree to yourname_play_tree and play around with
 * it.
 * You have to change orcabehavior.C to use your playtree.
 * Change the makefile in this directory. Just replace
 * orcaplaytree.o with yournameplaytree.o .
 * That should be it.
 * Any problems mailto support.
 ***********************************************************/

void orca_play_tree()
{
    /***********************************************************
     * First thing we do is looking at the playmodes and start
     * a subtree accordingly
     ***********************************************************/
    if (Mem)
    {
	
	switch ( Mem->PlayMode ){
	    /* play_on mode */
	case PM_Play_On: 
	    play_on_tree();
	    break;
	    
	    /* offense modes */
	case PM_My_Kick_In: 
	case PM_My_Corner_Kick:
	case PM_My_Kick_Off:
	case PM_My_Free_Kick:
	case PM_My_Goalie_Free_Kick:
	case PM_My_Offside_Kick:
	case PM_My_Goal_Kick:
	    offensive_positioning_tree();
	    break;
	    
	    /* defense modes */
	case PM_Their_Kick_In:
	case PM_Their_Corner_Kick:
	case PM_Their_Goal_Kick:
      case PM_Their_Kick_Off:
	case PM_Their_Offside_Kick:
	case PM_Their_Free_Kick: 
	case PM_Their_Goalie_Free_Kick:
	    defensive_positioning_tree();            
	    break;
	
      default: break;
      }
    }
}

/***********************************************************
 * The play_on_tree is active while the game is in
 * progress. It should distinguish wether you're
 * playing offense or defense  and wether you have
 *  the ball or not.
 ***********************************************************/

void play_on_tree()
{
    
    if(Mem->CP_goalie){
       goalie_tree();
    }
    cout << "player No. " << Mem->MyNumber << "is not a goalie "; 
    else{
        if (Mem->TeamInPossession() == Mem->MySide) 
        {    
            if (Mem->BallPossessor() == Mem->MyNumber)
            {
            play_with_ball_tree();
            }
            else 
            {
                offensive_positioning_tree();            
            }
        
        } else 
        {
            defensive_positioning_tree();        
        }
    
    }
        
}
/***********************************************************
 * Offensive positioning is supposed to be:
 * One mate has the ball and gets it to the goal, the others
 * go to the goal also and try to be valuable partners for
 * a pass.
 ***********************************************************/

void offensive_positioning_tree()
{
    Unum fastestTeammate;
    
    if (Mem->BallMoving()) {
        fastestTeammate = Mem->FastestTeammateToBall(); 
        
    } else {
        fastestTeammate = Mem->ClosestTeammateToBall();
    }
    
    if (fastestTeammate == Mem->MyNumber)
    {
        test_go_to_ball();
    }
    else
    {
        test_go_to_point(Mem->MarkerAngleFromBody(Mem->RM_Their_Goal),10,70);
    }
    
}


/***********************************************************
 * Try to get the ball back from them lousy Opponents!!!
 ***********************************************************/

void defensive_positioning_tree()
{
    test_go_to_ball();
}


/***********************************************************
 * Do something while you got the ball
 ***********************************************************/

void play_with_ball_tree()
{
    test_go_to_ball();
}

void goalie_tree()
{
    if(Mem->MyNumber == Mem->ClosestTeammateToBall()){
       test_go_to_ball();
       cout << "Goalie is Player " << Mem->MyNumber << " and goes to ball."
    }
    else
    {
        test_go_to_ball();
	    cout << "Goalie is Player " << Mem->MyNumber << " and goes to home-position."

    }
}
        
