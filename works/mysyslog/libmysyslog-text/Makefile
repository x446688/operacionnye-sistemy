.PHONY: all clean deb
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog-text.c
TARGET = libmysyslog-text.so

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -I/usr/include/mysyslog -o $@ $(SRC)

clean:
	rm -fr $(TARGET)
	rm -fr package/usr/bin
	rm $(TARGET).deb
	
deb: $(TARGET)
	mkdir -p package/usr/lib/x86_64-linux-gnu
	cp $< package/usr/lib/x86_64-linux-gnu
	dpkg-deb --build ./package
	mv package.deb libmysyslog-text.deb