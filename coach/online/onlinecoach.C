
#include "communication.h"
#include "../types.h"
#include "onlinecoach.h"
#include "parametervalues.h"
#include "../TimeSlice.h"
#include "analyzestats.h"
#include <stdlib.h>
#include <iostream.h>
/* #include <GetOpt.h> */
#include <string>
#include <vector.h>
#include "sstream"
#include <assert.h>


typedef struct {
  int ballInOwnHalf;
  int goals;
  int goalieHasBall;
  int teamHasBall;
} TrainerInfo;


Socket m_socket;		// socket to server
volatile char sendbuf[MAXLENGTH];	// buffer for coach-messages
volatile char receivebuf[MAXLENGTH];	// buffer for messages from the server
int verbose=0;			// print infos?
int side;			// side on which coach's team plays on
bool lastCommandSucceeded=false;// did we get an "(ok...)" message for the last command?
playerinfo_t * playerinfo;	// holds info that will be passed to TimeSlice
				// our players are in [0..10], others in [11..21]
ballinfo_t * ballinfo;		// dito
int servertime;		// timestamp of server messages
short playmode;		// which Playmode are we in? enum _PlayMode of types.h	
string teamname;		// name of the coach's team
string opponentname="opponent";	// 2do if we use a database, we need the opponent's name
vector<TimeSlice*> slices;	// each cycle is represented as a TimeSlice in this vector

int setplayCounter = 0; //how long are we in a setplay configuration
int lastPlaymode = -1; //the playmode of last cycle

//<Trainer-Variables>
TrainerInfo left = {0, 0, 0, 0};
TrainerInfo right = {0, 0, 0, 0};
bool halftime = false;
bool extended = false;
bool gameover = false;
bool trainer = false;
vector<double> ballXHistory;
//</Trainer-Variables>


// Objects that contain parameter values:
Server_param * Serverparam;
Player_param * Playerparam;
Player_type * Playertype[7];

// some function declarations:
// CM_TokRule * sendTestMessage(short &lastAdvice);
void markOlinePositioning(short &lastAdvice);
CM_TokRule * buildMarkToken(vector<int> offenders, vector<int> marking);
bool sendCM_Message(CM_Message* message);
vector<CM_TokRule *> *advicePositionToken(bool mode, bool includeMarking);
CM_TokRule * buildoffsidetoken(void);
CM_PlayModeType whichPlaymode(TimeSlice * slice);
vector<CM_TokRule *> * findSetPlay(void);
CM_TokRule * getConditionalOpponentPositions(bool mode);
bool assignKickPlayer(short unum);
void checkForSetplay(void);

void usage(){
  cerr << "Usage: onlinecoach"
    "-v		verbose"
    "-h hostname	host the server runs on"
    "-p port	port of the socket to server"
    "-t teamname	name of coach's team" 
    "-T             this is a trainer (not a coach)" << endl;
  exit(1);
}


int main(int argc, char ** argv){

  //GetOpt getopt(argc, argv, "vh:p:t:T");
  	int option_char;
  	int port=6002;			// port of socket to server
  	string hostname="localhost";		// host the server runs on

	short lastDefine=0; // when did we send last messages? This should be...
	short lastAdvice=0; // ...-Server_param.clang_win_size actually.
	short lastInfo=15;	//0;  // small offset to advices to reduce computation load
	short lastMeta=0; 
	static bool COPSoffense=true;	// send offensive and defensive COPS turn by turn

	/*
  	while ((option_char = getopt()) != EOF) {
    		switch (option_char) {
		case 'v': verbose = 1; break;
		case 'h': hostname = getopt.optarg; break;
		case 'p': port = atoi(getopt.optarg); break;
		case 't': teamname=getopt.optarg; break;
		case 'T': trainer = true; break;
		default: usage();
    		}
  	}
	*/
	for (int i = 1; i < argc; i++) {
	  if (strcmp(argv[i], "-v") == 0) {
	    verbose = 1;
	    continue;
	  }
	  if (strcmp(argv[i], "-h") == 0) {
	    hostname = argv[i + 1];
	    i++;
	    continue;
	  }
	  if (strcmp(argv[i], "-p") == 0) {
	    port = atoi(argv[i + 1]);
	    i++;
	    continue;
	  }
	  if (strcmp(argv[i], "-t") == 0) {
	    teamname = argv[i + 1];
	    i++;
	    continue;
	  }
	  if (strcmp(argv[i], "-T") == 0) {
	    trainer = true;
	    continue;
	  }
	  usage();
	}

	m_socket=open_connection(const_cast<char *>(hostname.c_str()), port);
	if(m_socket.socketfd==-1){
		cerr << "Couldn't open socket." << endl;
		exit(1);
	}


	if (!trainer) {
	  if(!init_to_server(const_cast<char *>(teamname.c_str()))){
	    cerr << "Can't send  init to server." << endl;
	    exit(1);
	  }
	  
	  if(verbose)
	    cout << "Sent init to server." << endl;
	  

	  // wait
	  if (wait_message() == 0){
	    cerr << "Wait failed." << endl;
	    exit(1);
	  }
	  
	  // parse init-return from server
	  if (!(side=parse_init_return())) {
	    cerr << "Could not parse return of init message" << endl;
	    exit(1);
	  }
	  
	  if(verbose){
	    cout << "Server acknowledged init-message." << endl;
	    cout << "We are on side " << (side==LEFT?"l":"r") << endl;
	  }
	}
  	sigset_t sigfullmask = init_handler();
	
	Eye(true);	// server will send us visual info periodically
	if (trainer) {
	  Ear(true);
	}
	
	
	vector<CM_Token*> infoTokensToBeSent;
	//vector<CM_TokRule*> adviceTokensToBeSent;
	int i = 0;
	while (wait_for_signals(&sigfullmask)){	 // loop getting info
	  if (trainer) {
	    if (halftime) {
	      if (verbose) cout << "Halftime: starting again..." << endl;
	      sprintf(const_cast<char *>(sendbuf), "(start)");
	      send_message();
	      halftime = false;
	    }
	    if (extended) {
	      if (verbose) cout << "Extended: stoping game..." << endl;
	      gameover = true;
	      extended = false;
	      /*
	      sprintf(const_cast<char *>(sendbuf), "(start)");
	      send_message();
	      extended = false;
	      */
	    }
	    if (gameover) {
	      if(m_socket.socketfd != -1) closesocket(m_socket.socketfd);
	      report();
	      return 0;
	    }
	    continue;

	  }
		
		// checkForSetplay();
		
		// analyzeSlices(slices);	// count cycles of ballpossession etc.
		
		// we can send clang-messages if we waited enoughed cycles
		// we start to analyze when we can send messages again
		if(servertime-lastInfo>Serverparam->clang_win_size){		
			lastInfo=servertime;
			infoTokensToBeSent.clear();	// remove previous tokens
			
			/*
			// Test heterogenous player-message:
			CM_TokRule * heterotoken=sendTestMessage(lastInfo);
			if(heterotoken)
				infoTokensToBeSent.push_back(heterotoken);
			*/

			// opponent positioning in defense and offense:
			// (aka Conditional Opponent Positioning Scheme: COPS :-))
			CM_Token * COPStoken=NULL;
			if(COPSoffense)
				COPStoken=getConditionalOpponentPositions(true);
			else
				COPStoken=getConditionalOpponentPositions(false);
			COPSoffense=!COPSoffense;
			if(COPStoken!=NULL)
				infoTokensToBeSent.push_back(COPStoken);
			
			// setplay tokens
			vector<CM_TokRule*> * setplaytokens=findSetPlay();
			cout << "bla: setplaytokens->size(): " <<setplaytokens->size()<<endl;
			for(int i=0; i<setplaytokens->size(); i++){
				infoTokensToBeSent.push_back((*setplaytokens)[i]);
				(*setplaytokens)[i]->PrintPretty(cout, "bla: setplay: ");
			}
			
			// just fill the tokens in the message:
			if(infoTokensToBeSent.size()>0){
				CM_InfoMessage *info=new CM_InfoMessage(infoTokensToBeSent);
				// now print it:
				if(verbose){
					cout << "#########################" << endl;
					info->Print(cout);
					cout << "#########################" << endl;
				}
				// now send it:
				sendCM_Message(info);
			}
		}
		if(servertime-lastAdvice>Serverparam->clang_win_size){		
			markOlinePositioning(lastAdvice);
		}

		
	};

  	if(m_socket.socketfd != -1) closesocket(m_socket.socketfd);

  	if(verbose)
		cout << "Coach finished." << endl;
	return 0;
}

