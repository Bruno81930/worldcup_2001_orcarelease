
#ifndef _MY_GEOMETRY_H_
#define _MY_GEOMETRY_H_


#include "types.h"
#include <vector.h>

typedef struct{		// difference to pos_t: coordinates are double!
	double x;
	double y;
}global_pos;


typedef struct{		// specifies a region that is a union of grid-sections
	int top;	// row of topmost section
	int left;	// column of leftmost section
	int bottom;
	int right;
	int confidence;	// percentage of how often the player is in this region
} regionOfSections;




/** A RectGrid contains a lattice which accumulates the cycles in which a player
 * is in the different sections.
 */
// number of vertical regions:
#define VER_SECTIONS 7
// number of horizontal regions:
#define HOR_SECTIONS 11
class RectGrid{
	int section[HOR_SECTIONS][VER_SECTIONS];	// the lattice
public:
	void incrementSection(int x, int y);		// add one to section
	void setSection(int x, int y, int value);	// set value of section
	int  getSection(int x, int y);			// get value of section
	void printLattice(void);			// print lattice to stdout
	global_pos* topleft(int x, int y);		// get top,left coord of section
	global_pos* bottomright(int x, int y);		// get bottom,right coord of sect.
	int getVER_SECTIONS();
	int getHOR_SECTIONS();
	RectGrid(void);					// constructor
};


regionOfSections* unionSections(vector<int> toBeUnioned, RectGrid * grid);
			// gets a vector of sections and unions them
int overlayRegions(regionOfSections * off, regionOfSections * def);
void sortRegions(vector<regionOfSections*> &regions, short mode, vector<int> &unums,
				short sortside);
global_pos getXY(regionOfSections * region, short clockwise);


// contains info about players' homeposition:
// in each field[x][y] there is a vector containing indizes 0..10
// of players that have their (or one of their) homepositions there
typedef	vector<int>  Teampositions[HOR_SECTIONS][VER_SECTIONS];



double distanceBetweenTwoPositions(pos_t a, pos_t b);
double angleBetweenTwoPositions(pos_t a, pos_t b);

double distanceBetweenTwoPositions(global_pos a, global_pos b);
double angleBetweenTwoPositions(global_pos a, global_pos b);

bool betweenAngles(double angle, double val1, double val2);

double transform_coord(short x);	// transform coordinates
double transform_coordV3(long x);	// transform coordinates
double mysin(double x);
double mycos(double x);

global_pos pointOnRay(global_pos start,double angle, double length);
void intersectVert(global_pos start, global_pos vectorline, double x,
                         global_pos &result);
void intersectHoriz(global_pos start, global_pos vectorline, double y,
                         global_pos &result);
double scale360(double angle);
double scale_from_to(double value, double now_low, double now_high, double to_low,
		     double to_high);
		     
double doubleabs(double a);
double Deg2Rad(double x);
double Rad2Deg(double x);

void qsortX(global_pos[11], int, int);

#endif

