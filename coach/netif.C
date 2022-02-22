/* -*- Mode: C -*-
 *Header:
 *File: netif.C (for C++ & cc)
 *Author: Noda Itsuki
 *Date: 1996/02/14
 *EndHeader:
 */

/*
 *Copyright:

    Copyright (C) 1996-2000 Electrotechnical Laboratory. 
    	Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
    Copyright (C) 2000 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Daniel Polani, Itsuki Noda

    This file is a part of SoccerServer.

    SoccerServer is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */


// [2000/01/14:I.Noda]
//  escape prototype of recvfrom 
//  for escape conversion of signed-unsigned ints 
#define recvfrom _escape_recvfrom
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef SYSV
#ifndef Solaris
#include <strings.h>
#endif
#endif
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifdef Solaris
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <math.h>
#include <fcntl.h>

#include <iostream.h>
#include <strstream.h>

#include "param.h"
#include "types.h"
#include "netif.h"

// [2000/01/14:I.Noda]
//  escape prototype of recvfrom 
//  for escape conversion of signed-unsigned ints 
#undef recvfrom
//  prototype of recvfrom for escape conversion of signed-unsigned ints 
//
extern "C" {
   int recvfrom(int s, void *buf, int len, unsigned int flags,
		struct sockaddr *from, SOCKLEN_T *fromlen);
} ;
//

/*
 *===================================================================
 *  UDP Send
 *===================================================================
 */

void Port::init(void)
{
	struct sockaddr_in	my_addr ;
	int one ;

	strcpy(host, "localhost") ;
	port = DEFAULT_PORT_NUMBER ;

	/* open UDP socket */
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cerr << "can't open socket." << endl ;
		exit(-1) ;
	}

	/* set non-blocking */
	one = 1 ;
	if (ioctl(socketfd, FIONBIO, &one) < 0) {
		cerr << "can't ioctl on socket." << endl ;
		exit(-1) ;
	}

	/* bind local address */
	//bzero((char *) &my_addr, sizeof(my_addr)) ;
	memset((char *) &my_addr, 0, sizeof(my_addr)) ;
	my_addr.sin_family			= AF_INET ;
	my_addr.sin_addr.s_addr		= htonl(INADDR_ANY) ;
	my_addr.sin_port			= htons(port) ;

	if (bind(socketfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
		cerr << "can't bind local address." << endl ;
		exit(-1) ;
	}

	//bzero((char *)&monitor_addr, sizeof(monitor_addr)) ;
	memset((char *)&monitor_addr, 0, sizeof(monitor_addr)) ;
	monitor_addr.sin_family		= AF_INET ;

	top.next = NULL ;
}

void Port::send_info(dispinfo_t *msg, unsigned long host, int port)
{
	struct sockaddr_in cli_addr ;
	int n ;

	//bzero((char *) &cli_addr, sizeof(cli_addr)) ;
	memset((char *) &cli_addr, 0, sizeof(cli_addr)) ;
	cli_addr.sin_family			= AF_INET ;
	cli_addr.sin_addr.s_addr	= htonl(host) ;
	cli_addr.sin_port			= htons(port) ;

	n = sizeof(dispinfo_t) ;
	if(sendto(socketfd, (char *)msg, n, 0,
				(struct sockaddr *) &cli_addr, sizeof(cli_addr)) != n){
#ifdef DEBUG
		cerr << "sendto error on socket." << endl ;
#endif
	}
}

int Port::recv_info(void)
{
	int n;
	SOCKLEN_T len ;

	//bzero(rbuf, 256) ;
	memset(rbuf, 0, 256) ;

	len = sizeof(monitor_addr) ;
	n = recvfrom(socketfd, (char *)&rbuf, sizeof(rbuf), 0, 
					(struct sockaddr *)&monitor_addr, &len) ;

	if (!strcmp(rbuf, "(dispinit)")) {
		displist_t *p ;
		unsigned long host = ntohl(monitor_addr.sin_addr.s_addr) ;
		int port = ntohs(monitor_addr.sin_port) ;

		for (p = &top; p->next != NULL; p = p->next) ;

		p->next = (displist_t *)malloc(sizeof(displist_t)) ;
		if (p->next == NULL) {
			perror("malloc") ;
			exit(1) ;
		}
		p->next->shost = host ;
		p->next->sport = port ;
		p->next->next = NULL ;
	}

	return n ;
}
