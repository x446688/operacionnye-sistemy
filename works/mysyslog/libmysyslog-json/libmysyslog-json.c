#include "libmysyslog-json.h"
#include "../libmysyslog/libmysyslog.h"

void
write_json(const char* msg, const char* ps, int level, int format, FILE* file)
{
    time_t t = time(NULL);
	char* timestr = asctime(localtime(&t)); // until a better solution comes to mind
	timestr[strlen(timestr)-sizeof(char)] = '\0'; // asctime
    struct json_object *root = json_object_new_object();
    switch (format) {
		case FORMAT_COMPACT:
            json_object_object_add(root, "timestamp", 
                                   json_object_new_int64(t));
			break;
		case FORMAT_NORMAL:
            json_object_object_add(root, "timestamp", 
                                   json_object_new_string(timestr));
			break;
		default:
            json_object_object_add(root, "timestamp", 
                                   json_object_new_int64(t));
			break;
	}
    json_object_object_add(root, "log_level", 
                           json_object_new_string(log_level_strings[level]));
    json_object_object_add(root, "process", 
                           json_object_new_string(ps));
    json_object_object_add(root, "message", 
                           json_object_new_string(msg));
	fprintf(file, "%s\n", 
			json_object_to_json_string(root));	  	
}