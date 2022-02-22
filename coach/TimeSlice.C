

// 2do in passanalyzing functions tolerance threshold should depend on distance
// 2do test all new_showinfo-using functions with new logfiles! (version 3)
//		especially no-message-logfiles to test player_t.mode
// 2do in calculate_shootdist: parameters depend on heterogenous player!
//		KICKABLE_MARGIN also.


#include "TimeSlice.h"
#include "geometry.h"
#include "types.h"
#include "param.h"
#include "statsutil.h"
#include <vector.h>
#include <iostream.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <math.h>
//using namespace std;

#define DBG_COACH 32
#include "../logging.h"

#define LOG(L, M) MAKELOG((DBG_COACH, L, (M).c_str()))

#define OWNER_RADIUS 1.1
// coordinates of goal posts:
#define GOAL_POST_X 52.5
#define GOAL_POST_LY -7.02
#define GOAL_POST_RY 7.02
// max number of cycles to propagate events like pass, goalkick and loosing-ball:
#define MAX_TIME 40
// marker that feature is irrelevant or out of bounds:
#define IRRELEVANT 41
// tolerance threshold for ball meeting position
#define TOL_THRESHOLD 2.1
// KICKPOWERRATE changes from server version 6 to 7:
#define KICKPOWERRATE6 0.016
#define KICKPOWERRATE7 0.027

// values for kickhappened:
#define NOTEAM 0
#define LEFTTEAM 1
#define RIGHTTEAM 2
#define BOTHTEAMS 3

//minimum percentage for a section to be unioned into a defense- or offense-regionOfSections
#define UNION_THRESHOLD 30
// minimum percentage for a section to be the only section for defense/offense
#define ONLY_THRESHOLD 70

//size of rectangle used to determine homeposition if all sections are below UNION_THRESHOLD
#define TEST_SIZE_X 1
#define TEST_SIZE_Y 1

static global_pos leftPostr={GOAL_POST_X, GOAL_POST_LY};// positions of right-goal posts
static global_pos rightPostr={GOAL_POST_X, GOAL_POST_RY};
static global_pos leftPostl={-GOAL_POST_X, -GOAL_POST_LY};// positions of left-goal posts
static global_pos rightPostl={-GOAL_POST_X, -GOAL_POST_RY};

/*
string operator+(string left, const char* right) {
  string s(right);
  return left + s;
}
*/

string TimeSlice::itoa(int i) {
  char buffer[32];
  sprintf(buffer, "%d", i);
  string s(buffer);
  return s;
}

string TimeSlice::itoa(size_t i) {
  return itoa((int)i);
}

string TimeSlice::dtoa(double d) {
  char buffer[128];
  sprintf(buffer, "%f", d);
  string s(buffer);
  return s;
}

string TimeSlice::itoa(double d) {
  return dtoa(d);
}

short* TimeSlice::getPlayersAroundBall(double radius, int side) {
  short* playersAroundBall = (short*)malloc(11 * sizeof(short));
  int j = 0;
  int start = side == LEFT ? 0 : 11;
  int end = side == LEFT ? 10 : 21;
    
  for (int i = start; i <= end; i++) {
    if (distanceBetweenTwoPositions(playerinfo[i].pos, ballPos) <= radius) {
      playersAroundBall[j] = i;
      j++;
    }
  }
  
  if (j == 0) {
    return NULL;
  }

  while (j < 11) {
    playersAroundBall[j] = -1;
    j++;
  }

  return playersAroundBall;
}



/** calculates the PMK of each player in a team with the ball (x- or y-coord), and
  * returns the mean and deviation via reference
  */
void TimeSlice::meanAndDeviationOfPMKOfBallAndPlayers(int timeframe, short side,
			bool xcoord, double &pmkmean, double &pmkvar){
	if(side==NEUTRAL){
		LOG(20, string("Warning, using meanAndDeviationOfPMK...() with neutral side"));
		return;
	}
	
	vector<double> pmks;
	
	for(int p=1; p<12; p++){
		pmks.push_back(getPMKOfBallAndPlayer(timeframe,side,p,xcoord));
	}
	
	pmkmean=getMeanOfVector(pmks);
	pmkvar=getDeviationOfVector(pmks,pmkmean);
	return;
}


/** returns the "Produkt-Moment-Korrelation" of Ballposition and player position in
  * x- or y-coordinates 
  * unum: 1-11, side=LEFT|RIGHT
  */
double TimeSlice::getPMKOfBallAndPlayer(int timeframe, short side, int unum, bool xcoord){
	TimeSlice * recent=this;
	int cyclesback=0;
	vector<double> ballcoord;	// will contain the coordinate of the ball
	vector<double> playercoord;	// corresponding player coordinate in each cycle
	
	short playerindex=(unum - 1) + (side == LEFT ? 0 : 11);
	if(playerindex<0 || playerindex>21){
	  LOG(20, string("Warning, using getPMKOfBallAndPlayer() with wrong values."));
	  return 0;
	}
	
	while(recent!=NULL && cyclesback<timeframe){
		if(xcoord){
			ballcoord.push_back(recent->ballinfo.pos.x);
			playercoord.push_back(recent->playerinfo[playerindex].pos.x);
		}else{
			ballcoord.push_back(recent->ballinfo.pos.y);		
			playercoord.push_back(recent->playerinfo[playerindex].pos.y);
		}
		cyclesback++;
		recent=recent->predecessor;
	}
	return getPMKOfVectors(ballcoord, playercoord);
}





/** calculates the mean and deviation of a team's or player's x and y positions
  * for the team on side (both absolute and relative to ball), and returns the
  * results via reference.
  * If unum is 0 team values are returned, otherwise values for player unum (1-11).
  * So, in order to get a mean of action radius, one has to do a loop over 1-11 calling
  * this method and calculate the mean.
  * Note that the means are ALWAYS in left-hand orientation, no matter if 
  * side is LEFT or RIGHT.
  * 2do dependent on ballpossession? 
  */
void TimeSlice::getTeamPlayerMeanAndDeviationXY(int timeframe, short side, int unum,
    global_pos & mean, global_pos & deviation,global_pos & bmean, global_pos & bdeviation){
	TimeSlice * recent=this;
	int cyclesback=0;
	vector<double> xvalues;		// absolute coordinates
	vector<double> yvalues;
	vector<double> xvaluesball;	// relative to ball
	vector<double> yvaluesball;
	
	int start, end;
	
	if(side==LEFT){
		start=0;
		end=11;
	}else if(side==RIGHT){
		start=11;
		end=22;
	}else{
	  LOG(20, string("Warning, trying to calculate something for NEUTRAL team."));
	  return;
	}
	
	// fill vector of x and y coordinates of all players
	while((recent!=NULL) && (cyclesback<timeframe)){
		for(int i=start; i<end; i++){
			if((unum==0)		||	// team
			   (side==LEFT && unum==i+1) ||
			   (side==RIGHT && unum==i-10)){
			   	// absolute coordinates:
				xvalues.push_back(recent->playerinfo[i].pos.x);
				yvalues.push_back(recent->playerinfo[i].pos.y);
				// relative to ball:
				xvaluesball.push_back(recent->playerinfo[i].pos.x
					-recent->ballinfo.pos.x);
				yvaluesball.push_back(recent->playerinfo[i].pos.y
					-recent->ballinfo.pos.y);
				
			}
		}
		recent=recent->predecessor;
		cyclesback++;
	}
	
	// calculate mean:
	mean.x=getMeanOfVector(xvalues);
	mean.y=getMeanOfVector(yvalues);
	bmean.x=getMeanOfVector(xvaluesball);
	bmean.y=getMeanOfVector(yvaluesball);
	
	// calculate deviation:
	deviation.x=getDeviationOfVector(xvalues, bmean.x);
	deviation.y=getDeviationOfVector(yvalues, bmean.y);
	bdeviation.x=getDeviationOfVector(xvaluesball, bmean.x);
	bdeviation.y=getDeviationOfVector(yvaluesball, bmean.y);
	
	return;
}




/** check whether a player dribbled.
  * definition of dribbling (we look backward):
  * - A player has the ball.
  * - go back until
  *	- someone else has the ball (no dribble, abort)
  *	- he did a dribbling action within DRIBBLE_INTERVALL cycles (abort)
  *	- player is at least DRIBBLE_DIST away from his original position
  *		and still has the ball.
  *		- then find the cycle in which he kicked the ball (that is the dribble
  *			timestamp)
  *
  */
#define DRIBBLE_DIST 1.5
#define DRIBBLE_INTERVALL 18
void TimeSlice::findDribble(){
	//cout << "bla: possessingPlayer=" << possessingPlayer << endl;
	if(possessingPlayer==-1){	// abort
	  LOG(20, string("No balldrib event."));
	  return;
	}
	
	int cyclesback=0;		// we will go back MAX_TIME at most
	
	TimeSlice * recent=this;
	while(recent->predecessor!=NULL && cyclesback<MAX_TIME){
		recent=recent->predecessor;
		
		if(recent->dribblingPlayerIndex==possessingPlayer &&
			cyclesback<DRIBBLE_INTERVALL){
		  LOG(20, string("No balldrib event. Within interval."));
		  return;			// he did a dribbling within interval
		}
		if(recent->possessingPlayer!=possessingPlayer &&
		   recent->possessingPlayer!=-1){
		  LOG(20, "No balldrib event. Someone else had the ball:"
		      + itoa(recent->possessingPlayer));
		  return;			// someone else had the ball
		}
		
		//cout << "bla: Travelled so far: " << distanceBetweenTwoPositions(recent->playerinfo[recent->possessingPlayer].pos, playerinfo[possessingPlayer].pos) << endl;
					  
		if(distanceBetweenTwoPositions(recent->playerinfo[recent->possessingPlayer].pos,
				playerinfo[possessingPlayer].pos)>DRIBBLE_DIST &&
			recent->playerinfo[recent->possessingPlayer].playeraction.action==kick){
			dribblingPlayerIndex=possessingPlayer;	// he dribbled
			LOG(20, "Player Unum " + itoa(dribblingPlayerIndex+1)+" dribbled to this position.");
			return;
		}
		cyclesback++;
	}
	LOG(20, string("No balldrib event. Went too far back."));
	return;
}



// 2do this should of course return some kind of structure in the future (and not void).
/** This function tries to find regularities in the passing behavior.
  * For each pass that occurred we determine how many opponents are in each "pass-region",
  * check which players are involved (receiver and shooter), and basically do
  * monomial learning.
  */
