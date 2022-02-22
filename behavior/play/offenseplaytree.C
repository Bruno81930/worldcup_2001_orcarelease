//_____________________________________________________________________________
//
//  offenseplaytree.C
//_____________________________________________________________________________

#include "offenseplaytree.h"

static bool dribbling = false;
static int mate_ppps[12];
int num_pref_pass_pts = 15;
int count_best_line = 1;
static int pppIndex[15];

Vector ppps[15];

Vector goal_ppps[7] = {
    Vector(42.5, 20),
    Vector(42.5, -20),
    Vector(42.5, 30),
    Vector(42.5, -30),
    Vector(42.5, 10),
    Vector(42.5, -10), 
    Vector(42.5, 0),
};

Vector ppp[] = {
    Vector(42.5, 20),
    Vector(42.5, -20),
    Vector(42.5, 30),
    Vector(42.5, -30),
    Vector(42.5, 34),
    Vector(42.5, -34),
    Vector(42.5, 10),
    Vector(42.5, -10),
    //Vector(30, -34),
    //Vector(30, -34),
    Vector(40, 34),
    Vector(40, -34),
    //Vector(10, -34),
    //Vector(10, -34), 
    Vector(20, 34),
    Vector(20, -34),
    Vector(0, 34),
    Vector(0, -34),
    Vector(42.5, 0)
};

Vector ppps_left[] = {
    Vector(42.5, 20),
    Vector(42.5, -20),
    Vector(42.5, 30),
    Vector(42.5, -30),
    Vector(42.5, 34),
    Vector(42.5, -34),
    Vector(42.5, 10),
    Vector(42.5, -10),
    //Vector(30, -34),
    //Vector(30, -34),
    Vector(40, -34),
    Vector(40, -34),
    //Vector(10, -34),
    //Vector(10, -34), 
    Vector(20, -34),
    Vector(20, -34),
    Vector(0, -34),
    Vector(0, -34),
    Vector(42.5, 0)
};

Vector ppps_right[] = {
    Vector(42.5, 20),
    Vector(42.5, -20),
    Vector(42.5, 30),
    Vector(42.5, -30),
    Vector(42.5, 34),
    Vector(42.5, -34),
    Vector(42.5, 10),
    Vector(42.5, -10),
    //Vector(30, 34),
    //Vector(30, 34),
    Vector(40, 34),
    Vector(40, 34),
    //Vector(10, 34),
    //Vector(10, 34), 
    Vector(20, 34),
    Vector(20, 34),
    Vector(0, 34),
    Vector(0, 34),
    Vector(42.5, 0)
};



