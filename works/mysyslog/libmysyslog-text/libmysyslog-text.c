/* The libmysyslog-text.c file contains the main function to write
text to a file, defined by a FILE pointer. */

#include "libmysyslog-text.h"
#include <mysyslog/libmysyslog.h>

/* the write_txt function takes a message, a process name, 
a format, a log level and a file and writes the message 
with the following process name, format, level respectively. 
returns number of bytes written or a negative value if an 
error occured. */
int
write_txt (const char *msg, const char *ps,
           int level, int format, FILE * file)
{
  time_t t = time (NULL);
  char *timestr = asctime (localtime (&t));
  int res_ret;
  timestr[strlen (timestr) - sizeof (char)] = '\0';
  switch (format)
    {
    case FORMAT_COMPACT:
      res_ret = fprintf (file, "%ld %s %s %s\n", (intmax_t) t,
                         log_level_strings[level], __progname, msg);
      break;
    case FORMAT_NORMAL:
      res_ret = fprintf (file, "%s %s %s %s\n", timestr,
                         log_level_strings[level], __progname, msg);
      break;
    default:
      break;
    }
  return res_ret;
}
