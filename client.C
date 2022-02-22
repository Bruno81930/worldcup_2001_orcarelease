/* -*- Mode: C++ -*- */

/* client.C
 * CMUnited99 (soccer client for Robocup99)
 * Peter Stone <pstone@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Peter Stone
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy and distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 * For more information, please see http://www.cs.cmu.edu/~robosoccer/
 */


#include "client.h"
#include "types.h"
#include "netif.h"
#include "Memory.h"
#include "parse.h"
#include "utils.h"
#include "behave.h"
#include "communication/communicate.h"
#include <deque>
#include <string>

void      send_initialize_message();
void      parse_initialize_message(char *);
Bool      wait_for_signals(sigset_t *);
sigset_t  init_handler();
void      sigio_handler(); 
void      sigalrm_handler();
void      send_action();
void      resend_last_action();

/* Global variables -- don't want to reallocate buffers each time */

sigset_t sigiomask, sigalrmask;

//Memory  Global_Mem;
//Memory *const Mem = &Global_Mem;
Memory *Mem;

char     recvbuf[MAXMESG];	
char     sendbuf[MAXMESG];	

char     *GLOBAL_sense_body_message = "(sense_body)";

int      alrsigs_since_iosig=0;
//deque<string> udp_buffer(10);
/****************************************************************************************/

int main(int argc, char *argv[])
{
  Mem = new Memory();
  if ( Mem == NULL ){
    my_error("couldn't allocate Mem");
    exit(0);
  }

  Mem->GetOptions(argc,argv);

  Socket sock = init_connection(Mem->SP_host,Mem->SP_port);

  Mem->sock = &sock;

  if(Mem->sock->socketfd == -1) {
    cerr << "Can't open connection for player" << endl;
    exit(-1);
  }

  send_initialize_message();

  if ( wait_message(recvbuf, Mem->sock) == 0 )
    my_error("wait_message failed");

  parse_initialize_message(recvbuf);

  Mem->Initialize();

  sigset_t sigfullmask = init_handler();	      

  while ( Mem->ServerAlive == TRUE && wait_for_signals(&sigfullmask) );

  if (Mem->sock->socketfd != -1) close_connection(Mem->sock);

  printf("Shutting down player %d\n",Mem->MyNumber);
}




/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

/* Send initialize message */
void send_initialize_message()
{
  if (Mem->IP_reconnect)
    sprintf(sendbuf, "(reconnect %s %d)", Mem->MyTeamName, Mem->IP_reconnect);
  else if ( Mem->CP_goalie == TRUE && Mem->SP_version >= 4.00){
    sprintf(sendbuf, "(init %s (version %.2f) (goalie))", Mem->MyTeamName, Mem->SP_version);
  }
  else
    sprintf(sendbuf, "(init %s (version %.2f))", Mem->MyTeamName, Mem->SP_version);

  if(send_message(sendbuf, Mem->sock) == -1) abort(); 
}


/****************************************************************************************/

/* Parse initialize message */
void parse_initialize_message(char *recvbuf){
  char mode[100];
  if ( !(strncmp(recvbuf,"(init",4)) ) {
    /* It's an init msg */
    sscanf(recvbuf,"(init %c %d %[^)]",&Mem->MySide, &Mem->MyNumber, mode);
    Mem->ServerAlive = TRUE;
  }
  else if ( !(strncmp(recvbuf,"(reconnect",4)) ) {
    /* It's a reconnect msg */
    sscanf(recvbuf,"(reconnect %c %[^)]",&Mem->MySide, mode);
    Mem->MyNumber = Mem->IP_reconnect;
    printf("reconnecting to %d on side %c!\n",Mem->MyNumber,Mem->MySide);
    Mem->ServerAlive = TRUE;
  }
  else {
    my_error("Didn't get an init message: '%s'",recvbuf);
    Mem->ServerAlive = FALSE;
  }

  if ( Mem->CP_goalie && Mem->FP_goalie_number != Mem->MyNumber )
    my_error("goalie number inconsistent with me being goalie");

  if ( !Mem->CP_goalie && Mem->FP_goalie_number == Mem->MyNumber )
    my_error("I should be the goalie");
  
  if ( mode[0] == 'b' ){ /* Before_kick_off */ 
    Mem->SetPlayMode(PM_Before_Kick_Off);
    if ( Mem->MySide == 'l' )
      Mem->KickOffMode = KO_Mine;
    else 
      Mem->KickOffMode = KO_Theirs;
  }
  else                  /* Act as if the game's in progress */
    Mem->SetPlayMode(PM_Play_On);
}



