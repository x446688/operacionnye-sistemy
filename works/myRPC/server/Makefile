.PHONY: all clean deb
TARGET = myRPC-server
all: src/myRPC-server.c src/myRPC.h src/parser/parser.h src/mysyslog/libmysyslog.h
	test -f /usr/lib/x86_64-linux-gnu/libmysyslog.so && echo "mysyslog already installed" || ../bootstrap.sh
	test -d /etc/myRPC/ && echo "conf dir exists." || mkdir /etc/myRPC/
	: > /var/log/myRPC.log
	: > /etc/myRPC/myRPC.conf
	echo root > /etc/myRPC/users.conf
	chown -R `whoami` /etc/myRPC
	echo "port = 7777" > /etc/myRPC/myRPC.conf
	echo "socket_type = stream" >> /etc/myRPC/myRPC.conf
	gcc src/myRPC-server.c -o $(TARGET) -lmysyslog -ljson-c

clean:
	rm -rf $(TARGET)
	rm -rf package/usr

deb: $(TARGET)
	mkdir -p package/usr/bin
	mkdir -p package/usr/lib/systemd/system/
	mkdir -p package/etc/myRPC
	echo "port = 7777" > package/etc/myRPC/myRPC.conf
	echo "socket_type = stream" >> package/etc/myRPC/myRPC.conf
	echo "root" > package/etc/myRPC/users.conf
	cp myRPC-server.service package/user/lib/systemd/system/
	chmod 775 package/DEBIAN/postinst
	cp $< package/usr/bin
	dpkg-deb --build ./package $(TARGET).deb