void report(void) {
  cerr << "l," << left.goals << "," 
       << left.ballInOwnHalf << "," 
       << left.goalieHasBall << ","
       << left.teamHasBall <<endl;
  cerr << "r," << right.goals << "," 
       << right.ballInOwnHalf << "," 
       << right.goalieHasBall << "," 
       << right.teamHasBall << endl;

  cerr << "x,";
  for (int i = 0; i < ballXHistory.size(); i++) {
    cerr << ballXHistory[i];
    if (i < (ballXHistory.size() - 1)) {
      cerr << ",";
    }
  }
  cerr << endl;
}

bool playmodeIsRelevantForSetplay(int playmode) {
/*
  if (playmode == PM_FreeKick_Left ||
      playmode == PM_FreeKick_Right ||
      playmode == PM_KickIn_Left ||
      playmode == PM_KickIn_Right) {
    return true;
  }
  else {
    return false;
  }
*/
	if(slices.size()==0) return false;
	return slices[slices.size()-1]->teamsKickMode(side);

}

#define NEAR_BALL_RADIUS 3.0
#define SETPLAY_THRESHOLD 40
void checkForSetplay(void) {
  if (lastPlaymode == -1) {
    //do nothing, for this is the first cycle
  }
  else if (lastPlaymode == PM_PlayOn &&
	   playmodeIsRelevantForSetplay(playmode)) { //a setplay situation just started
    setplayCounter = 0;
  }
  else if (playmodeIsRelevantForSetplay(lastPlaymode) &&
	   lastPlaymode == playmode) { //we're in the same situation as last cycle
    setplayCounter++;
    TimeSlice* lastSlice = slices[slices.size() - 1];
    short* playersAroundBall = lastSlice->getPlayersAroundBall(NEAR_BALL_RADIUS, side);
    if (setplayCounter > SETPLAY_THRESHOLD &&
	playersAroundBall == NULL) {
      playerinfo_t* players = lastSlice->getPlayers();
      double smallestDistance = 1000.0;
      short bestPlayer = -1;
      int start = side == LEFT ? 0 : 11;
      int end = side == LEFT ? 10 : 21;
      for (int i = start; i <= end; i++) {
	if (distanceBetweenTwoPositions(players[i].pos, lastSlice->getBallPos()) < smallestDistance) {
	  smallestDistance = distanceBetweenTwoPositions(players[i].pos, lastSlice->getBallPos());
	  bestPlayer = i;
	}
      }
      int unum = side == LEFT ? bestPlayer + 1 : bestPlayer - 9;
      assignKickPlayer(unum);
    }
	
  }

  lastPlaymode = playmode;
}



/** This function identifies their forwards and assigns to each forward
  * one of our players depending on their homeregion similiarity.
  */
