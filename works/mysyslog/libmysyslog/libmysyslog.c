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
	printf("%s\n%s\n%d %d %d",msg,path,level,driver,format);
	FILE* file = fopen(path,"aw");
	if (file == NULL) {
        perror("fopen");
    }
	void* handle;
	switch(driver){
		// (const char* msg, int level, int format, const char* path)
		case DRIVER_UNKNOWN: return -1;
		case DRIVER_TEXT: 
			handle = dlopen("libmysyslog-text.so", RTLD_LAZY);
			if (!handle){
				perror("handle");
				return -1;
			}
			void (*write_txt) (const char* msg, 
							const char* ps,
							int level,
							int format,
							FILE* file) = dlsym(handle,"write_txt");
			write_txt(msg, "", level, format, file);
			break;
		case DRIVER_JSON: 
			handle = dlopen("libmysyslog-json.so", RTLD_LAZY);
			if (!handle){
				perror("handle");
				return -1;
			} // ИМПОРТИРОВАТЬ ТОЛЬКО ПРИ СОВПАДЕНИИ С КЕЙСОМ
			void (*write_json) (const char* msg, 
								const char* ps,
								int level,
								int format,
								FILE* file) = dlsym(handle,"write_json");
			write_json(msg, "", level, format, file);
			break;
	}
	fclose(file);
	dlclose(handle);
	return 0;
}
