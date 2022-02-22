/* -*- Mode: C++ -*- */
/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Itsuki Noda,	Mikhail Prokopenko, Jan Wendler 
    Copyright (C) 1996-2000 Electrotechnical Laboratory. 
    	Itsuki Noda, Yasuo Kuniyoshi and Hitoshi Matsubara.
    Copyright (C) 2000, 2001 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Daniel Polani, Itsuki Noda,
	Mikhail Prokopenko, Jan Wendler 

    This file is a part of SoccerServer.

    This code is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

#ifndef _H_COACH_LANG
#define _H_COACH_LANG

#include <stream.h>
#include <vector.h>
#include <string>

#include "region.h"
#include "coach_lang_comp.h"

//using namespace std;

enum CoachMessageType {
  CMT_Meta,
  CMT_Freeform,
  CMT_Advice,
  CMT_Info,
  CMT_Define
} ;


/********* CM_Message *******************/

class CM_Message 
{
public:
  CM_Message() {}
  virtual ~CM_Message() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, CM_Message& m)
    { m.Print(os); return os; }

  virtual CoachMessageType getType() = 0;

  int getTimeRecv() { return time_recv; }
  void setTimeRecv(int time_recv) { this->time_recv = time_recv; }

protected:
  int time_recv;
} ;


/******** Meta Message ********************/

class CM_MetaMessage 
  : public CM_Message 
{
public:
  CM_MetaMessage() : CM_Message() {}
  CM_MetaMessage(vector<CM_MetaToken*>& vTokens) : CM_Message()
    { this->vTokens = vTokens; }
  ~CM_MetaMessage() { clearTokens(); }

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CoachMessageType getType() { return CMT_Meta; }

  vector<CM_MetaToken*>& getTokens() { return vTokens; }
  void setTokens(vector<CM_MetaToken*>& vTokens)
    { clearTokens(); this->vTokens = vTokens; }
  void clearTokens();
  void forgetTokens() { vTokens.clear(); }
  
private:
  vector<CM_MetaToken*> vTokens;
} ;


/******* Freeform Message *********************/

class CM_FreeformMessage 
  : public CM_Message 
{
public:
  CM_FreeformMessage(char* str = NULL);
  ~CM_FreeformMessage();

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CoachMessageType getType() { return CMT_Freeform; }

  void setStr(char* s);
  void setStrQuoteDelim(char* s);
    
private:
  char* str;
} ;

/******** Info Message ********************/
class CM_InfoMessage 
  : public CM_Message 
{
public:
  CM_InfoMessage() : CM_Message() {}
  CM_InfoMessage(vector<CM_Token*>& vTokens) : CM_Message()
    { this->vTokens = vTokens; }
  ~CM_InfoMessage() { clearTokens(); }

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CoachMessageType getType() { return CMT_Info; }

  vector<CM_Token*>& getTokens() { return vTokens; }
  void setTokens(vector<CM_Token*>& vTokens)
    { clearTokens(); this->vTokens = vTokens; }
  void clearTokens();
  void forgetTokens() { vTokens.clear(); }
  
private:
  vector<CM_Token*> vTokens;
} ;

/******** Advice Message ********************/
class CM_AdviceMessage 
  : public CM_Message 
{
public:
  CM_AdviceMessage() : CM_Message() {}
  CM_AdviceMessage(vector<CM_Token*>& vTokens) : CM_Message()
    { this->vTokens = vTokens; }
  ~CM_AdviceMessage() { clearTokens(); }

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  CoachMessageType getType() { return CMT_Advice; }

  vector<CM_Token*>& getTokens() { return vTokens; }
  void setTokens(vector<CM_Token*>& vTokens)
    { clearTokens(); this->vTokens = vTokens; }
  void clearTokens();
  void forgetTokens() { vTokens.clear(); }
  
private:
  vector<CM_Token*> vTokens;
} ;

/******** Define Message ********************/
class CM_DefineMessage
  : public CM_Message 
{
public:
  CM_DefineMessage() : CM_Message() {}
  CM_DefineMessage(vector<CM_DefineToken*>& vTokens) : CM_Message()
    { this->vTokens = vTokens; }
  virtual ~CM_DefineMessage() {}

  virtual void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);
  
  CoachMessageType getType() { return CMT_Define; }

  vector<CM_DefineToken*>& getTokens() { return vTokens; }
  void setTokens(vector<CM_DefineToken*>& vTokens)
    { clearTokens(); this->vTokens = vTokens; }
  void clearTokens();
  void forgetTokens() { vTokens.clear(); }
  
private:
  vector<CM_DefineToken*> vTokens;
};


#endif //_H_COACH_LANG
