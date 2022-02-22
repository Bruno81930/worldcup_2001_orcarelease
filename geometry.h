
/* -*- Mode: C++ -*- */

/* geometry.h
 * CMUnited99 (soccer client for Robocup99)
 * Peter Stone <pstone@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Peter Stone
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy and distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 * For more information, please see http://www.cs.cmu.edu/~robosoccer/
 */


#ifndef _GEOMETRY_
#define _GEOMETRY_

#include "utils.h"
#include <vector.h>

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                        Vector Class                                          */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/


class Vector {
public:
  float x;
  float y;
  
  Vector(float vx = 0, float vy = 0) { x = vx; y = vy; }
  Vector operator -() { return Vector(-x, -y); }
  Vector operator +(const Vector &a) { return Vector(x + a.x, y + a.y); }
  Vector operator -(const Vector &a) { return Vector(x - a.x, y - a.y); }
  Vector operator *(const float &a) { return Vector(x * a, y * a); }
  Vector operator *(const Vector &a) { return Vector(x * a.x, y * a.y); }
  Vector operator /(const float &a) { return Vector(x / a, y / a); }
  Vector operator /(const Vector &a) { return Vector(x / a.x, y / a.y); }
  void operator =(const float &a) { x = a; y = a; }
  void operator +=(const Vector &a) { x += a.x;  y += a.y; }
  void operator +=(const float &a) { x += a;  y += a; }
  void operator -=(const Vector &a) { x -= a.x;  y -= a.y; }
  void operator -=(const float &a) { x -= a;  y -= a; }
  void operator *=(const float &a) { x *= a;  y *= a; }
  void operator /=(const float &a) { x /= a;  y /= a; }
  bool operator !=(const Vector &a) { return (x != a.x) || (y != a.y); }
  bool operator !=(const float &a) { return (x != a) || (y != a); }
  bool operator ==(const Vector &a) { return (x == a.x) && (y == a.y); }
  friend ostream& operator <<(ostream & os, Vector v) { return os << "("<<v.x<<", "<<v.y<<")"; }
  void Print() { cout << *this << endl; }
  void ins(float vx = 0, float vy = 0) { x = vx; y = vy; }
  float mod() { return sqrt(x*x + y*y); }
  float mod2() { return x*x + y*y; }
  float dist(const Vector &a) { return (*this-a).mod(); }
  float dist2(const Vector &a) { return (*this-a).mod2(); }
    Vector Normalize() { return SetLength(1.0); }
    Vector SetLength(float len) { return (*this) * (len / mod()); }
    AngleDeg dir() { return ATan2(y,x); }

    AngleDeg arg() {
        if (0.0==x && 0.0==y) return 0.0;
        Value tmp= atan2(y,x);
        if (tmp < 0.0)
            return tmp+2*M_PI;
        else
            return tmp;
    }



  Vector rotate(AngleDeg angle)
  { return Vector(this->mod()*Cos(this->dir()+angle), this->mod()*Sin(this->dir()+angle)); }
  Vector Global2Relative(Vector orig, float ang);
  Vector Relative2Global(Vector orig, float ang);


  /**agn
   *    scales the given vector by a certain factor
   */
  Vector ScaleByFloat(const float &scale);

  /**agn
   *    scales the given vector to FP_scale_factor 
   */
  Vector Scale();

  Bool InFrontOf(const Vector &a) { return x > a.x ? TRUE : FALSE; }
  Bool InFrontOf(const float &a) { return x > a ? TRUE : FALSE; }
  Bool Behind(const Vector &a) { return x < a.x ? TRUE : FALSE; }
  Bool Behind(const float &a) { return x < a ? TRUE : FALSE; }


  Bool ApproxEqual(const Vector &a)
    { return (fabs(x-a.x) < FLOAT_EPS && fabs(y-a.y) < FLOAT_EPS) ? TRUE : FALSE; }

  Vector WeightedAverage(const Vector &a, float w)
    { return Vector((1.0-w)*x + w * a.x, (1.0-w)*y + w * a.y); }
};

inline Vector Polar2Vector(float mod, AngleDeg ang) {
  return Vector(mod * Cos(ang), mod * Sin(ang));
}

