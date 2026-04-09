# libmysyslog-daemon (systemd)

## What am I installing?
`libmysyslog-daemon` - is a daemon that was made to test the capabilities of libmysyslog and it has a good interface for library interaction. It provides a convenient configuration file and a service file. The daemon randomly outputs data on different log levels, using different drivers and formats.

## Installation
```sh
make deb
dpkg -i mysyslog-daemon.deb
```
This will install:
1.  `mysyslog-daemon` to `/usr/bin/`
2. `mysyslog.conf` to `/etc/mysyslog/`
3. `mysyslog-daemon.service` to `/usr/lib/systemd/system/`
## Usage

To test the daemon without running it daemonized (with systemd you can directly run the binary file from `/usr/bin/`).

```sh
mysyslog-daemon --conf_file /etc/mysyslog/mysyslog.conf --log_file /var/log/mysyslog/msl.log
```

You can also run the service with systemctl:

```sh
sudo systemctl start mysyslog-daemon.service
sudo systemctl status mysyslog-daemon.service
```

The list of all available options may be viewed using `--help` or `-h` as a parameter of the daemon binary.

`--help / -h` - print the help message  
`--conf_file / -c filename` - read configuration from `filename`  
`--test_conf / -t filename` - test configuration from `filename` for matching conf [file options](package/etc/mysyslog/mysyslog.conf)  
`--log_file / -l filename` - write logs to `filename`  
`--daemon / -d` - daemonize the application  
`--pid_file / -p filename` - PID file used by daemonized app  