#include <stdio.h>
#include <stdlib.h>
int main(void) {
	FILE *fp = fopen("data.txt","r");
	if (fp == NULL) {
		fprintf(stderr, "fopen failed in file %s at line #%d",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
	printf("Normal ret\n");
	return EXIT_SUCCESS;
}