#define PASS_REGIONS_X 20
#define PASS_REGIONS_Y 10
// ints in vector are 1-11:
typedef vector<int> threematrix[PASS_REGIONS_X][PASS_REGIONS_Y][2];
void TimeSlice::analyzePassStructure(int timeframe, short side){

	// We have a matrix that is the cross product of the players.
	// So every possible pair of players has an entry. This entry is a vector
	// of a structure that has a vector of unums of players in every region of both teams:
	// This is meant as:
	// vector<int[PASS_REGIONS_X][PASS_REGIONS_Y][2]> fieldForPass[11][11];
	vector<threematrix *> fieldForPass[11][11];

	TimeSlice * recent=this;	
	int cyclesback=0;
	
			// now check maximal timeframe TimeSlices:
	while(recent!=NULL && cyclesback<timeframe){
		//cout << "bla: timeTillPass is "<< recent->timeTillPass << " Servertime: "
		//     << recent->servertime << " cyclesback: " << cyclesback << endl;
		if(recent->timeTillPass==0 && recent->whichSidePassed==side){ // pass
			// now check where each player was:
			if(recent->passShooterUnum>11 || recent->passReceiverUnum>11 ||
			   recent->passShooterUnum<1 || recent->passReceiverUnum<1){
				LOG(20, "Warning, player unum of pass-player wrong: " +
				     itoa(recent->passShooterUnum) +  " " + itoa(recent->passReceiverUnum));
		   		cyclesback++;
				recent=recent->predecessor;
			 	continue;	// something wrong with this TimeSlice  
			}
			
			threematrix *field= (threematrix*)new threematrix;
				
			// There are more regions than players. So do a loop over all
			// players and assign them to regions:
			int x, y;	// index of the region
			for(int player=0; player<22; player++){
				x=int(recent->playerinfo[player].pos.x+PITCH_LENGTH/2)/
					PASS_REGIONS_X;
				y=int(recent->playerinfo[player].pos.y+PITCH_WIDTH/2)/
					PASS_REGIONS_Y;
				if(x<0 || x>=PASS_REGIONS_X || y<0 || y>=PASS_REGIONS_Y){
				  LOG(20, string("Warning, player involved in pass was outside of the field."));
				  continue;
				}
				if(player<11)
					(*field)[x][y][0].push_back(player+1);
				else
					(*field)[x][y][1].push_back(player+1);
			}
			fieldForPass[recent->passShooterUnum-1][recent->passReceiverUnum-1].
					push_back(field);
		}
		cyclesback++;
		recent=recent->predecessor;
	}
	
	// START TEMPORARY CODE BLOCK
	for(int shooter=0; shooter<11; shooter++){
		for(int receiver=0; receiver<11; receiver++){
			LOG(30, "Shooter " + itoa(shooter+1) + ", Receiver " + itoa(receiver+1) +
			    ": " + itoa((fieldForPass[shooter][receiver]).size()) +
			    " passes.");
			 // could even print regions....
		}
	}
	
	// END TEMPORARY CODE BLOCK
	
	// 2do return something
}

/**
  * This function goes back through the TimeSlices and creates negative and positive
  * foil examples for the specified event (dribble, pass, shoot) of a given team side.
  * It returns a vector of positive or negative examples, depending on value of positive.
  * These events are disjunct, but not complete. So, if timeTillGoalKick==0 or 
  * timeTillPass==0 or dribblingPlayerIndex!=-1 there was the respective action.
  * angle specifies the cone angle which is analyzed.
  */
vector<string> TimeSlice::foilPassDribbleShoot(int timeframe, Eventtype event, short side,
					       short mode, double angle){

	vector<string> examples = *(new vector<string>);

/*	// This should be in another function:
	FILE * foilfile;
	foilfile=fopen("tempfoil.d","w+");	// always the same file, always overwrite

  	if(foilfile==0) {
    		cerr << "ERROR: Cannot open foil-data-file" <<endl;
		return false;
  	}
	
	// create header
	// 2do this should depend on event
	// 2do we should use auxiliary predicates like "region_x_less" or so.
	// ....
	fprintf(foilfile,"#L: *0,*1,*2,*3,*4,*5,*6,*7,*8,*9,*10.\n\n");
	fprintf(foilfile,"pass( L,L,L,L, L,L,L,L, L,L,L,L, L,L,L,L, L,L,L,L, L,L,L,L,"
		"L,L,L,L, L,L,L,L, L,L,L,L, L,L,L,L, L,L,L,L)"
		" ############################################\n");
*/

	TimeSlice * recent=this;
	int cyclesback=0;

	if (mode == MODE_POSITIVE) {	// create positive examples
	  while (recent != NULL && cyclesback < timeframe) {
	    string foilString = generatePositiveFoilString(recent, event, angle, side);
	    if (foilString != "") {
	      examples.push_back(foilString);
	    }
	    recent = recent->predecessor;
	    cyclesback++;
	  }
	}
	else if (mode == MODE_NEGATIVE) {
	  while (recent != NULL && cyclesback < timeframe){
	    string foilString = generateNegativeFoilString(recent, event, angle, side);
	    if (foilString != "") {
	      examples.push_back(foilString);
	    }
	    recent=recent->predecessor;
	    cyclesback++;
	  }
	}
	else if (mode == MODE_TEST) {
	  while(recent != NULL && cyclesback < timeframe){
	    string foilString = generatePositiveFoilString(recent, event, angle, side);
	    if (foilString != "") {
	      examples.push_back(foilString + ": +");
	    }
	    foilString = generateNegativeFoilString(recent, event, angle, side);
	    if (foilString != "") {
	      examples.push_back(foilString + ": -");
	    }
	    recent = recent->predecessor;
	    cyclesback++;
	  }
	}
	else {
	  LOG(15, "foilPassDribbleShoot was called with an unknown mode: " + itoa(mode));
	}
	return examples;	
}

string TimeSlice::generatePositiveFoilString(TimeSlice* recent, Eventtype event, double angle, short side) {
  if(recent->possessingTeam==side){	// team has ball
    switch(event){
    case passevent:
      if(recent->timeTillPass == 0){
	return recent->foilString(recent, angle, side);
      }
      break;
    case dribbleevent:
      if(recent->dribblingPlayerIndex != -1){
	return recent->foilString(recent, angle, side);
      }
      break;
    case shootevent:
      if(recent->timeTillGoalkick == 0){
	return recent->foilString(recent, angle, side);
      }
      break;
    default:
      LOG(15, string("Warning, using foilPassDribbleShoot() with wrong event."));
    }
  }
  return "";
}

string TimeSlice::generateNegativeFoilString(TimeSlice* recent, Eventtype event, double angle, short side) {
  if(recent->possessingTeam==side){	// team has ball
    switch(event){
    case passevent:
      if(recent->timeTillGoalkick == 0 ||
	 recent->dribblingPlayerIndex != -1) {
	return recent->foilString(recent, angle, side);
      }
      break;
    case dribbleevent:
      if(recent->timeTillGoalkick==0 ||
	 recent->timeTillPass==0){
	return recent->foilString(recent, angle, side);
      }
      break;
    case shootevent:
      if(recent->timeTillPass==0 ||
	 recent->dribblingPlayerIndex!=-1){
	return recent->foilString(recent, angle, side);
      }
      break;
    default: LOG(15, string("Warning, using foilPassDribbleShoot() with wrong event."));
    }
  }
  return "";
}





/** returns a string representation of a TimeSlice for learning pass/dribble/shoot-
   * predicates in FOIL.
   * We assume that some player has the ball in TimeSlice recent.
   * We will return the following arguments:
   * (BallOwnerX, BallOwnerY, MatesInCone, OpponentsInCone, EndOfConeX, EndOfConeY,
      NearestMateX, NearestMateY, NearestOpponentX, NearestOpponentY)
   * The cone angle is specified by angle.
   * If the event in that TimeSlice is a pass, EndOfConeX/Y is the pass receiver.
   * If the event is dribbling, EndOfConeX/Y is a point EVENT_CONE_END m away in the
   * direction of the ballowner's body.
   * Same for shoot.
   */
#define EVENT_CONE_END 25
#define DISCRETE_STEP 2
#define ANGLE_DISCRETE_STEP 5
#define TEAM_MARKER "t"
#define KOORD_MARKER "k"
#define DISTANCE_MARKER "d"
#define ANGLE_MARKER "a"
string TimeSlice::foilString(TimeSlice * recent, double angle, short side){

	string example=*new string;
	double conelength=EVENT_CONE_END;
	int passshooterindex=0;	// only used for passes
	int passreceiverindex=0;
	
	
	if(!recent){
	  LOG(20, string("Warning, using foilString() with null-pointer."));
	  return "";
	}
	
	// calculate EndOfConeX/Y:
	double endx, endy;
	if(recent->timeTillPass==0){
		if(side==LEFT){
			passshooterindex=recent->passShooterUnum-1;
			passreceiverindex=recent->passReceiverUnum-1;
		}else{
			passshooterindex=recent->passShooterUnum-12;
			passreceiverindex=recent->passReceiverUnum-12;
		}
		if(passshooterindex!=recent->possessingPlayer){
		  LOG(20, string("Warning, inconsistency in possessingPlayer"));
		  return "";
		}
		endx=recent->playerinfo[passreceiverindex].pos.x;
		endy=recent->playerinfo[passreceiverindex].pos.y;
		conelength=distanceBetweenTwoPositions(
				recent->playerinfo[passreceiverindex].pos,
				recent->playerinfo[passshooterindex].pos);
	}else if((recent->timeTillGoalkick==0) || (recent->dribblingPlayerIndex!=-1)){
		global_pos endpos=pointOnRay(
			recent->playerinfo[recent->possessingPlayer].pos,
			recent->playerinfo[recent->possessingPlayer].bodyangle,
			EVENT_CONE_END);
		endx=endpos.x;
		endy=endpos.y;
	}else{
		LOG(20, string("Warning: trying to create foilString for eventless TimeSlice."));
		return "";
	}
	
	
	// now count players in cone:
	int mates=0;
	int opponents=0;
	double playerangle, playerdistance;
	for(int ind=0; ind<22; ind++){
		if(ind==recent->possessingPlayer) continue;
		playerangle=scale360(angleBetweenTwoPositions(
				recent->playerinfo[recent->possessingPlayer].pos,
			  	recent->playerinfo[ind].pos));
		// Now subtract bodyangle (if dribble or shot), or angle between pass mates
		if(recent->timeTillPass==0){
			if(ind==recent->passReceiverUnum-1) continue;
			playerangle=scale360(
				playerangle-angleBetweenTwoPositions(
					recent->playerinfo[passshooterindex].pos,
					recent->playerinfo[passreceiverindex].pos));
		}else{
			  playerangle=scale360(playerangle-
			  	recent->playerinfo[recent->possessingPlayer].bodyangle);
		}
		playerdistance=distanceBetweenTwoPositions(recent->playerinfo[ind].pos,
				  recent->playerinfo[recent->possessingPlayer].pos);
		if((playerdistance<=conelength) &&
		   (betweenAngles(playerangle,360-angle/2,angle/2))){
			// this player is in the cone
			if(ind<11){
				if(side==LEFT)
					mates++;
				else if(side==RIGHT)
					opponents++;	
			}else{
				if(side==RIGHT)
					mates++;
				else if(side==LEFT)
					opponents++;	
			}
		}
	
	}
	//find nearest Players:
	double* nearestMate = getNearestPlayerTo(recent->possessingPlayer, true);
	double* nearestOpponent = getNearestPlayerTo(recent->possessingPlayer, false);

	
	// now fill string:
	int tempvalue;

	tempvalue=((int)(recent->playerinfo[recent->possessingPlayer].pos.x)% DISCRETE_STEP)*DISCRETE_STEP;
	if(tempvalue>-53 && tempvalue<53)
				example+=KOORD_MARKER+itoa(tempvalue)+",";
	else
		return "";

	tempvalue=((int)(recent->playerinfo[recent->possessingPlayer].pos.y)% DISCRETE_STEP)*DISCRETE_STEP;
	if(tempvalue>-33 && tempvalue<33)
		example+= KOORD_MARKER+itoa(tempvalue)+",";
	else
		return "";

	if(mates>-1 && mates<12)
		example+=TEAM_MARKER+itoa(mates)+ ",";
	else
		return "";

	if(opponents>-1 && opponents<12)
		example+=TEAM_MARKER+itoa(opponents)+",";
	else
		return "";

	tempvalue=((int)(endx)%DISCRETE_STEP)*DISCRETE_STEP;
	if(tempvalue>-53 && tempvalue<53)
		example+=KOORD_MARKER+itoa(tempvalue)+",";
	else
		return "";

	tempvalue=((int)(endy)%DISCRETE_STEP)*DISCRETE_STEP;
	if(tempvalue>-33 && tempvalue<33)
		example+=KOORD_MARKER+itoa(tempvalue) + ",";
	else
		return "";

	int maxDistance = 20;

	tempvalue = ((int)(nearestMate[1]) % DISCRETE_STEP) * DISCRETE_STEP;
	if (tempvalue <= maxDistance)
	  example += DISTANCE_MARKER + itoa(tempvalue) + ",";
	else
	  return "";
	example += ANGLE_MARKER + itoa(((int)(nearestMate[2]) % ANGLE_DISCRETE_STEP) * ANGLE_DISCRETE_STEP)
	  + ",";

	tempvalue = ((int)(nearestOpponent[1]) % DISCRETE_STEP) * DISCRETE_STEP;
	if (tempvalue <= maxDistance)
	  example += DISTANCE_MARKER + itoa(tempvalue) + ",";
	else
	  return "";
	example += ANGLE_MARKER + itoa(((int)(nearestOpponent[2]) % ANGLE_DISCRETE_STEP) * 
				       ANGLE_DISCRETE_STEP);

	return example;
	

/* obsolete: region-based
	// There are more regions than players. So do a loop over all
	// players and assign them to regions:
	string foil="";
	int x, y;	// index of the region
	for(int player=0; player<22; player++){
		x=int(recent->playerinfo[player].pos.x+PITCH_LENGTH/2)/
			PASS_REGIONS_X;
		y=int(recent->playerinfo[player].pos.y+PITCH_WIDTH/2)/
			PASS_REGIONS_Y;
		if(x<0 || x>=PASS_REGIONS_X || y<0 || y>=PASS_REGIONS_Y){
			cerr << "Warning, player involved in pass was "
			     " outside of the field." << endl;
		}
		foil+=itoa(x)+","+itoa(y);
		if(player!=21) foil+=",";
	}
	fprintf(foilfile,"%s\n",foil.c_str());
	return;
*/
}