CM_Token * assignMarking(){
	// get classes of opponent players:
	global_pos averagePositions[11];
	vector<int> offenders;			// will contain indizes of offenders
	short *classes;				// class of each opponent player
	for (short k = 0; k < 11; k++) {	// all opponent players
	  averagePositions[k] =
	      slices[slices.size()-1]->calculateAveragePosition(k + 1, -side, 300);
	      		// note: opponent's side is negative to our side: -side
	
	  //if(slices[slices.size()-1]->classifyPlayer(averagePositions[k], k, -side)==
	  //		OFFENSE_CLASS){
	  //		offenders.push_back(k);
	  //}
	}
	classes=slices[slices.size()-1]->classifyPlayersRelative(averagePositions);
	for(short k=0; k<11; k++){		
		if(classes[k]==OFFENSE_CLASS){	
			offenders.push_back(k);
		}
	}
	
	
	// now we have the offenders.
	// assign marking players now. find our players that are "closest" to the offender


	vector<regionOfSections *> offenseRegions;	// offensive position of offenders
							// same order as in offenders
	vector<regionOfSections *> defenseRegions;	// defensive positions of our
							// players. order: unum
	
	// get offensive positions of offenders:						
	for(int off=0; off < offenders.size(); off++){
		offenseRegions.push_back(
			slices[slices.size()-1]->getDefenseOffenseRegion(offenders[off]+1,
				-side, 300, WE_HAVE_BALL)
		);
	}
	// get defensive positions of our players:
	for(int player=0; player<11; player++){
		defenseRegions.push_back(
			slices[slices.size()-1]->getDefenseOffenseRegion(player+1,
				side, 300, OPP_HAS_BALL)
		);
	
	}
	// now assign our players to offenders.
	// this can not be done exhaustingly...use greedy method:
	int match=0;		// contains value of overlapping regions
				// 2do some regions don't have joint sections at all.
				// ...in this case find some other assignment
	vector<int> marking;	// contains player indizes.
				// player with unum marking[0]+1 marks offender[0] andsoon
	for(int off=0; off<offenders.size(); off++){	// opponent to be marked
		int bestdefendersofar=12;
		int bestmatchsofar=-1;
		for(int def=0; def<11; def++){		// each of our players
			if(slices[slices.size()-1]->isGoalie(def, side))
				continue;	// except goalie		
			match=overlayRegions(offenseRegions[off],defenseRegions[def]);
			if(match>bestmatchsofar){	// new best match
				bool player_used_already=false;
				for(int i=0; i<marking.size(); i++){
					if(marking[i]==def) player_used_already=true;
						// player marks another opponent already
				}
				if(!player_used_already){	// remember match
					bestmatchsofar=match;
					bestdefendersofar=def;
				}
			}
		}
		marking.push_back(bestdefendersofar);	// Note: theoretically
							// bestdefendersofar can be 12!
	}
	// assignments of our defenders to their offenders are in marking now.
	if(verbose){
		cout << "Mark assignments follow:" << endl;
		for(int i=0; i<offenders.size(); i++){
			cout << "Offender unum" << (offenders[i]+1) << " should be marked "
				<< "by player unum " << (marking[i]+1) << endl;
		}
	}
	CM_Token * marktoken=NULL;
	// function that returns mark TOKEN with condition
	if(marking.size()>0){
		marktoken=buildMarkToken(offenders, marking);
			// Note: Can be NULL if there were no offenders.
		if(verbose){
			if(marktoken!=NULL)
				marktoken->PrintPretty(cout, "marktoken: ");
		}	
	}
	return marktoken;
}












/* this function provides advices for marking, the offside trap,
 * and positioning of the players
 * it builds a CM_Message and sends it to the server.
 *
 * 1. get offenders.
 * 2. assign our players to those offenders (if possible those that already are defenders)
 * 3. assign positions to all our players
 * 4. advice offside trap
 */
void markOlinePositioning(short &lastAdvice){
	static bool doingthisalready=false;	// flag to prevent this function from being
					// executed several times simultaneously
	if(doingthisalready){
		return;
	}
	if(slices.size()<2) return;	// nothing to analyze
	
	doingthisalready=true;		// lock

	if(verbose)
		cout << "Starting to do markOlinePositioning" << endl;
	
	// to reduce inconsistencies, marking will also be assigned
	//   in advicePositionToken	
	CM_Token * marktoken=NULL;
	
	// - positioning
	// get a token for offensive and defensive positioning.
	vector<CM_TokRule *> *defensivetokens;
	vector<CM_TokRule *> *offensivetokens;
	defensivetokens=advicePositionToken(false, true);
	offensivetokens=advicePositionToken(true, false);
	
	// offside region:
	CM_TokRule * offsidetoken=NULL;
	offsidetoken=buildoffsidetoken();
	// offsidetoken->PrintPretty(cout, "bla: offsidetoken");
			
	// now create CM_Message:
	CM_AdviceMessage *advice=new CM_AdviceMessage();
	
	// add tokens:
	vector<CM_Token*> tokens=advice->getTokens();
	if(marktoken!=NULL){
		tokens.push_back(marktoken);
	}
	if(offsidetoken!=NULL){
		tokens.push_back(offsidetoken);
	}

	if(offensivetokens)
		for(int i=0; i<offensivetokens->size(); i++){
			tokens.push_back((*offensivetokens)[i]);
		}
	if(defensivetokens)
		for(int i=0; i<defensivetokens->size(); i++){
			tokens.push_back((*defensivetokens)[i]);
		}

	advice->setTokens(tokens);

			
	// send CM_Message if there are tokens:
	if(tokens.size()>0){
		if(sendCM_Message(advice)){
			lastAdvice=servertime;
		}
	}else{
		if(verbose) cout << "There were no tokens. Nothing sent." << endl;
	}
	if(verbose)
		cout << "Unlocking markOlinePos()" << endl;
	doingthisalready=false;		// unlock this function
	// maybe change this later:
	lastAdvice=servertime;
	return;
}





bool sendCM_Message(CM_Message* message){
	ostringstream ost;
	message->Print(ost);
	string outstring=ost.str();
	sprintf(const_cast<char *>(sendbuf),"(say %s)", outstring.c_str());
	if(send_message()==-1){
		cerr << "CMMessage NOT sent.";
		return false;
	}else{
		if(verbose) cout << "CMMessage sent.";
		return true;
	}
}


bool sendCM_Message(char * message){
	sprintf(const_cast<char *>(sendbuf),"(say %s)", message);
	if(send_message()==-1){
		cerr << "CMMessage NOT sent.";
		return false;
	}else{
		if(verbose) cout << "CMMessage sent.";
		return true;
	}
}


