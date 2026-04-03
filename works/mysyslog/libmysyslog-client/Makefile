CC = gcc
SRC = src/syslog-client.c
LIB_DIR = ../libmysyslog
TEXT_DIR = ../libmysyslog-text
JSON_DIR = ../libmysyslog-json
TARGET = syslog-client

$(TARGET): $(SRC)
	$(CC) -Wall -Wextra -o $@ $(SRC) \
		-I$(LIB_DIR) -I$(TEXT_DIR) -I$(JSON_DIR)\
		-L$(LIB_DIR) -L$(TEXT_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-text -lmysyslog-json -ldl 
clean:
	rm -fr $(TARGET)