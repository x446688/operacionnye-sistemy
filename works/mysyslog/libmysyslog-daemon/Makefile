.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
CLIENT_SRC = src/syslog-daemon.c
LIB_DIR = ../libmysyslog
PARSE_DIR = libparse
JSON_DIR = ../libmysyslog-json
DAEMON-TARGET = syslog-daemon

$(DAEMON-TARGET): $(CLIENT_SRC)
	$(CC) -Wall -Wextra -g -o $@ $(CLIENT_SRC) \
		-I$(LIB_DIR) -I$(JSON_DIR) -I$(PARSE_DIR)\
		-L$(LIB_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-json -ldl 
clean:
	rm -fr $(DAEMON-TARGET)
	