Unum mates[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
Unum oppos[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
Vector  pt[] = {Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0), Vector(0,0)};

//_____________________________________________________________________________
//
//	Offense
//_____________________________________________________________________________

void offensive_positioning_tree()
{  

    if (Mem->HeardPassValid() && (Mem->GetHeardPasspartner() == Mem->MyNumber)) {  
   
	//MAKELOG((1, DBG_OFFENSE, " %d reacting to PASSMESSAGE to point (%d %d)", Mem->MyNumber, targetPos.x, targetPos.y));
	//cerr << "out_Ich (" << Mem->MyNumber << ") an Position (" << Mem->TeammateAbsolutePosition(Mem->GetHeardPasspartner()) << " höre PassPunkt (" << Mem->GetHeardPassTargetVector() << ")" << endl;
	orca_get_ball();	
	// SEA
	//orca_go_to_point(Mem->GetHeardPassTargetVector(), 0, 100);
    }
  
    else if (Mem->FastestTeammateToBall() == Mem->MyNumber) {
	orca_get_ball();
    }

    else {
	free_run();      
    }
}


//_____________________________________________________________________________
//
//	Ball Possession
//_____________________________________________________________________________

void play_with_ball_tree()
{
// Torschuss ?

    if (goal_kick()) {

	// cout << "Goal kick" << endl << endl;
	dribbling = false;
 
    }

    else if ((Mem->InOwnPenaltyArea() || (Mem->ClosestOpponentToBall() && !Mem->BallKickableForOpponent(Mem->ClosestOpponentToBall()))) &&
	     Mem->BallMoving() &&
	     !dribbling)   {
	// cout << "Stop ball" << endl << endl;
	orca_stop_ball();
    }     
/*

// temporary to test dribbling :)
else if ((Mem->NumTeammatesInCone(1.0, (Mem->MyPos()+(THEIR_GOAL_VECTOR-Mem->MyPos()).SetLength(12.0))) < 1)
&& (Mem->NumOpponentsInCone(1.0,(Mem->MyPos()+(THEIR_GOAL_VECTOR-Mem->MyPos()).SetLength(12.0))) < 1)) {

passpartner();   //SEA

orca_dribble();

dribbling=true;

cout << "Dribble" << endl;
cout << "x: " << Mem->MyX() << endl;
for(int i = 0; i < 15; ++i)
{
cout << pppIndex[i] << ": " << ppps[pppIndex[i]] << endl;
}
cout << endl;
}
*/
    else if (passpartner()) {

	orca_pass_ball(mates[0], pt[0]);
    
	dribbling = false;

/*    cout << "Pass" << endl;
      cout << "x: " << Mem->MyX() << endl;
      for(int i = 0; i < 15; ++i)
      {
      cout << pppIndex[i] << ": " << ppps[pppIndex[i]] << endl;
      }
      cout << endl;
*/  }

    else {

	int i;

	for (i = 0; i < count_best_line; i++) {

	    bool found = true;

	    for (int j = 1; j <= Mem->SP_team_size; j++) {

		if (InBetween(j, Mem->MyPos(), ppps[pppIndex[i]])) {

		    found = false; 

		    break;
		}
	    } 
      
	    if (found) {

		if ((Mem->numOpponentsInCircle(Mem->MyPos(), 2.0) < 1) && (Mem->MyX() < (52.5 - Mem->OP_max_goal_dist))) { 

		    //&& (Mem->NumOpponentsInCone(1.0,(Mem->MyPos()+(ppps[pppIndex[i]]-Mem->MyPos()).SetLength(12.0))) < 1
		    orca_dribble();
	    
		    dribbling = true;
	    
		    break;
		}
	  
		else  {
	   
		    kick_ball(PointInBetween(Mem->MyPos(), ppps[pppIndex[i]], (Mem->MyPos().dist(ppps[pppIndex[i]]))/2), KM_QuickestRelease, 1.0, TURN_NONE);
	  
		    dribbling = false;
	  
/*	 cout << "Clearance kick" << endl;
	 cout << "x: " << Mem->MyX() << endl;
         cout << ppps[pppIndex[i]] << endl;
    	 cout << endl;
*/	
		    break;
		}    
	    }
	}
    
	if (i == count_best_line) orca_hold_ball();
    }
}  



bool passpartner()
{ 
   
    if (Mem->BallX() < -20) {
    
	if (Mem->BallLocationSide() == 0) ppps = ppps_left;
    
	else ppps = ppps_right;
    }
  
    else ppps = ppp;
  
    direction();

    for (int i = 0; i < count_best_line; i++) {

	if (bestMates(pppIndex[i])) return true;
    }
  
    return false;
}



void direction()
{
    count_best_line = 1;
    int num_valid_pppoints = 0;
    int numOpposOnLine[num_pref_pass_pts]; 
  
    for (int i = 0; i < num_pref_pass_pts; i++) {
    
	if (Mem->MyX() > 25) 
	{
	    if(ppps[i].dist(THEIR_GOAL_VECTOR) < Mem->OP_max_goal_dist)
	    {
		pppIndex[num_valid_pppoints++] = i;
	    }
	}
	else
	{
	    pppIndex[num_valid_pppoints++] = i;
	}
      
    }

    for(int i = num_valid_pppoints; i < num_pref_pass_pts; i++)
    {
	pppIndex[i] = -1;
    }

    for(int i = 0; i < num_valid_pppoints; i++)
    {
	numOpposOnLine[i] = numOpponentsInPassway(ppps[i], 30.0, .0);

	//numOpposOnLine[i] = opponentsInCone(.6, ppps[i], Mem->MyPos());
    }

    Sort(num_valid_pppoints, pppIndex, numOpposOnLine);
  
    for (int i = 1; i < num_pref_pass_pts; i++) {
  
	if (numOpposOnLine[i] == numOpposOnLine[0])  count_best_line++;
    }
}



bool bestMates(int pppIndex)
{
    int numMates = numTeammatesInPassway(ppps[pppIndex], Mem->MyPos(), 20.0, .0, FALSE);
    //  int numMates = teammatesInCone(.6, ppps[pppIndex], Mem->MyPos(), FALSE);
 
    //cerr << "out_numMates: (" << numMates << ")" << endl;

    if (numMates != 0) {
    
	for (int i = 0; i < numMates; i++) {
      
	    pt[i] = getPassPosForMate(mates[i], ppps[pppIndex]);
	}
    
	if (bestPt(numMates)) return true;
    }

    return false;
}



bool bestPt(int numMates)
{
  
    float pt_weight[numMates];

    for (int i = 0; i < numMates; i++) {

	pt_weight[i] = 1000.0;
    }
     
    for (int i = 0; i < numMates; i++) {
    
	int numOppos = numOpponentsInPassway(ppps[i], 5.0, 2.0);

	if (Mem->MyPos().dist(pt[i]) < Mem->OP_min_pass_dist) continue;
    
	if (Mem->MyPos().dist(pt[i]) > Mem->OP_max_pass_dist) continue;
    
	if (numOppos > 0) {

	    //cerr << "numOppos_>0: " << numOppos << endl;
     
	    continue;
	}
 
	else {
	    //    if (opponentsInCone(.6, pt[i], Mem->MyPos()) > 0) continue; 
	    //cerr << "numOppos_=0: " << numOppos << endl;
      
	    pt_weight[i] = positionPassValue(pt[i]);
	}
    }   
  
    Sort1(numMates, pt, mates, pt_weight);

    if (pt_weight[0] != 1000.0) return true;

    return false;
}


         
Vector getPassPosForMate(Unum mate, Vector targetPos)
{
    Line line = LineFromTwoPoints(Mem->MyPos(), targetPos);
  
    return line.ProjectPoint(Mem->TeammateAbsolutePosition(mate));
}



void orca_stop_ball()
{
    //MAKELOG((1, DBG_OFFENSE, " %d stops ball", Mem->MyNumber));
    //cerr << "out_Ich (" << Mem->MyNumber << ") stoppe den Ball" << endl;
  
    //stop_ball();

    DoTurnKickCommand(dokick(0,0)); //  stop_ball();
}



void orca_hold_ball()
{
    //MAKELOG((1, DBG_OFFENSE, " %d holds ball", Mem->MyNumber));
    //cerr << "out_Ich (" << Mem->MyNumber << ") halte den Ball" << endl;
  
    hold_ball();
}

void orca_dribble()
{         

    //MAKELOG((1, DBG_OFFENSE, " %d dribbles ball", Mem->MyNumber));
    //cerr << "out_Ich (" << Mem->MyNumber << ") dribble den Ball" << endl; 
    if (Mem->numOpponentsInCircle(Mem->MyPos(), 10.0) < 0) scan_field_with_neck();


    float target_dir = (ppps[pppIndex[0]]-Mem->MyPos()).dir();
  
    kick_ball(ppps[pppIndex[0]], KM_QuickestRelease, Mem->SP_ball_speed_max * 0.2, RotToAvoidOpponent(target_dir + Mem->MyBodyAng()));

    //kick_ball(Vector(Mem->MyX() + 5.0, Mem->MyY()), KM_QuickestRelease, Mem->SP_ball_speed_max * 0.2, TURN_NONE);
}

void orca_pass_ball(Unum mate)
{
    orca_pass_ball(mate, getPassPosForMate(mate, mate_ppps[mate]));
}

void orca_pass_ball(Unum mate, Vector pos)
{
    //MAKELOG((1, DBG_OFFENSE, " %d passes to mates %d and point ( %d %d )", Mem->MyNumber, mate, pos.x, pos.y));
    //cerr << "out_Ich (" << Mem->MyNumber << ") passe zu (" << mate << ") an Punkt (" << pos << ")" << endl;
    // SEA
    Mem->SayPass(mate, pos, Mem->CurrentTime);       /////////////////////////////////////

    //KickMode mode = Mem->BestKickMode(Mem->AngleToFromBody(pt[0]));    
  
    ///kick_ball(pos, KM_QuickestRelease, Mem->SP_ball_speed_max * Mem->MyPos().dist(pos)/2, TURN_NONE); 
    kick_ball(pos, KM_QuickestRelease, Mem->SP_ball_speed_max, TURN_NONE); 
}



// Torschuss

bool goal_kick()
{
    bool kicked = false;
    Vector point_on_line;
    
    int num_points_per_side = Mem->OP_goal_kick_num_points_per_side;
    
    float post_offset = Mem->OP_goal_kick_post_offset;
    
    if (Mem->DistanceTo(THEIR_GOAL_VECTOR) < Mem->OP_max_goal_dist) {

       	for(int i = num_points_per_side; i >= 0; --i)  { //  ..., 3, 2, 1, 0
       	
	    if (kicked) break;
  			
	    for (int side = -1; side <= 1; side += 2)  { // -1, 1
       		
		if (i == 0 && side == 1)  break;   // check center of goal only once
       			
		point_on_line = THEIR_GOAL_VECTOR;
       			
		point_on_line.y = side * i * ((Mem->SP_goal_width/2 - post_offset) / num_points_per_side);
			
		if (Mem->NumOpponentsInCone(Mem->OP_cone_width_dist_ratio, point_on_line) == 0 || dribbling) {

		    kick_ball(point_on_line, KM_QuickestRelease, Mem->SP_ball_speed_max*2, TURN_NONE);

		    kicked = true;

		    break;
		}
	    }
       	}
    }

    return kicked;
}



float positionPassValue(Vector pos)
{

    float value;
 
    if(Mem->OP_trouble_value_weight > 0)
    {
	value = Mem->OP_trouble_value_weight * troubleValue(pos, Mem->OP_trouble_radius);
    }
    if(Mem->OP_position_value_weight > 0)
    {
	value -= Mem->OP_position_value_weight * positionValue(pos);
    }
    if(Mem->OP_goal_value_weight > 0)
    {
	value += Mem->OP_goal_value_weight * (pos.dist(THEIR_GOAL_VECTOR) / Mem->SP_pitch_length);
    }
  
    return value;
}

// wie frei steht ein Spieler meines Teams?

float troubleValue(Unum mate_unum, float radius)
{
    return troubleValue(Mem->TeammateAbsolutePosition(mate_unum), radius);
}


float troubleValue(Vector pos, float radius)
{
    float trouble = 0;

    Vector opp_pos;
    float mate_opp_dist;

    for(Unum opp_unum = 1; opp_unum <= Mem->SP_team_size; ++opp_unum)
    {
	if (!Mem->OpponentPositionValid(opp_unum))
	{
	    continue;
	}
	else
	{
	    opp_pos = Mem->OpponentAbsolutePosition(opp_unum);
	    mate_opp_dist = pos.dist(opp_pos);

	    if(mate_opp_dist < radius)
	    {
		trouble += pow(radius - mate_opp_dist, 2);
	    }
	}
    }

    return trouble;
}

float positionValue(Unum mate)
{
    return positionValue(Mem->TeammateAbsolutePosition(mate));
}


float positionValue(Vector pos)
{
    float x = pos.x / (Mem->SP_pitch_length / 2.0);

    float y = fabs(pos.y) / (Mem->SP_pitch_width / 2.0);

    //if(Mem->TeammateLocationSide(mate) != Mem->MyLocationSide()) { y *= -1.0; }

    float a = Mem->OP_position_value_x_weight;
    float b = Mem->OP_position_value_y_weight;

    return (a * x - b * x * y);

}


int teammatesInCone(float wid_dist_ratio, Vector end, Vector vert, Bool IncludeMe)
{
  
    int num = 0;

    Line l = LineFromTwoPoints(end, vert);
  
    for (Unum mate = 1; mate <= Mem->SP_team_size; mate++) {
    
	if (!IncludeMe && mate == Mem->MyNumber) continue;
    
	if (!Mem->TeammatePositionValid(mate)) continue;
       
	Vector pt = l.ProjectPoint(Mem->TeammateAbsolutePosition(mate));
    
	if (pt.dist(Mem->TeammateAbsolutePosition(mate)) < pt.dist(vert)*wid_dist_ratio*wid_dist_ratio && l.InBetween(pt, vert, end)) {
      
	    mates[num++] = mate;
	}    
    }

    return num;
}



int opponentsInCone(float wid_dist_ratio, Vector end, Vector vert)
{
  
    int num = 0;
  
    Line l = LineFromTwoPoints(end, vert);
  
    for (Unum opp = 1; opp <= Mem->SP_team_size; opp++) {

	if (!Mem->OpponentPositionValid(opp)) continue;
    
	Vector pt = l.ProjectPoint(Mem->OpponentAbsolutePosition(opp));
    
	if (pt.dist(Mem->OpponentAbsolutePosition(opp)) < pt.dist(vert)*wid_dist_ratio*wid_dist_ratio && l.InBetween(pt, vert, end)) {
      
	    oppos[num++] = opp;
	}    
    }
  
    //cerr << "out_numOppos (" << num << ")" << endl;
  
    return num;
}



void Sort(int length, int *elements, int *keys)
{  
  
    int keytemp;
  
    int eltemp;

    for (int i = 0; i < length; i++){
    
	for (int j = i+1; j < length; j++){
      
	    if ( keys[j] < keys[i] ){
	
		keytemp = keys[i];

		keys[i] = keys[j];
	
		keys[j] = keytemp;
	
		eltemp = elements[i];

		elements[i] = elements[j];
	
		elements[j] = eltemp;
	    }
	}
    }
}


void Sort1(int length, Vector *elements, Unum *mates, float *keys)
{  
  
    float keytemp;
  
    Vector eltemp;

    int matetemp;

    for (int i = 0; i < length; i++){
    
	for (int j = i+1; j < length; j++){
      
	    if ( keys[j] < keys[i] ){
	
		keytemp = keys[i];

		keys[i] = keys[j];
	
		keys[j] = keytemp;
	
		eltemp = elements[i];

		elements[i] = elements[j];
	
		elements[j] = eltemp;

		matetemp = mates[i];

		mates[i] = mates[j];
	
		mates[j] = matetemp;

	    }
	}
    }
}

// wieviele Gegner koennten den Pass abfangen?
 
int numOpponentsInPassway(Unum mate_unum, float angle, float extra_radius_ratio){
    return numOpponentsInPassway(Mem->TeammateAbsolutePosition(mate_unum),
                                 (Mem->BallPositionValid() ? Mem->BallAbsolutePosition() : Mem->MyPos()),
                                 angle, extra_radius_ratio);
}
 
int numOpponentsInPassway(Vector point, float angle, float extra_radius_ratio)
{
    return numOpponentsInPassway(point,
                                 Mem->MyPos(),
                                 angle, extra_radius_ratio);
}


int numOpponentsInPassway(Vector target_pos, Vector start_pos, float angle, float extra_radius_ratio)
{
    int num_opps = 0;
 
    ////cerr << Mem->MyNumber << " " << Mem->CurrentTime.t << " opps " << ": ";
    for(Unum opp_unum = 1; opp_unum <= Mem->SP_team_size; ++opp_unum) {
 
        ////cerr << " " << opp_unum;
 
        if (!Mem->OpponentPositionValid(opp_unum))
        {
            ////cerr << "? ";
            continue;
        }
 
        if(opponentInPassway(opp_unum,
			     start_pos,
			     target_pos,
			     angle,
			     extra_radius_ratio))
        {
            oppos[num_opps++] = opp_unum;
            ///num_opps++;
        }
    }
 
    ////cerr << endl;
    return num_opps;
}


int numTeammatesInPassway(Vector target_pos, Vector start_pos, float angle, float extra_radius_ratio, Bool IncludeMe)
{
    int num_mates = 0;
 
    for(Unum mate_unum = 1; mate_unum <= Mem->SP_team_size; ++mate_unum) {
 
	if (!IncludeMe && mate_unum == Mem->MyNumber) continue;

	if (!Mem->TeammatePositionValid(mate_unum)) continue;
 
        if(mateInPassway(mate_unum, start_pos, target_pos, angle, extra_radius_ratio))
        {
            mates[num_mates++] = mate_unum;
            //num_mates++;
        }
    }
 
    return num_mates;
}
 
bool opponentInPassway(Unum unum, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio)
{
    return playerInPassway(Mem->TheirSide, unum,
			   start_pos, target_pos, pass_angle, extra_radius_ratio);
}
 
bool mateInPassway(Unum unum, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio)
{
    return playerInPassway(Mem->MySide, unum,
			   start_pos, target_pos, pass_angle, extra_radius_ratio);
}
 
bool playerInPassway(char side, Unum unum, Vector start_pos, Vector target_pos,
		     float pass_angle, float extra_radius_ratio)
{
    return posInPassway(Mem->PlayerAbsolutePosition(side, unum),
                        start_pos, target_pos, pass_angle, extra_radius_ratio);
}

bool posInPassway(Vector pos, Vector start_pos, Vector target_pos, float pass_angle, float extra_radius_ratio)
{
    float target_angle = GetNormalizeAngleDeg((target_pos - start_pos).dir());
    float pos_angle = GetNormalizeAngleDeg((pos - start_pos).dir());
    float dist = start_pos.dist(pos);
    float radius = (1 + extra_radius_ratio) * start_pos.dist(target_pos);
 
 
    float pos_target_angle = fabs(GetNormalizeAngleDeg(target_angle - pos_angle));
 
    //cerr << "\t" << pos_target_angle << " < " << pass_angle;
 
    if(dist < radius && pos_target_angle < pass_angle) {
        ////cerr << "+ ";
        return true;
    }
 
    ////cerr << "- ";
    return false;
}
 
void free_run()
{
    free_run(Mem->currentAdjustedPosition->Home, Mem->currentAdjustedPosition->HomeRange / 2);
}
 
 
void free_run(Vector home_pos, float range)
{
    orca_go_to_point(getFreeRunPos(home_pos, range), .5, 80);
}

 
Vector getFreeRunPos(Vector home_pos, float range)
{
    Vector adjustedHome = home_pos;
    float homeRange = range;
    Vector ball_pos;
    if(Mem->BallPositionValid())
    {
        ball_pos = Mem->BallAbsolutePosition();
    }
    else
    {
        ball_pos = Vector(0,0);
    }
 
    Vector positions [8];
 
 
    positions[0] = Vector(adjustedHome.x + homeRange, adjustedHome.y);
    positions[1] = PointInBetween(adjustedHome, ball_pos, homeRange);
    positions[2] = PointInBetween(adjustedHome, THEIR_GOAL_VECTOR, homeRange);
    positions[3] = adjustedHome;
    positions[4] = Mem->MyPos();
    positions[5] = Vector(Mem->MyPos().x + 1.0, Mem->MyPos().y);
    positions[6] = PointInBetween(Mem->MyPos(), ball_pos, 1.0);
    positions[7] = PointInBetween(Mem->MyPos(), THEIR_GOAL_VECTOR, 1.0);
 
    int bestIndex = 0;
    float bestValue = 100000.0;
    float tempValue = 100000.0;
 
    int numOppsInPassway = 12;
    int minOppsInPassway = 12;
 
    for(int i = 0; i < 8; i++)
    {
        if(adjustedHome.dist(positions[i]) > homeRange)
        {
            positions[i] = PointInBetween(adjustedHome, positions[i], homeRange);
        }
 
        if((numOppsInPassway =
            numOpponentsInPassway(positions[i], ball_pos,
                                  Mem->OP_passway_angle, Mem->OP_passway_buffer))
           <= minOppsInPassway)
        {
            if((tempValue = positionPassValue(positions[i])) < bestValue)
            {
                bestIndex = i;
                bestValue = tempValue;
                minOppsInPassway = numOppsInPassway;
            }
        }
    }
 
    return positions[bestIndex];
}

//===============================================
// Unum bestPasspartner(Region region = NULL)
//===============================================
Unum bestPasspartner(Region* region = NULL)
{
    
    int pppis[Mem->pppoints->num_points()];
    
    //cerr << "NumValidPoints: " << Mem->pppoints->num_points() << endl;


    for (int i=0; i < Mem->pppoints->num_points(); i++) { pppis[i] = i; }
    
    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << "before RegionCheck" << endl;
    if(pppRegionCheck(pppis, region) <= 0) return Unum_Unknown;
    
    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << "before oppCheck" << endl;
    if(pppOppCheck(pppis) <= 0) return Unum_Unknown;
    
    Unum bestMate;
    
    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << "before bestMate" << endl;
    bestMate = pppBestMate(pppis);
    
    
    return bestMate;
}

int pppRegionCheck(int* pppis, Region* region)
{

    if(region == NULL) return Mem->pppoints->num_points();

    int num_valid_pts = 0;
    int i;
    for (i=0; i < Mem->pppoints->num_points(); i++)
    {
	if(pppis[i] < 0) break;

	if(Mem->checkWithin(*Mem->pppoints->get_point(pppis[i]), region,NULL,NULL,true))
	{
	    pppis[num_valid_pts++] = pppis[i];
	}
    }

    for(int j=num_valid_pts; j < Mem->pppoints->num_points(); j++) pppis[j] = -1;
	
    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << "in RegionCheck " << num_valid_pts << endl;

    return num_valid_pts;
}

int pppOppCheck(int* pppis)
{
    int num_valid_pts = 0;
    int min_opps = Mem->SP_team_size + 1;
    int i;
    for (i=0; i < Mem->pppoints->num_points(); i++)
    {
	if(pppis[i] < 0) break;

	int num_opps =
	    numOpponentsInPassway(
		*Mem->pppoints->get_point(pppis[i]), 
		Mem->OP_passway_angle * 1.4,
		Mem->OP_passway_buffer);

	if(num_opps < min_opps) num_valid_pts = 0;

	if(num_opps <= min_opps)
	{
	    pppis[num_valid_pts++] = pppis[i];
	}
    }

    for(int j=num_valid_pts; j < Mem->pppoints->num_points(); j++) pppis[j] = -1;

    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << "in oppCheck " << num_valid_pts << endl;
    return num_valid_pts;
}

Unum pppBestMate(int* pppis)
{
    Unum bestMate = Unum_Unknown;

    int i;
    ////float bestMateValue = positionPassValue(Mem->MyPos());
    float bestMateValue = -100000000;
    float tempMateValue;


    for (int mate_unum = 2; mate_unum <= Mem->SP_team_size; mate_unum++)
    {
	if (mate_unum == Mem->MyNumber) continue;

	for (i=0; i < Mem->pppoints->num_points(); i++)
	{
	    if (pppis[i] < 0) break;

	    if (! pppMateEligibleForPass(mate_unum, *Mem->pppoints->get_point(pppis[i]))) continue;

	    Vector passPos = getPassPosForMate(mate_unum, *Mem->pppoints->get_point(pppis[i]));

	    //if (passPos == NULL) continue;

	    if (Mem->DistanceTo(passPos) > Mem->OP_max_pass_dist) continue;

	    if (Mem->DistanceTo(passPos) < Mem->OP_min_pass_dist) continue;

	    if (numOpponentsInPassway(
		    passPos,
		    Mem->OP_passway_angle,
		    Mem->OP_passway_buffer) > 0) continue;

	    tempMateValue = positionPassValue(passPos);

	    if(tempMateValue > bestMateValue)
	    {
		bestMate = mate_unum;
		bestMateValue = tempMateValue;
		mate_ppps[mate_unum] = pppis[i];
	    }
	}
    }

    //cerr << "At time " << Mem->CurrentTime.t << " ";
    //cerr << Mem->MyNumber << " found " << bestMate << endl;

    return bestMate;
//	return 10;
}

bool pppMateEligibleForPass(Unum mate, Vector target_pos)
{
    if(!Mem->TeammatePositionValid(mate)) return false;

    return posInPassway(Mem->TeammateAbsolutePosition(mate),
			Mem->MyPos(),
			target_pos,
			Mem->OP_passway_angle, Mem->OP_passway_buffer);

}



