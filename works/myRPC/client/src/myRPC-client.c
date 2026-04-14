#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pwd.h>
#include <mysyslog/libmysyslog.h>

void
print_help ()
{
  printf ("Usage: myrpc-client [OPTIONS]\n");
  printf ("Options:\n");
  printf ("  -c, --command BASH_COMMAND  Command to execute\n");
  printf ("  -h, --host    IP_ADDRESS    Server IP\n");
  printf ("  -p, --port    PORT          Server port\n");
  printf ("  -s  --stream                Use stream socket\n");
  printf ("  -d, --dgram                 Use datagram socket\n");
  printf ("      --help                  Display this help and exit\n");
}

int
main (int argc, char *argv[])
{
  char *command = NULL;
  char *server_ip = NULL;
  int port = 0;
  int use_stream = 1;
  int opt;

  static struct option long_options[] = {
    {"command", required_argument, 0, 'c'},
    {"host", required_argument, 0, 'h'},
    {"port", required_argument, 0, 'p'},
    {"stream", no_argument, 0, 's'},
    {"dgram", no_argument, 0, 'd'},
    {"help", no_argument, 0, '?'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  while ((opt =
          getopt_long (argc, argv, "c:h:p:sd", long_options,
                       &option_index)) != -1)
    {
      switch (opt)
        {
        case 'c':
          command = strdup (optarg);
          break;
        case 'h':
          server_ip = strdup (optarg);
          break;
        case 'p':
          port = atoi (optarg);
          break;
        case 's':
          use_stream = 1;
          break;
        case 'd':
          use_stream = 0;
          break;
        case 0:
          print_help ();
          return 0;
        default:
          break;
        }
    }
  if (!command || !server_ip || !port)
    {
      fprintf (stderr, "Error: Missing required arguments\n");
      print_help ();
      return 1;
    }
  struct passwd *pw = getpwuid (getuid ());
  char *username = pw->pw_name;
  char request[BSIZE];
  printf ("Connecting to %s:%d", server_ip, port);
  snprintf (request, BSIZE, "%s:%s", username, command);
  mysyslog ("Connecting to the server...", LOG_LVL_INFO, 1, 1,
            "/var/log/myrpc.log");
  int sockfd;
  if (use_stream)
    {
      sockfd = socket (AF_INET, SOCK_STREAM, 0);
    }
  else
    {
      sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    }
  if (sockfd < 0)
    {
      mysyslog ("Socket creation failed", LOG_LVL_ERROR, 1, 1,
                "/var/log/myrpc.log");
      perror ("Socket creation failed");
      return 1;
    }
  struct sockaddr_in servaddr;
  memset (&servaddr, 0, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = port;
  inet_pton (AF_INET, server_ip, &servaddr.sin_addr);
  if (use_stream)
    {
      if (connect (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) <
          0)
        {
          mysyslog ("Connection failed", LOG_LVL_ERROR, 1, 1,
                    "/var/log/myrpc.log");
          perror ("Connection failed");
          close (sockfd);
          return 1;
        }
      mysyslog ("Successfully connected to server", LOG_LVL_INFO, 1, 1,
                "/var/log/myrpc.log");
      send (sockfd, request, strlen (request), 0);
      char response[BSIZE];
      socklen_t len = sizeof (servaddr);
      int n =
        recvfrom (sockfd, response, BSIZE, 0, (struct sockaddr *) &servaddr,
                  &len);
      response[n] = '\0';
      printf ("Server response: %s\n", response);
      mysyslog ("Received server response", LOG_LVL_INFO, 1, 1,
                "/var/log/myrpc.log");
    }
  close (sockfd);
  return 0;
}
