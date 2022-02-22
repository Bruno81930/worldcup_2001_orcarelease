#include "effector.h"
#include "../Memory.h"
#include "../clangparser/coach_lang_comp.h"
#include "../client.h"
#include "../behave.h"
#include "../kick.h"
#include "../dribble.h"
#include "../behavior/play/offenseplaytree.h"

/// the tolerated distance to a target point
#define TARGET_BUFFER 0.5
/// for teammateFasterToPos
#define BUFFER 5
/// velocity of the ball while dribbling (not tested)
#define DRIBBLE_VEL 1.2

/**********************************************************************
 * - only absolute Points supported
 *   Note: selector provides us with regions that were evaluated and contain
 *   only absolute points.
 * - MarkLineRegionPlayer should consider client's position to avoid unneccesary
 *   distances or turns
 * - the SFLS_Act... -actions are handled as CM_Act... -actions, where
 *   possible: PlayerUnums have been evaluated earlier
 *   Note: As of now, the selector provides the effector with SFLS_UnumSets that
 *   contain only one Unum anyway.
 * - ballToRegion has to be changed for better ball-handling skills
 *   -> BTO_All ??? 2do This should select score (if close to goal etc.),
 *	pass (if passpartner exists), or dribble
 *    - passing to a region was intended to mean: find a passpartner in that region!
 *	- use bestPasspartner(Region *) in offenseplaytree.Ch
 */

/**********************************************************************/
Vector getPointInConcaveQuad(RegQuad *regQuad)
{
    MAKELOG((40,DBG_OTHER,"EFFECTOR:: quad region concave"));
    Vector line1_1 = Vector(((RegPointSimple*)regQuad->getPt(1))->getX(), ((RegPointSimple*)regQuad->getPt(1))->getY()) -
	Vector(((RegPointSimple*)regQuad->getPt(0))->getX(), ((RegPointSimple*)regQuad->getPt(0))->getY());
    Vector line1_2 = Vector(((RegPointSimple*)regQuad->getPt(3))->getX(), ((RegPointSimple*)regQuad->getPt(3))->getY()) -
	Vector(((RegPointSimple*)regQuad->getPt(0))->getX(), ((RegPointSimple*)regQuad->getPt(0))->getY());
    
    Vector line2_1 = Vector(((RegPointSimple*)regQuad->getPt(1))->getX(), ((RegPointSimple*)regQuad->getPt(1))->getY()) -
	Vector(((RegPointSimple*)regQuad->getPt(2))->getX(), ((RegPointSimple*)regQuad->getPt(2))->getY());
    Vector line2_2 = Vector(((RegPointSimple*)regQuad->getPt(3))->getX(), ((RegPointSimple*)regQuad->getPt(3))->getY()) -
	Vector(((RegPointSimple*)regQuad->getPt(2))->getX(), ((RegPointSimple*)regQuad->getPt(2))->getY());
    
    float ang1 = line1_1.dir() - line1_2.dir();
    float ang2 = line2_1.dir() - line2_2.dir();
    
    NormalizeAngleDeg(&ang1);
    NormalizeAngleDeg(&ang2);
    Line half_ang1;
    half_ang1.LineFromTwoPoints(Vector(((RegPointSimple*)regQuad->getPt(0))->getX(), ((RegPointSimple*)regQuad->getPt(0))->getY()),
				Polar2Vector(line1_1.mod()+line1_2.mod(), ang1 /2.0 + line1_2.dir()));;
    Line half_ang2;
    half_ang2.LineFromTwoPoints(Vector(((RegPointSimple*)regQuad->getPt(2))->getX(), ((RegPointSimple*)regQuad->getPt(2))->getY()),
				Polar2Vector(line2_1.mod()+line2_2.mod(), ang2 /2.0 + line2_2.dir()));;
    return half_ang1.intersection(half_ang2);
}