// This function gets a "(see_global ...)" message and fills the resulting values into
// playerinfo[], ballinfo and servertime.
void parse_see_global(char * message){
  bool see = false;

	// just to make sure we get a see_global message:
  if(strncmp("(see_global", const_cast<char *>(receivebuf), 11) != 0 &&
     strncmp("(see ", const_cast<char *>(receivebuf), 5) != 0) {
		cerr << "parse_see_global needs a (see_global ...) or (see ...) message and not" << endl
		     << "  " << const_cast<char *>(receivebuf) << endl;
		exit(1);
	}
  if (strncmp("(see ", const_cast<char *>(receivebuf), 5) == 0) {
    see = true;
  }

		// read timestamp
	sscanf(const_cast<char *>(receivebuf+12), "%d", &servertime);
	

	// read objects.
	// structure looks like this:
	// ((g r|l) X Y)		if goal
	// ((b) X Y VELX VELY)		if ball
	// ((p "TEAMNAME" UNUM [goalie]) X Y VELX VELY BODYANGLE NECKANGLE)	if player
	int	playerindex;	// index of player in playerinfo[]
	int 	playernumber;	// number of player (1-11)
	char * charpos=strchr(const_cast<char *>(receivebuf+12), '(');
					// pointer to first '(' in ((NAME) DESC)

	char * otherpos;
	while(charpos!=NULL){
		assert(*charpos=='(');
		switch(charpos[2]){	// b, g or p?
			case 'p':
				otherpos=charpos;
				charpos=strchr(&charpos[see ? 10 : 5], ' ');
					// to next ' ' after teamname
					// teamnames must not contain blanks
				assert(*charpos=' ');
				sscanf(&charpos[1], "%d",
					&playernumber); // read UNUM
				if (!see) { //see_global
				  assert(*(charpos-1)=='"');
				  *(charpos-1)='\0';		// erase '"'
				}
				if(strcmp(&otherpos[see ? 9 : 5],
					teamname.c_str())==0){
					if(side==LEFT)
						playerindex=-1+playernumber;
					else
						playerindex=10+playernumber;
					playerinfo[playerindex].side=side;
				}else{
					if(side==LEFT)
						playerindex=10+playernumber;
					else
						playerindex=-1+playernumber;
					playerinfo[playerindex].side=-side;
				}
				
				otherpos=strchr(&charpos[1], 'g');
					// to next 'g'
				if((otherpos!=NULL) && (otherpos-6<charpos)){
					// it's the goalie
					playerinfo[playerindex].isGoalie=true;
				}else{
					playerinfo[playerindex].isGoalie=false;
				}
				charpos=strchr(charpos, ')');
					// to next ')'
				assert(*charpos==')');
				sscanf(&charpos[2], "%lf",
					&(playerinfo[playerindex].pos.x));
									// read X
				charpos=strchr(&charpos[2], ' ');
					// to next ' '
				assert(*charpos==' ');
				sscanf(&charpos[1], "%lf",
					&(playerinfo[playerindex].pos.y));
									// read Y
				charpos=strchr(&charpos[1], ' ');
				assert(*charpos==' ');
					// to next ' '
				sscanf(&charpos[1], "%lf",
					&(playerinfo[playerindex].velocity.x));
									//read VELX
				charpos=strchr(&charpos[1], ' ');
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&(playerinfo[playerindex].velocity.y));
									//read VELY
				charpos=strchr(&charpos[1], ' ');	
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&(playerinfo[playerindex].bodyangle));
									//read BODYANGLE
				charpos=strchr(&charpos[1], ' ');
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&(playerinfo[playerindex].neckangle));
									//read NECKANGLE
				charpos=strchr(&charpos[1], '(');
						// to next '('
						// either point to next '(' or NULL
				break;

			case 'b':
				sscanf(&charpos[see ? 8 : 5], "%lf",
					&ballinfo->pos.x);   // read X
				charpos=strchr(&charpos[see ? 8 : 5], ' ');
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&ballinfo->pos.y);   // read Y
				charpos=strchr(&charpos[1], ' ');
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&ballinfo->velocity.x);//read VELX
				charpos=strchr(&charpos[1], ' ');
					// to next ' '
				assert(*charpos==' ');	
				sscanf(&charpos[1], "%lf",
					&ballinfo->velocity.y);//read VELY
				charpos=strchr(&charpos[1], '(');
				// to next '('
							// this will result in NULL at the
							// end and will terminate while.
				break;
			case 'g': 	// Not sure what the goal info is for...
				charpos=strchr(&charpos[2], '(');
					// skip it
				break;
	
			default:
				cerr << "There was something wrong in the see_global-msg:"
				     << charpos[2] << charpos[3] << charpos[4] << endl;
				exit(1);
		}
	}
	if (trainer) { //the trainer does not get the info about the goalie :-(
	  playerinfo[0].isGoalie = true;
	}

	// now put the info in a TimeSlice:
	TimeSlice * newslice;
	int slicenumber=slices.size();
	if(slicenumber>0){	// can we link backwards? or is this the first slice?
		int lastslice=slicenumber-1;
		newslice=new TimeSlice(playerinfo, ballinfo, servertime,
		      slices[lastslice], verbose, false, 3,&teamname, &opponentname,
		      playmode);
	}else{
		newslice=new TimeSlice(playerinfo, ballinfo, servertime,
			  NULL, verbose, false, 3,  &teamname, &opponentname, playmode);
	}
	slices.push_back(newslice);

	// it's necessary to fill in the action values (at least for kick)
	// so check if the ballspeed increased since last slice.
	slicenumber=slices.size();
	if(slicenumber>1){
		global_pos oldballvelocity=slices[slicenumber-2]->getBallVelocity();
		if((fabs(ballinfo->velocity.x)>fabs(oldballvelocity.x)) ||
		   (fabs(ballinfo->velocity.y)>fabs(oldballvelocity.y)) ||
		   (ballinfo->velocity.x>0.05 && ballinfo->velocity.x<-0.05) ||
		   (ballinfo->velocity.y>0.05 && ballinfo->velocity.y<-0.05)){
		   	// Note: last two conditionsare for situations in which the ball
			// changes only its direction after a kick.
			
			// set action in TimeSlice and analyse pass, kick etc.:
			slices[slices.size()-1]->processAnonymousKick();
	   	}
	}

	
	if (trainer) {
	  if (ballinfo->pos.x > 0) {
	    right.ballInOwnHalf++;
	  }
	  else {
	    left.ballInOwnHalf++;
	  }

	  if (newslice->goalieHasBall(LEFT)) {
	    left.goalieHasBall++;
	  }
	  else if (newslice->goalieHasBall(RIGHT)) {
	    right.goalieHasBall++;
	  }
	  
	  if (newslice->ballPossessedByLeftTeam()) {
	    left.teamHasBall++;
	  }
	  else if (newslice->ballPossessedByRightTeam()) {
	    right.teamHasBall++;
	  }
	  ballXHistory.push_back(ballinfo->pos.x);
	}
}



