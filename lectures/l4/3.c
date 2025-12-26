#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	int rez = 0;
	opterr = 0;
	while ((rez = getopt(argc, argv, "ab:C::d")) != -1) {
		switch (rez) {
		case 'a': printf("found arg \"a\".\n"); break;
		case 'b': printf("found arg \"b = %s\".\n", optarg); break;
		case 'C': printf("found arg \"C = %s\".\n", optarg); break;
		case 'd': printf("found arg \"d\".\n"); break;
		case '?': printf("ERROR!!!"); break;
		}
	}
}
