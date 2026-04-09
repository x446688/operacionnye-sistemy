CC = gcc
SRC = src/mysyslog-client.c
LIB_DIR = ../libmysyslog
TEXT_DIR = ../libmysyslog-text
JSON_DIR = ../libmysyslog-json
TARGET = mysyslog-client

$(TARGET): $(SRC)
	$(CC) -Wall -Wextra -o $@ $(SRC) \
		-I$(LIB_DIR) -I$(TEXT_DIR) -I$(JSON_DIR)\
		-L$(abspath$(LIB_DIR)) -L$(TEXT_DIR) -L$(JSON_DIR)\
		-lmysyslog -lmysyslog-text -lmysyslog-json -ldl 

clean:
	rm -fr $(TARGET)
	rm -fr package/usr/bin
	rm $(TARGET).deb
	
deb: $(TARGET)
	mkdir -p package/usr/bin
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/bin
	dpkg-deb --build ./package
	mv package.deb libmysyslog-client.deb