// parse auditory information
// 2do until now only referee's playmode change messages are parsed
void parseHear(char* message){
	if(strncmp("(hear", message, 5) != 0) return;	// is it indeed a hear-message?
	
	int heartimestamp;	// when was the message sent? (not used yet)
	sscanf(message, "(hear %d", &heartimestamp);

	char * PlayModeStrings[] = PLAYMODE_STRINGS;
	char * charpos;
	char * charpos2;
	charpos=strchr(&message[6], ' ');  // find position after timestamp
	charpos2=strchr(&charpos[1], ' '); // find position after sender
	*charpos2='\0';			   // terminate string with zero byte
	
	if(strcmp("referee", &charpos[1])!=0) return;	// message was not from referee
	
	charpos=strchr(&charpos2[1], ')'); //find position after playmode (if there is one)
	*charpos='\0';	// terminate with zero byte

	for(int n=0; n<PM_MAX; n++){	    // try all playmodes
		if(strcmp(&charpos2[1], PlayModeStrings[n])==0){
			if(verbose) cout << "New playmode: "<<PlayModeStrings[n] << endl;
			if (strcmp(PlayModeStrings[n], "time_over") == 0 ||
			    strcmp(PlayModeStrings[n], "time_up") == 0) {
			  gameover = true;
			}
			playmode=n;
			return;
		}
	}
	
	// goal_Side_Score are not in PlayModeStrings. Test them:
	if(strncmp("goal_l", &charpos2[1], 6) ==0){
	  left.goals++;
	  playmode=PM_AfterGoal_Left;
	  return;
	}
	if(strncmp("goal_r", &charpos2[1], 6) ==0){
	  right.goals++;
	  playmode=PM_AfterGoal_Right;
	  return;
	}
	if(strncmp("half_time", &charpos2[1], 9) == 0) {
	  halftime = true;
	  return;
	}
	if (strncmp("time_extended", &charpos2[1], 13) == 0) {
	  extended = true;
	  return;
	}
	if (verbose) {
	  cerr << "Did not understand referee's playmode message:"
	       << string(&charpos2[1]) << endl;
	}
	return;
}





// recognize message type and parse it
void parse_server_msgs(){
	int playertype;
	
	// check if message is a server_param - msg:
	if(strncmp("(server_param", const_cast<char *>(receivebuf), 13)==0){
		Serverparam=new Server_param(const_cast<char *>(receivebuf));
		return;
	}
	
	// check if message is a player_param - msg:
	if(strncmp("(player_param", const_cast<char *>(receivebuf), 13)==0){
		Playerparam=new Player_param(const_cast<char *>(receivebuf));
		return;
	}

	// check if message is a player_type - msg:
	if(strncmp("(player_type", const_cast<char *>(receivebuf), 12)==0){
		playertype=(int)receivebuf[14]-(int)'0';
		Playertype[playertype]=new Player_type(const_cast<char *>(receivebuf));
		return;
	}
	
	if((strncmp("(error", const_cast<char *>(receivebuf), 6)==0) ||
	   (strncmp("(warning", const_cast<char *>(receivebuf), 8)==0)){	
		lastCommandSucceeded=false;
		return;
	}

	if(strncmp("(ok", const_cast<char *>(receivebuf), 3)==0){
		if(verbose) cout << "Last command was ok." << endl;		
		lastCommandSucceeded=true;
		return;
	}
	

	if(strncmp("(hear", const_cast<char *>(receivebuf), 5)==0){	
		parseHear(const_cast<char *>(receivebuf));
		return;
	}


	if(strncmp("(see_global", const_cast<char *>(receivebuf), 11) == 0 ||
	   strncmp("(see ", const_cast<char *>(receivebuf), 5) == 0){
		ballinfo=new ballinfo_t;
		playerinfo=new playerinfo_t[22];
		parse_see_global(const_cast<char *>(receivebuf));
		return;
	}

	cerr << "Hm, received something I didn't understand:" << endl << "  " << 
	  const_cast<char *>(receivebuf) << endl;
	return;	
}




/** This function returns a TOKEN that can be added to a CM_AdviceMessage.
 *  offenders is a vector that contains the indices of the opponent offenders.
 *  marking contains the indices of the team's players that should mark the opponent
 *  in the corresponding offenders-element: e.g. the player with the index marking[0]
 *  should mark the opponent with the index offenders[0]
 */
CM_TokRule * buildMarkToken(vector<int> offenders, vector<int> marking){
	if(marking.size()==0) return NULL;
	vector<CM_Directive *> directiveVector;
	for(int pairCount=0; pairCount<marking.size(); pairCount++){
		UnumSet *unumSet = new UnumSet(11);
		UnumSet *opponentSet = new UnumSet(11);
		if(marking[pairCount]+1<12){
			unumSet->addNum(marking[pairCount]+1);
			opponentSet->addNum(offenders[pairCount]+1);
			CM_ActMark * action= new CM_ActMark(*opponentSet);
			CM_Directive *directive=new CM_DirCommand(true, true, *unumSet,
										 action);
			directiveVector.push_back(directive);
		}
	}
	CM_CondBallOwner *condition=NULL;
	UnumSet *unumSet = new UnumSet(11);
	unumSet->addNum(0); 			// any player
	condition=new CM_CondBallOwner(false,*unumSet); 

	CM_TokRule *marktoken=NULL;
	if(directiveVector.size()>0){
		marktoken=
		   new CM_TokRule(servertime+30+Serverparam->clang_win_size, condition, directiveVector);
	}
	return marktoken;
}




/** This function creates a RegQuad for the Coach language from our regionOfSections-type.
 *  it also negates coordinates if our team plays on the right side.
 */
RegQuad * createRegQuad(regionOfSections * region){
	if(region==NULL) return NULL;
	short factor;
	if(side==RIGHT)
		factor=-1;	// negate coord., if we are on right side
	else
		factor=1;
	// create four points:
	global_pos point;	// point from regionOfSections
	RegPointSimple* points[4];
	for(int clockwise=0; clockwise<4; clockwise++){
		point=getXY(region, clockwise);
		points[clockwise]=new RegPointSimple(factor * point.x, factor * point.y);
	}
	
	// create RegQuad:
	RegQuad * regQuad = new RegQuad(points[0], points[1],points[2], points[3]);
	return regQuad;
}





/**
 *  This function returns a token that contains an advice for each player about his
 *  positioning in defensive (false) or offensive (true) mode.
 *  offensive: find holes in defense, place offenders there and position auxiliary forces
 *  accordingly
 *  defense: place two defenders and/or midfielders in the same region as the three 
 *  foremost opponent players per region and the other four players to the next foremost
 *  regions.
 *  if mode is false and includeMarking is true, mark assigment directives will be
 *  included
 */
