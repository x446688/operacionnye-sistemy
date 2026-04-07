# libmysyslog

## What am I installing?
`libmysyslog` - is a library that allows developers to output text to logfiles using text drivers.

## Installation
```sh
make deb
dpkg -i libmysyslog.deb
```
This will install `libmysyslog.so` to `/usr/lib`.
## Usage
```c
#include <libmysyslog.h>
mysyslog (
          const char* msg, 
          int level, 
          int driver, 
          int format, 
          const char* path
)
```

`msg` - the message that needs to be passed to the log  
`level` - defined log level (check out [`libmysyslog.h`](libmysyslog.h) for log_level)  
`driver` - defined driver (one or more of [`libmysyslog-text`](../libmysyslog-text) or [`libmysyslog-json`](../libmysyslog-json) is required to be installed)  
`format` - defined timestamp output format (check out [`libmysyslog.h`](libmysyslog.h) for format)  
`path` - log file path  