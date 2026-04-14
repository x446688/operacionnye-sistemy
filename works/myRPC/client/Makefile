.PHONY: all
TARGET = myRPC-client
all: src/myRPC-client.c src/mysyslog/libmysyslog.h
	chown `whoami` /var/log/myRPC.log
	test -f /usr/lib/x86_64-linux-gnu/libmysyslog.so && echo "mysyslog already installed" || ../bootstrap.sh
	gcc src/myRPC-client.c -o $(TARGET) -lmysyslog -ljson-c
