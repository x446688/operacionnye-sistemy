#include "libmysyslog-text.h"
#include "../libmysyslog/libmysyslog.h"


void
write_txt(const char* msg, const char* path, int level)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	if (fd < 0) {
		perror("open");
	}
	char intbuf[BSIZE];
	snprintf(intbuf, BSIZE, "%jd %s TODO %s\n", 
			 (intmax_t)time(NULL), log_level_strings[level], msg);	  	
	write(fd, intbuf, (size_t)strlen(intbuf));
	close(fd);
}
