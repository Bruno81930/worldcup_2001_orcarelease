#include <math.h>
#include "statsutil.h"

/** gets a vector of double values and returns the arithemtic mean*/
double getMeanOfVector(vector<double> values){
	if(values.size()==0){
		cerr << "Warning, calculating mean of empty vector." << endl;
		return 0;
	}

	double sum=0;
	for(int i=0; i<values.size(); i++){
		sum+=values[i];
	}

	return sum/values.size();
}


/** gets a vector of doubles and the mean and returns the deviation */
double getDeviationOfVector(vector<double> values, double mean){
	if(values.size()==0){
		cerr << "Warning, calculating deviation of empty vector." << endl;
		return 0;
	}

	double sum=0;
	for(int i=0; i<values.size(); i++){
		sum+=(values[i]-mean)*(values[i]-mean);
	}

	return sqrt(sum/(values.size()-1));
}



/** calculates the "Produkt-Moment-Korrelation" of two vectors */
double getPMKOfVectors(vector<double> xvalues, vector<double> yvalues, double xmean,
				double ymean){
	if((xvalues.size()!= yvalues.size()) || xvalues.size()==0){
		cerr << "Warning, trying to calculate PMK for nonsense vectors." << endl;
		return 0;
	}

	double topsum=0;
	double xsum=0;
	double ysum=0;

	for(int i=0; i<xvalues.size(); i++){
		topsum+=(xvalues[i]-xmean)*(yvalues[i]-ymean);
		xsum+=(xvalues[i]-xmean)*(xvalues[i]-xmean);
		ysum+=(yvalues[i]-ymean)*(yvalues[i]-ymean);
	}
	return topsum/sqrt(xsum*ysum);

}




/** same as overloaded getPMKofVectors(), but calculates means on its own */
double getPMKOfVectors(vector<double> xvalues, vector<double> yvalues){
	double xmean=getMeanOfVector(xvalues);
	double ymean=getMeanOfVector(yvalues);
	return getPMKOfVectors(xvalues, yvalues, xmean, ymean);
}





