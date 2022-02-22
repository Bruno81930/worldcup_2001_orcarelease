#include "types.h"
#include "geometry.h"
#include "param.h"
#include <math.h>
#include <iostream.h>
#include <netinet/in.h>

#define PI 3.141592654


void qsortX(global_pos pos[11], int left, int right) {
  int i, last;
  if (left >= right) return;

  global_pos tmp = pos[left];
  pos[left] = pos[(left + right) / 2];
  pos[(left + right) / 2] = tmp;

  last = left;
  for (i = left + 1; i <= right; i++) {
    if (pos[i].x < pos[left].x) {
      last++;
      tmp = pos[last];
      pos[last] = pos[i];
      pos[i] = tmp;
    }
  }
  tmp = pos[left];
  pos[left] = pos[last];
  pos[last] = tmp;
  qsortX(pos, left, last - 1);
  qsortX(pos, last + 1, right);
    
}


// 2do maybe add noice-tolerance-threshold?
// is angle between angles val1 and val2?
// all values have to be in 0..360
// val2 has to be the angle to the position clockwise to val1
bool betweenAngles(double angle, double val1, double val2){
	if((angle<0) || (angle>360) || (val1<0) || (val1>360) || (val2<0) || (val2>360)){
		cerr << "Using betweenAngles with wrong values. Scale to 0..360 first!!"
		     << endl;
		exit(1);
	}
	if(val1>val2){	// val1 to the left, val2 to the right
			// eg. val1=330, val2=20, angle=10 or angle=350
		if((angle>val1) && (angle<360)) return true;
		if((angle<val2) && (angle>=0)) return true;
		return false;
	}
	if(val1<=val2){
		if((angle>=val1) && (angle<=val2)) return true; else return false;
	}
	cerr << "Should never reach this point in betweenAngles!" << endl;
	return false;
}





// transforms value, which is between now_low and now_high, to intervall to_low, to_high
double scale_from_to(double value, double now_low, double now_high, double to_low,
		     double to_high){
	double intervall_now=now_high-now_low;
	double intervall_to=to_high-to_low;
	if(intervall_now==0) cerr << "Division by zero in scale_from_to!" << endl;
	return ((value-now_low)/intervall_now)*intervall_to+to_low;
}





// transform angles into 0..360
double scale360(double angle){
	while(angle<0) angle+=360;
	while(angle>360) angle-=360;
	return angle;
}



// returns a point that is on the ray, that starts at start with the specified angle,
// at the specified distance
global_pos pointOnRay(global_pos start,double angle, double length){
	global_pos end=*new global_pos;
	end.x=start.x+mycos(angle)*length;
	end.y=start.y-mysin(angle)*length;
	return end;
}



// calculate intersection between horizontal line at y and line from start+n*vectorline
void intersectHoriz(global_pos start, global_pos vectorline, double y, global_pos &result){
	double n;

	if(vectorline.y!=0){
		n=(y-start.y)/vectorline.y;
	}else{
		result.y=y+10.0;// user knows, that point.y = y should be true, so
		result.x=0.0; 	// signal that line never meets y
		return;
	}
	if(n<0){	// opposite direction??
		result.y=y+10.0;// user knows, that point.y = y should be true, so
		result.x=0.0; 	// signal that line never meets y
		return;
	}
	result.y=y;
	result.x=start.x + n*vectorline.x;
}

// calculate intersection between vertical line at x and line from start+n*vectorline
void intersectVert(global_pos start, global_pos vectorline, double x, global_pos &result){
	double n;

	if(vectorline.x!=0){
		n=(x-start.x)/vectorline.x;
	}else{
		result.x=x+10.0;// user knows, that point.x = x should be true, so
		result.y=0.0; 	// signal that line never meets x
		return;
	}
	if(n<0){	// opposite direction??
		result.x=x+10.0;// user knows, that point.x = x should be true, so
		result.y=0.0; 	// signal that line never meets x
		return;
	}
	result.x=x;
	result.y=start.y + n*vectorline.y;
}




double transform_coord(short x){
	return (signed short)ntohs(x)/16.0;
}


double transform_coordV3(long x){
	return (long)(ntohl(x))/SHOWINFO_SCALE2;
}


double Deg2Rad(double x){
	return (x * PI/180.0);
}


double Rad2Deg(double x){
 	return (x * 180.0/PI);
}


// takes angles, converts to radians
double mysin(double x){
	return sin(Deg2Rad(x));
}



// takes angles, converts to radians
double mycos(double x){
	return cos(Deg2Rad(x));
}





double myAtan2(double x, double y){
	if(y<0)
		return((180.0/PI)*(-1*acos(x)));
	else
		return((180.0/PI)*acos(x));
}



// return positive value of double a
double doubleabs(double a){
	if(a<0) return -a;
	return a;
}



double distanceBetweenTwoPositions(pos_t a, pos_t b){
	// transform into host byte order and scale:
	double ax=transform_coord(a.x);
	double ay=transform_coord(a.y);
	double bx=transform_coord(b.x);
	double by=transform_coord(b.y);
	
	return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}


