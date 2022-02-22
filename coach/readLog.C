// 2do include extra info from logfile version 3 into playerinfo[] (deltax etc.)



#include <stdio.h>
#include "types.h"
#include "TimeSlice.h"
#include "param.h"
#include "geometry.h"
#include "Foil.h"
#include <iostream.h>
#include <fstream.h>
#include <vector.h>
#include <netinet/in.h>
// #include "dbAccess.h"
#include <GetOpt.h>
#include "consts.h"
//using namespace std;



// global variable signalling whether messages are included in logfile
bool messagesInLogfile=false;
int logversion;			// logfile protocol version
int verbose = 0;
string teamNameLeft="";		// Name of team that covers [0-10] (vice versa in version
				// 2 & 3)
string teamNameRight="";
int scoreleft, scoreright;	// used only with logfile version 3
server_params_t server_params;	// parameters of server, only used in logfile version 3
player_params_t player_params;	// hetero player paramters
player_type_t player_type;	// parameters of a specific player_type 
player_type_t player_types[7];	// one player_type_t for each player type

static  short servertime;

string itoa(int i) {
  char buffer[32];
  sprintf(buffer, "%d", i);
  string s(buffer);
  return s;
}

// This function gathers statistics about ballpossession and ballposition
void countCycles(vector<TimeSlice*> slices){
	int cyclesBallLeft=0;	// how many cycles is ball in the left half?
	int cyclesBallRight=0;
	int cyclesPossessionLeft=0;// how many cycles is the ball possessed by left team?
	int cyclesPossessionRight=0;
	
  	for(int i=0; i<slices.size(); i++){
		if((slices[i]->getPlayMode()!= PM_AfterGoal_Left) &&
		   (slices[i]->getPlayMode()!= PM_AfterGoal_Right)){
			if(slices[i]->ballInLeftHalf()) cyclesBallLeft++;
			if(slices[i]->ballInRightHalf()) cyclesBallRight++;
			if(slices[i]->ballPossessedByLeftTeam()) cyclesPossessionLeft++;
			if(slices[i]->ballPossessedByRightTeam()) cyclesPossessionRight++;
		}
	}
	cout << "Ball in left half : " << cyclesBallLeft << endl;
	cout << "Ball in right half: " << cyclesBallRight << endl;
	cout << "Ball possessed by left team : " << cyclesPossessionLeft << endl;
	cout << "Ball possessed by right team : " << cyclesPossessionRight << endl;
	// print statistical results to cerr
	cerr << cyclesBallLeft/(double)cyclesBallRight << endl;
	cerr << cyclesPossessionLeft/(double)cyclesPossessionRight << endl;	
}



// saves pass data about the left team in C5.0 style for left team (if side=false) or right
// team (if side=true)
void saveC5PassData(vector<TimeSlice*> slices, string savefile, bool side,
								bool regionaldata){
  FILE *outfp;
  
  // open file always for append
  outfp=fopen(savefile.c_str(),"a");

  if(outfp==0) {
    cerr << "Cannot open " << savefile.c_str() <<endl;
    exit(IO_ERROR);
  }

  for(int i=0; i<slices.size(); i++){
  	if(regionaldata)
		slices[i]->saveC5RegionalPassData(side, outfp);
	else
		slices[i]->saveC5PassData(side, outfp);
  }
  fclose(outfp);
  return;
}






