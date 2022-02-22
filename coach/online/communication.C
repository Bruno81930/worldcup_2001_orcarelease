#include "communication.h"
#include "../types.h"
#include "onlinecoach.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <string>
#include <assert.h>

#define MAX_ALARMS_SINCE_IO 10

extern volatile char sendbuf[MAXLENGTH];	// buffer for coach-messages
extern volatile char receivebuf[MAXLENGTH];	// buffer for messages from the server
extern int verbose;			// print info?
extern Socket m_socket; 		// socket to server
extern bool lastCommandSucceeded; // did we get an "(ok...)" message for the last command?

sigset_t sigiomask, sigalrmask;	
int alrsigs_since_iosig=0;


Socket open_connection(char host[], int port) {
  struct hostent    *host_ent;
  struct in_addr      *addr_ptr;
  struct sockaddr_in    cli_addr;
  int           socket_desc, val;
  Socket        m_socket;

  m_socket.socketfd = -1;  //should change later when creating socket, otherwise error 

  if((host_ent = (struct hostent *)gethostbyname(host)) == NULL) {
    // Check if a numeric address
    if((int)inet_addr(host) == -1){
      exit(-1);
    }
  }
  else{
    addr_ptr = (struct in_addr *) *host_ent->h_addr_list;
    host = inet_ntoa(*addr_ptr);
  }

  sigset_t sigmask;
  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGIO);
  sigprocmask(SIG_BLOCK, &sigmask, NULL);

  // Open UDP socket.
  if( (socket_desc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    return m_socket;   /* Can't open socket. */
  }

  //  Bind any local address.
  bzero((char *) &cli_addr, sizeof(cli_addr));
  cli_addr.sin_family       = AF_INET;
  cli_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
  cli_addr.sin_port     = htons(0);

  if (fcntl(socket_desc, F_SETOWN, getpid()) == -1) return m_socket;
  val = fcntl(socket_desc, F_GETFL, 0);
  val |= O_NDELAY;
  val |= FASYNC;
  fcntl(socket_desc, F_SETFL, val) ;

  if(bind(socket_desc, (struct sockaddr *) &cli_addr,
      sizeof(cli_addr)) < 0){
    return m_socket;  /* Can't bind local address */
  }

   // Fill in the structure with the address of the server.
  m_socket.socketfd = socket_desc;

  bzero((char *) &m_socket.serv_addr, sizeof(m_socket.serv_addr)) ;
  m_socket.serv_addr.sin_family     = AF_INET;
  m_socket.serv_addr.sin_addr.s_addr    = inet_addr(host);
  m_socket.serv_addr.sin_port       = htons(port);

  return m_socket;
}




bool init_to_server(char * teamname){
 	//cout << "bla" << endl;	
  
  sprintf(const_cast<char *>(sendbuf), "(init %s (version 7.00))", teamname);
  if(send_message() == -1) return false;
  return true;
}



int send_message(){
  int messagelength;

  // cout << "bla2" << endl;	
  lastCommandSucceeded=false;
  // if(!sendbuf) cout << "Hmmmm" << endl;
  messagelength = strlen(const_cast<char *>(sendbuf));

  if(verbose){
  	cout << "Sending: " << string(const_cast<char *>(sendbuf)) << endl;
  }
  
  
  if(sendto(m_socket.socketfd, const_cast<char *>(sendbuf), messagelength, 0,
      (struct sockaddr *)&m_socket.serv_addr, sizeof(m_socket.serv_addr)) != messagelength)
    return (-1) ;
  else
    return 0 ;
    
    return (-1);
}



int receive_message(){
  int n;
  socklen_t servlen;
  struct sockaddr_in serv_addr;

  servlen = sizeof(serv_addr);
  n = recvfrom(m_socket.socketfd, const_cast<char *>(receivebuf), MAXLENGTH, 0,
           (struct sockaddr *)&serv_addr, &servlen);
  //cout << "receivebuf: " << *(new string(const_cast<char *>(receivebuf))) << endl;
  if(n < 0){
    if( n == -1 && errno == EWOULDBLOCK){
      receivebuf[0] = '\0' ;
      receivebuf[1] = 'A'; 	// <timo> inserted </timo>
      receivebuf[2] = '\0'; 	// <timo> inserted </timo>
      return 0 ;
    }
    else
      return (-1) ;
  }else{
    assert(n<MAXLENGTH);
    receivebuf[n] = '\0' ;

    m_socket.serv_addr.sin_port = serv_addr.sin_port ;

    if(n == 0)
      return 0 ;
    else
      return 1 ;
  }
}