/**
 * This function needs a player index and bool indicating if the next opponent (false) or next
 * teammate is needed (true). It returns a double* which has allocated memory for three doubles.
 * The returned vector contains the index of the nearest player, the distance to that player and
 * the angle to that player (in this order).
 */
double* TimeSlice::getNearestPlayerTo(short player, bool teammate) {
  double distance = 0.0;
  double angle = 0.0;
  double nearestPlayer;

  for (short i = 0; i < 21; i++) {
    if (i != player &&
	((playerinfo[i].side == playerinfo[player].side &&
	  teammate) ||
	 (playerinfo[i].side != playerinfo[player].side &&
	  !teammate))) {
      double pd = distanceBetweenTwoPositions(playerinfo[player].pos, playerinfo[i].pos);
      if (pd > distance) {
	distance = pd;
	angle = angleBetweenTwoPositions(playerinfo[player].pos, playerinfo[i].pos);
	nearestPlayer = i;
      }
    }
  }

  double* results = (double*)malloc(3 * sizeof(double));
  results[0] = nearestPlayer;
  results[1] = distance;
  results[2] = scale360(angle);

  return results;
}


// 2do toFoil() needs an update so that it uses useful attributes like regions etc.: ####
// ######################################################################################
// returns a representation of this TimeSlice which is usable by Foil 6.4
// NOT USED ANYMORE
string TimeSlice::toFoil(void) {
	
  string foil = 
    dtoa(ballPos.x) + ", " +
    dtoa(ballPos.y) + ", " +
    itoa(possessingTeam) + ", ";
  
  for (int i = 0; i < 8; i++) {
    foil +=
      itoa(player[i].side) + ", " +
      dtoa(player[i].ballangle) + ", " +
      dtoa(player[i].distance);
    if (i != 7) {
      foil += ", ";
    }
  }

  return foil;
}

// checks whether the goalie on side "side" has the ball:
bool TimeSlice::goalieHasBall(short side){
	if(side==LEFT){
		if(possessingPlayer>10) return false;
		return isGoalie(possessingPlayer, side);
	}else{
		if(possessingPlayer<11) return false;
		return isGoalie(possessingPlayer-11, side);
	}
}


// unum: 1 to 11
global_pos TimeSlice::getPlayerPos(short side, short unum){
	short player_index = (unum - 1) + (side == LEFT ? 0 : 11);
	return playerinfo[player_index].pos;
}


// use this only if you are sure that you already know the goalie of side
// returns 1..11
short TimeSlice::getGoalie(short side){
	for(int i=0; i<11; i++)
		if(isGoalie(i,side)) return (i+1);
	return -1;
}

// index: 0..10
bool TimeSlice::isGoalie(short index, short side){
	short player_index = index  + (side == LEFT ? 0 : 11);
	if(player_index<0 || player_index>21) return false;
	return playerinfo[index].isGoalie;
}


// analyzes the positions of players in a certain mode (like WE_HAVE_BALL).
// returns a structure which contains a vector of indizes (0..10) for each section.
// The indizes specify players that have their (or one of their) homepositions in that
// section.
Teampositions* TimeSlice::getTeampositions(short side, short timeframe, short mode){
	Teampositions *teampositions;		// will contain pos. of all players
	regionOfSections *regions=NULL;		// will contain borders for one
							// player								
	teampositions=(Teampositions*)new Teampositions;
	
	for(int playerunum=1; playerunum<12; playerunum++){
		regions=getDefenseOffenseRegion(playerunum, side, timeframe, mode);
			// get homeregion of one player
		// fill the sections (which are in regions) into teampositions:
		for(int x=regions->left; x<=regions->right; x++){
			for(int y=regions->top; y<=regions->bottom; y++){
				(*teampositions)[x][y].push_back(playerunum);
			}
		}
	}
	return teampositions;
}


// creates a struct that contains information about the rectangular region that the
// given player is in in the given mode
// unum: 1-11
regionOfSections* TimeSlice::getDefenseOffenseRegion(short unum,short side, short timeframe,
		short mode){
	// get distribution of player positions when in given mode:
	RectGrid * grid=getLatticeForPlayer(unum, side, timeframe, mode);
	// now check whether regions must be unioned or whether there is one main region:
	vector<int> toBeUnioned; // contains zero or even number of ints
				 // if even: column and rows of sections
				 // 2do will this vector be deleted automatically??
	int percentage;
  	for(int sectionx=0; sectionx<HOR_SECTIONS; sectionx++){
		for(int sectiony=0; sectiony<VER_SECTIONS; sectiony++){
			percentage=grid->getSection(sectionx, sectiony);
			if(percentage>ONLY_THRESHOLD){
				// there is a main region. return it.
				regionOfSections* mainReg=new regionOfSections;
				mainReg->top=sectiony;
				mainReg->bottom=sectiony;
				mainReg->left=sectionx;
				mainReg->right=sectionx;
				mainReg->confidence=percentage;
				return mainReg;		// done
			}else if(percentage>UNION_THRESHOLD){
				// add section to vector:
				toBeUnioned.push_back(sectionx);
				toBeUnioned.push_back(sectiony);
			}
		}
	}
	if(toBeUnioned.size()<2*2){		
		// all sections are below UNION_THRESHOLD or only one section above
		// for example:
		// 0    0    0    0    0    0    0    12   10   0    
		// 0    0    0    0    0    0    17   2    24   0    
		// 0    0    0    0    12   20   3    0    0    0    
		// 0    0    0    0    0    0    0    0    0    0    
		// 0    0    0    0    0    0    0    0    0    0 
		// 1. get convex hull of filled sections
		// 2. test all rectangles of size (TEST_SIZE_X*TEST_SIZE_Y) and
		//	save the best
		regionOfSections * convex_hull=NULL;		// will contain the coords of
							// the convex hull
		vector<int> test_rectangle;		// will contain sections of
							// the test_rectangle
		regionOfSections * best_rect=NULL;	//will contain the best rect so far
		regionOfSections * recent_rect=NULL;
		int bestconfidence=0;			// best confidence so far
		// get all sections that will determine the convex hull
		for(int sectionx=0; sectionx<HOR_SECTIONS; sectionx++){
			for(int sectiony=0; sectiony<VER_SECTIONS; sectiony++){
				if(grid->getSection(sectionx, sectiony)>0){
					toBeUnioned.push_back(sectionx);
					toBeUnioned.push_back(sectiony);
				}
			}
		}
		// create convex_hull
		convex_hull=unionSections(toBeUnioned, grid);
		// now test all rectangles within convex_hull:
		for(int testsizex=0; testsizex<=TEST_SIZE_X; testsizex++){
		for(int testsizey=0; testsizey<=TEST_SIZE_Y; testsizey++){
		for(int startx=convex_hull->left; startx+testsizex<=convex_hull->right;
									startx++){
			for(int starty=convex_hull->top; starty+testsizey<=
				convex_hull->bottom; starty++){	// all start points
				// now fill rectangle with sections
				test_rectangle.clear();
				for(int x=startx; x<=startx+testsizex && x<HOR_SECTIONS;
										 x++){
					for(int y=starty; y<=starty+testsizey &&
							y < VER_SECTIONS; y++){
						test_rectangle.push_back(x);
						test_rectangle.push_back(y);
					}
				}
				// union testrectangle to compute confidence
				recent_rect=unionSections(test_rectangle, grid);
				if(recent_rect->confidence>=bestconfidence){
					best_rect=recent_rect;
					bestconfidence=recent_rect->confidence;
				}
			}
		}
		}
		}
		if(!best_rect){	// Error. Now create an inconsistent regionOfSections:
			best_rect=new regionOfSections;
			best_rect->top=VER_SECTIONS;
			best_rect->bottom=0;
			best_rect->left=HOR_SECTIONS;
			best_rect->right=0;
		}
		return best_rect;	// return best		
	}
	// now union regions and return them
	return unionSections(toBeUnioned, grid);
}