// extracts info from showinfo_t structures and transforms them so that TimeSlice can use
// them
// sets player- and ballinfo
// only one of showinfo and new_showinfo is actually instantiated. Check logversion
void createInfos(showinfo_t showinfo, short_showinfo_t2 new_showinfo,
	vector<msginfo_t*> messages, playerinfo_t *playerinfo, ballinfo_t *ballinfo){
	// servertime:
  	if(logversion==2)
      		servertime=(signed short)ntohs(showinfo.time);
  	else
      		servertime=(signed short)ntohs(new_showinfo.time);
	// ballinfo:
	if(logversion==2){
  		ballinfo->pos.x=transform_coord(showinfo.pos[0].x);
  		ballinfo->pos.y=transform_coord(showinfo.pos[0].y);
  	}else{
  		ballinfo->pos.x=transform_coordV3(new_showinfo.ball.x);
  		ballinfo->pos.y=transform_coordV3(new_showinfo.ball.y);
        // in version 3 we even get info about deltax and deltay
        ballinfo->velocity.x=transform_coordV3(new_showinfo.ball.deltax);
        ballinfo->velocity.y=transform_coordV3(new_showinfo.ball.deltay);
  	}
	
	// playerinfo:
  	for(int i=1; i<23; i++){
		// positions and angles:
    		if(logversion==2){
			playerinfo[i-1].mode=(signed short)ntohs(showinfo.pos[i].enable);
    			playerinfo[i-1].pos.x=transform_coord(showinfo.pos[i].x);
			playerinfo[i-1].pos.y=transform_coord(showinfo.pos[i].y);
			if(i<12)
	 			playerinfo[i-1].side=LEFT;
			else
				playerinfo[i-1].side=RIGHT;
			playerinfo[i-1].bodyangle=(signed short)ntohs(showinfo.pos[i].angle);
			playerinfo[i-1].neckangle=0;
	
    		}else{
			playerinfo[i-1].mode=(signed short)ntohs(new_showinfo.pos[i-1].mode);
    			playerinfo[i-1].pos.x=transform_coordV3(new_showinfo.pos[i-1].x);
    			playerinfo[i-1].pos.y=transform_coordV3(new_showinfo.pos[i-1].y);
			// transform to Deg and scale:
			playerinfo[i-1].bodyangle=scale360(Rad2Deg(
	                 ((long)ntohl(new_showinfo.pos[i-1].body_angle))/SHOWINFO_SCALE2));
			playerinfo[i-1].neckangle=scale360(Rad2Deg(
	       		 ((long)ntohl(new_showinfo.pos[i-1].head_angle))/SHOWINFO_SCALE2));
			if(i<12)
				playerinfo[i-1].side=LEFT;
			else
				playerinfo[i-1].side=RIGHT;
    		}
	} // of for (players)
	
	
	// actions:
	// only dash and kick are relevant
	// in case of dash set direction to 0
	// Relevant messages look like this:
	// Recv OSBAllstars_5: (turn -20.058601)
	
	// initialize actions[]
	for(int i=0; i<22; i++){
		playerinfo[i].playeraction.action=none;
		playerinfo[i].playeraction.power=0.0;
		playerinfo[i].playeraction.direction=0.0;
	}

	for(int i=0; i<messages.size(); i++){ // messagesize is 0 if there are no messages
					      // in the logfile
		if(strncmp(messages[i]->message, "Recv", 4)==0){
		char * character;	// Pointer to character between name 
					// und number
		char * character2;// Pointer to ':' after number
		int index;	// Position in array actions[]
		character=strrchr(messages[i]->message, '_');
		character2=strrchr(messages[i]->message, ':');
		*character = '\0'; // replace "_" with \0
		*character2 = '\0';// replace ":" with \0
		 
		// identify teamname:(starts at pos. message[5])
		if(strncmp(&(messages[i]->message[5]),teamNameLeft.data(),
				string(&(messages[i]->message[5])).length())==0){
			// left team 
			index=-1;
		}else{
			// right team
			index=10;
      		}
      		// now, identify player number and add it
      		index+=(int)strtol(character+1,NULL,0);
      		if (verbose) {
			//cout << "Action of player # " << index << endl;
      		}
      		// identify actiontype
      		character2+=3;	// set to start of action
      		if(strncmp(character2, "dash", 4)==0){
			playerinfo[index].playeraction.action=dash;
      		}else{
			if(strncmp(character2, "kick", 4)==0){
				playerinfo[index].playeraction.action=kick;
			} else{
				if (verbose) {
					//cout << "Neither dash nor kick" << endl;
				}
				continue; // Neither dash nor kick
			}
      		}
		character2+=5;	// set to start of power
		char *rest;	// points to rest after callinf of strtod
      		playerinfo[index].playeraction.power=strtod(character2, &rest);
      		if (verbose) {
			//cout << "Power: " 
			//     << playerinfo[index].playeraction.power << endl;
      		}
      		if(playerinfo[index].playeraction.action==kick){
			// read direction
			playerinfo[index].playeraction.direction=strtod(rest, NULL);
			if (verbose) {
				//cout << "Richtung: " << actions[index].direction << endl;
			}
      		}
    	} // of if
	} // of for (messages)	
	if(!messagesInLogfile){
		for(int i=0; i<22; i++){
			// cout << "bla: playerinfomode: " << playerinfo[i].mode << endl;
			if((playerinfo[i].mode & 0x02) != 0){
				playerinfo[i].playeraction.action=kick;
			}
		}
	}	
	
				
	// 2do extras from version 3 like delta_x, delta_y
	// 2do ........
		
  	// free mem for messages
  	for(int i=0; i<messages.size(); i++)
    		delete(messages[i]);
}




// get teamnames and store them in global variables
void getTeamNames(showinfo_t showinfo){
	teamNameLeft=string(showinfo.team[0].name);
	teamNameRight=string(showinfo.team[1].name);
	if(verbose)
	  cout << "Got Teamnames: " << teamNameLeft << " " << teamNameRight << endl;
	return;
}




