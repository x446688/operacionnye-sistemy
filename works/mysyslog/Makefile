.PHONY: all clean run
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g -I./libmysyslog -I./libmysyslog-text -I./libmysyslog-json
LDFLAGS = -shared -ldl

JSON_C_CFLAGS = $(shell pkg-config --cflags json-c)
JSON_C_LIBS = $(shell pkg-config --libs json-c)

$(info "Loaded $(JSON_C_CFLAGS) & $(JSON_C_LIBS)")

LIB_DIR = libmysyslog
TEXT_DIR = libmysyslog-text
JSON_DIR = libmysyslog-json
CLIENT_DIR = libmysyslog-client

LIB_TARGET = $(LIB_DIR)/libmysyslog.so
TEXT_TARGET = $(TEXT_DIR)/libmysyslog-text.so
JSON_TARGET = $(JSON_DIR)/libmysyslog-json.so
CLIENT_TARGET = $(CLIENT_DIR)/syslog-client

LIB_SRC = $(LIB_DIR)/libmysyslog.c
TEXT_SRC = $(TEXT_DIR)/libmysyslog-text.c
JSON_SRC = $(JSON_DIR)/libmysyslog-json.c
CLIENT_SRC = $(CLIENT_DIR)/syslog-client.c

LIB_HEADER = $(LIB_DIR)/libmysyslog.h
JSON_HEADER = $(JSON_DIR)/libmysyslog-json.h
TEXT_HEADER = $(TEXT_DIR)/libmysyslog-text.h

all: $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) $(CLIENT_TARGET)

$(LIB_TARGET): $(LIB_SRC) $(LIB_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LIB_SRC)

$(TEXT_TARGET): $(TEXT_SRC) $(TEXT_HEADER) $(LIB_HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(TEXT_SRC)

$(JSON_TARGET): $(JSON_SRC) $(JSON_HEADER) $(LIB_HEADER)
	$(CC) $(CFLAGS) $(JSON_C_CFLAGS) $(LDFLAGS) -o $@ $(JSON_SRC) $(JSON_C_LIBS)

$(CLIENT_TARGET): $(CLIENT_SRC) $(LIB_HEADER) $(TEXT_HEADER) $(JSON_HEADER)
	$(CC) -Wall -Wextra -g -o $@ $(CLIENT_SRC) \
		-I$(LIB_DIR) -I$(TEXT_DIR) -I$(JSON_DIR)\
		-L$(LIB_DIR) -L$(TEXT_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-text -lmysyslog-json -ldl \
		-Wl,-rpath,$(shell pwd)/$(LIB_DIR) -Wl,-rpath,$(shell pwd)/$(TEXT_DIR) -Wl,-rpath,$(shell pwd)/$(JSON_DIR)

clean:
	rm -f $(LIB_TARGET) $(TEXT_TARGET) $(JSON_TARGET) $(CLIENT_TARGET)
	rm -f $(LIB_DIR)/*.o $(TEXT_DIR)/*.o $(CLIENT_DIR)/*.o $(JSON_DIR)/*.o
	rm -f *.txt

run: $(CLIENT_TARGET)
	LD_LIBRARY_PATH=$(LIB_DIR):$(TEXT_DIR):$(JSON_DIR) ./$(CLIENT_TARGET)