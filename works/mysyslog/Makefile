.PHONY: deb-all repo deb

deb-all:
	make deb
	make -C libmysyslog deb
	dpkg -i libmysyslog/libmysyslog.deb
	make -C libmysyslog-json deb
	dpkg -i libmysyslog-json/libmysyslog-json.deb
	make -C libmysyslog-text deb
	dpkg -i libmysyslog-text/libmysyslog-text.deb
	make -C libmysyslog-client deb
	dpkg -i libmysyslog-client/libmysyslog-client.deb
	make -C libmysyslog-daemon deb
	dpkg -i libmysyslog-daemon/libmysyslog-daemon.deb
	make -C libmysyslog-daemon deb
	dpkg -i libmysyslog-text/libmysyslog-text.deb

clean:
	make -C libmysyslog clean
	make -C libmysyslog-client clean
	make -C libmysyslog-daemon clean
	make -C libmysyslog-json clean
	make -C libmysyslog-text clean

repo: $(deb-all)
	test -d /srv/repo/conf || sudo mkdir -p /srv/repo/conf
	cp -r repo/conf/* /srv/repo/conf
	sudo reprepro -b /srv/repo export
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 libmysyslog/libmysyslog.deb
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 libmysyslog-client/libmysyslog-client.deb
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 libmysyslog-daemon/libmysyslog-daemon.deb
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 libmysyslog-json/libmysyslog-json.deb
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 libmysyslog-text/libmysyslog-text.deb
	sudo reprepro -P -V -b /srv/repo includedeb 1.7_x86-64 mysyslog-meta.deb
	sudo apt install apache2
	sudo ln -sf /srv/repo /var/www/html/repo
	sudo sed -i '/DocumentRoot \/var\/www\/html/a\\n\t<Directory \/var/\www/html\/repo>\n\t\tOptions Indexes MultiViews FollowSymLinks\n\t\tAllowOverride None\n\t\tOrder Deny,Allow\n\t\tRequire all granted\n\t<\/Directory>' /etc/apache2/sites-enabled/000-default.conf
	sudo systemctl restart apache2

mrproper: clean
	rm -rf /srv/repo/

deb:
	dpkg-deb --build ./package
	mv package.deb mysyslog-meta.deb