void read_header(FILE *in){
  // checks if logile starts with ULG
  char character[4];
  if(fread(character,sizeof(char),4,in) != 4){
    fclose(in);
    cerr << "Cannot read from file" << endl;
    exit(1);
  }
  
  if (verbose) {
    cout << "Format: " << character[0] << character[1] << character[2] << endl;
  }
  
  if((character[0]=='U') || (character[1]=='L') || (character[2]=='G')){
    	if(verbose) cout << "ULG Logformat." << endl;
  }else{
  	cerr << "Wrong logfile format!" << endl;
	exit(1);
  }

  logversion=short(character[3]);

  if (verbose) {
    cout << "Version: " << logversion << endl;
  }
}


/*
vector<TimeSlice*> loadFromDatabase(string where, string dbUrl) {
  dbAccess db(dbUrl);
  string sql = "SELECT ball_pos, team, nearest, pass, goal_try, lose_ball, pred, action, pos FROM "
    "time_frame";
  db.select(sql);
}
*/ 


/*
// writes TimeSlice-Vector into database
void saveToDatabase(vector<TimeSlice*> slices, string dbUrl){
  dbAccess db(dbUrl);
  int last_id = -1;
  //TODO: iterate by going over the predecessor pointer
  for (std::vector<TimeSlice*>::iterator slice = slices.begin(); slice != slices.end(); slice++) {
    int ballPos = db.getID("pos");
    int action = db.getID("action_list");
    int nearest = db.getID("player_list");
    int pos = db.getID("pos_list");
    int id = db.getID("time_frame");
    string sql = "INSERT INTO time_frame (id, ball_pos, team, nearest, pass, goal_try, lose_ball, pred, "
      "action, pos) VALUES (" + db.itoa(id) + ", " + db.itoa(ballPos) + ", " +
      db.itoa((*slice)->getPossessingTeam()) + ", " + db.itoa(nearest) + ", " +
      db.itoa((*slice)->getTimeTillPass()) + ", " +
      db.itoa((*slice)->getTimeTillGoalkick()) + ", " + 
      db.itoa((*slice)->getTimeTillLosingBall()) + ", "  + db.itoa(last_id) + ", " +
      db.itoa(action) + ", " + db.itoa(pos) + ")";
    if ((verbose & DB_VERBOSE) == DB_VERBOSE) {
      cout << sql << endl;
    }
    db.exec(sql);

    //saving ball position:
    global_pos ball = (*slice)->getBallPos();
    sql = "INSERT INTO pos (id, x, y) VALUES (" +
      db.itoa(ballPos) + ", " + db.dtoa(ball.x) + ", " + db.dtoa(ball.y) + ")";
    db.exec(sql);

    //saving actions in n2m list:
    //Action* actions = (*slice)->getActions();
    playerinfo_t* players = (*slice)->getPlayers();
    int action_id = db.getID("action", 22);
    int pos_id = db.getID("pos", 22);
    for (int i = 0; i < 22; i++) {
      int chosen_action;
      Action* player_action = &(players[i].playeraction);
      if (player_action->action == none) {
	chosen_action = 0;
      }
      else if (player_action->action == dash) {
	chosen_action = 1;
      }
      else {
	chosen_action = 2;
      }
      sql = "INSERT INTO action (id, action, power, direction) VALUES (" +
	db.itoa(action_id) + ", " + db.itoa(chosen_action) + ", " + 
	db.dtoa(player_action->power) + ", " + 
	db.dtoa(player_action->direction) + ")";
      if ((verbose & DB_VERBOSE) == DB_VERBOSE) {
	cout << "  " << sql << endl;
      }
      db.exec(sql);
      sql = "INSERT INTO action_list (id, action) VALUES (" +
	db.itoa(action) + ", " + db.itoa(action_id) + ")";
      db.exec(sql);
      action_id++;
      
    //saving postitions in n2m list:
      global_pos* position = &(players[i].pos);
      sql = "INSERT INTO pos (id, x, y) VALUES (" +
	db.itoa(pos_id) + ", " + db.itoa(position->x) + ", " + db.itoa(position->y) + ")";
      db.exec(sql);
      sql = "INSERT INTO pos_list (id, pos) VALUES (" +
	db.itoa(pos) + ", " + db.itoa(pos_id) + ")";
      db.exec(sql);
      pos_id++;
    }

    //saving nearest players in n2m list
    playerinfo_t* player = (*slice)->getNearest();
    id = db.getID("player", 8);
    for (int i = 0; i < 8; i++) {
      sql = "INSERT INTO player (id, side, angle, distance) VALUES (" +
	db.itoa(id) + ", " + db.itoa(player[i].side) + ", " + db.itoa(player[i].bodyangle)
	+ ", " + db.itoa(player[i].distance) + ")";
      db.exec(sql);
      sql = "INSERT INTO player_list (id, player) VALUES (" +
	db.itoa(nearest) + ", " + db.itoa(id) + ")";
      id++;
    }
  }
}
*/


