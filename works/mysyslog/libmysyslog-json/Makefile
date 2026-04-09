.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g -ljson-c
LDFLAGS = -shared -ldl
SRC = libmysyslog-json.c
TARGET = libmysyslog-json.so
JSON_C_CFLAGS = $(shell pkg-config --cflags json-c)
JSON_C_LIBS = $(shell pkg-config --libs json-c)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(JSON_C_CFLAGS) -I/usr/include/mysyslog $(LDFLAGS) -o $@ $(SRC) $(JSON_C_LIBS)

clean:
	rm -fr $(TARGET)
	rm -fr package/usr/lib
	rm $(TARGET).deb

deb: $(TARGET)
	mkdir -p package/usr/lib/x86_64-linux-gnu
	cp $< package/usr/lib/x86_64-linux-gnu
	dpkg-deb --build ./package
	mv package.deb libmysyslog-json.deb