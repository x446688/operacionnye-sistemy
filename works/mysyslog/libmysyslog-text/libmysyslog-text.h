#define BSIZE 4096
#ifndef LIBMYSYSLOG_TEXT
#define LIBMYSYSLOG_TEXT
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "libmysyslog.h"
#include <stdint.h>
#include <string.h>

void 
write_txt(const char* msg, const char* path, int level);

#endif
