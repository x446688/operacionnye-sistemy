#pragma once
#ifndef BSIZE
#define BSIZE 4096
#endif
extern char *__progname; 
const char * log_level_strings [] = 
{
	"   DEBUG", 
	"    INFO", 
	"    WARN", 
	"   ERROR", 
	"CRITICAL",  
};

enum log_level
{
	LOG_LVL_DEBUG,
	LOG_LVL_INFO, 
	LOG_LVL_WARN, 
	LOG_LVL_ERROR,  
	LOG_LVL_CRITICAL
};

enum driver
{	
	DRIVER_UNKNOWN,
	DRIVER_TEXT,
	DRIVER_JSON
};

enum format
{
	FORMAT_COMPACT,
	FORMAT_NORMAL	
};
int
mysyslog (const char* msg, int level, int driver, int format,
		  const char* path);