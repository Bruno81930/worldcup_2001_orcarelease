/*
 *Header:
 *File: netif.h (for C++)
 *Author: Noda Itsuki
 *Date: 1996/12/1
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


#ifdef	Solaris
#include <sys/filio.h>
#define	bcopy(a, b)	memset(a, 0, b)
#else
#include <sys/ioctl.h>
#endif
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>

/*
 *===================================================================
 *Part: Port class
 *===================================================================
 */
class Port {
	public: 
		int					socketfd ;			/* socket discriptor */
		char				host[256] ;
		int					port ;
		struct sockaddr_in	monitor_addr ;		/* monitor addr structure */
		char				rbuf[256] ;			/* recv buffer */
		displist_t			top ;

		void	init(void) ;	
		void	send_info(dispinfo_t *, unsigned long host, int port) ;
		int		recv_info(void) ;
		void	monitor_init(void) ;
} ;
