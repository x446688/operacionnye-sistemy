#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dlfcn.h>
#include "libmysyslog.h"

int
main (int argc, char **argv)
{
	const char* msg;
	int level = 0;
	int driver = 0;
	int format = 0;
	const char* path;
	int c;

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"msg",     required_argument, 0,  'm' },
            {"level",  required_argument,       0,  'l' },
            {"driver",  required_argument, 0,   'd' },
            {"format", required_argument,       0,  'f' },
            {"path",  required_argument, 0, 'p' },
            {0,         0,                 0,  0 }
        };

        c = getopt_long(argc, argv, "m:l:d:f:p:",
                 long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

		case 'm':
			msg = optarg;
			break;

		case 'l':
			level = atoi(optarg);
			break;

		case 'd':
			driver = atoi(optarg);
			break;

		case 'f':
			format = atoi(optarg);
			break;
		case 'p':
			path = optarg;
			break;
		case '?':
				break;
		default:
				printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
	mysyslog(msg, level, driver, format, path);
	return 0;
}