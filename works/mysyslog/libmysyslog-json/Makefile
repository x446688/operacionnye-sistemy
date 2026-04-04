.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g -ljson-c
LDFLAGS = -shared -ldl
SRC = libmysyslog-json.c
TARGET = libmysyslog-json.so
JSON_C_CFLAGS = $(shell pkg-config --cflags json-c)
JSON_C_LIBS = $(shell pkg-config --libs json-c)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(JSON_C_CFLAGS) $(LDFLAGS) -o $@ $(SRC) $(JSON_C_LIBS)
clean:
	rm -fr $(TARGET)
	