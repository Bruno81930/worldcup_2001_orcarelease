/* -*- Mode: C++ -*- */

/* utils.h
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


#ifndef _UTILS_
#define _UTILS_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBG_CMU		1
#define DBG_OFFENSE	2
#define DBG_DEFENSE	4
#define DBG_GOALIE	8
#define DBG_COMM	16
#define DBG_COACH	32
#define DBG_TRAINER	64
#define DBG_FORM	128
#define DBG_SFL		256
#define DBG_OTHER	512
#define DBG_ANY         2<<30
#define DBG_ALL            ~0

#ifndef LOG_LEVELS
#define LOG_LEVELS  (DBG_ANY)
#endif

#ifdef DEBUG
#define MAKELOG(a) makelog a
#else
#define MAKELOG(a) ;
#endif

int makelog(int plevel, int facility, const char* format, ...);

void logparameter(int facility, int level);
void logFile(FILE* poutstream);
void logFileName(const char* plogfilename);

#ifdef __cplusplus
}
#endif


#define my_stamp printf("%d:%d.%d ",Mem->MyNumber,Mem->CurrentTime.t,Mem->CurrentTime.s);

int dump_core(char*);

void my_error(char*);
void my_error(char*,int);
void my_error(char*,int,int);
void my_error(char*,int,int,int);
void my_error(char*,int,int,int,int);
void my_error(char*,int,int,int,int,int);
void my_error(char*,int,int,int,int,int,int);
void my_error(char*,int,int,int,int,int,char,int);
void my_error(char*,float);
void my_error(char*,float,float);
void my_error(char*,float,float,float);
void my_error(char*,float,float,float,float);
void my_error(char*,float,int);
void my_error(char*,char*);
void my_error(char*,char,int,int);
void my_error(char*,char,int,float,float);

int closest_int( float x );

typedef float Value;
typedef float AngleRad;
typedef float AngleDeg;

inline AngleDeg Rad2Deg(AngleRad x) { return x * 180 / M_PI; }
inline AngleRad Deg2Rad(AngleDeg x) { return x * M_PI / 180; }

/* needed? */
/* inline float cos(AngleRad x) { return cos((float) x); } */
/* inline float sin(AngleRad x) { return sin((float) x); } */
/* inline float tan(AngleRad x) { return tan((float) x); } */

inline float Cos(AngleDeg x) { return cos(Deg2Rad(x)); }
inline float Sin(AngleDeg x) { return sin(Deg2Rad(x)); }
inline float Tan(AngleDeg x) { return tan(Deg2Rad(x)); }
inline AngleDeg ACos(float x)           { return ((x) >= 1 ? 0 : ((x) <= -1 ? 180 : (Rad2Deg(acos(x))))); }
inline AngleDeg ASin(float x)           { return ((x) >= 1 ? 90 : ((x) <= -1 ? -90 : (Rad2Deg(asin(x))))); }
inline AngleDeg ATan(float x)           { return (Rad2Deg(atan(x))); }
inline AngleDeg ATan2(float x, float y) { return ((x == 0 && y == 0) ? 0 : (Rad2Deg(atan2(x,y)))); } 

void NormalizeAngleDeg(int*);
void NormalizeAngleDeg(AngleDeg*);
void NormalizeAngleRad(AngleRad*);
AngleDeg GetNormalizeAngleDeg(AngleDeg);
float GetDistance(float *x, float *y, float *a, float *b);

#define FLOAT_EPS .001

#define Mod(a,b) (a - (b)*(int)((a)/(b)))
#define Sign(x) ((x) >= 0 ? 1 : -1)

float int_pow(float x, int p);
inline int Sqr(int x){ return x*x; }
inline float Sqr(float x) { return x*x; }
inline  float Exp(float x, int y) { float a = 1; for (int i=0; i<y; i++) a*=x; return a; }

inline float SumInfGeomSeries(float first_term, float r)
{ return first_term / (1 - r); }
float SumGeomSeries(float first_term, float r, int n);
/* returns -1 on error */
float SolveForLengthGeomSeries(float first_term, float r, float sum);
float SolveForFirstTermGeomSeries(float r, int n, float sum);
inline float SolveForFirstTermInfGeomSeries(float r, float sum)
{ return sum * (1 - r); }