/**********************************************************************/
Vector getPointInConvexQuad(RegQuad *regQuad)
{
    Vector center;
    Vector tmp;
    for (int i=0; i<4; i++)
    {
	tmp = Vector(((RegPointSimple*)regQuad->getPt(i))->getX(), ((RegPointSimple*)regQuad->getPt(i))->getY());
	center += tmp;
    }
    center = center / 4;
    return center;
}
/**********************************************************************/
Vector getPointInRegion(Region* region)
{
    /// returns (hopefully) the center of the region
    if(classNameStartsWith(region, "(pt")){
    	return *new Vector(((RegPointSimple *)region)->getX(), ((RegPointSimple *)region)->getY());
    }
    if(classNameStartsWith(region, "(home")){
	//cout <<" POINT IN HOME"<<endl;
    	if(Mem->amInMyHomeRange()){
		if(Mem->BallPositionValid()){
			//return 	PointInBetween(Mem->currentAdjustedPosition->Home, Mem->BallAbsolutePosition(), Mem->currentAdjustedPosition->HomeRange);
			// to be replaced by
		        return smartAdjustedHomePos();
		}else{
			return(Mem->currentPosition->Home);
		}
	}else{
		
		if(Mem->BallPositionValid()){
			//return (Mem->currentAdjustedPosition->Home);
			// to be replaced by
			return (smartAdjustedHomePos());
		}else{
			return (Mem->currentPosition->Home);
		}
	} 
    }
    if(classNameStartsWith(region,"(null"))
    {
	MAKELOG((20,DBG_OTHER,"null region used"));
	return *new Vector(0.0);
    }
    if(classNameStartsWith(region,"(reg"))	// union
    {
    	// return a point in an arbitrary region
	if(((RegUnion*)region)->getNumRegions()>0){
		return getPointInRegion(((RegUnion*)region)->getRegion(0));
	}else{
		MAKELOG((20,DBG_OTHER,"Warning, RegUnion with zero regions."));
		return *new Vector(0,0);
	}
    }    
    if(classNameStartsWith(region,"\""))
    {

	RegNamed regNamed = *((RegNamed*)region);
	char * regName = regNamed.getName();
	for (int i=0; (unsigned)i<Mem->RegionNames.size(); i++)
	{
	    //if (!RegionNames) continue;
	    if (strcmp(Mem->RegionNames[i], regName) == 0)
		return getPointInRegion(Mem->definedRegions[i]);
	}
    }
    if(classNameStartsWith(region,"(quad"))
    {
	return getPointInConvexQuad((RegQuad*)region);
    }
    if(classNameStartsWith(region,"(arc"))
    {
	MAKELOG((40,DBG_OTHER,"RegArc used"));
	RegArc *regArc = (RegArc*) region;
	return *new Vector(((RegPointSimple*)regArc->getCenter())->getX(),((RegPointSimple*)regArc->getCenter())->getY());
    }
    if(PrettyclassNameStartsWith(((RegPoint*)region),"Point(" ))
    {

	return *new Vector(((RegPointSimple*)region)->getX(), ((RegPointSimple*)region)->getY());
    }
    MAKELOG((20,DBG_OTHER,"Error, some RegionType () slipped through!"));
    assert(0);
    return Vector(0,0);
}
/**********************************************************************/

int teammateFasterToPos(Vector center)
{    
    Unum opp = Mem->ClosestOpponentTo(center), 
	mate = Mem->ClosestTeammateTo(center);
    int mate_cycles = Mem->TeammatePredictedCyclesToPoint(mate,center);
    int opp_cycles = Mem->OpponentPredictedCyclesToPoint(opp,center);
    if (mate_cycles+BUFFER < opp_cycles)
	/// Our mate is faster at center-pos
    {
	return 1;
    } else 
    {
	return 0;
    }
}
/**********************************************************************/