/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

/* set time interval between the sensor receiving and command sending */ 
inline void set_timer() {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  itv.it_interval.tv_usec = Mem->TimerInterval * 1000;
  itv.it_value.tv_sec = 0;
  itv.it_value.tv_usec = Mem->TimerInterval * 1000;
  setitimer(ITIMER_REAL, &itv, NULL);
}

inline void set_timer(int usec) {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  itv.it_interval.tv_usec = Mem->TimerInterval * 1000;
  itv.it_value.tv_sec = 0;
  itv.it_value.tv_usec = usec;
  setitimer(ITIMER_REAL, &itv, NULL);
}


/****************************************************************************************/

sigset_t init_handler() { 
  sigemptyset(&sigalrmask);
  sigaddset(&sigalrmask, SIGALRM);
  sigemptyset(&sigiomask);
  sigaddset(&sigiomask, SIGIO);
  
  struct sigaction sigact;
  sigact.sa_flags = 0;
  sigact.sa_mask = sigiomask;

#ifdef Solaris
  sigact.sa_handler = (void (*)(int))sigalrm_handler; 
#else
  sigact.sa_handler = (void (*)(int))sigalrm_handler; 
#endif

  sigaction(SIGALRM, &sigact, NULL);
  sigact.sa_mask = sigalrmask;

#ifdef Solaris
  sigact.sa_handler = (void (*)(int))sigio_handler; 
#else
  sigact.sa_handler = (void (*)(int))sigio_handler; 
#endif

  sigaction(SIGIO, &sigact, NULL);
  set_timer();
  sigprocmask(SIG_UNBLOCK, &sigiomask, NULL);
  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);

  sigset_t sigsetmask;
  sigprocmask(SIG_BLOCK, NULL, &sigsetmask);   /* Get's the currently unblocked signals */
  return sigsetmask;   
}


/****************************************************************************************/

/* suspend the process until one of the signals comes through */
/* could check for situation to kill client, return FALSE     */
/* i.e. too many actions with no sensory input coming in      */
Bool wait_for_signals(sigset_t *mask){
  sigsuspend(mask);
  return TRUE;
}


/****************************************************************************************/

/* SIGIO handler: receive and parse messages from server */
void sigio_handler() {
  sigprocmask(SIG_BLOCK, &sigalrmask, NULL);  
  int counter = 0;

  Time StartTime = Mem->CurrentTime;

  while (receive_message(recvbuf, Mem->sock) == 1) {
  //  printf("message: %s", recvbuf);
    Parse(recvbuf);
    counter++;
  }

  if ( Mem->CurrentTime - StartTime > 1 && StartTime.s == 0 && Mem->CurrentTime.s == 0 )
    my_error("Received several steps at once -- missing action ops!!! (%d %d)",
	     StartTime.t,StartTime.s);

  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);  

  alrsigs_since_iosig=0;

  // if (counter>1) printf("Got %d messages\n",counter);
}


/****************************************************************************************/

