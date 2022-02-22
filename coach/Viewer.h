#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <GetOpt.h>
#include "param.h"
#include "types.h"
#define PI M_PI
#include "netif.h"
#include "TimeSlice.h"
#include "consts.h"

#define WAIT		100000

class Viewer {
  Port port;
  bool not_scaled;

 public:
  Viewer(void);
  int main(int argc, char* argv[]);

 protected:
  void init(void);
  void usage(void);
  dispinfo_t* drawTimeSlice(TimeSlice*);
  dispinfo_t* showTimeSliceInfos(TimeSlice*, int);
  vector<dispinfo_t*> Viewer::drawRegionalTimeSlice(TimeSlice*, int);
  string itoa(int);
};

