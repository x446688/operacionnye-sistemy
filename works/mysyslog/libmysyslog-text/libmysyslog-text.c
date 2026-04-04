#include "libmysyslog-text.h"
#include "../libmysyslog/libmysyslog.h"

void
write_txt(const char* msg, const char* ps, int level, int format, FILE* file)
{	
	time_t t = time(NULL);
	char* timestr = asctime(localtime(&t)); // until a better solution comes to mind
	timestr[strlen(timestr)-sizeof(char)] = '\0'; // asctime
	switch (format) {
		case FORMAT_COMPACT:
			fprintf(file,"%ld %s %s %s\n", (intmax_t)t, log_level_strings[level], __progname, msg);
			break;
		case FORMAT_NORMAL:
			fprintf(file,"%s %s %s %s\n", timestr, log_level_strings[level], __progname , msg);
			break;
		default:
			break;
	}
}