/* SIGALRM handler: extract and send first command in commandlist */
void sigalrm_handler() {
  sigprocmask(SIG_BLOCK, &sigiomask, NULL);  

  if ( Mem->LastInterruptTime != Mem->CurrentTime ){ 
    if ( !Mem->ClockStopped && Mem->CurrentTime-1 != Mem->LastInterruptTime ) my_error("Missed a cycle??"); 
    if ( !Mem->ClockStopped && Mem->InterruptsThisCycle < Mem->CP_interrupts_per_cycle-1 )
	my_error("Only %d interrupts last cycle",Mem->InterruptsThisCycle);
    Mem->LastInterruptTime = Mem->CurrentTime; 
    Mem->InterruptsThisCycle = 0; 
    //cout << endl;
  } 
  Mem->InterruptsThisCycle++; 

  ///cerr << "PRIORITY SAY MESSAGE: " << Mem->MyNumber << endl;
// PRIORITY COMMUNICATION
  if ( Mem->InterruptsThisCycle >= Mem->MyCommunicationInterrupt())
  {
	  if ( Mem->Say.time < Mem->CurrentTime-1 )
	  {
		  if(Mem->PlayedPass()) communicate();
	  }
	  if ( Mem->Say.valid() ){
		  if ( Mem->Say.time < Mem->CurrentTime-1 ) my_error("old say");
		  
		  ////cerr << "PRIORITY " << Mem->Say.priority << " SAY MESSAGE (";
		  ////cerr << Mem->MyNumber << ", " << Mem->CurrentTime.t << "." << Mem->CurrentTime.s << " i" << Mem->InterruptsThisCycle << ")" << endl;
		  send_message( Mem->Say.command, Mem->sock );
		  Mem->Say.type = CMD_none; /* so it's no longer valid */
		  Mem->Say.priority--;
	  }
  }
// END PRIORITY COMMUNICATION
  
  //cout << ".";

  /* Don't act until near the end of a cycle */
  /* there's some leeway in case there aren't enough interrupts in the cycle */
  if ( !Mem->ClockStopped && Mem->CP_interrupts_per_cycle - Mem->InterruptsThisCycle >
       Mem->CP_interrupts_left_to_act)
    return;


  if (Mem->ClockStopped)
    Mem->StoppedClockMSec += Mem->TimerInterval;

  if (alrsigs_since_iosig++ > Mem->CP_interrupts_per_cycle * 20){
	  Mem->ServerAlive = FALSE;
	  return;
  }
  
  /* If a sight is definitely coming every cycle, don't act until getting the sight */
  /* Don't wait if we're in transition to a longer sight interval                   */
  if ( Mem->MySightInterval() < Mem->SP_simulator_step && Mem->LastSightTime < Mem->CurrentTime &&
       !((Mem->ChangeView.valid() || Mem->ChangeView.valid(Mem->CurrentTime-1)) &&
	 (Mem->ChangeView.width > Mem->ViewWidth || Mem->ChangeView.qual > Mem->ViewQuality)) )   {
    Mem->LogAction4(200,"Waiting for sight... (%d %d)",
		    Mem->ChangeView.valid(),Mem->ChangeView.valid(Mem->CurrentTime-1));
    return;
  }

  if ( Mem->CurrentTime > Mem->LastActionOpTime ){

    if ( !Mem->ClockStopped && Mem->CurrentTime-1 != Mem->LastActionOpTime && Mem->LastActionOpTime != 0 )
      my_error("Missed a cycle!!  (%d %d)",Mem->LastActionOpTime.t,Mem->LastActionOpTime.s);

    if ( Mem->NewSight ) Mem->FirstActionOpSinceLastSight = TRUE;

/*if ( 0 && Mem->MyCurrentFormationType() != FT_433 ){ my_error("?? pre-update %d\n"); dump_core("dump"); }*/

    Mem->update();
    behave();

/*if ( 0 && Mem->MyCurrentFormationType() != FT_433 ){ my_error("?? post-behave %d\n"); dump_core("dump"); }*/

    Mem->LastActionOpTime = Mem->CurrentTime;
    Mem->FirstActionOpSinceLastSight = FALSE;
  }

/*if ( 0 && Mem->MyCurrentFormationType() != FT_433 ){ my_error("?? pre-action %d\n"); dump_core("dump"); }*/

  /* Whether or not to wait between sending network packets is an interesting decsision.
     In versions of the server after 5.23, the server reads *all* commands off a socket at
     at every time step, so we could try to send all of our commands as soon as they are
     ready. However, on an ethernet, this can lead to lots of collisions and such, so it
     may degrade network performance
     To send everything without waiting, comment in this next line */
  //#define SEND_ALL_AT_ONCE
  
  /* the server now accepts multiple commands together  (after 5.23)
  if (0 && Mem->TooSoonForAnotherSend()) {
    Mem->LogAction2(200, "It's too soon to send another command. Waiting");
  } else {
  */
  
  if ( Mem->Action->valid() ) {
    send_action();
  }
#ifndef SEND_ALL_AT_ONCE
  else
#endif	
  if ( Mem->ResendNeeded() ){
    resend_last_action();
  }
#ifndef SEND_ALL_AT_ONCE
  else
#endif	
  if ( Mem->TurnNeck.valid() ){
    if ( Mem->TurnNeck.time < Mem->CurrentTime-1 ) my_error("old turn_neck");

    send_message( Mem->TurnNeck.command, Mem->sock );
    Mem->turn_necks++;
    Mem->TurnNeck.type = CMD_none; /* so it's no longer valid */
  }
#ifndef SEND_ALL_AT_ONCE
  else
#endif	
  if ( Mem->ChangeView.valid() ){
    if ( Mem->ChangeView.time < Mem->CurrentTime-1 ) my_error("old change_view");

    send_message( Mem->ChangeView.command, Mem->sock );
    Mem->ChangeView.type = CMD_none; /* so it's no longer valid */
  }
#ifndef SEND_ALL_AT_ONCE
  else
#endif	
      if ( Mem->SP_sense_body_step > Mem->SP_simulator_step )  {
	  /* only if we won't get a sense_body each cycle by default */
	  my_error("Sending sense_body");
	  send_message(GLOBAL_sense_body_message,Mem->sock);  
      } 
#ifndef SEND_ALL_AT_ONCE
      else
#endif	
	  if ( Mem->Say.valid() && Mem->Say.priority <= 0 ){
	      ////if ( Mem->Say.time < Mem->CurrentTime-1 ) my_error("old say");
	      
  ////cerr << "NON-PRIORITY SAY ("; cerr << Mem->MyNumber << ", " << Mem->CurrentTime.t << "." << Mem->CurrentTime.s << " i" << Mem->InterruptsThisCycle << ")" << endl;
	  ////send_message( Mem->Say.command, Mem->sock );
	  ////Mem->Say.type = CMD_none; /* so it's no longer valid */
	  }
  sigprocmask(SIG_UNBLOCK, &sigiomask, NULL);  
}




