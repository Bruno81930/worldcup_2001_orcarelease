#include "Foil.h"
#include <fstream>

Foil::Foil(string executable, string options, bool deb) {
  output = "/tmp/foil.tmp";
  filePostfix = "_header.foil";
  exec = executable + " " + options + " -v0" + " >" + output;
  debug = deb;
}

void Foil::go(short side, TimeSlice* slice) {
  go(side, slice, "pass", passevent);
  go(side, slice, "dribble", dribbleevent);
  go(side, slice, "shoot", shootevent);
}

void Foil::go(short side, TimeSlice* slice, string file, Eventtype event) {
  int timeframe = 6000;
  double test_ratio = 4.0 / 5.0;
  int numberOfTests = (int)(test_ratio * timeframe);
  int numberOfExamples = timeframe - numberOfTests; 
  double cone_angle = 45.0;

  if (debug) {
    debugFile = fopen("/tmp/foil.output", "w");
  }

  process = popen(exec.c_str(), "w");

  string filename = file + filePostfix;
  ifstream header(filename.c_str());
  string line;
  while (getline(header, line)) {
    if (line == "§") break; //EOF
    send(line);
  }

  vector<string> positives = slice->foilPassDribbleShoot(numberOfExamples, event, side, 
							 MODE_POSITIVE, cone_angle);
  vector<string> negatives = slice->foilPassDribbleShoot(numberOfExamples, event, side, 
							 MODE_NEGATIVE, cone_angle);

  send(positives);
  send(";");
  send(negatives);
  send(".");

  send("");
  send(file);

  for (int i = 0; i < numberOfExamples; i++) {
    if (slice == NULL) {
      break;
    }
    slice = slice->getPredecessor();
  }

  vector<string> tests = slice->foilPassDribbleShoot(numberOfTests, event, side, MODE_TEST, cone_angle);
  send(tests);
  send(".");

  pclose(process);
  fclose(debugFile);

  ifstream foilResult(output.c_str());
  short lineMode = 0;
  vector<string> relations = *(new vector<string>);
  string time;
  int errors, trials;
  while (getline(foilResult, line)) {
    if (strncmp(line.c_str(), "pass:", 5) == 0) {
      lineMode = 1;
    }
    else if (strncmp(line.c_str(), "Time ", 5) == 0) {
      lineMode = 2;
    }
    else if (strncmp(line.c_str(), "Summary: ", 9) == 0) {
      lineMode = 3;
    }

    switch(lineMode) {
    case 1: 
      if (line != "") {
	relations.push_back(line);
      }
      break;
    case 2:
      time = line;
      lineMode = 0;
      break;
    case 3:
      sscanf(line.c_str(), "Summary: %d errors in %d trials", &errors, &trials);
      lineMode = 0;
      break;
    default:
      break;
    }
  }
  
  cout << "\n\n\nResult:\n=======\n\n";
  for (int i = 0; i < relations.size(); i++) {
    cout << relations[i] << endl;
  }
  cout << endl << "Errorrate: " << (((double)errors / (double)trials) * 100) << "%\n\n\n";

}

void Foil::send(vector<string> strings) {
  for (int i = 0; i < strings.size(); i++) {
    send(strings[i]);
  }
}

void Foil::send(string s) {
  fprintf(process, "%s\n", s.c_str());
  if (debug) {
    fprintf(debugFile, "%s\n", s.c_str());
  }
  cerr << s << endl;
}
