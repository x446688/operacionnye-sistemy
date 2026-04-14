typedef struct config_t config_t;
#define CONFIG_SOCKTYPE_MAX_BYTES 256
struct config_t
{
  int port;
  char socket_type[CONFIG_SOCKTYPE_MAX_BYTES];
};