/****************************************************************************************/
/****************************************************************************************/
/****************************************************************************************/

/* insert turn/dash/kick commands in commandlist */

void turn(AngleDeg ang) 
{
  NormalizeAngleDeg(&ang); 

  /* turn so that the actual turn is the desired turn */
  /* pos.rotate( ang/(1.0 + SP_inertia_moment * MySpeed()) ); */
  if ( Mem->MyVelConf() ) ang *= (1 + Mem->SP_inertia_moment * Mem->MySpeed());

  if ( ang > Mem->SP_max_moment ) ang = Mem->SP_max_moment;
  if ( ang < Mem->SP_min_moment ) ang = Mem->SP_min_moment;

  if (ang < .1 && ang > -.1) {
    Mem->Action->type = CMD_none;
    return;          /* No turn           */
  }

  Mem->Action->type = CMD_turn;
  Mem->Action->power = 0;
  Mem->Action->angle = ang;
  Mem->Action->time = Mem->CurrentTime;

  if (Mem->TurnNeck.time == Mem->CurrentTime){ /* Already did a turn_neck     */
    /* adjust as much as possible for the turn */
    Mem->TurnNeck.angle -= ang;
    if ( Mem->MyNeckRelAng() + Mem->TurnNeck.angle < Mem->SP_min_neck_angle )
      Mem->TurnNeck.angle = Mem->SP_min_neck_angle - Mem->MyNeckRelAng();
    if ( Mem->MyNeckRelAng() + Mem->TurnNeck.angle > Mem->SP_max_neck_angle )
      Mem->TurnNeck.angle = Mem->SP_max_neck_angle - Mem->MyNeckRelAng();
  }

  sprintf(Mem->Action->command,"(turn %.2f)", ang);
  Mem->LogAction3(150, "turn %f", ang);
}  

