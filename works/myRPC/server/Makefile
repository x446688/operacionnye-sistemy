.PHONY: all
TARGET = myRPC-server
all: src/myRPC-server.c src/myRPC.h src/parser/parser.h src/mysyslog/libmysyslog.h
	test -f /usr/lib/x86_64-linux-gnu/libmysyslog.so && echo "mysyslog already installed" || ../bootstrap.sh
	test -d /etc/myRPC/ && echo "conf dir exists." || mkdir /etc/myRPC/
	: > /var/log/myRPC.log
	: > /etc/myRPC/myRPC.conf
	echo root > /etc/myRPC/users.conf
	chown -R `whoami` /etc/myRPC
	echo "port = 7777" > /etc/myRPC/myRPC.conf
	echo "socket_type = stream" >> /etc/myRPC/myRPC.conf
	gcc src/myRPC-server.c -o $(TARGET) -lmysyslog -ljson-c
