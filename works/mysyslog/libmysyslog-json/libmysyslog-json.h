/* libmysyslog-json.h contains useful definitions and enumerations for 
the libmysyslog json driver*/
#include <time.h>
#include <json-c/json.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BSIZE 4096
int
write_json (const char *msg, const char *ps,
            int level, int format, FILE * file);
