/**
 *
 * Facilities:
 * -----------
 *
 *  DBG_ANY = if no facility is given
 *  DBG_ALL = always
 *    1	    = Defaultplaytree 
 *    2	    = Communication
 *    4	    = Memory
 *  
 * Levels:
 * -------
 *    1     = very important
 *   ...
 *    99    = least important
 *
 **/

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#define DBG_FACILITY1       1
#define DBG_FACILITY2       2
#define DBG_FACILITY3       4
#define DBG_ANY         2<<30
#define DBG_ALL            ~0

#ifndef LOG_LEVELS
#define LOG_LEVELS  (DBG_ANY)
#endif

#ifdef DEBUG
#define MAKELOG(a) makelog a
#else
#define MAKELOG(a) ;
#endif

int makelog(int plevel, int facility, const char* format, ...);

void logparameter(int facility, int level);
//void logFile(FILE* poutstream);
void logFileName(const char* plogfilename);

#ifdef __cplusplus
}
#endif

#endif


