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
#include <stdio.h>
#include "coach_lang.h"

#ifdef COACH_LANG_TESTER
#define FREEFORM_MESS_LENGTH 128
#else
#include "field.h"
extern Stadium Std; //defined in main.C
#define FREEFORM_MESS_LENGTH (Std.server_param->SayCoachMsgSize)
#endif

/******** Meta Message ********************/

void CM_MetaMessage::clearTokens()
{
  vector<CM_MetaToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    delete (*token_iter);
}

void CM_MetaMessage::Print(ostream& out)
{
  out << "(meta";
  vector<CM_MetaToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    out << " " << **token_iter;
  out << ")";
}

void CM_MetaMessage::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Meta" << endl;
  vector<CM_MetaToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    (*token_iter)->PrintPretty(out, line_header + " - ");
    //out << line_header << "  " << (**token_iter) << endl;
}

/******** Freeform Message ********************/

CM_FreeformMessage::CM_FreeformMessage(char* str)
  : CM_Message()
{
  this->str = new char[FREEFORM_MESS_LENGTH+1];
  setStr(str);
}

CM_FreeformMessage:: ~CM_FreeformMessage()
{
  delete [] str;
}

void CM_FreeformMessage::setStr(char* s)
{
  if (s == NULL)
    s[0] = 0;
  else {
    strncpy(str,s,FREEFORM_MESS_LENGTH);
    str[FREEFORM_MESS_LENGTH] = 0;
  }
}

void CM_FreeformMessage::setStrQuoteDelim(char* s)
{
  strncpy(str,s+1,FREEFORM_MESS_LENGTH);
  str[FREEFORM_MESS_LENGTH] = 0;
  int len = strlen(str);
  if (str[len-1] == '"')
    str[len-1] = 0;
  else 
    printf("no end quote! %c", str[len-1]);
}


void CM_FreeformMessage::Print(ostream& out)
{
  out << "(freeform \"" << str << "\")";
}

void CM_FreeformMessage::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Freeform" << endl;
  out << line_header << "  " << "\"" << str << "\"" << endl;
}

/******** Info Message ********************/

void CM_InfoMessage::Print(ostream& out)
{
  out << "(info ";
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    out << " " << **token_iter;
  out << ")";
}

void CM_InfoMessage::clearTokens()
{
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    delete (*token_iter);
  vTokens.clear();
}

void CM_InfoMessage::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Info" << endl;
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    (*token_iter)->PrintPretty(out, line_header + " - ");
    //out << line_header << "  " << (**token_iter) << endl;
}

/******** Advice Message ********************/
  
void CM_AdviceMessage::Print(ostream& out)
{
  out << "(advice ";
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    out << " " << **token_iter;
  out << ")";
}

void CM_AdviceMessage::clearTokens()
{
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    delete (*token_iter);
  vTokens.clear();
}

void CM_AdviceMessage::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Advice" << endl;
  vector<CM_Token*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    (*token_iter)->PrintPretty(out, line_header + " - ");
    //out << line_header << " " << (**token_iter) << endl;
}

/******** Define Message ********************/
  
void CM_DefineMessage::Print(ostream& out)
{
  out << "(define";
  vector<CM_DefineToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    out << " " << **token_iter;
  out << ")";
}

void CM_DefineMessage::clearTokens()
{
  vector<CM_DefineToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    delete (*token_iter);
  vTokens.clear();
}

void CM_DefineMessage::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Define" << endl;
  vector<CM_DefineToken*>::iterator token_iter;
  for (token_iter = vTokens.begin(); token_iter != vTokens.end(); ++token_iter) 
    (*token_iter)->PrintPretty(out, line_header + " - ");
    //out << line_header << "  " << (**token_iter) << endl;
}
