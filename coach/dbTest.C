#include "dbAccess.h"
#include "consts.h"
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Please supply a database connection string" << endl;
    exit(1);
  }
  dbAccess db(argv[1]);

  db.getID("player");
}

/*
#include <libpq++.H>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Please supply a database connection string" << endl;
    exit(1);
  }
  PgDatabase data(argv[1]);
  
  if ( data.ConnectionBad() ) {
    cerr << "Connection to database '" << argv[1] << "' failed." << endl
	 << "Error returned: " << data.ErrorMessage() << endl;
    exit(1);
  }
  if (!data.ExecCommandOk("SELECT player FROM indices")) {
    cerr << "Executing \"SELECT player FROM indices\" failed: " << data.ErrorMessage() << endl;
    exit(DB_ERROR);
  }
  exit(0);
}
*/