#define signf(x) ( ((x) > 0.0) ? 1.0 : -1.0) 
inline float Round(float x, int p=0)
{
  x *= int_pow(10.0, -p);
  if (fmod(x, 1.0) >= .5)
    return ceil(x) / int_pow(10.0, -p);
  else
    return floor(x) / int_pow(10.0, -p);
} 

extern const char char_for_num_array[16];
inline char char_for_num(int num)
{ return char_for_num_array[num]; }



/* returns a pointer to a static buffer, so be careful! */
char* repeat_char(char c, int n);

class Time {
public:
  int t; /* time from the server */
  int s; /* stopped clock cycles */

  Time(int vt = 0, int vs = 0) { t = vt; s = vs; }
  Time operator - (const int &a);
  int  operator - (const Time &a);
  Time operator + (const int &a);
  int  operator % (const int &a) { return (t+s)%a; }
  void operator -=(const int &a) { *this = *this - a; }
  void operator -=(const Time &a){ *this = *this - a; }
  void operator +=(const int &a) { *this = *this + a; }
  void operator ++()             { *this += 1; }
  void operator --()             { *this -= 1; }
  Time operator = (const int &a) { t = a; s = 0; return *this; }
  bool operator ==(const Time &a) { return (s == a.s) && (t == a.t); }
  bool operator ==(const int &a)  { return t == a; }
  bool operator !=(const Time &a) { return (s != a.s) || (t != a.t); }
  bool operator !=(const int &a)  { return t != a; }
  bool operator < (const Time &a) { return ( t < a.t ) || ( t == a.t && s < a.s ); }
  bool operator < (const int &a)  { return t < a; }
  bool operator <=(const Time &a) { return ( t < a.t ) || ( t == a.t && s <= a.s ); }
  bool operator <=(const int &a)  { return t <= a; }
  bool operator > (const Time &a) { return ( t > a.t ) || ( t == a.t && s > a.s ); }
  bool operator > (const int &a)  { return t > a; }
  bool operator >=(const Time &a) { return ( t > a.t ) || ( t == a.t && s >= a.s ); }
  bool operator >=(const int &a)  { return t >= a; }
  bool operator !() { return (s == 0) && (t == 0); }

  Bool CanISubtract(const Time &a);
};

#define Min(x,y) ((x) < (y) ? (x) : (y))
#define Max(x,y) ((x) > (y) ? (x) : (y))
#define MinMax(min, x, max) Min(Max((min),(x)), (max))
/*inline float Min(float x, float y)
{ return x < y ? x : y; }
inline int Min(int x, int y)
{ return x < y ? x : y; }
inline Time Min(Time x, Time y)
{ return x < y ? x : y; }
inline float Max(float x, float y)
{ return x > y ? x : y; }
inline int Max(int x, int y)
{ return x > y ? x : y; }
inline float Max(int x, float y)
{ return x > y ? x : y; }
inline float Max(float x, int y)
{ return x > y ? x : y; }
inline Time Max(Time x, Time y)
{ return x > y ? x : y; }
inline float MinMax(float min, float x, float max)
{ return Min(Max(min,x), max); }
inline int MinMax(int min, int x, int max)
{ return Min(Max(min,x), max); }
inline Time MinMax(Time min, Time x, Time max)
{ return Min(Max(min,x), max); }
*/

extern double get_double(char **str_ptr);
extern double get_double(char *str);
extern float  get_float (char **str_ptr);
extern float  get_float (char *str);
extern int    get_int   (char **str_ptr);
extern int    get_int   (char *str);
extern void   get_word  (char **str_ptr);
extern void   get_next_word (char **str_ptr);
extern void   get_token  (char **str_ptr);
extern void   advance_to(char c, char **str_ptr);
extern void   advance_past_space(char **str_ptr);

extern int put_int(char *str, int num);
extern int put_float(char *str, float fnum, int precision);

extern void BubbleSort  (int length, int *elements, float *keys);
extern int  BinarySearch(int length, float *elements, float key);
extern void StrReplace  (char *str, char oldchar, char newchar);

extern int   int_random(int n);
extern float range_random(float lo, float hi);
extern int   very_random_int(int n);

extern float weighted_avg(float val1, float val2, float w1, float w2);

extern void GetStampedName( char *name, char *outputName );
#endif