/****************************************************************************************/

void dash(float power) 
{
  if ( Mem->PlayMode == PM_Before_Kick_Off ) return;

  if (power > Mem->SP_max_power) my_error("Can't dash that fast: %.1f",power); 
  if (power < Mem->SP_min_power) my_error("Can't dash that 'slow': %.1f",power);

  /* Factor for stamina--don't dash more than stamina or more than necessary to get you to max speed */
  Mem->VerifyDash(&power);

  if (fabs(power) < 1){
    Mem->Action->type = CMD_none;
    return;                         /* No dash           */
  }
  
  Mem->Action->type = CMD_dash;
  Mem->Action->power = power;
  Mem->Action->angle = 0;
  Mem->Action->time = Mem->CurrentTime;

  sprintf(Mem->Action->command, "(dash %.2f)", power);
  Mem->LogAction3(150, "dash %f", power);
}

/****************************************************************************************/

void kick(float power, AngleDeg dir) 
{
  if ( !(Mem->BallKickable()) ) my_error("Can't kick a ball that's too far away");

  if ( Mem->PlayMode == PM_Before_Kick_Off ) return;

  if (power > Mem->SP_max_power) my_error("Can't kick that hard");
  if (power < 0 ) my_error("Can't kick < 0");
  NormalizeAngleDeg(&dir);

  Mem->Action->type = CMD_kick;
  Mem->Action->power = power;
  Mem->Action->angle = dir;
  Mem->Action->time = Mem->CurrentTime;

  sprintf(Mem->Action->command, "(kick %.2f %.2f)", power, dir);
  Mem->LogAction4(150, "kick %f %f", power, dir);
}

/****************************************************************************************/

void goalie_catch(AngleDeg dir) 
{
  if ( !(Mem->BallCatchable()) ) my_error("Can't catch a ball that's too far away");
  if ( !Mem->CP_goalie ) my_error("Only goalies can catch");

  if ( Mem->PlayMode == PM_Before_Kick_Off ) return;

  NormalizeAngleDeg(&dir);

  Mem->Action->type = CMD_catch;
  Mem->Action->power = 0;
  Mem->Action->angle = dir;
  Mem->Action->time = Mem->CurrentTime;

  sprintf(Mem->Action->command, "(catch %.2f)", dir);
  Mem->LogAction3(150, "catch %f",  dir);
  //cout << "BALL CAUGHT" << endl;
}


/****************************************************************************************/

void move(float x, float y) 
{
  if ( ! (Mem->PlayMode == PM_Before_Kick_Off || 
	  (Mem->CP_goalie && Mem->PlayMode == PM_My_Goalie_Free_Kick)) )
    my_error("Can only move in before kickoff mode (or after goalie catch)");

  /* Perhaps here convert to a position on the field */
  if ( fabs(y) > Mem->SP_pitch_width/2 || x > 0 || x < -Mem->SP_pitch_length/2 )
    my_error("Must move to a place on the pitch");

  if ( Mem->PlayMode == PM_My_Goalie_Free_Kick && !Mem->OwnPenaltyArea.IsWithin(Vector(x,y)) )
    my_error("Must move to a place within penalty area");

  Mem->Action->type = CMD_move;
  Mem->Action->x = x;
  Mem->Action->y = y;
  Mem->Action->time =  Mem->CurrentTime;

  sprintf(Mem->Action->command, "(move %.2f %.2f)", x, y);
  Mem->LogAction4(150, "move %f %f", x, y);
}

/****************************************************************************************/

void disconnect()
{
  Mem->Action->type = CMD_bye;
  Mem->Action->time =  Mem->CurrentTime;

  sprintf(Mem->Action->command, "(bye)");
}

/****************************************************************************************/

