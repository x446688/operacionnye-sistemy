.PHONY: all clean run
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g -I./libmysyslog -I./libmysyslog-text
LDFLAGS = -shared -ldl

LIB_DIR = libmysyslog
TEXT_DIR = libmysyslog-text
CLIENT_DIR = libmysyslog-client

LIB_TARGET = $(LIB_DIR)/libmysyslog.so
TEXT_TARGET = $(TEXT_DIR)/libmysyslog-text.so
CLIENT_TARGET = $(CLIENT_DIR)/syslog-client

LIB_SRC = $(LIB_DIR)/libmysyslog.c
TEXT_SRC = $(TEXT_DIR)/libmysyslog-text.c
CLIENT_SRC = $(CLIENT_DIR)/syslog-client.c

LIB_HEADER = $(LIB_DIR)/libmysyslog.h
TEXT_HEADER = $(TEXT_DIR)/libmysyslog-text.h

all: $(LIB_TARGET) $(TEXT_TARGET) $(CLIENT_TARGET)

$(LIB_TARGET): $(LIB_SRC) $(LIB_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LIB_SRC)

$(TEXT_TARGET): $(TEXT_SRC) $(TEXT_HEADER) $(LIB_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(TEXT_SRC)

$(CLIENT_TARGET): $(CLIENT_SRC) $(LIB_HEADER) $(TEXT_HEADER)
	$(CC) -Wall -Wextra -g -o $@ $(CLIENT_SRC) \
		-I$(LIB_DIR) -I$(TEXT_DIR) \
		-L$(LIB_DIR) -L$(TEXT_DIR) \
		-lmysyslog -lmysyslog-text -ldl \
		-Wl,-rpath=$(shell pwd)/$(LIB_DIR):$(shell pwd)/$(TEXT_DIR)

clean:
	rm -f $(LIB_TARGET) $(TEXT_TARGET) $(CLIENT_TARGET)
	rm -f $(LIB_DIR)/*.o $(TEXT_DIR)/*.o $(CLIENT_DIR)/*.o
	rm -f *.txt

run: $(CLIENT_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$(TEXT_DIR) ./$(CLIENT_TARGET)