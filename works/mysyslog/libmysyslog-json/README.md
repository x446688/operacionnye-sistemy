# libmysyslog-json

## What am I installing?
`libmysyslog-text` - the json driver for the libmysyslog library. 

## Installation

> Note: Requires precisely json-c5 to be installed

```sh
make deb
dpkg -i libmysyslog-json.deb
```
This will install `libmysyslog-json.so` to `/usr/lib/`.
## Usage
The driver is used with the main `libmysyslog.so` library or `libmysyslog-client`. For more information please review [`libmysyslog.c`](../libmysyslog/libmysyslog.c) on examples of use.