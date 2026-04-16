CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog.c
TARGET = libmysyslog.so
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -fr $(TARGET)
	rm -fr package/usr/libs
	rm $(TARGET).deb
	rm -rf package/usr/include

deb: $(TARGET)
	mkdir -p package/usr/lib/x86_64-linux-gnu
	mkdir -p package/usr/include/mysyslog
	cp libmysyslog.h package/usr/include/mysyslog
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/lib/x86_64-linux-gnu
	dpkg-deb --build ./package
	mv package.deb libmysyslog.deb