void turn_neck(AngleDeg ang) 
{
  NormalizeAngleDeg(&ang); 

  if (ang == 0) {
    Mem->LogAction2(150, "Ignoring turn_neck 0");
    return;
  }  

  if ( ang > Mem->SP_max_neck_moment ) ang = Mem->SP_max_neck_moment;
  if ( ang < Mem->SP_min_neck_moment ) ang = Mem->SP_min_neck_moment;

  if ( Mem->MyNeckRelAng() + ang > Mem->SP_max_neck_angle ) {
    ang = Mem->SP_max_neck_angle - Mem->MyNeckRelAng();
    my_error("Can't turn neck that much");
  }
  
  if ( Mem->MyNeckRelAng() + ang < Mem->SP_min_neck_angle ) {
    ang = Mem->SP_min_neck_angle - Mem->MyNeckRelAng();
    my_error("Can't turn neck that little");
  }
  
  Mem->TurnNeck.type = CMD_turn_neck;
  Mem->TurnNeck.power = 0;
  Mem->TurnNeck.angle = ang;
  Mem->TurnNeck.time = Mem->CurrentTime;  

  sprintf(Mem->TurnNeck.command,"(turn_neck %.2f)", ang);
  Mem->LogAction3(150, "turn_neck %f", ang);
}

/****************************************************************************************/

void change_view(Vqual qual, Vwidth width) 
{
  if ( qual==Mem->ViewQuality  && width==Mem->ViewWidth )
    return;  /* my_error("Nothing to change about view"); */

  Mem->ChangeView.type  = CMD_change_view;
  Mem->ChangeView.qual  = qual;
  Mem->ChangeView.width = width;
  Mem->ChangeView.time  = Mem->CurrentTime;

  char qual_string[10], width_string[10];
  switch (qual){
  case VQ_High: sprintf(qual_string,"high"); break;
  case VQ_Low:  sprintf(qual_string,"low");  break;
  }
  switch (width){
  case VW_Narrow: sprintf(width_string,"narrow"); break;
  case VW_Normal: sprintf(width_string,"normal"); break;
  case VW_Wide:   sprintf(width_string,"wide");   break;
  }

  sprintf(Mem->ChangeView.command, "(change_view %s %s)", width_string, qual_string);
  Mem->LogAction4(150, "change_view %s %s",  width_string, qual_string);
}

/****************************************************************************************/
/* COMMUNICATION */
/****************************************************************************************/

void priority_say(char * message)
{
	Mem->Say.priority = 5;
	say(message);
}

void say(char * message) 
{
    Mem->Say.type  = CMD_say;
    Mem->Say.time  = Mem->CurrentTime;
    //cerr<<"COM_SAY_MESSAGE: "<<Mem->MyNumber<<" :*: "<<message<<endl;
    sprintf(Mem->Say.command, "(say %s )", message);
    //Mem->LogAction3(150, "say %s", message);
}

/****************************************************************************************/
/* END COMMUNICATION **/
/****************************************************************************************/

void send_action()
{	
  if ( !(Mem->Action->valid(Mem->CurrentTime)) )
    my_error("Old action %d %d",Mem->Action->time.t,Mem->Action->time.s);

  send_message(Mem->Action->command, Mem->sock);

  switch (Mem->Action->type){
  case CMD_kick:
    Mem->kicks++;
    Mem->GetBall()->set_past_kick(Mem->LastActionPower(), Mem->LastActionAngle(),
				  Mem->CurrentTime);
    break;
  case CMD_dash: Mem->dashes++; break;
  case CMD_turn: Mem->turns++;  break;
  default: ;
  }

  Command *tmp = Mem->LastAction;
  Mem->LastAction = Mem->Action;
  Mem->Action     = tmp;

  Mem->Action->type = CMD_none; /* So it's invalid */
  Mem->NewAction = TRUE;
}

/****************************************************************************************/

void resend_last_action()
{
  if ( Mem->LastActionType() == Mem->ResendType ){
    my_stamp; printf("resending\n");
    send_message(Mem->LastAction->command, Mem->sock);

    switch (Mem->LastActionType()){
    case CMD_kick: Mem->kicks++;  break;
    case CMD_dash: Mem->dashes++; break;
    case CMD_turn: Mem->turns++;  break;
    default: ;
    }
  }
  else 
    my_error("last action isn't a %d",Mem->ResendType);

  Mem->RequestResend = FALSE;
}