double angleBetweenTwoPositions(pos_t a, pos_t b){
	// transform into host byte order and scale:
	double ax=transform_coord(a.x);
	double ay=transform_coord(a.y);
	double bx=transform_coord(b.x);
	double by=transform_coord(b.y);

	
	double xdiff=bx-ax;
	double ydiff=by-ay;
	double length=sqrt(xdiff*xdiff+ydiff*ydiff);
	double adir;

	if(length!=0.0){
		xdiff/=length;
		ydiff/=length;
		adir=myAtan2(xdiff,ydiff);
	}else{
		adir=0.0;
	}
	return adir;
}



double distanceBetweenTwoPositions(global_pos a, global_pos b){
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}




double angleBetweenTwoPositions(global_pos a, global_pos b){
	double xdiff=b.x-a.x;
	double ydiff=b.y-a.y;
	double length=sqrt(xdiff*xdiff+ydiff*ydiff);
	double adir;

	if(length!=0.0){
		xdiff/=length;
		ydiff/=length;
		adir=myAtan2(xdiff,ydiff);
	}else{
		adir=0.0;
	}
	return adir;
}


// ############################################################
// ###################### Class RectGrid ######################
// ############################################################

// constructor
RectGrid::RectGrid(){
	// initialise section[][] with zero
	for(int x=0; x<HOR_SECTIONS; x++)
		for(int y=0; y<VER_SECTIONS; y++)
			section[x][y]=0;
}


// add one to section in RectGrid
void RectGrid::incrementSection(int x, int y){
	if((x<HOR_SECTIONS) && (x>-1) && (y<VER_SECTIONS) && (y>-1))
		section[x][y]++;
	else
		cerr << "Accessing non-existing section in RectGrid!" << endl;
	return;
}


// get value of specified section
int RectGrid::getSection(int x, int y){
	if((x<HOR_SECTIONS) && (x>-1) && (y<VER_SECTIONS) && (y>-1))
		return section[x][y];
	else
		cerr << "Accessing non-existing section in RectGrid!" << endl;
	return -1;
}


// set value of specified section
void RectGrid::setSection(int x, int y, int value){
	if((x<HOR_SECTIONS) && (x>-1) && (y<VER_SECTIONS) && (y>-1))
		section[x][y]=value;
	else
		cerr << "Accessing non-existing section in RectGrid!" << endl;
	return;
}


// print lattice to stdout
void RectGrid::printLattice(){
	for(int y=0; y<VER_SECTIONS; y++){
		for(int x=0; x<HOR_SECTIONS; x++){
			cout << section[x][y] << "   ";
			if(section[x][y]<10) cout << " ";
		}
		cout << endl;
	}
	return;
}



// get top-left coordinate of specified section
global_pos* RectGrid::topleft(int x, int y){
	global_pos *topleft=new global_pos;
	topleft->x=-(PITCH_LENGTH/2)+x*(PITCH_LENGTH/HOR_SECTIONS);
	topleft->y=-(PITCH_WIDTH/2)+y*(PITCH_WIDTH/VER_SECTIONS);
	return topleft;
}


// get bottom-right coordinate of specified section
global_pos* RectGrid::bottomright(int x, int y){
	global_pos *bottomright=new global_pos;
	bottomright->x=-(PITCH_LENGTH/2)+(x+1)*(PITCH_LENGTH/HOR_SECTIONS);
	bottomright->y=-(PITCH_WIDTH/2)+(y+1)*(PITCH_WIDTH/VER_SECTIONS);
	return bottomright;
}

int RectGrid::getVER_SECTIONS(){
	return VER_SECTIONS;
}


int RectGrid::getHOR_SECTIONS(){
	return HOR_SECTIONS;
}


// ##################### END OF CLASS RECTGRID ##################


regionOfSections* unionSections(vector<int> toBeUnioned, RectGrid * grid){
	int top=grid->getVER_SECTIONS(),bottom=0,left=grid->getHOR_SECTIONS(),right=0;
	int auxtemp;
	for(int i=0; i<toBeUnioned.size(); i=i+2){
		// x (left, right)
		auxtemp=(int)toBeUnioned[i];
		if(auxtemp<left) left=auxtemp;
		if(auxtemp>right) right=auxtemp;
		// y (top, bottom)
		auxtemp=(int)toBeUnioned[i+1];
		if(auxtemp<top) top=auxtemp;
		if(auxtemp>bottom) bottom=auxtemp;
	}
	regionOfSections* unionedSections=NULL;
	unionedSections=new regionOfSections;
	unionedSections->top=top;
	unionedSections->bottom=bottom;
	unionedSections->left=left;
	unionedSections->right=right;
	unionedSections->confidence=0;
	for(int sectionx=left; sectionx<=right; sectionx++){
		for(int sectiony=top; sectiony<=bottom; sectiony++){
			unionedSections->confidence+=grid->getSection(sectionx, sectiony); 
		}
	}
	return unionedSections;
}



