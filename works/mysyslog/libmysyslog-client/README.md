# libmysyslog-client

## What am I installing?
`libmysyslog-client` - is an utility that was made to test the capabilities of libmysyslog and it has a good interface for library interaction.

## Installation
```sh
make deb
dpkg -i mysyslog-client.deb
```
This will install `mysyslog-client` to `/usr/bin`.

## Usage
```sh
mysyslog-client --msg "success" --path "log.txt" --driver "2" --format "1" --level "1"
```

`--msg / -m` - the message that needs to be passed to the log  
`--level / -l` - defined log level (check out [`libmysyslog.h`](../libmysyslog/libmysyslog.h) for log_level)  
`--driver / -d` - defined driver (one or more of [`libmysyslog-text`](../libmysyslog-text) or [`libmysyslog-json`](../libmysyslog-json) is required to be installed)  
`--format / -f` - defined timestamp output format (check out [`libmysyslog.h`](../libmysyslog/libmysyslog.h) for format)  
`--path / -p` - log file path