void ballToRegion(CM_ActBallToRegion *action)
{
    Region *targetregion=action->getRegion();
    Vector center = getPointInRegion(targetregion);
    Vector target_rel = center - Mem->MyPos();

    BallMoveTokenSet bmtSet = action->getBMTSet();
    Unum passpartner; // only used for 'p'
    
    /// marco:
    if(!Mem->BallKickable())
    {
	if ((Mem->FastestTeammateToBall() == Mem->MyNumber)
	||(Mem->ClosestTeammateToBall() == Mem->MyNumber))
	    orca_get_ball();
	else
	    scan_field_with_body();
    }
    /// :marco
    else 
    {
	if (bmtSet.isTokenMember(BTO_None))
	{
	    MAKELOG((20,DBG_OTHER,"EFFECTOR:: shouldn't get a BTO_None!"));
	} else if (bmtSet.isTokenMember(BTO_Score) && goal_kick())
	{
	    MAKELOG((50,DBG_OTHER,"bla: bto-scored"));
		// nothing to do, since goal_kick has been done in the if-condition above
	} else if (bmtSet.isTokenMember(BTO_Pass) &&
		   (passpartner= bestPasspartner(targetregion))!=Unum_Unknown)
	{
	    MAKELOG((50,DBG_OTHER,"bla: bto-passing"));
	    if(passpartner == Unum_Unknown) return;	// selector should do something (maybe we should return flag?)
	    orca_pass_ball(passpartner);
	    
	} else if (bmtSet.isTokenMember(BTO_Dribble))
	{ 
	    MAKELOG((50,DBG_OTHER,"bla: bto-dribbling"));
	    DribbleTo(center, Mem->CP_dribble_dash_pow, 1, 0, DM_Strict);
	    //MAKELOG((50,DBG_OTHER,"bla: fertig gedribbelt."));
	} else if (bmtSet.isTokenMember(BTO_Clear))
	{
	    MAKELOG((50,DBG_OTHER,"bla: bto-clearing"));
	    smart_kick_hard(target_rel.dir(), KM_HardestKick, 2*Mem->SP_ball_speed_max);
	} else if (bmtSet.isTokenMember(BTO_All))
	{
	    MAKELOG((20,DBG_OTHER,"EFFECTOR:: BTO_All used!"));
	    assert(0);
	}else{
	    MAKELOG((20,DBG_OTHER, "EFFECTOR::doing nothing??"));
	}
    }
}
/**********************************************************************/

Unum getUnumFromSet(UnumSet *unumSet)
{
    for (int i=1; i<12; i++)
    {
	if (unumSet->isNumMember(i))
	{
	    return i;
	}
    }
    MAKELOG((20,DBG_OTHER,"EFFECTOR:: no player in set!!!"));
    return -1;
}

