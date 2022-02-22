#include "dbAccess.h"
#include "consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

string dbAccess::itoa(int i) {
  char buffer[32];
  sprintf(buffer, "%d", i);
  string s(buffer);
  return s;
}

string dbAccess::dtoa(double d) {
  char buffer[128];
  sprintf(buffer, "%f", d);
  string s(buffer);
  return s;
}

dbAccess::dbAccess(string connect) : PgDatabase(connect.c_str()) {
  if (ConnectionBad()) {
    cerr << "Cannot connect to " << connect << ": " << ErrorMessage() << endl;
    exit(DB_ERROR);
  }
}

/* Executes a command on a database-handle.
   Exists if command fails. */
void dbAccess::exec(string sql) {
  if (!ExecCommandOk(sql.c_str())) {
    cerr << "Executing \"" << sql << "\" failed: " << ErrorMessage() << endl;
    exit(DB_ERROR);
  }
}

void dbAccess::select(string sql) {
  if (!ExecTuplesOk(sql.c_str())) {
    cerr << "Executing \"" << sql.c_str() << "\" failed: " << ErrorMessage() << endl;
    exit(DB_ERROR);
  }
  int rows = Tuples();
  int columns = Fields();
  vector<vector<string> > data;
  for (int i = 0; i < rows; i++) {
    vector<string> row;
    for (int i2 = 0; i2 < columns; i2++) {
      row.push_back(GetValue(i, i2));
    }
    data.push_back(row);
  }

}

/* gets a new id from a table (through the indices table) */
int dbAccess::getID(string table, int amount) {
  select("SELECT " + table + " FROM indices");
  int id = atoi(GetValue(0, 0));
  id += amount;
  exec("UPDATE indices SET " + table + " = " + itoa(id));

  return id;
}

int dbAccess::getID(string table) {
  return getID(table, 1);
}