// the first COUPLE regions will be assigned two times in defensive mode
// MUST be less than 6!
#define COUPLE 3
vector<CM_TokRule *> *advicePositionToken(bool mode, bool includeMarking){
	// positioning depends on ballowner (opp or we)
	vector<CM_TokRule *> *resulttokens=new vector<CM_TokRule*>; 
	
	CM_CondBallOwner *condition=NULL;
	UnumSet *unumSet = new UnumSet(11);
	unumSet->addNum(0); 			// any player
	condition=new CM_CondBallOwner(!mode,*unumSet); 
	
	// now get opponent positions:
	short analyseMode;
	if(mode)
		analyseMode=OPP_HAS_BALL;	// team is in offense, opp is in defense
	else
		analyseMode=WE_HAVE_BALL;	// team is in defense, opp has ball

	vector<regionOfSections *> oppRegions;	// positions of opponent, order: unum
	vector<int> oppUnums;			// numbers of opponents (for marking)
	for(int opp=1; opp < 12; opp++){
		oppRegions.push_back(
			slices[slices.size()-1]->getDefenseOffenseRegion(opp,-side, 300,
					analyseMode)
		);
	}
	for(int i=1; i<12; i++){
		oppUnums.push_back(i);	// just fill 1-11 into vector
	}

	// get our recent positions (to reduce transfer ways to new positions)
	if(mode)
		analyseMode=WE_HAVE_BALL;	// team is in offense and has ball
	else
		analyseMode=OPP_HAS_BALL;	// team is in defense and opp has ball

	vector<regionOfSections *> ourRegions;	// positions of opponent, order: unum
	for(int our=1; our < 12; our++){
		ourRegions.push_back(
			slices[slices.size()-1]->getDefenseOffenseRegion(our,side, 300,
					analyseMode)
		);
	}
	

	// now calculate our new positioning and then assign players to these positions
	vector<int> positioning;// contains player indizes.
			// player with unum positioning[0]+1 is in oppRegion[0] etc

	if(mode){	// offensive positioning:
		// 2do
		// ...
		
	}else{		// defensive positioning:
		// sort opponent regions (by right)
		sortRegions(oppRegions, 4, oppUnums, LEFT);
		// assign players to regions:
		// three foremost regions get two players each.
		// next four regions get one player each
		// this can not be done exhaustingly...use greedy method:
		int match=0;		// contains value of overlapping regions
		int assigntimes;
		for(int reg=0; reg<oppRegions.size() && positioning.size()<11; reg++){
					// region to be assigned
		   if(reg<COUPLE)	// first COUPLE regions will be assigned two times!
			assigntimes=2;
		   else
			assigntimes=1;
		   for(int count=1; count<=assigntimes; count ++){
			int bestdefendersofar=12;
			int bestmatchsofar=-1;
			for(int def=0; def<11; def++){		// each of our players
				if(slices[slices.size()-1]->isGoalie(def, side))
					continue;	// except goalie
				match=overlayRegions(oppRegions[reg],ourRegions[def]);
				if(match>bestmatchsofar){	// new best match
					bool player_used_already=false;
					for(int i=0; i<positioning.size(); i++){
						if(positioning[i]==def)
							 player_used_already=true;
							// player marks another opponent
							// already
					}
					if(!player_used_already){	// remember match
						bestmatchsofar=match;
						bestdefendersofar=def;
					}
				}
			}
			positioning.push_back(bestdefendersofar);// Note: theoretically
							// bestdefendersofar can be 12!
		    }
		}
		// in positioning all player indizes are assigned now.
		// Note: first COUPLE regions are assigned two times!
		if(includeMarking){
			// assign marking:
			vector<int> marking;
			for(int i=0; i<COUPLE; i=i+2){	// COUPLE players will mark
				if(i<positioning.size())
					marking.push_back(positioning[i]);
				else
				  cerr << "Warning, trying to assign non-existing indizes."
					     << endl;
			}
			CM_TokRule * marktoken=NULL;
			marktoken=buildMarkToken(oppUnums,marking);
			if(verbose)
				marktoken->Print(cout);
			resulttokens->push_back(marktoken);
		}
	}		
	// create directive for each player: (do our UNUM ACTION)  ACTION: (pos REG)
	vector<CM_Directive *> directiveVector;	 // one directive for each player
	for(int i=0; i<positioning.size(); i++){ // loop over regions and not players
		UnumSet *unumSet = new UnumSet(11);
		if(positioning[i]+1<12){
			unumSet->addNum(positioning[i]+1);
			Region *region = createRegQuad(oppRegions[i]);
			if(region==NULL) continue;	// error, skip this region
			CM_ActPosition * action= new CM_ActPosition(region);
			CM_Directive *directive=new CM_DirCommand(true, true, *unumSet,
										action);
			directiveVector.push_back(directive);
		}
	}
	// construct whole token:
	if(directiveVector.size()==0) return NULL;
	CM_TokRule * token=new CM_TokRule(servertime+30+Serverparam->clang_win_size, condition, directiveVector);
	if(verbose)
		token->Print(cout);
	resulttokens->push_back(token);
	return resulttokens;
}




/**
 *  This function returns a token that contains info for each opponent player about its
 *  positioning in defensive (false) or offensive (true) mode.
 */
CM_TokRule * getConditionalOpponentPositions(bool mode){
	static bool myturn=false;	// send only every second turn
	
	myturn=!myturn;
	if(!myturn) return NULL;

	// first get opponent positions depending on mode:
	short analyseMode;
	if(mode){
		analyseMode=WE_HAVE_BALL;
	}else{
		analyseMode=OPP_HAS_BALL;
	}
	vector<regionOfSections *> oppRegions;	// positions of opponent, order: unum
	for(int opp=1; opp < 12; opp++){
		oppRegions.push_back(
			slices[slices.size()-1]->getDefenseOffenseRegion(opp,-side, 500,
					analyseMode)
		);
	}
	// we got positions now.
	// create directives:
	vector<CM_Directive *> directiveVector;	 // one directive for each player
	for(int i=0; i<oppRegions.size(); i++){
		if(oppRegions[i]){
			UnumSet *unumSet = new UnumSet(11);
			unumSet->addNum(i+1);
			Region *region = createRegQuad(oppRegions[i]);
			if(region==NULL) continue;	// error, skip this region
			CM_ActPosition * action= new CM_ActPosition(region);
			CM_Directive *directive=new CM_DirCommand(true, false, *unumSet,
										action);
			directiveVector.push_back(directive);
		}
	}
	// create Condition:
	CM_CondBallOwner *condition=NULL;
	UnumSet *unumSet = new UnumSet(11);
	unumSet->addNum(0); 			// any player
	condition=new CM_CondBallOwner(!mode,*unumSet);
	// construct whole token:
	if(directiveVector.size()==0) return NULL;
	CM_TokRule * token=new CM_TokRule(servertime+30+Serverparam->clang_win_size, condition, directiveVector);
	if(verbose)
		token->PrintPretty(cout, "COPS: ");
	return token;
}