/**********************************************************************/
SFLSEffector::SFLSEffector(){
	movedalready=false;
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActPosition *actPosition)
{
    // If we can move, we move. If we have moved, we turn to the position.
    // Otherwise we "walk" there.
    Vector targetpos=getPointInRegion(actPosition->getRegion());
    if((Mem->PlayMode==PM_Before_Kick_Off || Mem->PlayMode==PM_My_Kick_Off ||
       Mem->PlayMode==PM_Their_Kick_Off) && !movedalready){
		movedalready=true;
		move(targetpos.x, targetpos.y);
    }else{
    	if(movedalready && !Mem->BallPositionValid()){	// hack!
		face_only_body_to_point(targetpos);
	}else{
		//target.Print();
		if(Mem->DistanceTo(targetpos)<TARGET_BUFFER) {
			if(Mem->BallPositionValid()){
				face_only_body_to_point(Mem->BallAbsolutePosition()); // if you reached point, turn to ball
				return;
			}else{
				scan_field_with_body();
				return;
			}
		}
		go_to_point(targetpos,TARGET_BUFFER);
	}
    }
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActHome *action)
{
    Mem->currentPosition->SetHome(getPointInRegion(action->getRegion()));
    MAKELOG((50,DBG_OTHER,"new homepos set"));
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActBallToRegion *action)
{
    ballToRegion(action);
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActBallToPlayer *action)
{
    Unum target_player;
    if(action->getType()==SFLSA_BallToPlayer)
    {
       SFLS_UnumSet *aset=((SFLS_ActBallToPlayer*)action)->getPlayers();
       aset->Print(cout);
       target_player = getUnumFromSet(aset);
    }else
    {
       UnumSet aset=action->getPlayers();
       aset.Print(cout);
       target_player = getUnumFromSet(&aset);
    }
    if (!target_player)
    {
 	MAKELOG((20,DBG_OTHER,"EFFECTOR:: no player to pass to"));
 	assert(0);
 	return;
    }
    /// passes ball with vel-at-point=1.0
    smart_pass (target_player); 
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActMark *action)
{
    coverOpponent(getUnumFromSet(const_cast<UnumSet*>(&action->getPlayers())));
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActMarkLinePlayer *action)
{
    // 2do macht das, was es soll?
    coverOpponent(getUnumFromSet(const_cast<UnumSet*>(&action->getPlayers())));
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActMarkLineRegion *action)
{
    if(!Mem->BallPositionValid()){
    	MAKELOG((35,DBG_OTHER,"ActMarkLineRegion:: BallPosition not valid."));
	return;
    }
    Region *targetregion=action->getRegion();
    Vector center = getPointInRegion(targetregion);
    Vector target=(center-Mem->BallAbsolutePosition())/2+Mem->BallAbsolutePosition();
    if(Mem->DistanceTo(target) < TARGET_BUFFER){
	face_only_body_to_point(Mem->BallAbsolutePosition());
	return;
    }
    go_to_point(target,TARGET_BUFFER);
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActOffsidesLine *action)
{
    Mem->PositionToPullOffsidePosition(getPointInRegion(action->getRegion()));
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActHetType *action)
{
    MAKELOG((25,DBG_OTHER,"EFFECTOR:: hetero- players not supported yet!!"));
}

/**********************************************************************/
void SFLSEffector::doAction(CM_ActNamed *action)
{
    char * actionName = action->getName();
    unsigned int i;
    int actionFound = 0;
    for (i=0; i<Mem->ActionNames.size(); i++)
    {
	if (strcmp(Mem->ActionNames[i],actionName)==0)
	{
	    actionFound = 1;
	    break;
	}
    }
    if (actionFound)
	doAction(Mem->definedActions[i]);
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActPosition *action)
{
    Vector targetpos=getPointInRegion(action->getRegion());
    if(Mem->PlayMode==PM_Before_Kick_Off || Mem->PlayMode==PM_My_Kick_Off ||
       Mem->PlayMode==PM_Their_Kick_Off){
       		if(!movedalready){
			movedalready=true;
			move(targetpos.x, targetpos.y);
		}
    }else{
    	if(Mem->DistanceTo(targetpos) > TARGET_BUFFER){	// go there
    		float maxDash = action->getDashPower();
    		go_to_point(getPointInRegion(action->getRegion()), TARGET_BUFFER, maxDash);
	}else{
		if(Mem->BallPositionValid()){	// turn to ball
			face_only_body_to_point(Mem->BallAbsolutePosition());
		}else{
			scan_field_with_body();	// look for ball
		}
	}
    }
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActState *action)
{
    /// set SFLS- state
    vector<string>::iterator it;
    int count;
    for (count=0, it=Mem->SFLS_StateNames.begin(); it!=Mem->SFLS_StateNames.end(); it++, count ++)
    {
	if (*it == action->getName())
	{
	    Mem->SFLS_StateValues[count] = action->getValue();;
	    break;
	}
    }
    if (it==Mem->SFLS_StateNames.end())
    {
	Mem->SFLS_StateNames.push_back(action->getName());
	Mem->SFLS_StateValues.push_back(action->getValue());
	if (Mem->SFLS_StateNames.size() != Mem->SFLS_StateValues.size())
	{
	    MAKELOG((20,DBG_OTHER,"EFFECTOR:: SFLS_StateNames.size() != SFLS_StateValues.size()"));
	}
    }
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActInterceptBall *action)
{

	if(Mem->BallPositionValid())
		 orca_get_ball();
	else
		scan_field_with_body();

    /*
if ((Mem->BallSpeed()<0.2) || (!Mem->MyInterceptionAble()))
	go_to_point(Mem->BallAbsolutePosition());
    else
	go_to_point(Mem->MyInterceptionPoint());
*/
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActMarkLinePlayer *action)
{
    doAction((CM_ActMarkLinePlayer*)action);
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActMark *action)
{
    doAction((CM_ActMark*)action);
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActBallToPlayer *action)
{
    doAction((CM_ActBallToPlayer*)action);
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActCatchBall *action)
{
    goalie_catch(Mem->BallRelativeToBodyPosition().dir());
}