int wait_message(){
  if (receive_message() == 1){
    	while (receive_message() == 1){};
    	return 1;
  }else{
  	for (int i = 0; i < 40; i++){
    		if (receive_message() == 1) return 1;
    		usleep(100000);
  	}
  }  
  return 0;
}




int parse_init_return(){
  if (!(strncmp(const_cast<char *>(receivebuf),"(init",4))){
  	// server acknowledged initialization
	if(receivebuf[6]=='r')
		return RIGHT;
	else
		return LEFT;
    }else{
	cerr << "Server did not acknowledge init-message. Sent instead: "
	     << *(new string(const_cast<char *>(receivebuf))) << endl;
    	return 0;
  }
}




/* we just use this to recognize when the server dies */
void sigalrm_handler(){
  alrsigs_since_iosig++;
  if(alrsigs_since_iosig > MAX_ALARMS_SINCE_IO){
    	cerr << "Too many alarms between ios from server. Should I Shut down?" << endl;
    	// exit(1);
  }
}




/* SIGIO handler: receive and parse messages from server */
void sigio_handler(){
  sigprocmask(SIG_BLOCK, &sigalrmask, NULL);
  int counter = 0;
  while (receive_message() == 1){
	// messages are in receivebuf now. parse it
	
	if(verbose){ 
	  //cout << "Now parsing: " << string(const_cast<char *>(receivebuf)) << endl;
	}
	parse_server_msgs();
	//if(verbose)
	//cout << "Parsed.\n" << endl;
  }
  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);

  alrsigs_since_iosig = 0;
}


// Brauchen wir den Timer ueberhaupt? Siehe init_handler()
/* set time interval between the sensor receiving and command sending */
inline void set_timer() {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  //itv.it_interval.tv_usec = Mem->CP_alarm_interval * 1000;
  //itv.it_interval.tv_usec = 200 * 1000;
  itv.it_value.tv_sec = 0;
  //itv.it_value.tv_usec = Mem->CP_alarm_interval * 1000;
  itv.it_value.tv_usec = 200 * 1000;
  setitimer(ITIMER_REAL, &itv, NULL);
}

inline void set_timer(int usec) {
  struct itimerval itv;
  itv.it_interval.tv_sec = 0;
  //itv.it_interval.tv_usec = Mem->CP_alarm_interval * 1000;
  itv.it_interval.tv_usec = 200 * 1000;
  itv.it_value.tv_sec = 0;
  itv.it_value.tv_usec = usec;
  setitimer(ITIMER_REAL, &itv, NULL);
}





sigset_t init_handler() {
  sigemptyset(&sigiomask);
  sigaddset(&sigiomask, SIGIO);
  struct sigaction sigact;
  sigact.sa_flags = 0;
  sigact.sa_mask = sigiomask;
  sigact.sa_handler = (void (*)(int))sigalrm_handler; 
  sigaction(SIGALRM, &sigact, NULL);
  sigact.sa_mask = sigalrmask;
  sigact.sa_handler = (void (*)(int))sigio_handler; 
  sigaction(SIGIO, &sigact, NULL);
  // set_timer();
  sigprocmask(SIG_UNBLOCK, &sigiomask, NULL);
  sigprocmask(SIG_UNBLOCK, &sigalrmask, NULL);
  sigset_t sigsetmask;
  sigprocmask(SIG_BLOCK, NULL, &sigsetmask);   //Get's the currently unblocked signals
  return sigsetmask;   
}





void Eye(bool onoff){
  sprintf(const_cast<char *>(sendbuf), "(eye %s)", onoff ? "on" : "off");
  send_message();
}

void Ear(bool onoff) {
  sprintf(const_cast<char *>(sendbuf), "(ear %s)", onoff ? "on" : "off");
  send_message();
}


/* suspend the process until one of the signals comes through */
/* could check for situation to kill client, return FALSE     */
/* i.e. too many actions with no sensory input coming in      */
bool wait_for_signals(sigset_t *mask){
	//cout << "bla: wait_for_signals" << endl;
  sigsuspend(mask);
	//cout << "bla: end of wait_for_signals" << endl;
  return true;
}


void closesocket(int handle){
	close(handle);
}










