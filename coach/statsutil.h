
#ifndef STATSUTIL_H_
#define STATSUTIL_H_

#include <vector.h>

double getMeanOfVector(vector<double> values);
double getDeviationOfVector(vector<double> values, double mean);


double getPMKOfVectors(vector<double> xvalues, vector<double> yvalues, double xmean,
				double ymean);
double getPMKOfVectors(vector<double> xvalues, vector<double> yvalues);

#endif

