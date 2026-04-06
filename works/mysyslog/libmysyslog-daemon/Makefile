CC = gcc
SRC = src/syslog-daemon.c
LIB_DIR = ../libmysyslog
PARSE_DIR = libparse
TEXT_DIR = ../libmysyslog-text
JSON_DIR = ../libmysyslog-json
TARGET = mysyslog-daemon

$(TARGET): $(SRC)
	$(CC) -Wall -Wextra -g -o $@ $(SRC) \
		-I$(LIB_DIR) -I$(TEXT_DIR) -I$(JSON_DIR) -I$(PARSE_DIR)\
		-L$(LIB_DIR) -L$(TEXT_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-text -lmysyslog-json -ldl 

clean:
	rm -fr $(TARGET)

deb: $(TARGET)
	mkdir -p package/usr/bin
	mkdir -p package/usr/lib
	mkdir -p package/etc/mysyslog
	mkdir -p package/usr/lib/systemd/system
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/bin
	dpkg-deb --build ./package
	mv package.deb libmysyslog-daemon.deb