/* -*- Mode: C++ -*- */
/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Itsuki Noda,	Mikhail Prokopenko, Jan Wendler

    This file is part of SoccerServer.

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

/* NOTE: This program is still under developement and is intended only to as a
   pre-release version provided to allow people to preview the coach language and
   semantics a bit.
*/

#include "clangparser.h"

CM_Message* g_pMess = NULL;
GarbageCollector g_GC;

extern "C"
{
  int parse_coach_lang_string(char*);
}


CM_Message* parse_coach_message(char message[])
{
    parse_coach_lang_string(message);

    g_GC.remove(g_pMess, GCE_pMess);
    g_GC.deleteAll();

/*
    if (g_pMess == NULL)
    {
      cout << "no message read" << endl;
    }
    else
    {
      cout << "read coach message: ";
      g_pMess->Print(cout);
      cout << endl;
    }
*/

    return g_pMess;
}
