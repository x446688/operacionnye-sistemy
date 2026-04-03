.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog-json.c
TARGET = libmysyslog-json.so

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(shell pkg-config --cflags json-c) $(LDFLAGS) -o $@ $(SRC)  $(shell pkg-config --libs json-c)
clean:
	rm -fr $(TARGET)
	