/** put offside line behind the two frontmost opponents (or behind middle line)
  * condition: opp has ball
  * it also negates coord., if our team plays on the right side
  */
CM_TokRule * buildoffsidetoken(){
	// build condition:
	CM_CondBallOwner *condition=NULL;
	UnumSet *unumSetOpp = new UnumSet(11);
	unumSetOpp->addNum(0); 			// any player
	condition=new CM_CondBallOwner(false,*unumSetOpp);	
	// now analyze position of offside trap:
	global_pos positions[11];
	for(int i=1; i<12; i++){
	     positions[i-1]=slices[slices.size()-1]->calculateAveragePosition(i, -side,
	     						      Serverparam->clang_win_size);
	}
	qsortX(positions, 0, 10);
	double offsideline;	// x-coordinate of offside trap
	if(side==RIGHT)
		offsideline=positions[9].x;
	else
		offsideline=positions[1].x;
	// now create a RegQuad and a directive:
	vector<CM_Directive *> directiveVector;	 // one directive, yet a vector is needed
	// all players:
	UnumSet *unumSetOur = new UnumSet(11);
	unumSetOur->addNum(0);
	// where:
	short factor;
	if(side==RIGHT)
		factor=-1;
	else
		factor=1;
	
	RegPointSimple* pts[4]={
		new RegPointSimple(factor*offsideline, factor*-PITCH_WIDTH/2),
		new RegPointSimple(factor*offsideline, factor*-PITCH_WIDTH/2),
		new RegPointSimple(factor*offsideline, factor*PITCH_WIDTH/2),
		new RegPointSimple(factor*offsideline, factor*PITCH_WIDTH/2)
	};
	Region * region = new RegQuad(pts[0], pts[1], pts[2], pts[3]);
	
	// create objects:
	CM_ActOffsidesLine * action=new CM_ActOffsidesLine(region);
	CM_Directive *directive=new CM_DirCommand(true, true, *unumSetOur, action);
	directiveVector.push_back(directive);
	CM_TokRule * token=new CM_TokRule(servertime+30+Serverparam->clang_win_size, condition, directiveVector);
	return token;
}



/** This function checks if opponent has simple set plays for different play-modes.
  * For each playmode there will be a token. The Playmode-Condition may be
  * extended by something like "goalie has ball".
  */
// may be extended to work with variable timeframe
// result could be used to be worked with (for positioning advices or so)
// so many slices will be counted
#define TIMEFRAME_SETPLAY 1800
// maximum difference to be judged as equal:
#define POS_THRESHOLD 5
// how long should the set play token be valid?
#define SETPLAY_TTL 1800
// how many cycles need to be between two events of the same playmode to be stored as two?
#define NECESS_INTERVAL 40
vector<CM_TokRule *> * findSetPlay(){
	static bool myturn=true;	// send only every second turn
	
	myturn=!myturn;

	vector<CM_TokRule*> *tokens=new vector<CM_TokRule*>;
	if(!myturn) return tokens;
	
	vector<global_pos> setPlayPos[18][11];	// 18 playmodes, 11 opponent players
	CM_PlayModeType playmodeInSlice;
	CM_PlayModeType lastPlaymode=CMPM_None;	// do not process long playmodes more than
						// once
	int newestslice=slices.size()-1;
	int lastTime=newestslice+NECESS_INTERVAL;
		
	for(int time=newestslice; time>newestslice-TIMEFRAME_SETPLAY &&
							time>=0; time--){// look back
		// check which playmode:
		playmodeInSlice=whichPlaymode(slices[time]);
		if(playmodeInSlice==CMPM_None || playmodeInSlice==CMPM_PlayOn ||
		   playmodeInSlice==CMPM_AfterGoal_Opp ||
		   playmodeInSlice==CMPM_AfterGoal_Our ||
		   time>lastTime-NECESS_INTERVAL ||
		   playmodeInSlice==lastPlaymode) continue;
		if(playmodeInSlice<0 || playmodeInSlice>17) return tokens;
		// Note: FREE_KICK_X is up to now always extended by "X's goalie has ball"
		// save each player's position:
		for(int opp=0; opp<11; opp++){
			// check if goalie has ball if FREE_KICK_X
			if(playmodeInSlice!=CMPM_FreeKick_Our ||
			   playmodeInSlice!=CMPM_FreeKick_Opp ||
			   (playmodeInSlice==CMPM_FreeKick_Our &&
			       slices[time]->goalieHasBall(side)) ||
			   (playmodeInSlice==CMPM_FreeKick_Opp &&
			    	slices[time]->goalieHasBall(-side)))
			{
				setPlayPos[playmodeInSlice][opp].push_back(
					slices[time]->getPlayerPos(-side, opp+1)
				);
				lastTime=time;
			}
		}
		lastPlaymode=playmodeInSlice;
	}
	// now check if in each entry there are at least two entries and the positions
	// differ only by POS_THRESHOLD
	for(int pm=0; pm<18; pm++){	// check each playmode
		// Note: FREE_KICK_X is up to now always extended by "X's goalie has ball"
		vector<CM_Directive *> directives=*new vector<CM_Directive*>;
				// one directive for each player that does set plays
		for(int opp=0; opp<11; opp++){	// check each opponent player
			if(setPlayPos[pm][opp].size()>1){
					// we need at least(!) two situations to recognize
					// set plays
				double maxx, maxy, minx, miny;
				double recentx, recenty;
				for(int sit=0; sit<setPlayPos[pm][opp].size(); sit++){
					// find range for x and y 
					if(sit==0){	// init range variables
						maxx=(setPlayPos[pm][opp])[sit].x;
						minx=maxx;
						maxy=(setPlayPos[pm][opp])[sit].y;
						miny=maxy;
						continue;
					}
					recentx=(setPlayPos[pm][opp])[sit].x;
					recenty=(setPlayPos[pm][opp])[sit].y;
					if(recentx>maxx) maxx=recentx;
					if(recentx<minx) minx=recentx;
					if(recenty>maxy) maxy=recenty;
					if(recenty<miny) miny=recenty;

				} // of sit
				// check if ranges are less than POS_THRESHOLD
				if((maxx-minx < POS_THRESHOLD) &&
				   (maxy-miny < POS_THRESHOLD))
				{
					// it is a setplay. create Directive
					UnumSet unumSet=*new UnumSet(11);
					unumSet.addNum(opp+1);
					
					
					RegPointSimple* pts[4]={
						new RegPointSimple(minx,miny),
						new RegPointSimple(maxx,miny),
						new RegPointSimple(maxx,miny),
						new RegPointSimple(minx,maxy)
					};
					RegQuad *region=new RegQuad(pts[0],pts[1],pts[2],
								pts[3]);
					CM_ActPosition *action=new CM_ActPosition(
							region);
					CM_DirCommand *directive=new CM_DirCommand(true,
							false, unumSet, action);
					directives.push_back(directive);
					
			
				}
			}// of if
		}// of opp
		// create token for playmode.
		// 2do if free_kick_X: x_goalie has ball
		if(directives.size()>0){
			CM_Condition *condition=new CM_CondPlayMode((CM_PlayModeType)(pm));
			CM_CondBallOwner *addthis=NULL;
			if(pm==CMPM_FreeKick_Our){
				UnumSet unumSet=*new UnumSet(11);
				addthis=new CM_CondBallOwner(true,unumSet);
			}else if(pm==CMPM_FreeKick_Opp){
				UnumSet unumSet=*new UnumSet(11);
				unumSet.addNum(slices[slices.size()-1]->getGoalie(-side));
				addthis=new CM_CondBallOwner(false,unumSet);
			}
			if(addthis){
				vector<CM_Condition*>condvector=*new vector<CM_Condition*>;
				condvector.push_back(condition);
				condvector.push_back(addthis);
				condition=new CM_CondAnd(condvector);
			}
			CM_TokRule *atoken=new CM_TokRule(SETPLAY_TTL,condition,directives);
			tokens->push_back(atoken);
		}
		
	} // of pm
	return tokens;
}



