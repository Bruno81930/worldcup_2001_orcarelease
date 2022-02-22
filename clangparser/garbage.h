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

/* 
   This files defines a class to maintain a list of pointers needing to be freed
 */

#ifndef _H_GARBAGE
#define _H_GARBAGE

#include <vector.h>
#include <list.h>

enum GC_elem_type {
  GCE_None,
  GCE_pMess,
  GCE_pMetaTok,
  GCE_pvpMetaTok,
  GCE_pDefTok,
  GCE_pvpDefTok,
  GCE_pAct,
  GCE_pDir,
  GCE_pvpDir,
  GCE_pCond,
  GCE_pvpCond,
  GCE_pTok,
  GCE_pvpTok,
  GCE_pReg,
  GCE_pvpReg,
  GCE_pUSet,
  GCE_pBMTSet,
  GCE_pChar,
};

class GCElement 
{
public:
  GCElement() { ptr = NULL; type = GCE_None; }
  GCElement(void* ptr, GC_elem_type type)
    { this->ptr = ptr; this->type = type; }
  
  void* ptr;
  GC_elem_type type;
  
  bool operator==(const GCElement& a) const;
};
  
class GarbageCollector {
public:
  GarbageCollector() {}
  ~GarbageCollector() { deleteAll(); }

  void add(void* ptr, GC_elem_type type);
  void remove(void* ptr, GC_elem_type type);
      
  void deleteAll();

private:
  list<GCElement> elems;
};


#endif
