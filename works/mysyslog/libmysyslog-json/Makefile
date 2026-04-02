.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog-json.c
TARGET = libmysyslog-json.so

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)
clean:
	rm -fr $(TARGET)
	