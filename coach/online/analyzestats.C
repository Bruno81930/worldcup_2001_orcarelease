
#include "../TimeSlice.h"
#include "../types.h"


extern int verbose;	// print debug information?



// This function gathers statistics about ballpossession and ballposition
// If erase is true, statistics will be reset
// Result is LEFT, if the ball is more often in the right half than in the left.
int whichSideDominates(TimeSlice* slice, bool erase){
	// Initialize with 1 to avoid division by zero:
	// how many cycles is ball in the left half?
	static int cyclesBallLeft=1;	
	static int cyclesBallRight=1;
	//how many cycles is the ball possessed by left team?
	static int cyclesPossessionLeft=1;
	static int cyclesPossessionRight=1;
	
	if(erase){
		cyclesBallLeft=1;	
		cyclesBallRight=1;
		cyclesPossessionLeft=1;
		cyclesPossessionRight=1;
	}
	
	// 2do count only if playmode is play_on or so...
	if(slice->ballInLeftHalf()) cyclesBallLeft++;
	if(slice->ballInRightHalf()) cyclesBallRight++;
	if(slice->ballPossessedByLeftTeam()) cyclesPossessionLeft++;
	if(slice->ballPossessedByRightTeam()) cyclesPossessionRight++;

	
	double dominatingHalf = cyclesBallLeft/(double)cyclesBallRight;
	double dominatingPoss = cyclesPossessionLeft/(double)cyclesPossessionRight;
	
	if(verbose) cout << "Dominatinghalf: " << dominatingHalf << endl;
	
	if(dominatingHalf<1)
		return LEFT;
	else{
		if(dominatingHalf!=1){
			return RIGHT;
		}else{
			return NEUTRAL;
		}
	}
}







void analyzeSlices(vector<TimeSlice*>slices){
	TimeSlice * newest;
	int newestIndex=slices.size()-1;	
						
	if(newestIndex<0) return;	// there is no slice yet
	
	newest=(TimeSlice *)slices[newestIndex];
	if(newest==NULL) cerr << "THIS SHOULD NEVER HAPPEN!" << endl;
	if(newest->getAlreadyAnalyzed()) return;	// we did this already
	newest->setAlreadyAnalyzed(true);		// lock
	
	// cycles between goal and kick_off do not count to domination:
	if((newest->getPlayMode()!=PM_AfterGoal_Left) &&
		(newest->getPlayMode()!=PM_AfterGoal_Right)){
		int domination=whichSideDominates(newest, false);
		// 2do use true to reset stats after applying new tactic and save timestamp
		if(domination==LEFT){
			if(verbose) cout << "Left team dominates." << endl;
		}else{
			if(domination==RIGHT){
				if(verbose) cout << "Right team dominates." << endl;
			}else{
				if(verbose) cout << "Neither team dominates." << endl;
			}
		}
	}
	// 2do do more stuff here
	// ...


}