// creates a RectGrid and accumulates positions of the last timeframe cycles for
// player unum (1-11) of side side
// depending on mode only cycles will be counted in which side has the ball (1
// WE_HAVE_BALL), has not the ball (2 OPP_HAS_BALL), ballpossession does not matter
// (0 ALL) , or player has ball (3 PLAYER_HAS_BALL)
RectGrid * TimeSlice::getLatticeForPlayer(short unum, short side, short timeframe,
			short mode){
  // init necessary variables:
  int i = 0;
  TimeSlice* slice = this;
  global_pos his_pos;			// the player position in the recent slice
  RectGrid* grid = new RectGrid();
  int sumOfCycles=0;			// how many cycles were counted? != timeframe...
  					//  ...if mode!=0
  double lengthofsection=PITCH_LENGTH/HOR_SECTIONS;
  double heigthofsection=PITCH_WIDTH/VER_SECTIONS;
  short player_index = (unum - 1) + (side == LEFT ? 0 : 11);
  
  // now go back and accumulate positions
  while((i<timeframe) && (slice!= NULL)){
  	// skip particular playmodes:
  	if((slice->playmode==PM_AfterGoal_Left) || (slice->playmode==PM_AfterGoal_Right)){
	  LOG(35, string("Skipping PM_AfterGoal_"));
	  slice=slice->predecessor;
	  continue;	
	}
  	his_pos.x=slice->playerinfo[player_index].pos.x;
	his_pos.y=slice->playerinfo[player_index].pos.y;
  	// is player inside field?
    	if (his_pos.x > (PITCH_LENGTH / 2)  ||    
	    his_pos.x < -(PITCH_LENGTH / 2) ||
	    his_pos.y > (PITCH_WIDTH / 2)  ||
	    his_pos.y < -(PITCH_WIDTH / 2)) {
	    		slice=slice->predecessor;
      			continue;
    	}

  	// now calculate appropriate section:
  	for(int sectionx=0; sectionx<HOR_SECTIONS; sectionx++){
		for(int sectiony=0; sectiony<VER_SECTIONS; sectiony++){
			if((his_pos.x>-(PITCH_LENGTH/2)+sectionx*lengthofsection) &&
			   (his_pos.x<=-(PITCH_LENGTH/2)+(sectionx+1)*lengthofsection) &&
			   (his_pos.y>-(PITCH_WIDTH/2)+sectiony*heigthofsection) &&
			   (his_pos.y<=-(PITCH_WIDTH/2)+(sectiony+1)*heigthofsection)){
			   	// increment section
				switch(mode){
					case ALL:	// all cycles
						grid->incrementSection(sectionx, sectiony);
						sumOfCycles++;
						break;
					case WE_HAVE_BALL:  // team has ball
						if(slice->possessingTeam==side){
						grid->incrementSection(sectionx, sectiony);
						sumOfCycles++;
						}
						break;
					case OPP_HAS_BALL: // opp has ball
						if((slice->possessingTeam!=side) &&
					   	   (slice->possessingTeam!=NEUTRAL)){
						grid->incrementSection(sectionx, sectiony);
						sumOfCycles++;
						}
						break;
					case PLAYER_HAS_BALL: // this player has ball
						if(slice->possessingPlayer==player_index){
						grid->incrementSection(sectionx,sectiony);
						sumOfCycles++;
						}
						break;
					default:
					  LOG(15, string("Using getLattice with wrong mode!"));
				}
			   }
		}
  	}
  
    	slice=slice->predecessor; // next slice
  }

  int percentage;  
  if(sumOfCycles>0){	// transform into percentage
  	for(int sectionx=0; sectionx<HOR_SECTIONS; sectionx++){
		for(int sectiony=0; sectiony<VER_SECTIONS; sectiony++){
  			percentage=(int)((grid->getSection(sectionx, sectiony)/
							(1.0*sumOfCycles))*100+0.5);
			grid->setSection(sectionx,sectiony,percentage);
  		}
	}
  }
  
  return grid;
}



// unum: 1-11
global_pos TimeSlice::calculateAveragePosition(short unum, short side, short timeframe) {
  int i = 0;
  TimeSlice* slice = this;
  global_pos sum = {0.0, 0.0};
  short player_index = (unum - 1) + (side == LEFT ? 0 : 11);
  while (i < timeframe && 
	 slice != NULL && slice->predecessor!=NULL) {
    slice = slice->getPredecessor();
    if (slice->playerinfo[player_index].pos.x > (PITCH_LENGTH / 2)  ||    
	slice->playerinfo[player_index].pos.x < -(PITCH_LENGTH / 2) ||
	slice->playerinfo[player_index].pos.y > (PITCH_WIDTH / 2)  ||
	slice->playerinfo[player_index].pos.y < -(PITCH_WIDTH / 2)) { //sanity check
      continue;
    }
    sum.x += slice->playerinfo[player_index].pos.x;
    sum.y += slice->playerinfo[player_index].pos.y;
    i++;
  }
  //printf("dividing %f by %d\n", sum.y, i);
  if(i!=0){
  	sum.x /= i;
 	sum.y /= i;
  }
  return sum;
}

string TimeSlice::classToString(short classification) {
  switch (classification) {
  case DEFENSE_CLASS: return "defender";
  case OFFENSE_CLASS: return "attacker";
  case MIDDLEFIELD_CLASS: return "middlefield";
  case GOALIE_CLASS: return "goalie";
  default: return "unknown";
  }
}

short TimeSlice::classifyPlayer(global_pos averagePosition, short unum, short side) {
  //short player_index = (unum - 1) * (side == LEFT ? 1 : 2);  
  //short player_index = (unum - 1) + (side == LEFT ? 0 : 11);  
  
  short left_third = -(short)((PITCH_LENGTH / 3) / 2);
  short right_third = (short)((PITCH_LENGTH / 3) / 2);

  if (averagePosition.x < left_third) {
    if (side == LEFT) {
      return DEFENSE_CLASS;
    }
    else {
      return OFFENSE_CLASS;
    }
  }
  if (averagePosition.x > right_third) {
    if (side == RIGHT) {
      return DEFENSE_CLASS;
    }
    else {
      return OFFENSE_CLASS;
    }
  }
  return MIDDLEFIELD_CLASS;
}



/** this method classifies players relative to each other.
 *  so a team that plays very defensively and is in its own half most of the time
 *  still has offenders (the frontmost players that are isolated a bit from the next
 *  class of midfielders).
 */
short* TimeSlice::classifyPlayersRelative(global_pos averagePositions[11]){
// UNDER CONSTRUCTION
  double biggest = 0;
  double second_biggest = 0;
  short biggest_index = -1;
  short second_biggest_index = -1;
  short* results = (short*)malloc(11 * sizeof(short));

  global_pos positions[11];
  short indizes[11];
  for (int i = 0; i < 11; i++) {
    positions[i] = averagePositions[i];
  }
  qsortX(positions, 0, 10);
  for (int i = 0; i < 11; i++) {
    for (int i2 = 0; i2 < 11; i2++) {
      if (positions[i].x == averagePositions[i2].x &&
	  positions[i].y == averagePositions[i2].y) {
	indizes[i] = i2;
	break;
      }
    }
  }

  for (int i = 1; i < 10; i++) {
    double delta = distanceBetweenTwoPositions(positions[i], positions[i + 1]);
    if (delta > biggest) {
      biggest = delta;
      biggest_index = i;
    }
    else if (delta > second_biggest) {
      second_biggest = delta;
      second_biggest_index = i;
    }
  }
  
  if (biggest_index > second_biggest_index) {
    short tmp = biggest_index;
    biggest_index = second_biggest_index;
    second_biggest_index = tmp;
  }
  results[indizes[0]] = GOALIE_CLASS;
  for (int i = 1; i < 11; i++) {
    if (i <= biggest_index) {
      results[indizes[i]] = DEFENSE_CLASS;
      continue;
    }
    if (i > second_biggest_index) {
      results[indizes[i]] = OFFENSE_CLASS;
      continue;
    }
    results[indizes[i]] = MIDDLEFIELD_CLASS;
  }

  return results;
  }

short TimeSlice::findGoalie(global_pos averagePositions[11], short side) {
  short goalie;
  double max = 0.0;
  
  for (int i = 0; i < 11; i++) {
    if (side == LEFT) {
      if (averagePositions[i].x < max) {
	max = averagePositions[i].x;
	goalie = i;
      }
    }
    else {
      if (averagePositions[i].x > max) {
	max = averagePositions[i].x;
	goalie = i;
      }
    }
  }
  return goalie;
}

// This function looks for passes. The definition of pass is different from that in
// create_timeTillPass(): Every ball movement from a player to a teammate that is not
// infered by four consecutive opponent possession cycles or an opponent kick is a pass.
// Note: a pass that was found by this function is ALWAYS a successful pass!
void TimeSlice::create_timeTillPass2(){
	// we find passes by going backwards.
	// if ball possessing player did not change from last to this cycle, either we
	// found a pass in the previous cycle or there is no pass. So abort:

	if((predecessor==NULL) || (possessingTeam==NEUTRAL) || (possessingPlayer==-1) ||
	   (predecessor->possessingPlayer==possessingPlayer)) return;

	// now look back:
	bool passyesno=false;	// was there a pass?
	int opponentcycle=0;	// is the ball controlled by the opponent?
	int cycles=1;		// how many cycles did we go back already?
	TimeSlice* recent=predecessor; // pointer to TimeSlice
	while((cycles<MAX_TIME) && (recent->timeTillPass==IRRELEVANT) &&
	      (recent->playmode==PM_PlayOn)){
			// no pass should take longer than MAX_TIME
		if((recent->possessingTeam!=NEUTRAL) &&
		   (recent->possessingTeam!=possessingTeam)){
		   	// opponent has ball contact
		   	if(opponentcycle>1){
				passyesno=false;// opponent had the ball in three cycles.				
				break;		// abort, there was no pass
			}else{
				if(recent->possessingPlayer!=-1 &&
				   recent->playerinfo[recent->possessingPlayer].playeraction.action
				    ==kick){
				 	passyesno=false;	// opponentplayer kicked
					break;	     		// the ball. abort
				}
				opponentcycle++;
			}
		   }else{	// neutral or team contact
		   	opponentcycle=0;
			// 2do is this still correct?: if neutral contact, continue. 
			// otherwise check who has the ball and did he kick?
		   	if(recent->possessingTeam!=NEUTRAL){
				if(recent->possessingPlayer!=-1 &&
				   recent->possessingPlayer==possessingPlayer){
					passyesno=false;	// nobody passes to himself
					break;			// abort.
				}
				
				// teammember controls the ball. did he kick?

				if(recent->possessingPlayer!=-1 &&
				   recent->playerinfo[recent->possessingPlayer].playeraction.action
				   ==kick){
				 	passyesno=true;
					if(recent->possessingPlayer<11){
						recent->whichSidePassed=LEFT;
					}else{
						recent->whichSidePassed=RIGHT;
					}
					break;		// at least we found a pass.  
				}	
			}
		   }
		   if((recent=recent->predecessor) == NULL){	// next slice
		   	passyesno=false;
			break;
		   }
		   cycles++;
	
	}

	// if there was a pass, propagate it:
	if(passyesno){
	  LOG(30, "Found pass from " + itoa(recent->possessingPlayer+1) + " to "
	       + itoa(possessingPlayer+1) + " that was shot at " + itoa(recent->servertime));
	  recent->passShooterUnum=(recent->possessingPlayer%11+1);
	  recent->passReceiverUnum=(possessingPlayer%11+1);
	  cycles=0;		
	  while((recent->predecessor!=NULL) && (cycles<MAX_TIME) &&
		(recent->timeTillPass==IRRELEVANT) && (recent->playmode==PM_PlayOn)){
	    recent->timeTillPass=cycles;
	    cycles++;
	    recent=recent->predecessor;
	  }
	}else{
	  LOG(30, itoa(servertime) + ": No pass found.");
	}
	return;
}




int TimeSlice::getPlayMode(){
	return this->playmode;
}

bool TimeSlice::getAlreadyAnalyzed(){
	return alreadyAnalyzed;
}
void TimeSlice::setAlreadyAnalyzed(bool yesno){
	alreadyAnalyzed=yesno;
}




// This function checks if the ball is in the left half
bool TimeSlice::ballInLeftHalf(){
	if(ballPos.x<0)
		return true;
	else
		return false;
}

// This function checks if the ball is in the left half
bool TimeSlice::ballInRightHalf(){
	if(ballPos.x>0)
		return true;
	else
		return false;
}


// This function checks if the ball is possessed by the left team
bool TimeSlice::ballPossessedByLeftTeam(){
  if(possessingTeam==LEFT)
  	return true;
  else
  	return false;
}


// This function checks if the ball is possessed by the left team
bool TimeSlice::ballPossessedByRightTeam(){
  if(possessingTeam==RIGHT)
  	return true;
  else
  	return false;
}

