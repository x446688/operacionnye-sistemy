#include "libmysyslog-json.h"
#include "../libmysyslog/libmysyslog.h"

int
write_json(const char* msg, const char* ps, int level, const char* path)
{
    unsigned long now = (unsigned long)time(NULL);
    struct json_object *root = json_object_new_object();
    json_object_object_add(root, "timestamp", 
        json_object_new_int64(now));
    json_object_object_add(root, "level", 
        json_object_new_string(log_level_strings[level]));
    json_object_object_add(root, "process", 
        json_object_new_string(ps));
    json_object_object_add(root, "message", 
        json_object_new_string(msg));
    int fd = open(path, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
    char intbuf[BSIZE];
	snprintf(intbuf, BSIZE, "%s\n", 
			 json_object_to_json_string(root));	  	
    int bw = write(fd, intbuf, (size_t)strlen(intbuf));
    close(fd);
    return bw;
}