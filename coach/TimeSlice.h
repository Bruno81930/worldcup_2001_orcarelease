#ifndef TIMESLICE_H_
#define TIMESLICE_H_

#include <vector.h>
#include "types.h"
#include "geometry.h"
#include <stdio.h>
#include <string>
//using namespace std;

// for regional-features
#define NUMBER_OF_REGIONS 3
#define REGION_SIZE 10
#define REGIONAL_FEATURES (NUMBER_OF_REGIONS * NUMBER_OF_REGIONS * 2 + 6)

//Classes to put players in:
#define OFFENSE_CLASS 1
#define DEFENSE_CLASS 2
#define MIDDLEFIELD_CLASS 3
#define GOALIE_CLASS 4

// Modes for positions-analysis (getLattice etc.)
#define ALL 0
#define WE_HAVE_BALL 1
#define OPP_HAS_BALL 2
#define PLAYER_HAS_BALL 3

//Foil-Modes
#define MODE_POSITIVE 1
#define MODE_NEGATIVE 2
#define MODE_TEST 3

//dump()-Colors
#define COLOR_YELLOW 1
#define COLOR_ORANGE 2

enum ActionPrimitiv{
	none,
	dash,
	kick
};


enum Eventtype{
	noevent,
	passevent,
	dribbleevent,
	shootevent
};


typedef struct{
	ActionPrimitiv action;	// dash or kick
	double power;	
	double direction;	// in case of dash set to zero
} Action;



typedef struct{
	global_pos 	pos;
	int 		side;	// LEFT, NEUTRAL, RIGHT
	double 		bodyangle;
	double 		neckangle;
	double 		distance;// distance to ball
	double 		ballangle; // angle to ball
	global_pos	velocity;
	short		mode;		// bits like KICK, KICK_FAULT etc.
	Action		playeraction;
	bool		isGoalie;	// is this the goalie?
	//Action * getAction();		// implementation in TimeSlice.C
	//global_pos * getPosition();	// dito
} playerinfo_t;


typedef struct{
	global_pos	pos;
	global_pos	velocity;
} ballinfo_t;



class TimeSlice{

    // Features:
    global_pos ballPos;		   // global position of ball
    short possessingTeam;          // LEFT, NEUTRAL, RIGHT
    playerinfo_t player[8];         // 8 nearest players to ball 
    short timeTillPass;            // how many cycles until pass happens? max. 40
    short timeTillGoalkick;    
    short timeTillLosingBall;      // how many cycles until possession of ball changes?
    // add more features here

    const string * nameLeft;	   // Pointers to the teamnames
    const string * nameRight;
	    
