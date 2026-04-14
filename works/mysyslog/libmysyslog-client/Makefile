CC = gcc
SRC = src/mysyslog-client.c
TARGET = mysyslog-client

$(TARGET): $(SRC)
	$(CC) -Wall -Wextra -o $@ $(SRC) \
		-lmysyslog -lmysyslog-text -lmysyslog-json -Wl,--no-as-needed -ldl

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