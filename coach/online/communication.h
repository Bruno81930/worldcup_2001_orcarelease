#ifndef _MY_COMMUNICATION_H_
#define _MY_COMMUNICATION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
//#include <netdb.h>


// maximal length of messages:
#define MAXLENGTH 5096


class Socket {
 public:
  int           socketfd ;
  struct sockaddr_in serv_addr;
};


Socket open_connection(char host[], int port);
bool init_to_server(char * teamname);
int send_message(void);
int receive_message(void);
int wait_message(void);
int parse_init_return(void);
sigset_t init_handler(void);
void Eye(bool onoff);
void Ear(bool onoff);
bool wait_for_signals(sigset_t *mask);
void closesocket(int handle);	// 2do is socket-handler really an int??

#endif

