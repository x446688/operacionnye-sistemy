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
install:
	cp $(TARGET) /usr/bin
	test -d /etc/mysyslog || mkdir /etc/mysyslog 
	test -d /var/log/mysyslog || mkdir /var/log/mysyslog
	touch /var/log/mysyslog/msl.log
	cp mysyslog.conf /etc/mysyslog
	chmod -R 755 /etc/mysyslog
	chmod -R 775 /var/log/mysyslog
	getent group mysyslog || addgroup --system mysyslog
	id mysyslog || adduser --system --ingroup mysyslog mysyslog
	chown -R root:mysyslog /var/log/mysyslog
	chmod 664 /var/log/mysyslog/msl.log
	cp mysyslog-daemon.service /usr/lib/systemd/system