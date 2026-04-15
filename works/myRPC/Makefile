.PHONY: all deb

all: 
	make -C server
	make -C client

deb: all
	make -C server deb
	make -C client deb