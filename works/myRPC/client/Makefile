.PHONY: all clean deb
TARGET = myRPC-client

$(TARGET): src/myRPC-client.c src/mysyslog/libmysyslog.h
	chown `whoami` /var/log/myRPC.log
	test -f /usr/lib/x86_64-linux-gnu/libmysyslog.so && echo "mysyslog already installed" || ../bootstrap.sh
	gcc src/myRPC-client.c -o $(TARGET) -lmysyslog -ljson-c

all: $(TARGET)
	
clean:
	rm -rf $(TARGET)
	rm -rf package/usr
	rm -rf $(TARGET).deb

deb: $(TARGET)
	mkdir -p package/usr/bin
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/bin
	dpkg-deb --build ./package $(TARGET).deb