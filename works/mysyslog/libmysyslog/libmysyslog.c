#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include <stdint.h> // Для конвертации времени в int
#include "libmysyslog.h"

int
mysyslog (const char* msg, int level, int driver, int format,
		  const char* path)
{
	switch(driver){
		case DRIVER_UNKNOWN: return -1;
		case DRIVER_TEXT: break;
		case DRIVER_JSON: break;
	}
	switch(format){
		case FORMAT_COMPACT: break;
		case FORMAT_NORMAL: break;
		case FORMAT_VERBOSE: break;
	}
	void* handle;
	handle = dlopen("libmysyslog-text.so", RTLD_LAZY);
	if (!handle){
		perror("handle");
		return -1;
	}
	void (*write_txt) (const char* msg, 
					   const char* path, 
					   int level) = dlsym(handle,"write_txt");
	write_txt(msg, path, level);
	dlclose(handle);
	return 0;
}
