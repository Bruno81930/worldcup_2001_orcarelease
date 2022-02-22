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

#include <iostream.h>
#include "coach_lang.h"
#include "garbage.h"

#define DEBUG(x) 

bool GCElement::operator==(const GCElement& a) const
{
  if (ptr != a.ptr)
    return false;
  if (type != a.type)
    cerr << "Garbage: ptrs equal (" << ptr << ") "
	 << "but types not ("
	 << type << " != " << a.type 
	 << ")"
	 << endl;
  return true;
}


void GarbageCollector::add(void* ptr, GC_elem_type type)
{
  elems.push_back(GCElement(ptr, type));
  DEBUG(cout << "Added " << ptr << endl);
}

void GarbageCollector::remove(void* ptr, GC_elem_type type)
{
  DEBUG(int prev_size = elems.size());
  elems.remove(GCElement(ptr, type));
  DEBUG(int new_size = elems.size());
  DEBUG(cout << "Removed " << ptr << "\t(" << prev_size << " -> " << new_size << ")" << endl);
  DEBUG(if (prev_size == new_size))
    DEBUG(cerr << "Garbage: Tried to remove element not in trash!" << endl);
}

void GarbageCollector::deleteAll()
{
  DEBUG(cout << "Garbage: deleteAll started" << endl);
  list<GCElement>::iterator elem_iter;
  for (elem_iter = elems.begin(); elem_iter != elems.end(); elem_iter++) {
    switch (elem_iter->type) {
    case GCE_pMess: delete ((CM_Message*)elem_iter->ptr); break;
    case GCE_pMetaTok: delete ((CM_MetaToken*)elem_iter->ptr); break;
    case GCE_pvpMetaTok: {
      vector<CM_MetaToken*>* p = (vector<CM_MetaToken*>*)elem_iter->ptr; 
      vector<CM_MetaToken*>::iterator iter;
      for (iter = p->begin(); iter != p->end(); ++iter) 
	delete (*iter);
      delete p;
    } break;
    case GCE_pDefTok: delete ((CM_DefineToken*)elem_iter->ptr); break;
    case GCE_pvpDefTok: {
      vector<CM_DefineToken*>* p = (vector<CM_DefineToken*>*)elem_iter->ptr; 
      vector<CM_DefineToken*>::iterator iter;
      for (iter = p->begin(); iter != p->end(); ++iter) 
	delete (*iter);
      delete p;
    } break;
    case GCE_pAct:       delete ((CM_Action*)elem_iter->ptr); break;
    case GCE_pDir:       delete ((CM_Directive*)elem_iter->ptr); break;
    case GCE_pvpDir: {
      vector<CM_Directive*>* p = (vector<CM_Directive*>*)elem_iter->ptr; 
      vector<CM_Directive*>::iterator iter;
      for (iter = p->begin(); iter != p->end(); ++iter) 
	delete (*iter);
      delete p;
    } break;
    case GCE_pCond:      delete ((CM_Condition**)elem_iter->ptr); break;
    case GCE_pvpCond: {
      vector<CM_Condition*>* p = (vector<CM_Condition*>*)elem_iter->ptr; 
      vector<CM_Condition*>::iterator iter;
      for (iter = p->begin(); iter != p->end(); ++iter) 
	delete (*iter);
      delete p;
    } break;
    case GCE_pTok:       delete ((CM_Token*)elem_iter->ptr); break;
    case GCE_pvpTok:     delete ((vector<CM_Token*>*)elem_iter->ptr); break;
    case GCE_pReg:       delete ((Region*)elem_iter->ptr); break;
    case GCE_pvpReg: {
      vector<Region*>* p = (vector<Region*>*)elem_iter->ptr; 
      vector<Region*>::iterator iter;
      for (iter = p->begin(); iter != p->end(); ++iter) 
	delete (*iter);
      delete p;
    } break;
    case GCE_pUSet:      delete ((UnumSet*)elem_iter->ptr); break;
    case GCE_pBMTSet:    delete ((BallMoveTokenSet*)elem_iter->ptr); break;
    case GCE_pChar:      delete ((char*)elem_iter->ptr); break;
    case GCE_None:       
    default:
      cerr << "Garbage: bad type " << elem_iter->type << endl;
      break;
    }
    DEBUG(cout << "Garbage: deleted " << elem_iter->ptr << " of type " << elem_iter->type << endl);
  }
  
  DEBUG(cout << "Garbage: deleteAll all elems deleted" << endl);
  elems.clear();
  DEBUG(cout << "Garbage: deleteAll finished" << endl);
}

