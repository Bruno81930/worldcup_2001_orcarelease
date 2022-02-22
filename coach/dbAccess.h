#include <string>
#include <libpq++.h>

class dbAccess : PgDatabase{ 
  
 public:
  dbAccess(string connect);
  string itoa(int i);
  string dtoa(double d);
  void exec(string sql);
  void select(string sql);
  int getID(string table);
  int getID(string table, int amount);
};
