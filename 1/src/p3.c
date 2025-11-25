#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
extern int errno;
int main() {
	int sz;
	char *c = (char *) calloc(100, sizeof(char));
	int fd = open("foo.txt", O_RDONLY | O_CREAT);
	printf("fd = %d\n", fd);
	if (fd == -1) {
		printf("Error Number %d \n", errno);
		perror("Error while opening file:");
	}
	sz = read(fd, c, 10);
	printf("called read(%d,c,10). %d bytes were read.\n", fd, sz);
	c[sz] = '\0';
	if (close(fd) < 0) {
		perror("Error while closing file:");
		exit(1);
	}
	printf("Closed the fd.\n");
	return 0;
}