inline Vector Vector::Global2Relative(Vector orig, float ang)
{
  return (*this - orig).rotate(-ang);
}

inline Vector Vector::Relative2Global(Vector orig, float ang)
{
  return (rotate(ang) + orig);
}

inline Vector operator *(const float &a, const Vector &v)
{ return Vector(v.x * a, v.y * a); }

inline Vector Vector::ScaleByFloat(const float &scale)
{
   //return ( Vector( ((x + 50.5) * (scale_x / 101.0)) - scale_x / 2, ((y + 34.0) * (scale_y / 68.0)) / scale_y / 2 ));
   return ( Vector( ( x*scale ), ( y*scale ) ) );
}


inline Vector Vector::Scale()
{
   return this->ScaleByFloat(3.0/7.0);
}


/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                       Ray Class                                              */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/* stores origin and direction (which is normalized) */
class Line;
class Rectangle;
class Quad;
class Ray 
{
public:
  Ray() 
    { origin = Vector(0,0); direction = Vector(1,0); }
  Ray(Vector orig, Vector dir);
  Ray(Vector orig, float ang)
    { origin = orig; direction = Polar2Vector(1.0, ang); }

  Bool OnRay(Vector pt);
  Bool InRightDir(Vector pt); // more lenient than above about distance off ray 
  
  Bool intersection(Ray r, Vector *pPt);
  Bool intersection(Line l, Vector *pPt);

  int CircleIntersect(float rad, Vector center,
		      Vector* psol1, Vector* psol2);

  Vector GetClosestPoint(Vector pt);

  Vector RectangleIntersection(Rectangle R);
  
protected:
  friend Line;
  friend Rectangle;
  Vector origin;
  Vector direction;
};

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                      Line Class                                              */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
class Line {
 public:
  Line(float x_coef=0.0, float y_coef=0.0, float constant=0.0);
  Line(Ray r) 
    { LineFromRay(r); }
  
  void LineFromTwoPoints(Vector pt1, Vector pt2);
  void LineFromRay(Ray r);
  void LineFromRay(Vector orig, Vector dir)
    { LineFromRay(Ray(orig, dir)); }
  void LineFromRay(Vector orig, AngleDeg dir)
    { LineFromRay(orig, Polar2Vector(1.0, dir)); }
  
  Bool PointOnLine(float x, float y);
  inline Bool PointOnLine(Vector pt) { return PointOnLine(pt.x, pt.y); }

  float dist(Vector pt);
  float dist2(Vector pt);
  float angle(); /* returns the angle of the line in [-90, 90] */
  
  Bool InBetween(Vector pt, Vector end1, Vector end2);
  Vector GetClosestPtInBetween(Vector pt, Vector end1, Vector end2);
  
  /* the buffer should really be linked to an option or something,
     but 1.0 is okay */
  inline Bool OnLine(Vector pt, float buffer = 1.0)
    { return (dist2(pt) < Sqr(buffer)) ? TRUE : FALSE; }
  
  Vector ProjectPointUsingCircle(Vector pt);
  inline Vector ProjectPoint(Vector pt) { return intersection(perpendicular(pt)); }

  float get_y(float x);
  float get_x(float y);
  Vector intersection(Line l);
  inline Line perpendicular(Vector pt) { return Line(B,-A,A*pt.y - B*pt.x); }

  Bool RayIntersection(Ray r, Vector *ppt);
  
  Line shift_y(float val)
    { return Line(A, B, C - val * B); }
  Line shift_x(float val)
    { return Line(A, B, C - val * A); }  

  /* returns whether the projection of pt1 is closer to targ_pt than the
     projection of pt2 */
  Bool IsPtCloserToPtOnLine(Vector pt1, Vector pt2, Vector targ_pt);
  
  Bool HalfPlaneTest(Vector pt); //return TRUE on top/left part of plane
  Bool SameSlope(Line l);
  
  friend ostream& operator <<(ostream & os, Line l)
    { return os << "#L("<<l.A<<", "<<l.B<<", "<<l.C<<")"; } 
  void Print(void) { cout << *this << endl; }

