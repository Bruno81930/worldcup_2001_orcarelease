/* -*- Mode: C++ -*- */
/*
 *Copyright:

    Copyright (C) 2001 RoboCup Soccer Server Maintainance Group.
    	Patrick Riley, Tom Howard, Itsuki Noda,	Mikhail Prokopenko, Jan Wendler 

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

/* This files defines a class region to specify regions for the coach messages */

#ifndef _H_REGION
#define _H_REGION

#include <stream.h>
#include <vector.h>
#include <string>

const int max_name_len = 40;

/* a small value that used as a buffer to determine if two floating point numbers
   are the same. */
//const double FLOAT_EPS = .000001;


class Region 
{
public:
  Region() {}
  virtual ~Region() {};

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  friend ostream& operator <<(ostream & os, Region& r)
    { r.Print(os); return os; } 
  
} ;

class RegNull
  : public Region
{
public:
  RegNull() : Region() {}
  ~RegNull() {}

  void Print(ostream& out)
    { out << "(null)"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "null region" << endl; }
      
} ;

class RegPoint;

class RegQuad
  : public Region
{
public:
  RegQuad() : Region() { pts[0] = pts[1] = pts[2] = pts[3] = NULL; }
  RegQuad(RegPoint* pt0, RegPoint* pt1, RegPoint* pt2, RegPoint* pt3) : Region()
    { pts[0] = pt0; pts[1] = pt1; pts[2] = pt2; pts[3] = pt3; }
  ~RegQuad();

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);

  /* class specific */
  RegPoint* getPt(int i) { return (i>=0 && i<=3) ? pts[i] : NULL; }
  bool      setPt(int i, RegPoint* pt);
  void      setAllPts(RegPoint* pt0, RegPoint* pt1, RegPoint* pt2, RegPoint* pt3);
  
private:
  RegPoint* pts[4];
} ;

class RegArc
  : public Region
{
public:
  RegArc();
  RegArc(RegPoint* center, float start_rad, float end_rad, float start_ang, float span_ang);
  ~RegArc();

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);

  /* class specific */
  RegPoint* getCenter() { return center; }
  float    getStartRad() { return start_rad; }
  float    getEndRad()   { return end_rad; }
  float    getStartAng() { return start_ang; }
  float    getSpanAng()   { return span_ang; }
  bool     setCenter(RegPoint* pt);
  bool     setRad(float start_rad, float end_rad);
  bool     setAng(float start_ang, float span_ang);
  
private:
  RegPoint* center;
  /* start rad <= end_rad */
  float start_rad, end_rad;
  float start_ang, span_ang;
} ;


class RegUnion
  : public Region
{
public:
  RegUnion() : Region() { }
  RegUnion(vector<Region*>& vecReg) : Region() { this->vecReg = vecReg; }
  ~RegUnion();

  void Print(ostream& out);
  void PrintPretty(ostream& out, string line_header);

  /* class specific */
  int getNumRegions() { return vecReg.size(); }
  Region* getRegion(unsigned i);
  bool    addRegion(Region* r);
  bool    replaceRegion(unsigned i, Region* r);
  void    setRegions(vector<Region*>& vecReg) 
    { this->vecReg = vecReg; }
  
private:
  vector<Region*> vecReg;
} ;

class RegNamed
  : public Region
{
public:
  RegNamed(char* name = NULL) : Region() { setName(name); }
  ~RegNamed() {}

  void Print(ostream& out)
    { out << "\"" << name << "\""; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "region named \"" << name << "\"" << endl; }

  /* class specific */
  char* getName() { return name; }
  void  setName(char* name)
    { if (name == NULL) this->name[0] = 0;
    else strncpy(this->name, name, max_name_len); this->name[max_name_len] = 0; }

private:
  char name[max_name_len+1];
} ;

/**** The various RegPoint s ****/
class RegPoint
  : public Region
{
public:
  RegPoint() : Region() { }
  virtual ~RegPoint() {}

  virtual void Print(ostream& out) = 0;
  virtual void PrintPretty(ostream& out, string line_header) = 0;
  //like PrintPretty, but no line header
  virtual void PrintSimple(ostream& out) = 0;

} ;

class RegPointSimple
  : public RegPoint
{
public:
  RegPointSimple(float x=0.0, float y=0.0) : RegPoint() { this->x = x; this->y = y;}
  ~RegPointSimple() {}

  void Print(ostream& out)
    { out << "(pt " << x << " " << y << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Point(" << x << ", " << y << ")" << endl; }
  void PrintSimple(ostream& out)
    { out << "Point(" << x << ", " << y << ")"; }
  
  /* class specific */
  float getX() { return x; }
  float getY() { return y; }
  void  setPt(float x, float y) { this->x = x; this->y = y;}

private:
  float x;
  float y;
} ;

class RegPointRelative
  : public RegPoint
{
public:
  RegPointRelative(float x=0.0, float y=0.0, RegPoint* pt=NULL) : RegPoint()
   { this->x = x; this->y = y; this->pt = pt; }
  ~RegPointRelative() { if (pt) delete pt; }

  void Print(ostream& out)
  { out << "(pt " << x << " " << y << " " << *pt << ")"; }
  void PrintPretty(ostream& out, string line_header);
  void PrintSimple(ostream& out);
  
  /* class specific */
  float getRelX() { return x; }
  float getRelY() { return y; }
  void  setRelVector(float x, float y) { this->x = x; this->y = y;}
  RegPoint* getPt() { return pt; }
  void      setPt(RegPoint* p) { if (pt) delete pt; pt = p; }
  
private:
  float x;
  float y;
  RegPoint* pt;
} ;

class RegPointBall
  : public RegPoint
{
public:
  RegPointBall() : RegPoint() {}
  ~RegPointBall() {}

  void Print(ostream& out)
  { out << "(pt ball)"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Point-Ball" << endl; }
  void PrintSimple(ostream& out)
    { out << "Point-Ball"; }    
} ;

class RegPointPlayer
  : public RegPoint
{
public:
  RegPointPlayer(bool our_side, int num) : RegPoint()
   { this->our_side = our_side; this->num = num; }
  ~RegPointPlayer() { }

  void Print(ostream& out)
  { out << "(pt " << (our_side ? "our" : "opp") << " " << num << ")"; }
  void PrintPretty(ostream& out, string line_header)
    { out << line_header << "Point-Player(" << (our_side ? "our team" : "opponent")
	  << " " << num << ")" << endl; }
  void PrintSimple(ostream& out)
    { out << "Point-Player(" << (our_side ? "our team" : "opponent")
	  << " " << num << ")"; }
  
  /* class specific */
  bool isOurSide()  { return our_side; }
  bool isTheirSide(){ return !our_side; }
  int  getNum() { return num; }
  
  void setOurSide(bool our_side) { this->our_side = our_side; }
  void setNum(int num) { this->num = num; }
  
private:
  bool our_side;
  int  num;
} ;

#endif

