#include <stdio.h>
#include <math.h>
#include <errno.h> /* errno definitions */
int main(void) {
	/* WILL create an error */
	asin(10.0);
	if (errno == EDOM) {
		perror ("Program Error Test");
	return 0;
	}
}
