#define BSIZE 4096
#ifndef LIBMYSYSLOG_TEXT
#define LIBMYSYSLOG_TEXT
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

void 
write_txt(const char* msg, const char* ps, int level, int format, FILE* filename);

#endif
