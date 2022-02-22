#include "Viewer.h"

Viewer::Viewer(void) {
  return;
}

int Viewer::main(int argc, char* argv[]) {
  GetOpt getopt(argc, argv, "f:nv");
  int option_char;
  bool verbose = false;
  string filename = "";

  while ((option_char = getopt()) != EOF) {
    switch (option_char) {
    case 'f': filename = getopt.optarg; break;
    case 'v': verbose = true; break;
	default: usage();
    }
  }

  if (filename == "") {
    usage();
  }
 
  init();

  int features;
  FILE* file = fopen(filename.c_str(), "r");
  fscanf(file, "%d\n", &features);
  vector<TimeSlice*> slices;

  int slice_counter = 0;
  while (!feof(file)) {
    TimeSlice* slice = new TimeSlice();
    if (features == REGIONAL_FEATURES) {
      if (verbose) {
	cerr << "loading regional slice" << endl;
      }
      slice->loadRegionalSliceFromFile(file);
      slice->undoRegionalScale();
    }
    else {
      if (verbose) {
	cerr << "loading non-regional slice: " << REGIONAL_FEATURES << " != " << features << endl;
      }
      slice->loadSliceFromFile(file);
      slice->undoScale();
    }
    slices.push_back(slice);
    slice_counter++;
  }
  fclose(file);

  int option;
  int current = -1;
  int work = 1;
  cout << "There are " << slice_counter << " slices loaded. Enter a servertime " <<
    "to view that slice (0 to quit): ";
  while (scanf("%d", &option)) {
    if (option == 0) {
      exit(0);
    }
    if (option == -1) {
      for (std::vector<TimeSlice*>::iterator slice = slices.begin(); slice != slices.end(); slice++) {
	cout << (*slice)->getServerTime() << endl;
      }
      continue;
    }
    int counter = 0;
    bool found = false;
    for (std::vector<TimeSlice*>::iterator slice = slices.begin(); slice != slices.end(); slice++) {
      if ((*slice)->getServerTime() == option) {
	found = true;
	option = counter + 1;
	break;
      }
      counter++;
    }
    if (!found) {
      cout << endl << "No slice for servertime " << option << " found" << endl;
      cout << "There are " << slice_counter << " slices loaded. Enter a servertime " <<
	"to view that slice (0 to quit): ";
      continue;
    }
    dispinfo_t* info = (dispinfo_t*)malloc(sizeof(dispinfo_t));
    info->mode = htons(DRAW_MODE);
    info->body.draw.mode = htons(DrawClear);
    port.send_info(info, port.top.next->shost, port.top.next->sport);
    if (features == 30) {
	dispinfo_t* info = drawTimeSlice(slices[option - 1]);
	info->body.show.time = htons(option);
	port.send_info(info, port.top.next->shost, port.top.next->sport) ;
    }
    else if (features == REGIONAL_FEATURES) {
      vector<dispinfo_t*> infos = drawRegionalTimeSlice(slices[option - 1], option);
	for (std::vector<dispinfo_t*>::iterator info = infos.begin(); info != infos.end(); info++) {
	  (*info)->body.show.time = htons(slices[option - 1]->getServerTime());
	  port.send_info(*info, port.top.next->shost, port.top.next->sport) ;
	}
    }
    info = showTimeSliceInfos(slices[option - 1], option);
    info->body.show.time = htons(option);
    port.send_info(info, port.top.next->shost, port.top.next->sport);
  }

  return 0;
}

void Viewer::usage(void) {
  cerr << "Usage: ./viewer -f filename\n";
  exit(USAGE_ERROR);
}

void Viewer::init(void) {
  struct timeval timeout ;
  port.init() ;
  cout << "Waiting for monitor!\n" ;
  
  while (1) {
    port.recv_info();
    if (!strcmp(port.rbuf, "(dispinit)")) {
      break;
    }
    timeout.tv_sec = 0;
    timeout.tv_usec = WAIT;
    select(0, NULL, NULL, NULL, &timeout);
  }
}

string Viewer::itoa(int i) {
  char buffer[32];
  sprintf(buffer, "%d", i);
  string s(buffer);
  return s;
}

dispinfo_t* Viewer::showTimeSliceInfos(TimeSlice* slice, int index) {
  dispinfo_t* picture = (dispinfo_t*)malloc(sizeof(dispinfo_t));
  
  picture->mode = htons(MSG_MODE);

  picture->body.msg.board = htons(LOG_BOARD);
  string s =  itoa(index) + ": pass, goalkick, losing ball in "
    + itoa(slice->getTimeTillPass()) + ", " 
    + itoa(slice->getTimeTillGoalkick()) + ", " 
    + itoa(slice->getTimeTillLosingBall()) + "\n";
  strcpy(picture->body.msg.message, s.c_str());

  return picture;
}

