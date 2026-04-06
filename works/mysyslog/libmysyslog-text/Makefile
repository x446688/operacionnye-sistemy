.PHONY: all
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog-text.c
TARGET = libmysyslog-text.so

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

clean:
	rm -fr $(TARGET)
	
deb: $(TARGET)
	mkdir -p package/usr/lib
	cp $< package/usr/lib
	dpkg-deb --build ./package
	mv package.deb libmysyslog-text.deb