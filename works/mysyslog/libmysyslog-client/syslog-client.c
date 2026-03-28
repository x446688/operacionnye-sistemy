#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libmysyslog/libmysyslog.h"
int
main ()
{
	mysyslog("dummy", LOG_LVL_INFO, DRIVER_JSON, FORMAT_NORMAL, "./test.txt");
	//mysyslog("dummy", LOG_LVL_INFO, DRIVER_JSON, FORMAT_COMPACT, "./test.txt");
}