    // intern variables that will not be saved into the database and will not be clustered
    TimeSlice *predecessor;     // link backwards, to analyze passes etc.
    ballinfo_t	 ballinfo;	// pos and vel of ball
    playerinfo_t *playerinfo;// positions, directions, actions and velocities of players
    				// note: difference to player[]: distance and angle to
				// ball are not calculated.
    short servertime;		// time of server, useful to sync when watching logplayer
    short kickhappened;		// necessary for logfiles that don't contain messages.
    				// this will be checked by successor, so it can analyze
				// kicks. 0: no kick. 1 left team, 2 right team, 3 both
    short whichSidePassed;	// LEFT, RIGHT
    short passShooterUnum;	// this is defined only if timeTillPass=0
    short passReceiverUnum;	// this is defined only if timeTillPass=0
    int playmode;		// enum _PlayMode of types.h
    bool alreadyAnalyzed;	// has onlinecoach analyzed this slice yet?
    int possessingPlayer;	// which player possesses the ball? index: 0..21
    int dribblingPlayerIndex;	// -1 if there was no dribble-event
    short playerAttributes[22]; // for dump(), to mark players you are interested in

public:
    //Constructor:
    TimeSlice(playerinfo_t *playerinfo, ballinfo_t *ballinfo, short s_time,
		TimeSlice *predecessor, int v, bool messagesInLogfile, int logversion,
		const string* nameLeftarg, const string * nameRightarg, int pmode);
    TimeSlice(void);
    void processAnonymousKick(void);
    // functions to get feature values:
    global_pos getBallPos(void) { return ballPos; };
    global_pos getBallVelocity(void) { return ballinfo.velocity; };
    global_pos getPlayerPos(short side, short unum);
    short getPossessingTeam(void) { return possessingTeam; };
    short getPossessingPlayer(void) { return possessingPlayer; };	// 0..21
    double* getNearestPlayerTo(short, bool);
    playerinfo_t * getNearest(void) { return player; };
    short getTimeTillPass(void) { return timeTillPass; };
    short getTimeTillGoalkick(void) { return timeTillGoalkick; };
    short getTimeTillLosingBall(void) { return timeTillLosingBall; };
    short getServerTime(void) { return servertime; };
    global_pos calculateAveragePosition(short, short, short);
    short classifyPlayer(global_pos, short, short);
    short* classifyPlayersRelative(global_pos[11]);
    short findGoalie(global_pos[11], short);
    string classToString(short);
    // auxiliary functions:
    TimeSlice* getPredecessor(void) { return predecessor; };
    playerinfo_t * getPlayers(void) { return playerinfo; };
    //short[NUMBER_OF_REGIONS][NUMBER_OF_REGIONS][2] getRegions(void) { return regions; };
    short regions[NUMBER_OF_REGIONS][NUMBER_OF_REGIONS][2];
    void scaleAll1000(void);
    void undoScale(void);
    void saveSliceToFile(FILE * outfp);
    void loadSliceFromFile(FILE * file);
    void loadRegionalSliceFromFile(FILE*);
    void computeGlobalPlayerPos(void);
    double differenceBetweenSlice(TimeSlice * otherslice);
    void saveC5PassData(bool side, FILE * outfp);
    bool ballPossessedByRightTeam();
    bool ballPossessedByLeftTeam();
    bool ballInRightHalf();
    bool ballInLeftHalf();
    int getPlayMode();
    bool getAlreadyAnalyzed();
    void setAlreadyAnalyzed(bool yesno);
    void saveRegionalSliceToFile(FILE*, bool);
    void createRegions(void);
    void scaleAllRegional1000(void);
    double differenceBetweenRegionalSlice(TimeSlice * otherslice);
    void undoRegionalScale(void);
    void saveC5RegionalPassData(bool side, FILE * outfp);
    RectGrid * getLatticeForPlayer(short unum, short side, short timeframe, short mode);
    regionOfSections* getDefenseOffenseRegion(short unum,short side, short timeframe,
		short mode);
    Teampositions* getTeampositions(short side, short timeframe, short mode);
    bool isGoalie(short index, short side);
    bool goalieHasBall(short side);
    short getGoalie(short side);
    string toFoil(void);
    string foilString(TimeSlice * recent, double angle, short side);
    void analyzePassStructure(int timeframe, short side);// 2do this will change from void to something
    vector<string> foilPassDribbleShoot(int timeframe, Eventtype event, short side, short mode,
    					double angle);
   void getTeamPlayerMeanAndDeviationXY(int timeframe, short side, int unum,
    global_pos & mean, global_pos & deviation,global_pos & bmean, global_pos & bdeviation);
   double getPMKOfBallAndPlayer(int timeframe, short side, int unum, bool xcoord);
   void meanAndDeviationOfPMKOfBallAndPlayers(int timeframe, short side,
			bool xcoord, double &pmkmean, double &pmkvar);
   void dump(string);
   void setPlayerAttribute(short player, short attr) { playerAttributes[player] = attr; };
   short findClosestPartner(short unum, short side);
   short findBestPassPartner(short unum, short side);
   bool teamsKickMode(short side);
   short* getPlayersAroundBall(double radius, int side);

   
protected:
    // all functions operate on playerinfo[] and ballinfo
    string itoa(int);
    string itoa(size_t);
    string itoa(double);
    string dtoa(double);
    void create_ballPos(void);
    int create_possessingTeam(void);
    void create_players(void);
    void create_timeTillPass(int logversion);
    void create_timeTillPass2(void);
    	// check whether pass happens and propagate back until approaching value != -1
    void create_timeTillGoalkick(int logversion);
    void create_timeTillLosingBall(void);
    bool reachGoal(Action action, int playerindex, int logversion);
    double calculate_shootdist(Action action, int playerindex,
                          double x, double expected_y, double &kickpower, int logversion);
    bool willBallTravelThatFar(double shootdist, double velocity);
    void analyzePreviousKick(void);
    void calculate_ballvector(double &velocity, double &direction);
    void set_kickhappened(void);
    void findDribble(void);
    string generatePositiveFoilString(TimeSlice* recent, Eventtype event, double angle, short side);
    string generateNegativeFoilString(TimeSlice* recent, Eventtype event, double angle, short side);

    
};


#endif	