void TimeSlice::createRegions(void) {
  //Initialisation:
  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
      regions[i][i2][0] = 0;
      regions[i][i2][1] = 0;
    }
  }  
    
  int region_length = NUMBER_OF_REGIONS * REGION_SIZE;
  global_pos left_upper_corner;
  left_upper_corner.x = ballPos.x - (region_length / 2);
  left_upper_corner.y = ballPos.y - (region_length / 2);

  if (servertime == 1188) {
    LOG(40, "upper_corner: " + itoa(left_upper_corner.x) + ", " + itoa(left_upper_corner.y));
    LOG(40, "ball_pos: " + itoa(ballPos.x) + ", " + itoa(ballPos.y));
  }

  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {        //x
    for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {   //y
      for (int i3 = 0; i3 < 22; i3++) {                //players
	if (playerinfo[i3].pos.x > (left_upper_corner.x + (i * REGION_SIZE)) &&
	    playerinfo[i3].pos.x <= (left_upper_corner.x + ((i + 1) * REGION_SIZE)) &&
	    playerinfo[i3].pos.y > (left_upper_corner.y + (i2 * REGION_SIZE)) &&
	    playerinfo[i3].pos.y <= (left_upper_corner.y + ((i2 + 1) * REGION_SIZE))) {
	  if (servertime == 1188) {
	    LOG(40, "  " + itoa(i3) + ": " + itoa(playerinfo[i3].pos.x) + ", "
	         + itoa(playerinfo[i3].pos.y) + " matched");
	  }
	  regions[i][i2][playerinfo[i3].side == LEFT ? 0 : 1]++;
	}
      }
    }
  }
}

// This function saves data about passes for C5.0
// for the left (side=false) or right (side=true) team.
// Use this BEFORE scaling to 0..1000!
void TimeSlice::saveC5PassData(bool side, FILE * outfp){

	// do not save slices in which team side does not have the ball
	if(side){
		if(possessingTeam!=RIGHT) return;
	}else{
		if(possessingTeam!=LEFT) return;
	}
	
	// save slice-features in C5.0-format:
	fprintf(outfp, "%g,%g,", ballPos.x, ballPos.y);	// save ballPos
	// save possessingTeam
	if(possessingTeam==RIGHT)			// note: if scaled: 0
		fprintf(outfp, "right,");		
	else if(possessingTeam==NEUTRAL)		// note: if scaled: 500
		fprintf(outfp, "none,");
	else if(possessingTeam==LEFT)		// note: if scaled: 1000
		fprintf(outfp, "left,");
	else LOG(25, string("possessingTeam unprecise!"));
		
	for(int i=0; i<8; i++){				// save player
		if(player[i].side==LEFT)		// note: if sclaed: 1000
			fprintf(outfp,"left,");
		else if(player[i].side==RIGHT)		// note: if scaled: 0
			fprintf(outfp,"right,");
		else LOG(25, string("player.side unprecise!"));
		fprintf(outfp, "%g,%g,", player[i].ballangle,
				    player[i].distance);
	}
	// save TimeTillPass
	// maybe more distinctions for better prediction?? 2do
	if((timeTillPass==0) && (whichSidePassed==RIGHT && side) ||
	                        (whichSidePassed==LEFT && !side)){
		fprintf(outfp, "yes\n");
	}else{
		fprintf(outfp, "no\n");
	}
	
	return;
};




// This function saves data about passes for C5.0
// for the left (side=false) or right (side=true) team.
// USE this BEFORE scaling!
void TimeSlice::saveC5RegionalPassData(bool side, FILE * outfp){

	// do not save slices in which team side does not have the ball
	if(side){
		if(possessingTeam!=RIGHT) return;
	}else{
		if(possessingTeam!=LEFT) return;
	}

	
	// save slice-features in C5.0-format:
	fprintf(outfp, "%g,%g,", ballPos.x, ballPos.y);	// save ballPos
	// save possessingTeam
	if(possessingTeam==RIGHT)		// note: if sclaed: 0
		fprintf(outfp, "right,");		
	else if(possessingTeam==NEUTRAL)	// note: if scaled: 500
		fprintf(outfp, "none,");
	else if(possessingTeam==LEFT)		// note: if scaled: 1000
		fprintf(outfp, "left,");
	else LOG(25, string("possessingTeam unprecise!"));
		
  	for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    		for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
			fprintf(outfp, "%d,%d,", regions[i][i2][0],regions[i][i2][1]);
		}
	}

	
	// save TimeTillPass
	// maybe more distinctions for better prediction?? 2do
	if((timeTillPass==0) && (whichSidePassed==RIGHT && side) ||
	                        (whichSidePassed==LEFT && !side)){ 
		fprintf(outfp, "yes\n");
	}else{
		fprintf(outfp, "no\n");
	}
	
	return;
};








// use this only if there are no messages in the logfile.
// this function checks if and which teams did a kick.
void TimeSlice::set_kickhappened(){
	for(int i=0; i<22; i++){
		if(playerinfo[i].playeraction.action==kick){
        		if(i<11){
            			if((kickhappened==NOTEAM) || (kickhappened==RIGHTTEAM))
                			kickhappened+=LEFTTEAM;
        		}else{
           			if((kickhappened==NOTEAM)||(kickhappened==LEFTTEAM))
               				kickhappened+=RIGHTTEAM;
        		}
          		LOG(30, itoa(servertime) + ": There was a kick!");
		}
	}
}



// calculate difference between this and another TimeSlice
double TimeSlice::differenceBetweenSlice(TimeSlice * otherslice){
	double difference=0;	// total difference

	if(otherslice==NULL) return 0;
	
	difference+=doubleabs(ballPos.x-otherslice->ballPos.x);
	difference+=doubleabs(ballPos.y-otherslice->ballPos.y);
	difference+=doubleabs(possessingTeam-otherslice->possessingTeam);
	
	for(int i=0; i<8; i++){
		difference+=doubleabs(player[i].side-otherslice->player[i].side);
		difference+=doubleabs(player[i].distance-otherslice->player[i].distance);
		difference+=doubleabs(player[i].ballangle-otherslice->player[i].ballangle);
		// add more if more features are saved into database and/or file
	}
	difference+=doubleabs(timeTillGoalkick-otherslice->timeTillGoalkick);	
	difference+=doubleabs(timeTillPass-otherslice->timeTillPass);
	difference+=doubleabs(timeTillLosingBall-otherslice->timeTillLosingBall);
	return difference;
}





// calculate difference between this and another TimeSlice
double TimeSlice::differenceBetweenRegionalSlice(TimeSlice * otherslice){
	double difference=0;	// total difference

	if(otherslice==NULL) return 0;
	
	difference+=doubleabs(ballPos.x-otherslice->ballPos.x);
	difference+=doubleabs(ballPos.y-otherslice->ballPos.y);
	difference+=doubleabs(possessingTeam-otherslice->possessingTeam);
	
  	for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    		for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
      			difference+=doubleabs(regions[i][i2][0]-
			                      otherslice->regions[i][i2][0]);
			difference+=doubleabs(regions[i][i2][1]-
						otherslice->regions[i][i2][1]);
    		}
  	}

	
	difference+=doubleabs(timeTillGoalkick-otherslice->timeTillGoalkick);	
	difference+=doubleabs(timeTillPass-otherslice->timeTillPass);
	difference+=doubleabs(timeTillLosingBall-otherslice->timeTillLosingBall);
	return difference;
}










void TimeSlice::saveRegionalSliceToFile(FILE* file, bool saveEmptyRegions) {
  bool go = false;
  if (!saveEmptyRegions) {
    for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
      for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
	if (regions[i][i2][0] != 0 ||
	    regions[i][i2][1] != 0) {
	  go = true;
	  break;
	}
      }
      if (go) {
	break;
      }
    }
    if (!go) {
      return;
    }
  }
  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
      fprintf(file, "%d %d ", regions[i][i2][0], regions[i][i2][1]);
    }
  }
  fprintf(file, "%g %g ", ballPos.x, ballPos.y);	// save ballPos
  fprintf(file, "%d ", possessingTeam);		        // save possessingTeam
  fprintf(file, "%d %d %d %d\n", timeTillPass, timeTillGoalkick, timeTillLosingBall, servertime);
}

void TimeSlice::saveSliceToFile(FILE * outfp){
	fprintf(outfp, "%g %g ", ballPos.x, ballPos.y);	// save ballPos
	fprintf(outfp, "%d ", possessingTeam);		// save possessingTeam
	for(int i=0; i<8; i++){				// save player
		fprintf(outfp, "%d %g %g ", player[i].side, player[i].ballangle,
					    player[i].distance);
	}
	// save TimeTillX
	fprintf(outfp, "%d %d %d\n", timeTillPass, timeTillGoalkick, timeTillLosingBall);
	return;
}

void TimeSlice::loadSliceFromFile(FILE* file) {
  fscanf(file, "%lg %lg ", &ballPos.x, &ballPos.y);
  double team;
  fscanf(file, "%lg ", &team);
  possessingTeam = (short)rint(team);
  for (int i = 0; i < 8; i++) {
    double side;
    fscanf(file, "%lg %lg %lg ", &side, &player[i].ballangle, &player[i].distance);
    player[i].side = (short)rint(side);
  }
  double pass, kick, ball;
  fscanf(file, "%lg %lg %lg\n", &pass, &kick, &ball);
  timeTillPass = (short)rint(pass);
  timeTillGoalkick = (short)rint(kick);
  timeTillLosingBall = (short)rint(ball);
}

void TimeSlice::loadRegionalSliceFromFile(FILE* file) {
  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
      double left, right;
      fscanf(file, "%lg %lg ", &left, &right);
      regions[i][i2][0] = (short)rint(left);
      regions[i][i2][1] = (short)rint(right);
    }
  }
  fscanf(file, "%lg %lg ", &ballPos.x, &ballPos.y);
  
  fscanf(file, "%lg ", &possessingTeam);

  double pass, kick, ball, time;
  fscanf(file, "%lg %lg %lg %lg\n", &pass, &kick, &ball, &time);
  timeTillPass = (short)rint(pass);
  timeTillGoalkick = (short)rint(kick);
  timeTillLosingBall = (short)rint(ball);
  servertime = (short)rint(time);
}

double deg2rad(double d) {
  return d * (M_PI / 180);
}

void TimeSlice::computeGlobalPlayerPos() {
  for (int i = 0; i < 8; i++) {
    player[i].pos.x = ballPos.x - cos(deg2rad(player[i].ballangle)) * player[i].distance;
    player[i].pos.y = ballPos.y - sin(deg2rad(player[i].ballangle)) * player[i].distance;
  }
}



// scale all values into 0..1000
void TimeSlice::scaleAllRegional1000(){
	// ballPos -52.5..52.5 -> 0..1000 and -34.0..34.0 -> 0..1000
	ballPos.x = scale_from_to(ballPos.x, -52.5, 52.5, 0, 1000);
	ballPos.y = scale_from_to(ballPos.y, -52.5, 52.5, 0, 1000);
		
	// possessingTeam -1, 0, 1 -> 0, 500, 1000
	possessingTeam=(int)scale_from_to(possessingTeam, -1.0, 1.0, 0.0, 1000.0);
				// rounding doesn't matter (499~500)
	

        for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    		for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
			// values 0,3 are just educated guesses
      			regions[i][i2][0]=(int)scale_from_to(regions[i][i2][0], 0,3, 0, 1000);
			regions[i][i2][1]=(int)scale_from_to(regions[i][i2][1], 0,3, 0, 1000);
    		}
  	}

	
	
	
	// timeTillPass, timeTillGoalkick, timeTillLosingBall 0..40 -> 0..1000
	// rounding to it doesn't matter much
	timeTillPass=(int)scale_from_to(timeTillPass, 0, 40, 0, 1000);
	timeTillGoalkick=(int)scale_from_to(timeTillGoalkick, 0, 40, 0, 1000);
	timeTillLosingBall=(int)scale_from_to(timeTillLosingBall, 0, 40, 0, 1000);	
}








