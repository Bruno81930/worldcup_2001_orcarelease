/*! \file
 *
 * log messages from modules and output them
 *
 */

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include "logging.h"

static int debuglevel=99;
static int facility=LOG_LEVELS;
static FILE* outstream=NULL;

static int
vmakelog(int plevel, int facility, const char* format, va_list parms)
{
  if(!outstream)
    vfprintf(stderr, format, parms);
  else
    vfprintf(outstream, format, parms);
  fflush(outstream);
  return 1;  
}

void
logparameter(int pfacility, int plevel)
{
  if(pfacility!=-1)
    facility=pfacility;
  if(plevel!=-1)
    debuglevel=plevel;
}

void
logFile(FILE* poutstream)
{
  if(outstream&&outstream!=stdout&&outstream!=stderr)
    fclose(outstream);
  outstream=poutstream;
}

void
logFileName(const char* pfilename)
{
  FILE *file=fopen(pfilename, "w");
  assert(file);
  logFile(file);
}

int
makelog(int plevel, int pfacility, const char* format, ...)
{
  int retval=0;
  va_list valist;

  if(!(facility&pfacility)||(plevel>debuglevel)) {
    return 1;
  }
  va_start(valist, format);
  retval=vmakelog(plevel, pfacility, format, valist);
  va_end(valist);
  return 1;  
}