void saveToFile(vector<TimeSlice*> slices, string filename, bool appendToFile, bool regionalData, string saveEmpty){
  FILE *outfp;
  int lastSavedSlice=0;	// which slice was the latest that was saved into the file?
  
  // open file
  if(appendToFile){
  	outfp=fopen(filename.c_str(),"a");
  }else{
  	outfp=fopen(filename.c_str(),"w");
  }
  
  if(outfp==0) {
    cerr << "Cannot open " << filename.c_str() <<endl;
    exit(IO_ERROR);
  }
  
  if(!appendToFile) { //save number of features only if new file
    fprintf(outfp, "%d\n", regionalData ? REGIONAL_FEATURES : 30); 
  }
  for(int i=0; i<slices.size(); i++){
	  if (regionalData) {
	    slices[i]->saveRegionalSliceToFile(outfp, saveEmpty == "1" ? true : false);	// save slice
	  }
	  // don't save if difference between this and last saved slice is too small
	  // btw: 75 is the difference if only the timeTill-features change
	  else if((i==0) || (slices[i]->differenceBetweenSlice(slices[lastSavedSlice])>75)){
	      slices[i]->saveSliceToFile(outfp);	// save slice
	  }
	  lastSavedSlice=i;
  }
  fclose(outfp);
  return;
}



//prints usage information on stderr and exits with USAGE_ERROR
void usage(void) {
  cerr << "Usage: ./readLog -v                                  be verbose\n"
    "                 -f filename                         logfile to be read\n"
    "                 -d postgres connection string       dbname=...;user=...;...\n"
    "                 -s filename                         save to file filename\n"
    "                 -p filename                         save C5.0 pass data about left team\n"
    "                 -P filename                         save C5.0 pass data about right team\n"
    "                 -a                                  append to file\n"
    "                 -r                                  save features in regional format\n"
    "                 -x extension                        to misuse readLog for other purposes ;-)\n"
    "                    showAveragePosition\n"
    "                    rectgrid\n"
    "                    foil\n"
    "                    foilpass\n"
    "                 -X extensionArgs                    args to -x \n"
    "                    startingSlice,SlicesToGoBack\n"
    "                    timeslice,timeframe:playerNumber#side+mode?outputFile\n"
    "                    outputfilename\n"
    "                    PathToFoilBin (for foilpass)\n";
   
  exit(USAGE_ERROR);
}