// scale all values into 0..1000
void TimeSlice::scaleAll1000(){
	// ballPos -52.5..52.5 -> 0..1000 and -34.0..34.0 -> 0..1000
	ballPos.x = scale_from_to(ballPos.x, -52.5, 52.5, 0, 1000);
	ballPos.y = scale_from_to(ballPos.y, -52.5, 52.5, 0, 1000);
		
	// possessingTeam -1, 0, 1 -> 0, 500, 1000
	possessingTeam=(int)scale_from_to(possessingTeam, -1.0, 1.0, 0.0, 1000.0);
				// rounding doesn't matter (499~500)
	
	for(int i=0; i<8; i++){
		// Player:
		// 	side -1, 0, 1 -> 0, 500, 1000
		player[i].side=(int)scale_from_to(player[i].side, -1.0, 1.0, 0.0, 1000.0);
		//	angle 0..360 -> 0..1000
		player[i].ballangle=scale_from_to(player[i].ballangle, 0, 360, 0, 1000);
		//	distance 0..30 -> 0..1000 (30 is an estimate!)
		player[i].distance=scale_from_to(player[i].distance, 0, 30, 0, 1000);
	}
	
	
	// timeTillPass, timeTillGoalkick, timeTillLosingBall 0..40 -> 0..1000
	// rounding to it doesn't matter much
	timeTillPass=(int)scale_from_to(timeTillPass, 0, 40, 0, 1000);
	timeTillGoalkick=(int)scale_from_to(timeTillGoalkick, 0, 40, 0, 1000);
	timeTillLosingBall=(int)scale_from_to(timeTillLosingBall, 0, 40, 0, 1000);	
}


void TimeSlice::undoRegionalScale() {
  ballPos.x = scale_from_to(ballPos.x, 0, 1000, -52.5, 52.5);
  ballPos.y = scale_from_to(ballPos.y, 0, 1000, -52.5, 52.5); 

  possessingTeam = (short)rint(scale_from_to(possessingTeam, 0.0, 1000.0, -1.0, 1.0));


  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
	for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
	  regions[i][i2][0]=(int)rint(scale_from_to(regions[i][i2][0], 0,1000, 0, 3));
	  regions[i][i2][1]=(int)rint(scale_from_to(regions[i][i2][1], 0,1000, 0, 3));
    	}
  }

  timeTillPass = (int)rint(scale_from_to(timeTillPass, 0, 1000, 0, 40));
  timeTillGoalkick = (int)rint(scale_from_to(timeTillGoalkick, 0, 1000, 0, 40));
  timeTillLosingBall = (int)rint(scale_from_to(timeTillLosingBall, 0, 1000, 0, 40));
}






void TimeSlice::undoScale() {
  ballPos.x = scale_from_to(ballPos.x, 0, 1000, -52.5, 52.5);
  ballPos.y = scale_from_to(ballPos.y, 0, 1000, -52.5, 52.5); 

  possessingTeam = (short)rint(scale_from_to(possessingTeam, 0.0, 1000.0, -1.0, 1.0));

  for (int i = 0; i < 8; i++) {
    player[i].side = (short)rint(scale_from_to(player[i].side, 0.0, 1000.0, -1.0, 1.0));
    player[i].ballangle = scale_from_to(player[i].ballangle, 0, 1000, 0, 360);
    player[i].distance = scale_from_to(player[i].distance, 0, 1000, 0, 30);
  }

  timeTillPass = (int)rint(scale_from_to(timeTillPass, 0, 1000, 0, 40));
  timeTillGoalkick = (int)rint(scale_from_to(timeTillGoalkick, 0, 1000, 0, 40));
  timeTillLosingBall = (int)rint(scale_from_to(timeTillLosingBall, 0, 1000, 0, 40));
}


// check whether ballpossession changed within 40 slices.
// point of loosing ball is when ball becomes neutral and will be controlled by opponent
// afterwards
void TimeSlice::create_timeTillLosingBall(){
	int timecount=1;	// count how many cycles we went back
	TimeSlice *recent=predecessor;
	bool possessionchanged=false;

	if(possessingTeam==NEUTRAL) return; // we are in a neutral phase
		
	while((timecount<MAX_TIME) && (recent!=NULL) && (!possessionchanged)){
		if((recent->possessingTeam!=NEUTRAL) &&    // problem: abrupt changes
							   // like kick_off
		   (recent->possessingTeam!=possessingTeam)){
			// possession of ball changed
			possessionchanged=true;
		}else{
			if(recent->possessingTeam==possessingTeam){
				// possession did NOT change
				break;
			}else{
				// ball is neutral, so go back
				timecount++;
				recent=recent->predecessor;
			}
		}
	}
	if(possessionchanged){
	  LOG(30, string("Possession of ball changed!"));
	  // propagate back
	  timecount=1;
	  while((timecount<MAX_TIME) && (recent!=NULL) &&
		(recent->possessingTeam!=possessingTeam)){
	    recent->timeTillLosingBall=timecount;
	    timecount++;
	    recent=recent->predecessor;
	  }
	}
}




// analyzePreviousKick computes the ballvector from the positions during the kick and
// the positions directly AFTER the kick. So the ballvector is computed in the cycle
// after the kick.
void TimeSlice::analyzePreviousKick(void){
        LOG(30, string("Starting analyzing previous kick"));
	// calculate direction and velocity of ball
	// check wheter it will reach goal or teammate
	// in one of the two cases propagate back
	double velocity;	// distance that the ball moved since the last slice
	double direction;	// direction that the ball moves from last slice
	global_pos entrypoint;  // where does the ball cross the goalline?
	double shootdist=1000.0;// how far does the ball have to travel?
	global_pos ballvector;	// vector of moving ball
	
	if(predecessor==NULL) return;
	
	calculate_ballvector(velocity, direction);
	LOG(30, "Velocity: " + itoa(velocity) + " Direction: " + itoa(direction));
	ballvector.x=mycos(direction)*velocity;		// 2do! Is this correct for
	ballvector.y=mysin(direction)*velocity;		// kicks from right to left? TEST!
	LOG(30, "ballvector calculated: " + itoa(ballvector.x) + ", " + itoa(ballvector.y));
	// now check if ball is aimed at goal
	LOG(30, "Kicking Team: " + itoa(predecessor->kickhappened));
	if((predecessor->kickhappened==LEFTTEAM) ||
	   (predecessor->kickhappened==BOTHTEAMS)){
		// goal kick on right goal?
		LOG(30, "Kickdir: " + itoa(scale360(direction)));
	    	LOG(30, "RDir1: " + itoa(angleBetweenTwoPositions(ballPos, leftPostr)) +
		    " RDir2: " + itoa(angleBetweenTwoPositions(ballPos, rightPostr)));
		if(betweenAngles(scale360(direction),
       	           scale360(angleBetweenTwoPositions(ballPos, leftPostr)),
		   scale360(angleBetweenTwoPositions(ballPos, rightPostr)))){
		 	// travels in direction of goal. will it reach the goal?
			double x=GOAL_POST_X;
			intersectVert(ballPos, ballvector, x, entrypoint);
			LOG(30, "Will meet line x=" + itoa(x) + " at y=" + itoa(entrypoint.y));
			
			double difference;
			difference=entrypoint.x-x;
			if(difference<0) difference=-difference;
			
			if(difference<1.5){// tolerance threshold
				shootdist=distanceBetweenTwoPositions(ballPos, entrypoint);
			}else{		// no intersection possible! kick goes straight up
				shootdist=1000.0;// no kickpower should accomplish this
			}
			LOG(30, "Difference: " + itoa(difference) + " shootdist: "
			       + itoa(shootdist) + " Velocity: " + itoa(velocity));
			
		}
	}	
	if(((predecessor->kickhappened==RIGHTTEAM) ||
	    (predecessor->kickhappened==BOTHTEAMS)) &&(shootdist==1000.0)){
		// goal kick on left goal?
       	        LOG(30, "Kickdir: " + itoa(scale360(direction)));
	        LOG(30, "LDir1: " + itoa(angleBetweenTwoPositions(ballPos, leftPostl))
		    + " LDir2: " + itoa(angleBetweenTwoPositions(ballPos, rightPostl)));
	    	if(betweenAngles(scale360(direction),
		   scale360(angleBetweenTwoPositions(ballPos, leftPostl)),
		   scale360(angleBetweenTwoPositions(ballPos, rightPostl)))){
		 	// travels in direction of goal. will it reach the goal?
			double x=-GOAL_POST_X;
			intersectVert(ballPos, ballvector, x, entrypoint);
			LOG(30, "Will meet line x=" + itoa(x) + " at y=" + itoa(entrypoint.y));
			
			double difference;
			difference=entrypoint.x-x;
			if(difference<0) difference=-difference;
			
			if(difference<1.5){// tolerance threshold
				shootdist=distanceBetweenTwoPositions(ballPos, entrypoint);
			}else{		// no intersection possible! kick goes straight up
				shootdist=1000.0;// no kickpower should accomplish this
			}
			LOG(30, "Difference: " + itoa(difference) + " shootdist: "
			       + itoa(shootdist) + " Velocity: " + itoa(velocity));
		}
	}
	// will ball travel shootdist far?
	if(willBallTravelThatFar(shootdist, velocity)){
		// propagate goalkick
	        LOG(30, string("Propagating goalkick. #########"));
		TimeSlice *recent=this->predecessor;
		int timeTo=0;	// counts cycles until event
		// now propagate back MAX_TIME or until approaching
		// value != IRRELEVANT
		while((recent!=NULL) &&
		      (recent->timeTillGoalkick==IRRELEVANT) &&
		      (timeTo<MAX_TIME) &&
		      (recent->playmode==PM_PlayOn)){
		  	recent->timeTillGoalkick=(short)timeTo;
			timeTo++;
			recent=(TimeSlice*)(recent->predecessor); 
				// next slice
		}						
	}else{
	  LOG(30, string("Won't reach goal due to slow speed."));
	}
	shootdist=1000.0;
	LOG(30, string("Checking for passes now."));
	// now check for passes:
	int mainstart, mainend;
	if(predecessor->kickhappened==RIGHTTEAM){
		mainstart=11;
		mainend=22;
	}else{
		mainstart=0;	
		if(predecessor->kickhappened==BOTHTEAMS)
			mainend=22;
		else
			mainend=11;
	}
	for(int i=mainstart; i<mainend; i++){
		if(predecessor->playerinfo[i].playeraction.action==kick){
			// now, check if any teammate is in direction of the kick
			int start, end;	// boundaries of loop
			if(i<11){
				start=0;
				end=11;
			}else{
				start=11;
				end=22;
			}
			for(int receiver=start; receiver<end; receiver++){
				if(receiver==i) continue;// player doesn't pass to himself
				global_pos receiverpos;
				receiverpos.x=playerinfo[receiver].pos.x;
				receiverpos.y=playerinfo[receiver].pos.y;
				
				double rel_dir=angleBetweenTwoPositions(ballPos,
									receiverpos);
				shootdist=distanceBetweenTwoPositions(ballPos,receiverpos);
				if(shootdist<2.6) continue;	// if player is too close,
								// pass is hard to discern
				global_pos entrypoint2;	// intersectionpoint with
							// horizontal line
				intersectVert(ballPos, ballvector, receiverpos.x,
					      entrypoint);
				intersectHoriz(ballPos, ballvector, receiverpos.y,
					      entrypoint2);

				double difference;	
				double difference2;				 
				difference=entrypoint.y-receiverpos.y; //How far away does
				                  // the ball meet the expected position?
				if(difference<0) difference=-difference;
				difference2=entrypoint2.x-receiverpos.x;//How far away does
				                  // the ball meet the expected position?
				if(difference2<0) difference2=-difference2;

				if(((difference<2.1) || (difference2<2.1)) &&
								// tolerance threshold
				   (entrypoint.x==receiverpos.x) &&
				   (entrypoint2.y==receiverpos.y)){
					shootdist=distanceBetweenTwoPositions(ballPos,
					                                       entrypoint);
				}else{// no intersection possible!
					shootdist=1000.0;
						// no kickpower should accomplish this
				}	      
				if(willBallTravelThatFar(shootdist, velocity)){
					LOG(30, string("Starting to propagate passes.") +
					    string(" Without messages. #########"));
					LOG(30, "Player " + itoa(i) + " passes to "
					     + "Player " + itoa(receiver));
				  	if(receiver<11)
				  		predecessor->whichSidePassed=LEFT;
					else
						predecessor->whichSidePassed=RIGHT;
					// propagate pass
					TimeSlice *recent=this->predecessor;
					int timeTo=0;	// counts cycles until event
					recent->passShooterUnum=i+1;
					recent->passReceiverUnum=receiver+1;
					// now propagate back MAX_TIME or until approaching
					// value != IRRELEVANT
					while((recent!=NULL) &&
					      (recent->timeTillPass==IRRELEVANT) &&
					      (timeTo<MAX_TIME) &&
					      (recent->playmode==PM_PlayOn)){
					  	recent->timeTillPass=(short)timeTo;
						timeTo++;
						recent=(TimeSlice*)(recent->predecessor);
						// next slice
					}
					return; // we can leave the function
				}
			}
		}
	}
}