CM_PlayModeType whichPlaymode(TimeSlice * slice){
	PlayMode sliceplaymode=(PlayMode)(slice->getPlayMode());
	//cout << "bla: sliceplaymode: " << (int)(sliceplaymode) << endl;
	if(side==LEFT){
		switch(sliceplaymode){
			case PM_Null		:	return CMPM_None;
			case PM_BeforeKickOff	:	return CMPM_BeforeKickOff;
			case PM_TimeOver	:	return CMPM_TimeOver;
			case PM_PlayOn		:	return CMPM_PlayOn;
			case PM_KickOff_Left	:	return CMPM_KickOff_Our;
			case PM_KickOff_Right	:	return CMPM_KickOff_Opp;
			case PM_KickIn_Left	:	return CMPM_KickIn_Our;
			case PM_KickIn_Right	:	return CMPM_KickIn_Opp;
			case PM_FreeKick_Left	:	return CMPM_FreeKick_Our;
			case PM_FreeKick_Right	:	return CMPM_FreeKick_Opp;
			case PM_CornerKick_Left	:	return CMPM_CornerKick_Our;
			case PM_CornerKick_Right:	return CMPM_CornerKick_Opp;
			case PM_GoalKick_Left	:	return CMPM_GoalKick_Our;
			case PM_GoalKick_Right	:	return CMPM_GoalKick_Opp;
			case PM_AfterGoal_Left	:	return CMPM_AfterGoal_Our;
			case PM_AfterGoal_Right	:	return CMPM_AfterGoal_Opp;
			case PM_Drop_Ball	:	return CMPM_None;
			case PM_OffSide_Left	:	return CMPM_None;
			case PM_OffSide_Right	:	return CMPM_None;
			default: cerr << "Warning, strange playmode." << endl;
				 return CMPM_None;
		}
	
	}else{
		switch(sliceplaymode){
			case PM_Null		:	return CMPM_None;
			case PM_BeforeKickOff	:	return CMPM_BeforeKickOff;
			case PM_TimeOver	:	return CMPM_TimeOver;
			case PM_PlayOn		:	return CMPM_PlayOn;
			case PM_KickOff_Left	:	return CMPM_KickOff_Opp;
			case PM_KickOff_Right	:	return CMPM_KickOff_Our;
			case PM_KickIn_Left	:	return CMPM_KickIn_Opp;
			case PM_KickIn_Right	:	return CMPM_KickIn_Our;
			case PM_FreeKick_Left	:	return CMPM_FreeKick_Opp;
			case PM_FreeKick_Right	:	return CMPM_FreeKick_Our;
			case PM_CornerKick_Left	:	return CMPM_CornerKick_Opp;
			case PM_CornerKick_Right:	return CMPM_CornerKick_Our;
			case PM_GoalKick_Left	:	return CMPM_GoalKick_Opp;
			case PM_GoalKick_Right	:	return CMPM_GoalKick_Our;
			case PM_AfterGoal_Left	:	return CMPM_AfterGoal_Opp;
			case PM_AfterGoal_Right	:	return CMPM_AfterGoal_Our;
			case PM_Drop_Ball	:	return CMPM_None;
			case PM_OffSide_Left	:	return CMPM_None;
			case PM_OffSide_Right	:	return CMPM_None;
			default: cerr << "Warning, strange playmode." << endl;
				 return CMPM_None;
		}
	
	}
	cerr << "Warning, something is wrong with our side-variable." << endl;
	return CMPM_None;
}


// same as in TimeSlice :-)
string itoa(int i) {
  char buffer[32];
  sprintf(buffer, "%d", i);
  string s(buffer);
  return s;
}


/** send a message that assigns a player to go to the ball and kick it.
  * something like (advice (50 (not (playm play_on)) (do our {unum} (bto "their_goal" {pd})))
  * we use "their_goal", knowing that it is defined in orca/sfl/behavior.sfl
  * this works with the Dirty Dozen only, anyway.
  */
bool assignKickPlayer(short unum){
	static int sentLastTime=0;
	if(sentLastTime>servertime-10) return false;	// hack...don't send every cycle
	// too lazy to do this via objects:
	string message="(advice (" + itoa(50+servertime) + " (not (playm play_on)) (do our {"+
	                      itoa(unum)+"} (bto \"their_goal\" {pd}))))";
	sentLastTime=servertime;
	return sendCM_Message(const_cast<char *>(message.c_str()));
}



