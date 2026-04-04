.PHONY: all clean run
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -I./libmysyslog -I./libmysyslog-text -I./libmysyslog-json
LDFLAGS = -shared -ldl

LIB_DIR = libmysyslog
TEXT_DIR = libmysyslog-text
JSON_DIR = libmysyslog-json
CLIENT_DIR = libmysyslog-client
DAEMON_DIR = libmysyslog-daemon
PARSER_DIR = $(DAEMON_DIR)/libparse

LIB_TARGET = $(LIB_DIR)/libmysyslog.so
TEXT_TARGET = $(TEXT_DIR)/libmysyslog-text.so
JSON_TARGET = $(JSON_DIR)/libmysyslog-json.so
CLIENT_TARGET = $(CLIENT_DIR)/syslog-client
DAEMON_TARGET = $(DAEMON_DIR)/syslog-daemon

LIB_SRC = $(LIB_DIR)/libmysyslog.c
TEXT_SRC = $(TEXT_DIR)/libmysyslog-text.c
JSON_SRC = $(JSON_DIR)/libmysyslog-json.c
CLIENT_SRC = $(CLIENT_DIR)/syslog-client.c
DAEMON_SRC = $(DAEMON_DIR)/syslog-daemon.c

LIB_HEADER = $(LIB_DIR)/libmysyslog.h
JSON_HEADER = $(JSON_DIR)/libmysyslog-json.h
TEXT_HEADER = $(TEXT_DIR)/libmysyslog-text.h
PARSER_HEADER = $(PARSER_DIR)/parser.h

all: $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) $(CLIENT_TARGET) $(DAEMON_TARGET)

$(DAEMON_TARGET): $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) $(PARSER_HEADER)
	make -C $(DAEMON_DIR)

$(CLIENT_TARGET): $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET)
	make -C $(CLIENT_DIR)

$(LIB_TARGET): $(LIB_SRC) $(LIB_HEADER)
	make -C $(LIB_DIR)

$(TEXT_TARGET): $(TEXT_SRC) $(TEXT_HEADER) $(LIB_HEADER) $(LIB_SRC)
	make -C $(TEXT_DIR)

$(JSON_TARGET): $(JSON_SRC) $(JSON_HEADER) $(LIB_HEADER) $(LIB_SRC)
	make -C $(JSON_DIR)

clean:
	rm -f $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) $(CLIENT_TARGET)
	rm -f $(LIB_DIR)/*.o $(TEXT_DIR)/*.o $(CLIENT_DIR)/*.o $(JSON_DIR)/*.o
	rm -f *.txt
	make -C $(LIB_DIR) clean
	make -C $(CLIENT_DIR) clean
	make -C $(DAEMON_DIR) clean
	make -C $(TEXT_DIR) clean
	make -C $(JSON_DIR) clean

install:
	cp $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) /usr/lib
	cp $(LIB_HEADER) $(JSON_HEADER) $(TEXT_HEADER) $(PARSER_HEADER) /usr/include
	make -C $(DAEMON_DIR) install
	make -C $(CLIENT_DIR) install