// if we don't have messages, we need to calculate the ballvector using
// previous and recent position
void TimeSlice::calculate_ballvector(double &velocity, double &direction){
	velocity=distanceBetweenTwoPositions(predecessor->ballPos, ballPos);
	direction=angleBetweenTwoPositions(predecessor->ballPos, ballPos);
}






// Let a kick be a pass if and only if the kickdirection goes directly to a teammate.
// Passes that depend on the receiver running are thus only "clearings"...
void TimeSlice::create_timeTillPass(int logversion){
	// note: even if team A has possession of the ball, a player of team B may be
	// close enough to kick the ball.
        LOG(30, string("Checking for passes."));
	for(int i=0; i<22; i++){
		if(playerinfo[i].playeraction.action==kick){ // kick?
			// unsuccesful due to kickradius?
			if(distanceBetweenTwoPositions(ballPos,playerinfo[i].pos)
						>KICKABLE_MARGIN)
				continue;
			// now, check if any teammate is in direction of the kick
			int start, end;	// boundaries of loop
			if(i<11){
				start=0;
				end=11;
			}else{
				start=11;
				end=22;
			}
			for(int receiver=start; receiver<end; receiver++){
				if(receiver==i) continue;// player doesn't pass to himself
				if(distanceBetweenTwoPositions(ballPos,
				             playerinfo[receiver].pos)<2.6) continue;
					     	// if partner is too close, pass is hard
						// to discern
				double velocity;
				double shootdistance;
				// how far until ball is on same height as receiver?
				shootdistance=calculate_shootdist(
				        playerinfo[i].playeraction, i,
					playerinfo[receiver].pos.x,
					playerinfo[receiver].pos.y, velocity, logversion);
				if(willBallTravelThatFar(shootdistance, velocity)){
    				        LOG(30, string("There was a pass."));
					if(receiver<11)
						whichSidePassed=LEFT;
					else
						whichSidePassed=RIGHT;
					// propagate pass
					TimeSlice *recent=this;
					int timeTo=0;	// counts cycles until event
					// now propagate back MAX_TIME or until approaching
					// value != IRRELEVANT
					while((recent!=NULL) &&
					      (recent->timeTillPass==IRRELEVANT) &&
					      (timeTo<MAX_TIME) &&
					      (recent->playmode==PM_PlayOn)){
					  	recent->timeTillPass=(short)timeTo;
						timeTo++;
						recent=(TimeSlice*)(recent->predecessor); 
							// next slice
					}
					return; // we can leave the function
				}
			}
			
		}
	}

}




// calculate vector of kicked ball
// calculate position of intersection with goalline
// return distance from ballpos to intersection with vertical line at x
// in case of Pass pass along expected_y to check whether pass is accurate. otherwise
// pass 100.0 as expected_y
double TimeSlice::calculate_shootdist(Action action, int playerindex,
                   double x, double expected_y, double &kickpower, int logversion){
		// playerindex: which player did the action?
	double direction; // direction of the moving ball
	double velocity;  // distance that the ball moves in one cycle
	double shootdist=0.0;

	// calculate velocity and direction before kick:
	if(predecessor!=NULL){
		direction=angleBetweenTwoPositions(predecessor->ballPos, ballPos);
		velocity=distanceBetweenTwoPositions(predecessor->ballPos, ballPos);
	}else{
		// there is no previous slice, vel. and dir. are zero
		direction=velocity=0.0;
	}

	LOG(30, "Before kick ball's direction was " + itoa(direction)
	    + " and velocity was " + itoa(velocity));
	double dist_ball; // distance between player and ball
	double dir_diff;  // rel. angle between facing dir and ball
	global_pos playerpos;
	playerpos.x=playerinfo[playerindex].pos.x;
	playerpos.y=playerinfo[playerindex].pos.y;
	dist_ball=distanceBetweenTwoPositions(playerpos, ballPos);
	dir_diff=scale360(angleBetweenTwoPositions(playerpos, ballPos)
	         -playerinfo[playerindex].bodyangle);
	LOG(30, "Dist. to ball: " + itoa(dist_ball) + " dir_diff: " + itoa(dir_diff));
	// now add kick velocity and direction
	// dir needs to consider body dir, vel needs KICK_POWER_RATE, dist and dir to ball
	if(logversion==2){
		kickpower=KICKPOWERRATE6*(1-0.25*dir_diff/180.0-0.25*
			 (dist_ball-PLAYER_SIZE-BALL_SIZE)/KICKABLE_MARGIN);
	}else{
		// 2do Parameters will change for heterogenous players!
		kickpower=KICKPOWERRATE7*(1-0.25*dir_diff/180.0-0.25*
			 (dist_ball-PLAYER_SIZE-BALL_SIZE)/KICKABLE_MARGIN);
	}
	kickpower*=action.power;	// power that will accelerate the ball
	global_pos vector_before;
	global_pos vector_kick;
	global_pos vector_result;
	vector_before.x=mycos(direction)*velocity;
	vector_before.y=mysin(direction)*velocity;
	// 2do bug when the following value is negativ, strange results arise
	// radian??
	vector_kick.x=mycos(action.direction+
	                  playerinfo[playerindex].bodyangle)*kickpower;
	vector_kick.y=mysin(action.direction+
	                  playerinfo[playerindex].bodyangle)*kickpower;
	vector_result.x=vector_before.x + vector_kick.x;
	vector_result.y=vector_before.y + vector_kick.y;
	// vector of ball is known now. now calculate intersection with vert. line
	global_pos entrypoint;
	global_pos entrypoint2; // used to cross-check in case of pass (with horizontal)
	intersectVert(ballPos, vector_result, x, entrypoint);
	if(expected_y!=100.0)
		intersectHoriz(ballPos, vector_result, expected_y, entrypoint2);
	double difference;
	double difference2;
	if(expected_y!=100.0){
		difference=entrypoint.y-expected_y; // How far away does the ball meet
						    // the expected position?
		difference2=entrypoint2.x-x;
	}else{
		difference=0.0;	
		difference2=0.0;
	}
	if(difference<0) difference=-difference;
	if(difference2<0) difference2=-difference2;
	if(((difference<TOL_THRESHOLD) || (difference2<TOL_THRESHOLD))
	 && (entrypoint.x==x) && (entrypoint2.y=expected_y)){	
		shootdist=distanceBetweenTwoPositions(ballPos, entrypoint);
	}else{		// no intersection possible! kick goes straight up
		shootdist=1000.0;	// no kickpower should accomplish this
	}
	// now calculate velocity and reference it back via kickpower
	kickpower=sqrt(vector_result.x*vector_result.x+vector_result.y*vector_result.y);
  return shootdist;
}


bool TimeSlice::willBallTravelThatFar(double shootdist, double velocity){
	if(shootdist>100.0) return false;
	while(velocity>0.01){
		if(shootdist<0) return true;
		shootdist-=velocity;	// balltravels velocity meters each cycle
		velocity*=BALL_DECAY;	// ballvelocity decays each cycle
	}
	
	return false;		
}




bool TimeSlice::reachGoal(Action action, int playerindex, int logversion){
	double velocity;	// velocity of the ball after kick
	double x;
	if(playerindex<11)
		x=GOAL_POST_X;
	else
		x=-GOAL_POST_X;
	double shootdist=calculate_shootdist(action, playerindex, x, 100.0, velocity,
									logversion);
	LOG(30, "Velocity in reachgoal(): " + itoa(velocity));
	// now calculate if velocity is enough to go that far
	return willBallTravelThatFar(shootdist, velocity);
}




void TimeSlice::create_timeTillGoalkick(int logversion){
	// index in playerinfo[] matches player numbers
	
  	LOG(30, string("Checking for goalkicks."));
	
	
	double kickdirection_global;

	global_pos player_pos;	// necessary to calculate
	double playerdir;
	
	// note: even if team A has possession of the ball, a player of team B may be
	// close enough to kick the ball.
	for(int i=0; i<22; i++){
		if(playerinfo[i].playeraction.action==kick){
			// player did a kick
			// unsuccesful when out of kickradius?
			if(distanceBetweenTwoPositions(ballPos,playerinfo[i].pos)
							>KICKABLE_MARGIN)
				continue;
			LOG(30, "Player-Nr. " + itoa(i+1) + " kicked.");
			player_pos.x=playerinfo[i].pos.x;
			player_pos.y=playerinfo[i].pos.y;			
			playerdir=playerinfo[i].bodyangle;
				
			// check if he kicked in direction of the goal
			LOG(30, "Facing dir: " + itoa(playerdir));
			LOG(30, "Pos: " + itoa(player_pos.x) + ", " + itoa(player_pos.y));
			kickdirection_global=playerinfo[i].playeraction.direction
					+ playerdir; // kickdirection+bodydirection
				
			global_pos leftPosttemp;
			global_pos rightPosttemp;
			if(i<11){
				leftPosttemp=leftPostr;
				rightPosttemp=rightPostr;
			}else{
				leftPosttemp=leftPostl;
				rightPosttemp=rightPostl;
			}
			LOG(30, "Spieler-Nr. " + itoa(i+1));
			LOG(30, "scale360(kickdir): " + itoa(scale360(kickdirection_global))
			    + " angle1: " + itoa(scale360(angleBetweenTwoPositions(ballPos, leftPosttemp))) 
			    + " angle2: " + 
			    itoa(scale360(angleBetweenTwoPositions(ballPos, rightPosttemp))));
			if(betweenAngles(scale360(kickdirection_global),
			    scale360(angleBetweenTwoPositions(player_pos,leftPosttemp)),
			    scale360(angleBetweenTwoPositions(player_pos,rightPosttemp)))){
			        LOG(30, string("correct direction to goal"));
				// kicking hard enough?
				if(reachGoal(playerinfo[i].playeraction, i, logversion)){
							// will it reach the goal?
					// propagate back goalkick
   				        LOG(30, string("will reach goal. propagating goalkick!"));
					TimeSlice *recent=this;
					int timeTo=0;	// counts cycles until event
					// now propagate back MAX_TIME or until
					// approaching value != IRRELEVANT
					while((recent!=NULL) &&
					      (recent->timeTillGoalkick==IRRELEVANT) &&
					      (timeTo<MAX_TIME) &&
					      (recent->playmode==PM_PlayOn)){
						//cout << "Prop: " << timeTo << endl;
					  	recent->timeTillGoalkick=(short)timeTo;
						timeTo++;
						recent=(TimeSlice*)(recent->predecessor); 
							// next slice
					}				
					return; // we can leave the function	
				}else{
				  LOG(30, string("Won't reach goal."));
				}
			}
		}
	}	
}