int main(int argc, char *argv[]){
  FILE	*infp;			// filehandler of logfile
  dispinfo_t	m_tBuf;		// buffer for dispinfo
  showinfo_t	showinfo;	// buffer for showinfo
  vector<msginfo_t*>	msginfos;	// buffer for msginfo-pointer
  msginfo_t	*msg;		// pointer to single message
  short		length;		// length of message
  char 		character;
  vector<TimeSlice*> slices;	// contains all TimeSlices
  bool 		createSlice=false;	// skip first message
  bool		lastIsShowstruct=false;	// ss last entry in logfile a showstruct?
  TimeSlice 	*neu=NULL;		// temporary auxiliary variable
  int		countingSlices=0;	// how many slices have we got?
  bool saveFeaturesToDatabase=false;	
  bool saveFeaturesToFile=false;
  bool saveC5PassLeft=false;		// save data for C5.0 about passes of left team?
  bool saveC5PassRight=false;		// save data for C5.0 about passes of right team?
  bool saveRegionalData=false;          // format for saveSlice
  bool appendToFile=false;		// append to file instead of overwriting?
  playerinfo_t * playerinfo;		// holds info that will be passed to TimeSlice
  ballinfo_t * ballinfo;		// dito
  int playmode;	
  
  // variables for version 3:
  short_showinfo_t2 short_showinfo;	// ball and player positions and states
  team_t teamleft, teamright;		// names and score

  

  GetOpt getopt(argc, argv, "ar:v:f:s:d:p:P:x:X:");
  int option_char;
  string logfile_name, database_name = "", savefilename="";
  string saveC5PassRightFile= "", saveC5PassLeftFile ="";
  string saveEmpty = "";
  string extension = "";
  string extensionArgs = "";

  while ((option_char = getopt()) != EOF) {
    switch (option_char) {
    	case 'a': appendToFile=true; break;
    	case 'v': verbose = atoi(getopt.optarg); break;
   	case 'f': logfile_name = getopt.optarg; break;
    	case 'd': database_name = getopt.optarg; saveFeaturesToDatabase=true; break;
	case 's': savefilename = getopt.optarg; saveFeaturesToFile=true; break;
	case 'p': saveC5PassLeftFile = getopt.optarg; saveC5PassLeft = true; break;
	case 'P': saveC5PassRightFile = getopt.optarg; saveC5PassRight = true; break;
        case 'r': saveRegionalData = true; saveEmpty = getopt.optarg; break;
        case 'x': extension = getopt.optarg; break;
        case 'X': extensionArgs = getopt.optarg; break;
	default: usage();
    }
  }

  if ((logfile_name == "") || (saveFeaturesToDatabase && (database_name == "")) ||
      ((saveFeaturesToFile) && (savefilename== "")) ||
      ((saveC5PassLeft) && (saveC5PassLeftFile=="")) ||
      ((saveC5PassRight) && (saveC5PassRightFile==""))) {
    usage();
  }

  // open file
  if((infp=fopen(logfile_name.c_str(),"rb"))==0) {
    cerr << "Cannot open " << logfile_name <<endl;
    exit(IO_ERROR);
  }

  if (verbose) {
    cout << "Start reading logfile: " << logfile_name << endl;
  }

  // read header:
  read_header(infp);


  // main loop:
  // read all infos one by one
  
  if(logversion==2){
     while(fread((char *) (&m_tBuf.mode), sizeof(m_tBuf.mode), 1, infp)	!= 0){

       switch(ntohs(m_tBuf.mode)){
    	case SHOW_MODE:
				// new showstruct, save old one
    	if (verbose) {
		cout << "\n\n" << endl;
      }
      countingSlices++;
      if(createSlice){
	int numberSlices=slices.size();
	if(numberSlices>0){
		playerinfo=new playerinfo_t[22];
		ballinfo=new ballinfo_t;
		createInfos(showinfo, short_showinfo, msginfos, playerinfo, ballinfo);
					// sets player- and ballinfo
		playmode=(int)showinfo.pmode;
		
	  	neu=new TimeSlice(playerinfo,ballinfo,servertime,
		    slices[numberSlices-1], verbose, messagesInLogfile, logversion,
		    &teamNameLeft, &teamNameRight, playmode);
	  // fill with values and link backwards
	}else{
		playerinfo=new playerinfo_t[22];
		ballinfo=new ballinfo_t;
		createInfos(showinfo, short_showinfo, msginfos, playerinfo, ballinfo);
					// sets player- and ballinfo
		playmode=(int)showinfo.pmode;

	  	neu=new TimeSlice(playerinfo, ballinfo, servertime,
			    NULL, verbose, messagesInLogfile, logversion,
			    &teamNameLeft, &teamNameRight, playmode);	
	  // fill with values,
	  // linking backwards is not possible,
	  // because it is the first slice
	}
	if(!neu){
	  cerr << "Unable to allocate new slice"
	       << endl;
	  exit(1);
	}
	slices.push_back(neu);	// put into vector
      }
      msginfos.clear(); // Set length of message-vector to zero
      // read showstruct
      	fread((showinfo_t *) (&showinfo),sizeof(showinfo),1,infp);
      	createSlice=true;
      	lastIsShowstruct=true;
      	break;
    case MSG_MODE:
	// read message
	messagesInLogfile=true;
	lastIsShowstruct=false;
	msg=new msginfo_t; 
      	if(!msg){
		cerr << "Not enough mem for new message!" << endl;
		exit(1);
      	}
      	fread((char *) (&(msg->board)), sizeof(msg->board),1,infp);
      	// read boardinfo
      	fread((char *) (&length), sizeof(length),1,infp);
      	// read length
      	//fgets(msg->message, length, infp);
	fread((char *) (&(msg->message)), sizeof(char),(signed short)ntohs(length),infp);
      	if (verbose) {
		cout << "message: " << msg->message;
      	}
      	msginfos.push_back(msg);				
      	//fread((char *) (&character), 1,1,infp);			
      	// skip 0
      	break;
    	default:
      		cerr << "Mode is wrong." << endl;
      		exit(1);
    	} // of switch
  	} // of while !EOF
}else if(logversion==3){

// ################################# NEW VERSION ################################


     while(fread((char *) (&m_tBuf.mode), sizeof(m_tBuf.mode), 1, infp)	!= 0){

       switch(ntohs(m_tBuf.mode)){
    	case SHOW_MODE:
					// new showstruct, save old one
    		if (verbose) {
			cout << "\n\n" << endl;
		}
      		countingSlices++;
      		if(createSlice){
			int numberSlices=slices.size();
			if(numberSlices>0){
				playerinfo=new playerinfo_t[22];
				ballinfo=new ballinfo_t;
				createInfos(showinfo, short_showinfo, msginfos, playerinfo,
									ballinfo);
						//sets player- and ballinfo
	  			neu=new TimeSlice(playerinfo, ballinfo,servertime,
			    	     slices[numberSlices-1], verbose, messagesInLogfile,
				     	logversion,&teamNameLeft, &teamNameRight,playmode);
	  			// fill with values and link backwards
			}else{
				playerinfo=new playerinfo_t[22];
				ballinfo=new ballinfo_t;
				createInfos(showinfo, short_showinfo, msginfos, playerinfo,
									ballinfo);
						//sets player- and ballinfo
	  			neu=new TimeSlice(playerinfo, ballinfo,servertime,
			    		NULL, verbose, messagesInLogfile, logversion,
					&teamNameLeft, &teamNameRight, playmode);
	  				// fill with values,
	  				// linking backwards is not possible,
	  				// because it is the first slice
			}
			if(!neu){
	  			cerr << "Unable to allocate new slice" << endl;
	  			exit(1);
			}
			slices.push_back(neu);	// put into vector
      		}
      		msginfos.clear(); // Set length of message-vector to zero
      		// read showstruct
      		fread((short_showinfo_t2*)(&short_showinfo),sizeof(short_showinfo),1,infp);
      		createSlice=true;
      		lastIsShowstruct=true;
		break;
    	case MSG_MODE:
		// read message
		messagesInLogfile=true;
		lastIsShowstruct=false;
		msg=new msginfo_t; 
      		if(!msg){
			cerr << "Not enough mem for new message!" << endl;
			exit(1);
      		}
	      	fread((char *) (&(msg->board)), sizeof(msg->board),1,infp);
		      	// read boardinfo
	      	fread((char *) (&length), sizeof(length),1,infp);
      			// read length
		fread((char *) (&(msg->message)), sizeof(char),
		     (signed short)ntohs(length),infp);
      		if (verbose) {
			cout << "message: " << msg->message;
	      	}
      		msginfos.push_back(msg);
		break;
	case PM_MODE:
		//if(verbose) cout << "PM_Mode" << endl;
		char playmodechar;
	      	fread((char *) (&playmodechar), sizeof(playmodechar),1,infp);
		playmode=(int)playmodechar;
		if(verbose) cout << "Playmode: " << playmode << endl;
		break;
	case TEAM_MODE:
		//if(verbose) cout << "Team_Mode" << endl;
	      	fread((team_t *) (&teamleft), sizeof(teamleft),1,infp);
	      	fread((team_t *) (&teamright), sizeof(teamright),1,infp);
		teamNameLeft=string(teamleft.name);
		teamNameRight=string(teamright.name);
		scoreleft=(signed short)ntohs(teamleft.score);
		scoreright=(signed short)ntohs(teamright.score);
		break;
	case PARAM_MODE:
	      	fread((server_params_t *) (&server_params), sizeof(server_params),1,infp);
		break;
	case PPARAM_MODE:
	      	fread((player_params_t *) (&player_params), sizeof(player_params),1,infp);
		break;	
	case PT_MODE:
	      	fread((player_type_t *) (&player_type), sizeof(player_type),1,infp);
		// 2do store this in an array of player types!!!
		// ...
		break;
	default:
		cerr << "Yo momma! Wrong mode in logfile: " << ntohs(m_tBuf.mode) << endl;
		exit(1);
       } // of switch
     } // of while !EOF

// ################################# ENDE NEUE VERSION ##########################
}


  // if showinfo is last entry, save one more TimeSlice
  if(lastIsShowstruct){
    int numberSlices=slices.size();

    if(numberSlices>1){
	playerinfo=new playerinfo_t[22];
	ballinfo=new ballinfo_t;
	createInfos(showinfo, short_showinfo, msginfos, playerinfo, ballinfo);
					// sets player- and ballinfo
      	neu=new TimeSlice(playerinfo, ballinfo,servertime,slices[numberSlices-1], verbose,
			messagesInLogfile, logversion,&teamNameLeft, &teamNameRight,
			playmode);
        slices.push_back(neu);
		// fill with values and link backwards
    }else{
	playerinfo=new playerinfo_t[22];
	ballinfo=new ballinfo_t;
	createInfos(showinfo, short_showinfo, msginfos, playerinfo, ballinfo);
					// sets player- and ballinfo
        neu=new TimeSlice(playerinfo, ballinfo,servertime, NULL, verbose,
			messagesInLogfile, logversion, &teamNameLeft,&teamNameRight,
			playmode);
        slices.push_back(neu);
    }
  }
  
  
  if(verbose){
 	cout << teamNameLeft << " vs. " << teamNameRight << endl;
 
	countCycles(slices);

	if(logversion==2){
  	    // print goalscore of last showinfo:
	    cout << "Goals: " << (signed short)ntohs(showinfo.team[0].score)
	     << " : " << (signed short)ntohs(showinfo.team[1].score) << "\n\n" << endl;
	}

	if(logversion==2){
		cerr << (signed short)ntohs(showinfo.team[0].score)-
	        (signed short)ntohs(showinfo.team[1].score)  << endl;
	}else if(logversion==3){
		cout << scoreleft << ":" << scoreright << endl;
		cerr << scoreleft-scoreright << endl;
	}
  }

  if (saveRegionalData) {	//The regions must be created before the scaling
   for (int k=0; k<slices.size(); k++) {
      slices[k]->createRegions();         
    }
  }

	/*
	cout << "About to start foilPassDribbleShoot" << endl;
	cout << "positives in 90° for left:" << endl;
	slices[slices.size()-1]->foilPassDribbleShoot(1800, passevent, LEFT, true, 90);
	cout << "negatives in 90° for left:" << endl;
	slices[slices.size()-1]->foilPassDribbleShoot(1800, passevent, LEFT, false, 90);
	cout << "Finished foilPassDribbleShoot" << endl;
	*/

	if(extension=="foilpass"){	
		cout << "About to start FOIL." << endl;
		Foil *foilobj=new Foil(extensionArgs.c_str(), "", true); // start FOIL
		cout << "Sending data to FOIL." << endl;
		foilobj->go(LEFT, slices[slices.size()-1], "pass", passevent);
		cout << "FOIL done." << endl;
	}

  // save C5 Data before scaling!
  if(saveC5PassLeft)
  	saveC5PassData(slices, saveC5PassLeftFile, false, saveRegionalData);
  if(saveC5PassRight)
  	saveC5PassData(slices, saveC5PassRightFile, true, saveRegionalData);


// START TEMPORARY BLOCK
/*
global_pos mean,bmean,dev,bdev;
short theside=RIGHT;
short factor=(theside==LEFT?1:-1);
slices[slices.size()-1]->getTeamPlayerMeanAndDeviationXY(6000, theside, 0, mean, dev,bmean,bdev);
cout << "Position analysis for " <<(theside==LEFT?"left":"right")
     <<" team (normalized to left-hand):" << endl;
cout << "Absolute x-mean of all players: " << (mean.x*factor) << "\n"
     << "Absolute y-mean of all players: " << (mean.y*factor) << "\n"
     << "Absolute x-deviation of all players: " << dev.x << "\n"
     << "Absolute y-deviation of all players: " << dev.y << "\n" << endl;
cout << "Ballrelative x-mean of all players: " << (bmean.x*factor) << "\n"
     << "Ballrelative y-mean of all players: " << (bmean.y*factor) << "\n"
     << "Ballrelative x-deviation of all players: " << bdev.x << "\n"
     << "Ballrelative y-deviation of all players: " << bdev.y << endl;
*/
    double pmkmean, pmkdev;
    slices[slices.size()-1]->meanAndDeviationOfPMKOfBallAndPlayers(6000, LEFT, true,
    		pmkmean, pmkdev);
    cout << "Players of side LEFT have mean PMK with ball-x: " << pmkmean
    	 << " and deviation: " << pmkdev << endl;
    
// END TEMPORARY BLOCK

  // Collin: Ich habe meine Extension hier eingebaut und nicht in dem von Dir weiter
  // unten eingeführten Extensionblock, damit ich mir das Zurückskalieren spare.
  if(extension == "rectgrid"){
      int slice_number = atoi(extensionArgs.substr(0, extensionArgs.find(',')).c_str());
      int timeframe =    atoi(extensionArgs.substr(extensionArgs.find(',') + 1).c_str());
      int playerunum=    atoi(extensionArgs.substr(extensionArgs.find(':') + 1).c_str());
      int playerside=    atoi(extensionArgs.substr(extensionArgs.find('#') + 1).c_str());
      int latticemode=   atoi(extensionArgs.substr(extensionArgs.find('+') + 1).c_str());
      string filename = "";
      filename = extensionArgs.substr(extensionArgs.find('?') + 1);
    		// LEFT, RIGHT
    
      if(slices[slice_number]!=NULL){
      	   RectGrid* grid=slices[slice_number]->getLatticeForPlayer(playerunum,
                                                    playerside, timeframe, latticemode);
	   if (verbose) {
	     cout << "Starting at slice " << slice_number << " and going back "
		  << timeframe << endl;
	     cout << "\nRectGrid for player # " << playerunum;
	     if(playerside==LEFT)
	       cout << " of side left" << endl;
	     else
	       cout << " of side right" << endl;
	     grid->printLattice();
	   }
	     
	if(latticemode==OPP_HAS_BALL){
		regionOfSections* defenseReg=slices[slice_number]->getDefenseOffenseRegion(
					playerunum,playerside,timeframe,OPP_HAS_BALL);
		if (verbose) {
		  cout << "Defense position in section(s): " << endl;
		  cout << defenseReg->left << ", " << defenseReg->top << "      "
		       << defenseReg->right << ", " << defenseReg->bottom << " Conf: "
		       << defenseReg->confidence << endl;
		}
	}
	if(latticemode==WE_HAVE_BALL){	
		regionOfSections* offenseReg=slices[slice_number]->getDefenseOffenseRegion(
					playerunum,playerside,timeframe, WE_HAVE_BALL);
		if (verbose) {
		  cout << "Offense position in section(s): " << endl;
		  cout << offenseReg->left << ", " << offenseReg->top << "      "
		       << offenseReg->right << ", " << offenseReg->bottom << " Conf: "
		       << offenseReg->confidence <<  endl;
		}
	}
	     
	Teampositions *teampositions=
		slices[slice_number]->getTeampositions(playerside,timeframe,latticemode);
	
	// Nur zu Testzwecken: 
	if (verbose) {
	  cout << "Teampositions for specified mode: " << endl;
	  for(int y=0; y<VER_SECTIONS; y++){
	    for(int x=0; x<HOR_SECTIONS; x++){
	      for(int elem=0; elem<(*teampositions)[x][y].size(); elem++){
		vector<int> auxvector=(*teampositions)[x][y];
		cout << ":" <<(int)(auxvector[elem]);
	      }
	      for(int space=5; space>(*teampositions)[x][y].size(); space--)
		cout << "  ";
	    }
	    cout << endl;
	  }
	}

	if (filename != "") {
	  FILE* file = fopen(filename.c_str(), "w");
	  fprintf(file, "=start\n");
	  for (int y = 0; y < VER_SECTIONS; y++) {
	    for (int x = 0; x < HOR_SECTIONS; x++) {
	      vector<int> auxvector=(*teampositions)[x][y];
	      for (int elem = 0; elem < (*teampositions)[x][y].size(); elem++) {
		fprintf(file, "%d ", (int)(auxvector[elem]));
	      }
	      fprintf(file, "\n");
	    }
	    fprintf(file, "=\n");
	  }
	  fprintf(file, "=end\n");
	  fclose(file);
	}
      }
  } //if (extension == "rectgrid")
  else if (extension == "foil") {
    ofstream out(extensionArgs.c_str(), ios::app);
    for (int k = 0; k < slices.size(); k++) {
      out << slices[k]->toFoil() << endl;
    }
    out << "." << endl;
    out.close();
  }

  // scale all values to 0..1000
  for (int k=0; k<slices.size(); k++) {
    if (saveRegionalData) {
      slices[k]->scaleAllRegional1000();
    }else{
    	slices[k]->scaleAll1000();
    }
  }
  
  // saving TimeSlice-Vector into database
//  if(saveFeaturesToDatabase)
//  	saveToDatabase(slices, database_name);
  if(saveFeaturesToFile)
  	saveToFile(slices, savefilename, appendToFile, saveRegionalData, saveEmpty);


  if (extension != "") {
    if (extension == "showAveragePosition") {
      if (extensionArgs == "") {
	usage();
      }
      int slice_number, timeframe;
      if (extensionArgs == "all") {
	slice_number = slices.size() - 1;
	timeframe = slices.size();
      }
      else {
	slice_number = atoi(extensionArgs.substr(0, extensionArgs.find(',')).c_str());
	timeframe = atoi(extensionArgs.substr(extensionArgs.find(',') + 1).c_str());
      }
      if (saveRegionalData) {
	slices[slice_number]->undoRegionalScale();
      }
      else {
	slices[slice_number]->undoScale();
      }
      for (int l = 0; l < 2; l++) {
	if (l == 0) {
	  cout << "LEFT:\n";
	}
	else {
	  cout <<  "RIGHT:\n";
	}
	global_pos averagePositions[11];
	for (short k = 0; k < 11; k++) {
	  averagePositions[k] = slices[slice_number]->calculateAveragePosition(k + 1, l == 0 ? LEFT : RIGHT,
									       timeframe);
	  //classes[k] = slices[slice_number]->classifyPlayer(averagePositions[k], k, l == 0 ? LEFT : RIGHT);
	}
	//short goalie = slices[slice_number]->findGoalie(averagePositions, l == 0 ? LEFT : RIGHT);
	short* classes = slices[slice_number]->classifyPlayersRelative(averagePositions);
	for (short k = 0; k < 11; k++) {
	  printf("  %d:\t(%.2f, %.2f)\t=>\t%s\n", k + 1, averagePositions[k].x, averagePositions[k].y, 
		 slices[slice_number]->classToString(classes[k]).c_str());
	}
      }
    }
    else if (extension == "dump") {
      string filePrefix = extensionArgs;
      for (int i = 0; i < (slices.size() - 10); i+=10) {
	short partner = slices[i]->findBestPassPartner(1, LEFT);
	slices[i]->setPlayerAttribute(0, COLOR_ORANGE);
	slices[i]->setPlayerAttribute(partner, COLOR_YELLOW);
	string dummy;
	int time = slices[i]->getServerTime();
	if (time < 10) {
	  dummy = "000";
	}
	else if (time < 100) {
	  dummy = "00";
	}
	else if (time < 1000) {
	  dummy = "0";
	}
	slices[i]->dump(filePrefix + dummy + itoa(time));
      }
    }
  }


  if (verbose) {
    cout << "Finished." << endl;
  }
  return 0;
}