vector<dispinfo_t*> Viewer::drawRegionalTimeSlice(TimeSlice* slice, int index) {
  vector<dispinfo_t*> infos;
  //slice->undoRegionalScale();
  global_pos ballPos = slice->getBallPos();

  //draw grid:
  for (int i = 0; i <= NUMBER_OF_REGIONS; i++) {
    dispinfo_t* info = (dispinfo_t*)malloc(sizeof(dispinfo_t));
    info->mode = htons(DRAW_MODE);
    info->body.draw.mode = htons(DrawLine);
    double x1, x2, y1, y2;
    x1 = ballPos.x - (REGION_SIZE * NUMBER_OF_REGIONS / 2);
    x2 = ballPos.x + (REGION_SIZE * NUMBER_OF_REGIONS / 2);
    y1 = (ballPos.y + (NUMBER_OF_REGIONS * REGION_SIZE / 2)) - (i * REGION_SIZE);
    y2 = y1;
    info->body.draw.object.linfo.x1 = htons((short)rint(x1));
    info->body.draw.object.linfo.y1 = htons((short)rint(y1));
    info->body.draw.object.linfo.x2 = htons((short)rint(x2));
    info->body.draw.object.linfo.y2 = htons((short)rint(y2));
    strcpy(info->body.draw.object.linfo.color, (index % 2) == 0 ? "blue" : "red");
    infos.push_back(info);

    info = (dispinfo_t*)malloc(sizeof(dispinfo_t));
    info->mode = htons(DRAW_MODE);
    info->body.draw.mode = htons(DrawLine);
    x1 = (ballPos.x + (NUMBER_OF_REGIONS * REGION_SIZE / 2)) - (i * REGION_SIZE);
    x2 = x1;
    y1 = ballPos.y - (REGION_SIZE * NUMBER_OF_REGIONS / 2);
    y2 = ballPos.y + (REGION_SIZE * NUMBER_OF_REGIONS / 2);
    info->body.draw.object.linfo.x1 = htons((short)rint(x1));
    info->body.draw.object.linfo.y1 = htons((short)rint(y1));
    info->body.draw.object.linfo.x2 = htons((short)rint(x2));
    info->body.draw.object.linfo.y2 = htons((short)rint(y2));
    strcpy(info->body.draw.object.linfo.color, (index % 2) == 0 ? "blue" : "red");
    infos.push_back(info);
  }
  
  //draw ball (center of grid):
  dispinfo_t* info = (dispinfo_t*)malloc(sizeof(dispinfo_t));
  info->mode = htons(SHOW_MODE);
  info->body.show.pos[0].enable = htons(DISABLE);
  info->body.show.pos[0].side = htons(NEUTRAL);
  info->body.show.pos[0].unum = htons(0);
  info->body.show.pos[0].angle = htons(0);
  info->body.show.pos[0].x = htons((short)rint(ballPos.x * SHOWINFO_SCALE));
  info->body.show.pos[0].y = htons((short)rint(ballPos.y * SHOWINFO_SCALE));
  int player_counter = 1;

  //draw players:
  double player_width = 2;
  int players_per_line = 3;
  for (int i = 0; i < NUMBER_OF_REGIONS; i++) {
    for (int i2 = 0; i2 < NUMBER_OF_REGIONS; i2++) {
      int region_player_counter = 0;
      for (int i4 = 0; i4 < 2; i4++) {
	for (int i3 = 0; i3 < slice->regions[i][i2][i4]; i3++) {
	  info->body.show.pos[player_counter].enable = htons(STAND);
	  /*
	  if (i4 == 0) {
	    cout << "drawing LEFT\n";
	  }
	  else {
	    cout << "drawing RIGHT\n";
	  }
	  */
	  info->body.show.pos[player_counter].side = htons(i4 == 0 ? LEFT : RIGHT);
	  info->body.show.pos[player_counter].unum = htons(player_counter);
	  info->body.show.pos[player_counter].angle = htons(0);
	  double x = (ballPos.x - (REGION_SIZE * NUMBER_OF_REGIONS / 2)) + 
	    (i * REGION_SIZE) + ((region_player_counter % players_per_line) * player_width) +
	    (player_width / 2);
	  double y = (ballPos.y - (REGION_SIZE * NUMBER_OF_REGIONS / 2)) + 
	    (i2 * REGION_SIZE) + (((int)(region_player_counter / players_per_line)) * player_width) 
	    + (player_width / 2);
	  info->body.show.pos[player_counter].x = htons((short)rint(x * SHOWINFO_SCALE));
	  info->body.show.pos[player_counter].y = htons((short)rint(y * SHOWINFO_SCALE));
	  player_counter++;
	  region_player_counter++;
	}
      }
    }
  }

  infos.push_back(info);
  
  return infos;
}

dispinfo_t* Viewer::drawTimeSlice(TimeSlice* slice) {
  dispinfo_t* picture = (dispinfo_t*)malloc(sizeof(dispinfo_t));

  picture->mode = htons(SHOW_MODE);

  //slice->undoScale();
  slice->computeGlobalPlayerPos();

  //Ball:
  global_pos ball = slice->getBallPos();
  picture->body.show.pos[0].enable = htons(DISABLE);
  picture->body.show.pos[0].side = htons(NEUTRAL);
  picture->body.show.pos[0].unum = htons(0);
  picture->body.show.pos[0].angle = htons(0);
  picture->body.show.pos[0].x = htons((short)rint(ball.x * SHOWINFO_SCALE));
  picture->body.show.pos[0].y = htons((short)rint(ball.y * SHOWINFO_SCALE));

  playerinfo_t* player = slice->getNearest();

  for (int i = 0; i < 8; i++) {
    picture->body.show.pos[i + 1].enable = htons(STAND);
    picture->body.show.pos[i + 1].side = htons(player[i].side);
    picture->body.show.pos[i + 1].unum = htons(i);
    picture->body.show.pos[i + 1].angle = htons((short)rint(player[i].bodyangle));
    picture->body.show.pos[i + 1].x = htons((short)rint(player[i].pos.x * SHOWINFO_SCALE));
    picture->body.show.pos[i + 1].y = htons((short)rint(player[i].pos.y * SHOWINFO_SCALE));
  }

  for (int i = 9; i < MAX_PLAYER * 2 + 1; i++) {
    picture->body.show.pos[i].enable = htons(DISABLE);
  }

  picture->body.show.pmode = htons(PM_PlayOn);

  strcpy(picture->body.show.team[0].name, "Left2Right");
  picture->body.show.team[0].score = htons(0);

  strcpy(picture->body.show.team[1].name, "Right2Left");
  picture->body.show.team[1].score = htons(0);

  return picture;
}



