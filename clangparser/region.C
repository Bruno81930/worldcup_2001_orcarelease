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

/* This files defines a class region to specify regions for the coach messages */

#include "region.h"

/*** RegQuad ***/

RegQuad::~RegQuad()
{
  if (pts[0]) delete pts[0];
  if (pts[1]) delete pts[1];
  if (pts[2]) delete pts[2];
  if (pts[3]) delete pts[3];
}
 
                                                                       
 
bool RegQuad::setPt(int i, RegPoint* pt)
{
  if (i < 0 || i > 3)
    return false;
  pts[i] = pt;
  return true;
}


void RegQuad::setAllPts(RegPoint* pt0, RegPoint* pt1, RegPoint* pt2, RegPoint* pt3)
{
 
 if (pts[0]) delete pts[0];
 if (pts[1]) delete pts[1];
 if (pts[2]) delete pts[2];
 if (pts[3]) delete pts[3];
 pts[0] = pt0;
 pts[1] = pt1;
 pts[2] = pt2;
 pts[3] = pt3;
}

void RegQuad::Print(ostream& out)
{
  out << "(quad";
  for (int i=0; i<4; i++)
    out << " " << *pts[i];
  out << ")";
}

void RegQuad::PrintPretty(ostream& out, string line_header)
{
  out << line_header
      << "Quadrangle: ";
  for (int i=0; i<4; i++) {
    pts[i]->PrintSimple(out);
    out << " ";
  }
  out << endl;
}



/*** RegArc ***/

RegArc::RegArc()
{
  center = NULL;
  start_rad = end_rad = 0.0;
  start_ang = span_ang = 0.0;
}

RegArc::RegArc(RegPoint* center, float start_rad, float end_rad, float start_ang, float span_ang)
{
  this->center = center;
  this->start_rad = start_rad;
  this->end_rad = end_rad;
  this->start_ang = start_ang;
  this->span_ang = span_ang;
}


RegArc::~RegArc()
{
  if (center) delete center;
}

void RegArc::Print(ostream& out)
{
  out << "(arc"
      << " " << *center
      << " " << start_rad 
      << " " << end_rad 
      << " " << start_ang 
      << " " << span_ang
      << ")";
}

void RegArc::PrintPretty(ostream& out, string line_header)
{
  out << line_header
      << "Arc: "
      << "center=";
  center->PrintSimple(out);
  out << "\tradius="
      << start_rad
      << " to "
      << end_rad
      << "\tangle="
      << start_ang
      << " for "
      << span_ang
      << endl;
}


bool RegArc::setCenter(RegPoint* pt)
{
  this->center = center;
  return true;
}

bool RegArc::setRad(float start_rad, float end_rad)
{
  if (start_rad > end_rad)
    return false;
  this->start_rad = start_rad;
  this->end_rad = end_rad;
  return true;
}

bool RegArc::setAng(float start_ang, float span_ang)
{
  if (start_ang > 180 || start_ang < -180)
    return false;
  this->start_ang = start_ang;
  this->span_ang = span_ang;
  return true;
}

/*** RegUnion ***/
RegUnion::~RegUnion()
{
  vector<Region*>::iterator iter;
  for (iter = vecReg.begin(); iter != vecReg.end(); ++iter)
    if (*iter != NULL) delete *iter;
}

void RegUnion::Print(ostream& out)
{
  out << "(reg";
  vector<Region*>::iterator iter;
  for (iter = vecReg.begin(); iter != vecReg.end(); ++iter)
    out << " " << **iter;
  out << ")";
}
void RegUnion::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Region Union:" << endl;
  vector<Region*>::iterator iter;
  for (iter = vecReg.begin(); iter != vecReg.end(); ++iter)
    (*iter)->PrintPretty(out, line_header + "o ");
}


Region* RegUnion::getRegion(unsigned i)
{
  if (i < 0 || i >= vecReg.size())
    return NULL;
  return vecReg[i];
}

bool RegUnion::addRegion(Region* r)
{
  vecReg.push_back(r);
  return true;
}

bool RegUnion::replaceRegion(unsigned i, Region* r)
{
  if (i < 0 || i >= vecReg.size())
    return false;
  delete vecReg[i];
  vecReg[i] = r;
  return true;
}


/*** RegPointRelative ***/
  
void RegPointRelative::PrintPretty(ostream& out, string line_header)
{
  out << line_header << "Point-Relative(" << x << ", " << y << " ";
  if (pt)
    pt->PrintSimple(out);
  else
    out << "NULL";
  out << ")" << endl;
}
void RegPointRelative::PrintSimple(ostream& out)
{
  out << "Point-Relative(" << x << ", " << y << " ";
  if (pt)
    pt->PrintSimple(out);
  else
    out << "NULL";
  out << ")" << endl;
}