/** checks how much two regions overlap */
int overlayRegions(regionOfSections * off, regionOfSections * def){
	if((off==NULL) || (def==NULL)){
		cerr << "THIS SHOULD NEVER EVER HAPPEN!" << endl;
		return 0;	
	}
	if((off->right < off->left) || (off->top > off->bottom) ||
	   (def->right < def->left) || (def->top > def->bottom)){
	 	cerr << "Warning, regionOfSections inconsistent." << endl;
		return 0;  
	}
	int xspan=0, yspan=0;	// range of overlapping region
	// first xspan:
	if(off->right <= def->right){	// off is more to the left
		if(off->left <= def->left){	// off extends to the left
			xspan=off->right - def->left+1;
			//cout << "x: a ist generell weiter links" << endl;
		}else{				// off is within def
			xspan=off->right - off->left+1;  // span of offense
			//cout << "x: a ist ganz drin" << endl;
		}	
	}else{				// off is more to the right
		if(def->left <= off->left){	// def at least up to the left end
			xspan=def->right - off->left+1;
			//cout << "x: a ist generell weiter rechts" << endl;			
		}else{				// def totally within off
			xspan=def->right - def->left+1;	// span of defense
			//cout << "x: b ist drin" << endl;			
		}
	}
	// now yspan:
	if(off->bottom <= def->bottom){	
		if(off->top <= def->top){	
			yspan=off->bottom - def->top+1;
			//cout << "y: a ist generell weiter oben" << endl;			
		}else{				
			yspan=off->bottom - off->top+1;  
			//cout << "y: a ist drin" << endl;			
		}	
	}else{				
		if(def->top <= off->top){
			yspan=def->bottom - off->top+1;
			//cout << "y: a ist generell weiter unten" << endl;
		}else{
			yspan=def->bottom - def->top+1;
			//cout << "y: b ist drin" << endl;			
		}
	}
	//cout << "xspan: " << xspan << " yspan: " << yspan << endl;
	// now calculate overlapping region:
	if((yspan<1) || (xspan<1))
		return 0;
	else
		return (xspan*yspan);

}



/** sort a vector of regionOfSections by a given component
 *  This will also maintain the vector of corresponding unums of the opponent players
 *  if sortside is LEFT, big values will be at the beginning.
 */
void sortRegions(vector<regionOfSections*> &regions, short mode, vector<int> &unums,
					short sortside){
  // Mmmmhhhh.......BubbleSort
  regionOfSections *auxi;			// auxiliary value for swapping
  short auxiUnum;
  short factor;					// negation factor
      
  if(sortside==LEFT)
  	factor=1;
  else
  	factor=-1;
  //cout << "bla: factor: " << factor << endl;


  bool 	swapped=true;
  while(swapped){
    swapped=false;
    for(int j=1; j<regions.size(); j++){
      if(		// compare depending on mode
	  ((mode==1) && (factor*regions[j]->top > factor*regions[j-1]->top)) ||
	  ((mode==2) && (factor*regions[j]->left > factor*regions[j-1]->left)) ||
      	  ((mode==3) && (factor*regions[j]->bottom > factor*regions[j-1]->bottom)) ||
          ((mode==4) && (factor*regions[j]->right > factor*regions[j-1]->right))
	){				// swap
      	swapped=true;
	
	auxi=regions[j-1];
	regions[j-1]=regions[j];
	regions[j]=auxi;
	
	auxiUnum=unums[j-1];
	unums[j-1]=unums[j];
	unums[j]=auxiUnum;
      }
    }
  }
}



/** return the XY-coordinates of the points in a regionOfSections.
 *  clockwise denotes the point, starting at top, left and continuing clockwise
 *  Note: this is about the outer points, so calculation is different between top&bottom
 *  and left*right.
 */
global_pos getXY(regionOfSections * region, short clockwise){
	double x,y;
	double length=PITCH_LENGTH/HOR_SECTIONS;
	double width=PITCH_WIDTH/VER_SECTIONS;
	global_pos *result=new global_pos;
	result->x=0;
	result->y=0;

	switch(clockwise){
		case 0:		// top-left
			x=-PITCH_LENGTH/2+region->left*length;
			y=-PITCH_WIDTH/2+region->top*width;
			break;
		case 1:		// top-rigt
			x=-PITCH_LENGTH/2+(region->right+1)*length;
			y=-PITCH_WIDTH/2+region->top*width;
			break;		
		case 2:		// bottom-right
			x=-PITCH_LENGTH/2+(region->right+1)*length;
			y=-PITCH_WIDTH/2+(region->bottom+1)*width;
			break;	
		case 3:		// bottom-left
			x=-PITCH_LENGTH/2+region->left*length;
			y=-PITCH_WIDTH/2+(region->bottom+1)*width;
			break;
		default:
			cerr << "Error: Using getXY with wrong argument" << endl;
			return *result;
	}
	result->x=x;
	result->y=y;
	return *result;
}



