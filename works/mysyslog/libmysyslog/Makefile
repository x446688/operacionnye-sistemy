CC = gcc
CFLAGS = -fPIC -Wall -Wextra -g
LDFLAGS = -shared -ldl
SRC = libmysyslog.c
TARGET = libmysyslog.so
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

clean:
	rm -fr $(TARGET)
	rm -fr package/usr/lib
	rm $(TARGET).deb

deb: $(TARGET)
	mkdir -p package/usr/lib
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/lib
	dpkg-deb --build ./package
	mv package.deb $(TARGET).deb