  // private:
  float A,B,C; /* the three coeffs in the line equation */
  /* Ax + By + C = 0 */
} ;

inline Line LineFromTwoPoints(Vector pt1, Vector pt2)
{ Line l; l.LineFromTwoPoints(pt1,pt2); return l; }
inline Line LineFromRay      (Ray r)
{ Line l; l.LineFromRay     (r); return l; }
inline Line LineFromRay      (Vector orig, Vector dir)
{ Line l; l.LineFromRay     (orig,dir); return l; }
inline Line LineFromRay      (Vector orig, AngleDeg dir)
{ Line l; l.LineFromRay     (orig,dir); return l; }



/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                        Rectangle Class                                       */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

class Rectangle {
public:

  Rectangle() ;
  Rectangle(const float left, const float right,
	    const float top, const float bottom) ;
  Rectangle(const Vector center, const Vector size) ;

  bool operator ==(const Rectangle &a)
  { return (left_x == a.left_x) && (right_x == a.right_x) &&
      (top_y == a.top_y) && (bottom_y == a.bottom_y); }

  inline float TopY()     { return top_y; }
  inline float BottomY()  { return bottom_y; }
  inline float RightX()   { return right_x; }
  inline float LeftX()    { return left_x; }

  inline Vector TopLeftCorner() { return Vector(left_x, top_y); }
  inline Vector TopRightCorner() { return Vector(right_x, top_y); }
  inline Vector BottomLeftCorner() { return Vector(left_x, bottom_y); }
  inline Vector BottomRightCorner() { return Vector(right_x, bottom_y); }

  inline Line LeftEdge() {return LineFromTwoPoints(TopLeftCorner(), BottomLeftCorner());}
  inline Line RightEdge() {return LineFromTwoPoints(TopRightCorner(), BottomRightCorner());}
  inline Line TopEdge() {return LineFromTwoPoints(TopLeftCorner(), TopRightCorner());}
  inline Line BottomEdge() {return LineFromTwoPoints(BottomLeftCorner(), BottomRightCorner());}

  Line GetEdge(int n);  //order: top, right, bot, left; starting at 0
  Vector GetPoint(int n); //order: TL, TR, BR, BL; starting at 0

  inline float Width()    { return right_x - left_x; }
  inline float Height()   { return bottom_y - top_y; }

  inline Vector Center()  { return Vector( LeftX() + Width()/2, TopY() + Height()/2 ); }
  inline Vector Size()  { return Vector( Width(),Height());}

  Bool   IsWithin(const Vector& p) ;
  Vector nearestHEdge(const Vector& p) ;
  Vector nearestVEdge(const Vector& p) ;
  Vector nearestEdge(const Vector& p) ;

  float  DistanceToEdge(const Vector& p);
  Vector AdjustToWithin(const Vector& p);

  Line nearestHEdgeLine(const Vector& p) ;
  Line nearestVEdgeLine(const Vector& p) ;
  Line nearestEdgeLine(const Vector& p) ;

  Vector random() ;

  Rectangle shrink(float val)
    { return expand(-val); }
  Rectangle expand(float val);

  Rectangle expandLeft(float val)
    {return Rectangle(left_x - val, right_x, top_y, bottom_y);}
  Rectangle expandRight(float val)
    {return Rectangle(left_x, right_x + val, top_y, bottom_y);}
  Rectangle expandTop(float val)
    {return Rectangle(left_x, right_x, top_y - val, bottom_y);}
  Rectangle expandBottom(float val)
    {return Rectangle(left_x, right_x, top_y, bottom_y + val);}
  Rectangle shrinkLeft(float val)
    { return expandLeft(-val); }
  Rectangle shrinkRight(float val)
    { return expandRight(-val); }
  Rectangle shrinkTop(float val)
    { return expandTop(-val); }
  Rectangle shrinkBottom(float val)
    { return expandBottom(-val); }

  friend ostream& operator <<(ostream & os, Rectangle r)
    { return os << "RECTANGLE:  x = " << r.LeftX() << " to " << r.RightX()
		<< "   y = " << r.TopY() << " to " << r.BottomY(); }
  void Print();

