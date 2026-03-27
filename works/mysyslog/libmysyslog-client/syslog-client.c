#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libmysyslog/libmysyslog.h"
int
main ()
{
	mysyslog("dummy", LOG_LVL_INFO, DRIVER_TEXT, 0, "./test.txt");
}