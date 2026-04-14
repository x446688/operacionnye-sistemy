/* The libmysyslog.c file contains the main logic of the libmysyslog 
library as well as handling calls to different drivers (text and json).

Libmysyslog is a library that allows it's user to output data to a logfile 
in different formats */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include <stdint.h>
#include "libmysyslog.h"

/* the mysyslog function takes a message, a format, 
a log level and a filepath and writes the message 
with the invoker's process name, format, level respectively
using the defined driver. returns 0 on success. */
int
mysyslog (const char *msg, int level, int driver, int format,
          const char *path)
{
  void *handle;
  FILE *file = fopen (path, "aw");
  if (file == NULL)
    perror ("libmysyslog.so fopen");
  switch (driver)
    {
    case DRIVER_UNKNOWN:
      fprintf (stderr, "libmysyslog could not load an available driver.");
      return -1;
    case DRIVER_TEXT:
      handle = dlopen ("libmysyslog-text.so", RTLD_LAZY);
      if (!handle)
        {
          fprintf (stderr, "libmysyslog-text.so: %s\n", dlerror ());
          return -1;
        }
      void (*write_txt) (const char *msg,
                         const char *ps,
                         int level,
                         int format,
                         FILE * file) = dlsym (handle, "write_txt");
      write_txt (msg, __progname, level, format, file);
      dlclose (handle);
      break;
    case DRIVER_JSON:
      handle = dlopen ("libmysyslog-json.so", RTLD_LAZY);
      if (!handle)
        {
          fprintf (stderr, "libmysyslog-json.so: %s\n", dlerror ());
          return -1;
        }
      void (*write_json) (const char *msg,
                          const char *ps,
                          int level,
                          int format,
                          FILE * file) = dlsym (handle, "write_json");
      write_json (msg, __progname, level, format, file);
      dlclose (handle);
      break;
    }
  fclose (file);
  return 0;
}