void TimeSlice::create_players(){
  // Copy all player positions into array, sort these by distance to ball
  // Keep 8 nearest players
	
  playerinfo_t	sortVector[22];


  for(int i=0; i<22; i++){
    	sortVector[i].pos.x=playerinfo[i].pos.x;   // initialize vector
	sortVector[i].pos.y=playerinfo[i].pos.y;   // initialize vector
 	sortVector[i].side=playerinfo[i].side;
  }

  // now sort:    Mmmmhhhh.......BubbleSort
  double 	dist1, dist2;
  playerinfo_t	auxi;				// auxiliary object for swapping
  global_pos 	balltemp;			// store xy of ball_t or pos[0] here

  balltemp.x=ballinfo.pos.x;
  balltemp.y=ballinfo.pos.y;
  
  
  bool 	swapped=true;
  while(swapped){
    swapped=false;
    for(int j=1; j<22; j++){
      dist1=distanceBetweenTwoPositions(sortVector[j-1].pos,balltemp);
      dist2=distanceBetweenTwoPositions(sortVector[j].pos,balltemp);
      if(dist1>dist2){		// swap
	swapped=true;
	auxi=sortVector[j-1];
	sortVector[j-1]=sortVector[j];
	sortVector[j]=auxi;
      }
    }
  }

  // keep first 8
  for(int i=0; i<8; i++){
    player[i].pos.x=sortVector[i].pos.x;
    player[i].pos.y=sortVector[i].pos.y;
    player[i].side=sortVector[i].side; 	// LEFT RIGHT 1 -1
    player[i].distance=distanceBetweenTwoPositions(sortVector[i].pos,balltemp);
    player[i].ballangle=scale360(angleBetweenTwoPositions(sortVector[i].pos,balltemp));
  }

}

void TimeSlice::create_ballPos(){
    ballPos.x=ballinfo.pos.x;
    ballPos.y=ballinfo.pos.y;
    LOG(30, "Ballpos: " + itoa(ballPos.x) + ", " + itoa(ballPos.y));
    return;
}

// 2do: how should we cope with play modes like kick_off after a goal?
int TimeSlice::create_possessingTeam(){
  double min_sofar=500.0;
  double recentDistance;
  int index_sofar=0;
  for(int i=1; i<23; i++){
    recentDistance=distanceBetweenTwoPositions(ballPos,playerinfo[i-1].pos);
    if(recentDistance<min_sofar){
      min_sofar=recentDistance;
      index_sofar=i;
    }
  }
  if(min_sofar>OWNER_RADIUS){	// nobody is near to the ball, ballowner team,but no player
  	if(!predecessor)
		return NEUTRAL;
	else{
		possessingPlayer=-1;
		return predecessor->possessingTeam;
	}
	
  }
  possessingPlayer=index_sofar-1;		// save index to controlling player
  return index_sofar<12? LEFT : RIGHT;
}



/** This function should only be called by the onlinecoach when it found that
  * there was a kick (by watching the ballspeed increase) but does not know who
  * did the kick.
  */
void TimeSlice::processAnonymousKick(void){
	// set action of BallPossessor to kick
	if(possessingPlayer==-1) return;
	playerinfo[possessingPlayer].playeraction.action=kick;
	// call functions that cope with kicks:
	set_kickhappened();	
	create_timeTillPass2();
	findDribble();
}

// unum 1-11, returns 1-11, 0 means "no partner found"
// maximal distance the Dirty Dozen can safely pass:?
#define MAX_PASS_DIST 20
// buffer between receiver and opponent
#define DANGEROUS_BUFFER 1.9
#define PASS_CONE_ANG 18.5
#define PASS_INVALIDATE 100.0
short TimeSlice::findBestPassPartner(short unum, short side) {
  int start = side == LEFT ? 0 : 11;
  int end = side == LEFT ? 11 : 22;
  int startopp = side == LEFT ? 11 : 0;
  int endopp = side == LEFT ? 22 : 11;
  
  int playerindex = (unum - 1) + (side == LEFT ? 0 : 11);

  vector<short> mateUnums;	// Unum of mates
  vector<double> mateDists;	// their distance to ballowner
  vector<double> mateAngs;	// their angle to the ballowner (global, absolute)

  // first, find all teammates that are within a MAX_PASS_DIST m radius
  double dist, ang;
  for(int mate=start; mate < end; mate++){
  	if(mate==playerindex) continue;	// do not pass to yourself
	
  	dist=distanceBetweenTwoPositions(playerinfo[playerindex].pos, playerinfo[mate].pos);
	ang=scale360(angleBetweenTwoPositions(playerinfo[playerindex].pos, playerinfo[mate].pos));
	
	if(dist < MAX_PASS_DIST){
		mateUnums.push_back(mate);
		mateDists.push_back(dist);
		mateAngs.push_back(ang);
	}
  }

  
  if(mateUnums.size()==0) return 0;	// no mate close enough
  
  // now check if mate stands in a free cone
  for(int passmate=0; passmate<mateUnums.size(); passmate++){
  	for(int opp=startopp; opp<endopp; opp++){
		// distance and angle of opponent to the ballowner:
	  	dist=distanceBetweenTwoPositions(playerinfo[playerindex].pos,playerinfo[opp].pos);
		ang=scale360(angleBetweenTwoPositions(playerinfo[playerindex].pos,playerinfo[opp].pos));
		
		
		// is opponent dangerous for this passmate?
		if(dist<mateDists[passmate]+DANGEROUS_BUFFER && // too close
		   betweenAngles(ang,
		                 scale360(mateAngs[passmate]-PASS_CONE_ANG),
				 scale360(mateAngs[passmate]+PASS_CONE_ANG))){	// in cone
				 	// opponent is between ballowner and passmate
					// invalidate passmate by setting its distance to a high value
					mateDists[passmate]=PASS_INVALIDATE;
				 }
  	}
  }
  
  // now, find most forward (except PASS_INVALIDATE)
  int factor;
  int bestUnum=-1;
  double bestX=-100.0;
  if(side==LEFT)
  	factor=1;
  else
  	factor=-1;	// right team plays in the other direction
  for(int mate=0; mate<mateUnums.size(); mate++){
  	if(factor*playerinfo[mateUnums[mate]].pos.x>bestX && mateDists[mate]<PASS_INVALIDATE){
		bestUnum=mateUnums[mate];
		bestX=factor*playerinfo[mateUnums[mate]].pos.x;
	}
  }

  bestUnum++;	// from index to unum
  if(bestUnum>11)
  	return (bestUnum-11);
  else
  	return bestUnum;
}

// simple way to find a passpartner: closest player of team side
short TimeSlice::findClosestPartner(short unum, short side){
  int start = side == LEFT ? 0 : 11;
  int end = side == LEFT ? 10 : 21;
  int p = (unum - 1) + ((side == LEFT ? 0 : 1) * 11);
  double best_dist = 100.0;
  int found = p;
  for (int i = start; i <= end; i++) {
    if (p != i) {
      double dist = distanceBetweenTwoPositions(playerinfo[p].pos, playerinfo[i].pos);
      if (dist < best_dist) {
	best_dist = dist;
	found = i;
      }
    }
  }
  return found;
}


/*
 * This function dumps playerinfo.pos.[xy], ballinfo.pos.[xy] and playerattributes into a file.
 * This file can be further processed by paintDumps.pl which will create an PNG-Image file
 * displaying the players and the ball.
 */
void TimeSlice::dump(string filename) {
  FILE* output = fopen(filename.c_str(), "w");
  fprintf(output, "b %lf %lf\n", ballinfo.pos.x, ballinfo.pos.y);
  for (int i = 0; i < 22; i++) {
    fprintf(output, "p %d %lf %lf %d\n", i, playerinfo[i].pos.x, playerinfo[i].pos.y, playerAttributes[i]);
  }
  fclose(output);
}


// checks if it is a playmode in which team side (LEFT or RIGHT) may kick
bool TimeSlice::teamsKickMode(short side){
	if(side==LEFT &&
	   (playmode==PM_BeforeKickOff ||
	   playmode==PM_KickOff_Left ||
	   playmode==PM_KickIn_Left ||
	   playmode==PM_FreeKick_Left ||
	   playmode==PM_CornerKick_Left ||
	   playmode==PM_GoalKick_Left))
	   return true;
	
	if(side==RIGHT &&
   	   (playmode==PM_KickOff_Right ||
   	   playmode==PM_KickIn_Right ||
   	   playmode==PM_FreeKick_Right ||
	   playmode==PM_CornerKick_Right ||
	   playmode==PM_GoalKick_Right))
		return true;

	return false;
}

/*
short TimeSlice::findPassPartnerForGoalie(short side) {

}
*/

TimeSlice::TimeSlice(void) {
  return;
}

TimeSlice::TimeSlice(playerinfo_t * playerinfoarg, ballinfo_t *ballinfoarg, short s_time,
		 TimeSlice *predecessorarg, int v, bool messagesInLogfile, int logversion,
		 const string* nameLeftarg, const string * nameRightarg, int pmode){
  playerinfo=playerinfoarg;
  ballinfo=*ballinfoarg;
  
  nameLeft=nameLeftarg;
  nameRight=nameRightarg;

  this->playmode=pmode;
  alreadyAnalyzed=false; 	// only used by onlinecoach so far

  whichSidePassed=NEUTRAL;	// does not matter actually

  predecessor=predecessorarg;	// link backwards
  servertime=s_time;
  
  kickhappened=NOTEAM;
  // check all playeractions and set kickhappened
  if(!messagesInLogfile)
  	set_kickhappened();
	
  
  LOG(30, "Servertime: " + itoa(servertime));
 
  timeTillPass		=IRRELEVANT;	// Marker: value not relevant yet        
  timeTillGoalkick	=IRRELEVANT;
  timeTillLosingBall	=IRRELEVANT;  
  dribblingPlayerIndex  =-1;

  // create all features
  create_ballPos();
  create_players();
  possessingTeam=create_possessingTeam();
  
  LOG(30, "Possessing Team: " + itoa(possessingTeam));


  if(messagesInLogfile){ // with messages (e.g. actions) it's easy
	create_timeTillGoalkick(logversion);
	// create_timeTillPass(logversion);
  }else{	// without messages
	if((predecessor!=NULL) && (predecessor->kickhappened!=NOTEAM)){
		// check whether pass, goalkick and propagate back	
		analyzePreviousKick();	
		// 2do conflicts with create_timeTillPass2() ??
	}
  }
	
  create_timeTillPass2();
  create_timeTillLosingBall();
  findDribble();

  /*
  // temporary test for findBestPassPartner
  if(teamsKickMode(RIGHT) && possessingPlayer>-1 && possessingTeam == RIGHT){
  	int ownerunum = (possessingPlayer>10? possessingPlayer+1-11 : possessingPlayer+1);
  	cout << "At cycle " << servertime << ", best PassPartner for ballowner is : " << findBestPassPartner(ownerunum,RIGHT) << "\n\n" << endl;
  }
  */

  //cout << "My address: " << this << " My predecessor: " << predecessor << endl;
  //cout << "TimeSlice done and message-memory freed." << endl;
  
  return;
}



