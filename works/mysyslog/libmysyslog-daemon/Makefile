CC = gcc
SRC = src/syslog-daemon.c
LIB_DIR = ../libmysyslog
PARSE_DIR = libparse
JSON_DIR = ../libmysyslog-json
TARGET = syslog-daemon

$(TARGET): $(SRC)
	$(CC) -Wall -Wextra -g -o $@ $(SRC) \
		-I$(LIB_DIR) -I$(JSON_DIR) -I$(PARSE_DIR)\
		-L$(LIB_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-json -ldl 
clean:
	rm -fr $(TARGET)
	