  Vector RayIntersection(Ray r);

private:
  float left_x ;
  float right_x ;
  float top_y ;
  float bottom_y ;

} ;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                        Quad Class                                            */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

class Quad {
public:
	
	Quad() ;

	Quad(const Vector corner1, const Vector corner2,
		 const Vector corner3, const Vector corner4) ;
	
	bool operator ==(const Quad &a)
		{ return (corner[1] == a.corner[1]) && (corner[2] == a.corner[2]) &&
			  (corner[3] == a.corner[3]) && (corner[4] == a.corner[4]); }

	Line GetEdge(int n);  
	Vector GetPoint(int n);
	Vector GetCorner(int n);
	
	Bool   IsWithin(const Vector& p) ;
	
	friend ostream& operator <<(ostream & os, Quad q)
		{ return 
			  os << "QUAD:  " 
				 << "corner1 = (" << q.corner[1].x << ", " << q.corner[1].y << ") ;  " 
				 << "corner2 = (" << q.corner[2].x << ", " << q.corner[2].y << ") ;  " 
				 << "corner3 = (" << q.corner[3].x << ", " << q.corner[3].y << ") ;  " 
				 << "corner4 = (" << q.corner[4].x << ", " << q.corner[4].y << ")"
			  ; 
		}
	void Print();
	
	
private:
	Vector corner[4] ;

} ;


/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                        Arc Class                                             */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

class Arc {
public:
	
	Arc() ;

	Arc(const Vector c, const float r1,
		 const float r2, const float start, const float span) ;
	
	bool operator ==(const Arc &a)
		{ 
			return (center == a.center) 
				&& (min_radius == a.min_radius)
				&& (max_radius == a.max_radius) 
				&& (angle_start == a.angle_start)
				&& (angle_span == a.angle_span); 
		}
	

	Vector GetCenter()     { return center; }
	
	float GetMinRadius()   { return min_radius; }
	float GetSmallRadius() { return min_radius; }
	
	float GetMaxRadius()   { return max_radius; }
	float GetLargeRadius() { return max_radius; }
	
	float GetAngleStart()  { return angle_start; }
	float GetStartAngle()  { return angle_start; }
	
	float GetAngleSpan()   { return angle_span; }
	float GetSpanAngle()   { return angle_span; }
	
	Bool   IsWithin(const Vector& p) ;
	
	friend ostream& operator <<(ostream & os, Arc a)
		{ return os << "ARC:  center = (" << a.center.x << ", " << a.center.y << ") ;  min_radius = " << a.min_radius << " ;  max_radius = " << a.max_radius << " ;  angle_start = " << a.angle_start << " ; angle_span = " << a.angle_span ; }
	void Print();
	
	
private:
	Vector center;
	float min_radius;
	float max_radius;
	float angle_start;
	float angle_span;

} ;
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                  Miscellaneous Geometry Functions                            */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

int QuadraticFormula(float a, float b, float c, float* psol1, float* psol2);

inline int RayCircleIntersect(Ray r, float rad, Vector center,
			      Vector* psol1, Vector* psol2)
{ return r.CircleIntersect(rad, center, psol1, psol2); }
  

int LineCircleIntersect(Line l, float rad, Vector center,
			Vector* psol1, Vector* psol2);

Vector AdjustPtToRectOnLine(Vector pt, Rectangle r, Line l);

Bool InBetween(Vector pt, Vector end1, Vector end2);

Vector PointInBetween(Vector pt1, Vector pt2, float pt1dist=0);

AngleDeg AngleBisect(AngleDeg a1, AngleDeg a2);

Vector GetClosestPtInBetween(Vector pt, Vector end1, Vector end2);

Bool IsPointInCone(Vector pt, float wid_dist_ratio, Vector end, Vector vert);


/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/*                  PointSet                                                    */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
#define BUF_SIZE 1024
#define COMMENT_SIGN '#'

class PointSet
{
        private:

        vector<Vector*> points;

        public:

        PointSet(char* filename);

        void read_file(char* filename);
        void print();

        int num_points();
        Vector* get_point(int i);
};
#endif