/**********************************************************************/
void SFLSEffector::doAction(SFLS_ActMarkRegionPlayer *action)
{
    // go to a point between region and the player
    // 2do this should consider where this agent is located now
    Vector centerOfRegion = getPointInRegion(action->getRegion());
    Unum opponent = getUnumFromSet(action->getPlayers());
    if(opponent==-1 || !Mem->OpponentPositionValid(opponent)){
        return;
    }
    Vector opponentPosition = Mem->OpponentAbsolutePosition(opponent);
    Vector targetPos = (centerOfRegion-opponentPosition)/2+opponentPosition;
    go_to_point(targetPos);
}

/**********************************************************************/
void SFLSEffector::doAction(CM_Action* action)
{
    CM_ActionType actionType = action->getType();
     switch(actionType)
     {
     case CMA_Position:
 	 doAction((CM_ActPosition*)action);
	 break;
     case CMA_Home:
 	/// set the new homepos
	 doAction((CM_ActHome*) action);
	 break;
     case CMA_BallToRegion: /// here we have to decide with which velocity
	 /// the ball should be kicked to region
	 doAction((CM_ActBallToRegion*)action);
	 break;
     case CMA_MarkLineRegion:
	 doAction((CM_ActMarkLineRegion*)action);
 	break;
     case CMA_OffsidesLine:
 	/// pull offside line
	 doAction((CM_ActOffsidesLine*)action);
	 break;
     case CMA_HetType:
	 /// set playertype for player
	 doAction((CM_ActHetType*)action);
	 break;
     case CMA_Named:
 	/// get the named action and do that action
 	doAction((CM_ActNamed*) action);
 	break;
     case SFLSA_Position:
 	/// move to that position
	 doAction((SFLS_ActPosition*)action);

 	break;
	/// 
     case SFLSA_MarkLineRegionPlayer:
	 /// move to a point on the line between region and player
   	 doAction((SFLS_ActMarkRegionPlayer*)action);
	 break;
     case SFLSA_State:
 	doAction((SFLS_ActState*) action);
 	break;
     case SFLSA_InterceptBall:
	 /// if ball is too slow, go to the ballposition directly, else
	 /// go to the interception point
	 doAction((SFLS_ActInterceptBall*)action);
	 break;
     case SFLSA_ActMarkLinePlayer:
     case CMA_MarkLinePlayer:
	 doAction((CM_ActMarkLinePlayer*)action);
	 break;
     case SFLSA_Mark:
     case CMA_Mark:
	 doAction((CM_ActMark*) action);
	 break;
     case CMA_BallToPlayer:
     case SFLSA_BallToPlayer:
	 /// best position and best velocity?
	 doAction((CM_ActBallToPlayer*)action);
 	break;
     case SFLSA_CatchBall:
 	/// for the goalie::: catch the ball 
	 doAction((SFLS_ActCatchBall*)action);
 	break;
     }
}
/**********************************************************************/
