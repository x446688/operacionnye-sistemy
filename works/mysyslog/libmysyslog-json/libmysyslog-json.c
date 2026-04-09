/* The libmysyslog-json.c file contains the main function to write jsonified
text to a file, defined by a FILE pointer. */

#include "libmysyslog-json.h"
#include <libmysyslog.h>

/* the write_json function takes a message, a process name, 
a format, a log level and a file and writes the message 
with the following process name, format, level respectively. 
returns number of bytes written or a negative value if an 
error occured. */
int
write_json (const char *msg, const char *ps,
            int level, int format, FILE * file)
{
  time_t t = time (NULL);
  char *timestr = asctime (localtime (&t));
  timestr[strlen (timestr) - sizeof (char)] = '\0';
  struct json_object *root = json_object_new_object ();
  switch (format)
    {
    case FORMAT_COMPACT:
      json_object_object_add (root, "timestamp", json_object_new_int64 (t));
      break;
    case FORMAT_NORMAL:
      json_object_object_add (root, "timestamp",
                              json_object_new_string (timestr));
      break;
    default:
      json_object_object_add (root, "timestamp", json_object_new_int64 (t));
      break;
    }
  json_object_object_add (root, "log_level",
                          json_object_new_string (log_level_strings[level]));
  json_object_object_add (root, "process", json_object_new_string (ps));
  json_object_object_add (root, "message", json_object_new_string (msg));
  int ret_res = fprintf (file, "%s\n",
                         json_object_to_json_string (root));
  return ret_res;
}
