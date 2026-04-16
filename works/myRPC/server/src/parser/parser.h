/*
  This file contains functions and definitions for the config parser
  Thank you, https://github.com/welljsjs/Config-Parser-C
*/
#define CONFIG_ARG_MAX_BYTES 128

typedef struct config_option config_option;
typedef config_option *config_option_t;

/*
  The defined config_option structure with a link to the previous element
  and a key = value pair.
*/
struct config_option
{
  config_option_t prev;
  char key[CONFIG_ARG_MAX_BYTES];
  char value[CONFIG_ARG_MAX_BYTES];
};

/*
  Parse the config file and return the last element of the key = value pair.
*/
config_option_t
read_config_file (char *path)
{
  FILE *fp;
  if ((fp = fopen (path, "r")) == NULL)
    {
      perror ("parser fopen()");
      return NULL;
    }
  config_option_t last_co_addr = NULL;
  while (1)
    {
      config_option_t co = NULL;
      if ((co = calloc (1, sizeof (config_option))) == NULL)
        continue;
      memset (co, 0, sizeof (config_option));
      co->prev = last_co_addr;
      if (fscanf (fp, "%s = %s", &co->key[0], &co->value[0]) != 2)
        {
          if (feof (fp))
            break;
          if (co->key[0] == '#')
            {
              while (fgetc (fp) != '\n')
                ;
              free (co);
              continue;
            }
          perror ("parser fscanf()");
          free (co);
          continue;
        }
      last_co_addr = co;
    }
  return last_co_addr;
}
