#include "orcabehavior.h"
#include "communication/communicate.h"
#include "play/goalie.h"
#include "play/defaultplaytree.h"
/***********************************************************
 * This is the place where to decide wether to train a
 * little more or go right into playmode
 ***********************************************************/

void orca_behavior(void)
{
    if ((Mem->CurrentTime % (Mem->OP_communicate_wait_cycles * 12) == Mem->MyNumber * Mem->OP_communicate_wait_cycles)
	|| (Mem->FastestTeammateToBall() == Mem->MyNumber)
	|| (Mem->Say.priority > 0))
    {
	communicate();
    }

    if(Mem->OP_use_sfls) {
	//cout << "SFLS-Zyklus.....START " << Mem->CurrentTime.t << endl;

	if(Mem->PlayMode==PM_My_Kick_Off)
	    Mem->setCurrentFormation("kick_off");
	if(Mem->PlayMode==PM_Their_Kick_Off)
	    Mem->setCurrentFormation("start");
        if((Mem->PlayMode==PM_My_Goalie_Free_Kick) ||(Mem->PlayMode==PM_My_Goal_Kick))
	    Mem->setCurrentFormation("goalie-kick-off");
	if(Mem->getEffectorMoved() && Mem->noMoveMode()){
	    Mem->setEffectorMoved(false);	// reset move-blocking
	    Mem->setCurrentFormation("4-4-2"); // set formation
	}
	if(Mem->BallPositionValid()){
	    //cout << "bla: BallPosition valid. adjusting current position: ";
	    Mem->adjustCurrentPosition();	// adjust home to ballposition
	    //Mem->currentAdjustedPosition->Home.Print(); cout << endl;
	}

	//Mem->evalToplevel();			// start matcher-selector-effector-phase
	//cout << "SFLS-Zyklus.....ENDE" << Mem->CurrentTime.t << "\n\n" << endl;

	Mem->sfls->one_cycle();
	trackLeastConfObjects();
    }
    else {


	if(Mem->CP_goalie == TRUE)
	{
	    goalie_play_tree();
	}
	else
	{
	    orca_play_tree();
	    trackLeastConfObjects();
	}

    }

}


void trackLeastConfObjects()
{
    static AngleDeg neck_angle = 0;
    // turn_neck_to_learn_more_about_world
    //if (Mem->CurrentTime % 3 == 0)
    {
	float leastConf = 1.0;
	float leastConfDist = 10000.0;
	float tempConf;
	float angle = 0.0;
	AngleDeg turn_angle;
	char side;


	if(Mem->BallPositionValid())
	{
	    leastConf = pow(Mem->BallPositionValid(), Mem->OP_ball_conf_pow);
	    angle = Mem->BallAngleFromBody();
	    leastConfDist = Mem->DistanceTo(Mem->BallAbsolutePosition());
	}

		
	for (int i=0; i<2; i++)
	{
	    if(i) { side = Mem->MySide; } else { side = Mem->TheirSide; }
	    for (int unum=1; unum<Mem->SP_team_size; unum++)
	    {
		if(unum == Mem->MyNumber && side == Mem->MySide) continue;
		if((tempConf = Mem->PlayerPositionValid(side, unum)))
		{
		    if((leastConf > tempConf)
		       || ((leastConf == tempConf) 
			   && (Mem->DistanceTo(Mem->PlayerAbsolutePosition(side,unum)) < leastConfDist)))
		    {
			leastConf = tempConf;
			angle = Mem->PlayerAngleFromBody(side, unum);
			leastConfDist = Mem->MyPos().dist(Mem->PlayerAbsolutePosition(side, unum));
		    }
		}
	    }
	}
			

	if(leastConf == 1.0)
	{
	    neck_angle *= -1.0;
	    if (Mem->CurrentTime % 2 == 0)
	    {
		neck_angle += Mem->SP_max_neck_angle / 2.0;
		if (neck_angle > Mem->SP_max_neck_angle)
		{
		    neck_angle = 0.0;
		}
	    }
	    turn_angle = neck_angle - Mem->MyNeckRelAng();
	}
	else
	{
	    if (fabs(angle) >= Mem->SP_max_neck_angle)
	    {
		angle = angle * Mem->SP_max_neck_angle / fabs(angle);
	    }

	    turn_angle = angle - Mem->MyNeckRelAng();
	}
		
	turn_neck(turn_angle);
    }
}
