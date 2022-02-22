#ifndef _FOIL_H_
#define _FOIL_H_

#include <vector.h>
#include <stdio.h>
#include <string>
#include "TimeSlice.h"



class Foil {
 public:
  Foil(string, string, bool);
  void go(short, TimeSlice*);
  void send(string);
  void send(vector<string>);
  string filePostfix;
  void go(short, TimeSlice*, string, Eventtype);
  
 private:
  string exec;
  string output;
  FILE* process;
  FILE* debugFile;
  bool debug;
  vector<string> facts;
};

#endif

