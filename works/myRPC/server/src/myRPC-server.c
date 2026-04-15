#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <getopt.h>
#include "parser/parser.h"
#include "myRPC.h"
#include <mysyslog/libmysyslog.h>
#include <json-c/json.h>

config_t server_config;
char *socket_type = NULL;
const char *app_name = NULL;
const char *server_log_file = "/var/log/myRPC.log";
const char *server_conf_file = "/etc/myRPC/myRPC.conf";
static char *pid_file_name = NULL;
static int pid_fd = -1;
static int port = -1;
static int status = NULL;
static int stop = 0;
static int xtreme_mode = 0;

void
print_help ()
{
  printf ("Usage: myRPC-server [OPTIONS]\n");
  printf ("Options:\n");
  printf ("  -c, --conf-file BASH_COMMAND  Load specified conf file\n");
  printf
    ("  -l, --log-file  FILE          Output logs to specified log file\n");
  printf
    ("  -d, --daemon                  Run myRPC-server in daemonized mode\n");
  printf ("  -p, --pid-file                Specify the pid file\n");
  printf ("  -h  --help                    Display this help and exit\n");
  printf ("  -x, --xtreme                  Force return stdout to result\n");
}

static void
daemonize ()
{
  pid_t pid = 0;
  int fd;

  /* Fork off the parent process */
  pid = fork ();

  /* An error occurred */
  if (pid < 0)
    {
      exit (EXIT_FAILURE);
    }

  /* Success: Let the parent terminate */
  if (pid > 0)
    {
      exit (EXIT_SUCCESS);
    }

  /* On success: The child process becomes session leader */
  if (setsid () < 0)
    {
      exit (EXIT_FAILURE);
    }

  /* Ignore signal sent from child to parent process */
  signal (SIGCHLD, SIG_IGN);

  /* Fork off for the second time */
  pid = fork ();

  /* An error occurred */
  if (pid < 0)
    {
      exit (EXIT_FAILURE);
    }

  /* Success: Let the parent terminate */
  if (pid > 0)
    {
      exit (EXIT_SUCCESS);
    }

  /* Set new file permissions */
  umask (0);

  /* Change the working directory to the root directory */
  /* or another appropriated directory */
  chdir ("/");

  /* Close all open file descriptors */
  for (fd = sysconf (_SC_OPEN_MAX); fd > 0; fd--)
    {
      close (fd);
    }

  /* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
  stdin = fopen ("/dev/null", "r");
  stdout = fopen ("/dev/null", "w+");
  stderr = fopen ("/dev/null", "w+");

  /* Try to write PID of daemon to lockfile */
  if (pid_file_name != NULL)
    {
      char str[256];
      pid_fd = open (pid_file_name, O_RDWR | O_CREAT, 0640);
      if (pid_fd < 0)
        {
          /* Can't open lockfile */
          exit (EXIT_FAILURE);
        }
      if (lockf (pid_fd, F_TLOCK, 0) < 0)
        {
          /* Can't lock file */
          exit (EXIT_FAILURE);
        }
      /* Get current PID */
      sprintf (str, "%d\n", getpid ());
      /* Write PID to lockfile */
      write (pid_fd, str, strlen (str));
      close (pid_fd);
    }
}

int
get_parsed_config_options (const char *path)
{
  config_option_t co;
  if ((co = read_config_file (path)) == NULL)
    {
      perror ("read_config_file()");
      return -1;
    }
  while (1)
    {
      if (strcmp (co->key, "port") == 0)
        {
          port = atoi (co->value);
          mysyslog (co->value, LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
        }
      if (strcmp (co->key, "socket_type") == 0)
        {
          socket_type = co->value;
          mysyslog (socket_type, LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
        }
      if (port >= 0 && port <= 65535 && socket_type != NULL)
        {
          return 0;
        }
      if (co->prev != NULL)
        {
          co = co->prev;
        }
      else
        {
          break;
        }
    }
  return -1;
}

void
handle_signal (int sig)
{
  if (sig == SIGINT)
    {
      /* Unlock and close lockfile */
      if (pid_fd != -1)
        {
          lockf (pid_fd, F_ULOCK, 0);
          close (pid_fd);
        }
      /* Try to delete lockfile */
      if (pid_file_name != NULL)
        {
          unlink (pid_file_name);
        }
      stop = 1;
      mysyslog ("Debug: stopping daemon...", LOG_LVL_DEBUG, 1, 1,
                server_log_file);
      /* Reset signal handling to default behavior */
      signal (SIGINT, SIG_DFL);
    }
  else if (sig == SIGHUP)
    {
      mysyslog ("Debug: reloading daemon config file ...", LOG_LVL_DEBUG, 1,
                1, server_log_file);
      get_parsed_config_options (server_conf_file);
    }
  else if (sig == SIGCHLD)
    {
      mysyslog ("Debug: received SIGCHLD signal", LOG_LVL_DEBUG, 1, 1,
                server_log_file);
    }
}

int
user_allowed (const char *username)
{
  FILE *file = fopen ("/etc/myRPC/users.conf", "r");
  if (!file)
    {
      mysyslog ("Failed to open users.conf", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
      perror ("Failed to open users.conf");
      return 0;
    }

  char line[256];
  int allowed = 1;
  while (fgets (line, sizeof (line), file))
    {
      line[strcspn (line, "\n")] = '\0';
      if (line[0] == '#' && strlen (line) != 0)
        {
          continue;
        }
      if (strcmp (line, username) == 0)
        {
          allowed = 0;
          break;
        }
    }
  fclose (file);
  return allowed;
}

int
execute_command (int argc, const char **argv, char *stdout_file,
                 char *stderr_file)
{
  int status;
  char cmd[BSIZE];
  int fd_stdout = open (stdout_file, O_WRONLY);
  if (fd_stdout == -1)
    {
      mysyslog ("Failed opening stdout file", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
    }
  int fd_stderr = open (stderr_file, O_WRONLY);
  if (fd_stderr == -1)
    {
      mysyslog ("Failed opening stderr file", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
    }
  pid_t pid = fork ();
  if (pid == -1)
    {
      mysyslog ("Error executing command", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
    }
  if (pid == 0)
    {
      mysyslog ("Child running", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
      int d2_stdout = dup2 (fd_stdout, 1);
      if (d2_stdout == -1)
        {
          mysyslog ("Failed copying file descriptor of stdout file",
                    LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
        }
      int d2_stderr = dup2 (fd_stderr, 2);
      if (d2_stderr == -1)
        {
          mysyslog ("Failed copying file descriptor of stderr file",
                    LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
        }
      execvp (argv[0], argv);
      if (errno = 2)
        printf ("Command '%s' is not a valid command.", argv[0]);
      exit (EXIT_FAILURE);
    }
  else
    {
      waitpid (pid, &status, 0);
      mysyslog (strerror (errno), LOG_LVL_CRITICAL, 1, 1, server_log_file);
      mysyslog ("Child executed command.", LOG_LVL_INFO, 1, 1,
                "/var/log/myRPC.log");
    }
  close (fd_stdout);
  close (fd_stderr);
  free (argv);
  return status;
}

int
main (int argc, char *argv[])
{
  static struct option long_options[] = {
    {"conf-file", required_argument, 0, 'c'},
    {"log-file", required_argument, 0, 'l'},
    {"daemon", no_argument, 0, 'd'},
    {"pid-file", required_argument, 0, 'p'},
    {"help", no_argument, 0, 0},
    {"x", no_argument, 0, 'x'},
    {0, 0, 0, 0}
  };
  int value, option_index = 0, ret;
  int start_daemonized = 0;
  /* Try to process all command line arguments */
  while ((value =
          getopt_long (argc, argv, "c:l:p:dhx", long_options,
                       &option_index)) != -1)
    {
      switch (value)
        {
        case 'c':
          server_conf_file = strdup (optarg);
          break;
        case 'l':
          server_log_file = strdup (optarg);
          break;
        case 'p':
          pid_file_name = strdup (optarg);
          break;
        case 'd':
          start_daemonized = 1;
          break;
        case 'x':
          mysyslog ("EXTREME_MODE=True", LOG_LVL_DEBUG, 1, 1,
                    server_log_file);
          xtreme_mode = 1;
          break;
        case 'h':
          print_help ();
        default:
          print_help ();
          return 1;
        }
    }

  if (start_daemonized == 1)
    {
      /* It is also possible to use glibc function deamon()
       * at this point, but it is useful to customize your daemon. */
      daemonize ();
      signal (SIGCHLD, SIG_DFL);
    }
  mysyslog ("Started server!", LOG_LVL_INFO, 1, 1, server_log_file);
  signal (SIGINT, handle_signal);
  signal (SIGHUP, handle_signal);
  if (get_parsed_config_options (server_conf_file) != 0)
    {
      mysyslog ("Error getting parsed config options", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
      return -1;
    }
  mysyslog (socket_type, LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
  int use_stream = strcmp (socket_type, "stream") == 0;
  mysyslog ("Setting up sockets...", LOG_LVL_INFO, 1, 1,
            "/var/log/myRPC.log");

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
                "/var/log/myRPC.log");
      perror ("Socket creation failed");
      return 1;
    }

  int opt = 1;
  if (setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0)
    {
      mysyslog ("setssockopt failed", LOG_LVL_ERROR, 1, 1,
                "/var/log/myRPC.log");
      perror ("setssockopt failed");
      close (sockfd);
      return 1;
    }

  struct sockaddr_in servaddr, cliaddr;
  socklen_t len;
  memset (&servaddr, 0, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons (port);

  if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0)
    {
      mysyslog ("Bind failed", LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
      perror ("Bind failed");
      close (sockfd);
      return 1;
    }

  if (use_stream)
    {
      listen (sockfd, 5);
      mysyslog ("Server listening (stream)", LOG_LVL_INFO, 1, 1,
                "/var/log/myRPC.log");
    }
  else
    {
      mysyslog ("Server listening (datagram)", LOG_LVL_INFO, 1, 1,
                "/var/log/myRPC.log");
    }

  while (stop == 0)
    {
      char buffer[BSIZE];
      int n;

      if (use_stream)
        {
          len = sizeof (cliaddr);
          int connfd = accept (sockfd, (struct sockaddr *) &cliaddr, &len);
          if (connfd < 0)
            {
              mysyslog ("Accept failed", LOG_LVL_ERROR, 1, 1,
                        "/var/log/myRPC.log");
              perror ("Accept failed");
              continue;
            }

          n = recv (connfd, buffer, BSIZE, 0);
          if (n <= 0)
            {
              close (connfd);
              continue;
            }

          buffer[n] = '\0';
          struct json_tokener *parser = json_tokener_new ();
          struct json_object *request_root = json_tokener_parse (buffer);
          struct json_object *received_login = json_object_new_object ();
          struct json_object *received_command = json_object_new_object ();
          struct json_object *response_root = json_object_new_object ();
          mysyslog (json_object_get_string (request_root), LOG_LVL_INFO, 1, 1,
                    "/var/log/myRPC.log");
          json_tokener_free (parser);
          mysyslog ("Received request", LOG_LVL_INFO, 1, 1,
                    "/var/log/myRPC.log");
          json_object_object_get_ex (request_root, "login", &received_login);
          json_object_object_get_ex (request_root, "command",
                                     &received_command);
          int cnt_spaces = 0;
          int status;
          char *username = json_object_get_string (received_login);
          char *command = json_object_get_string (received_command);
          char *sliding_token = strtok (command, " ");
          char **cmd_args = NULL;

          while (sliding_token != NULL)
            {
              ++cnt_spaces;
              cmd_args = realloc (cmd_args, sizeof (char *) * (cnt_spaces));
              if (cmd_args == NULL)
                {
                  mysyslog
                    ("Memory allocation failed while getting bash command",
                     LOG_LVL_CRITICAL, 1, 1, "/var/log/myRPC.log");
                }
              cmd_args[cnt_spaces - 1] = sliding_token;
              sliding_token = strtok (NULL, " ");
            }
          cmd_args = realloc (cmd_args, sizeof (char *) * cnt_spaces + 1);
          cmd_args[cnt_spaces] = NULL;
          char response[BSIZE];
          char result[BSIZE];

          if (user_allowed (username) == 0)
            {
              mysyslog ("User allowed", LOG_LVL_INFO, 1, 1,
                        "/var/log/myRPC.log");
              char stdout_file[] = "/tmp/myRPC_XXXXXX.stdout";
              char stderr_file[] = "/tmp/myRPC_XXXXXX.stderr";
              int err_m_stdout = mkstemps (stdout_file, 7);
              int err_m_stderr = mkstemps (stderr_file, 7); // suffix
              if (err_m_stdout == -1)
                {
                  printf ("%s\n", strerror (errno));
                  mysyslog ("Error creating /tmp/myRPC_XXXXXX.stdout",
                            LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
                }
              if (err_m_stderr == -1)
                {
                  printf ("%s\n", strerror (errno));
                  mysyslog ("Error creating /tmp/myRPC_XXXXXX.stderr",
                            LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
                }
              // if cmd_return_code != 0 return stderr else stdout
              int cmd_return_code =
                execute_command (cnt_spaces, cmd_args, stdout_file,
                                 stderr_file);
              mysyslog (strerror (errno), LOG_LVL_WARN, 1, 1,
                        server_log_file);
              json_object_object_add (response_root, "code",
                                      json_object_new_int (cmd_return_code ==
                                                           0 ? cmd_return_code
                                                           : 1));
              if (cmd_return_code == 0)
                {
                  mysyslog ("Command returned sucess", LOG_LVL_INFO, 1, 1,
                            "/var/log/myRPC.log");
                  FILE *f = fopen (stdout_file, "r");
                  if (f)
                    {
                      size_t read_bytes = fread (result, 1, BSIZE, f);
                      result[read_bytes] = '\0';
                      fclose (f);
                    }
                  else
                    {
                      strcpy (result, "Error reading stdout file.");
                      mysyslog ("Error reading stdout file", LOG_LVL_ERROR, 1,
                                1, "/var/log/myRPC.log");
                    }
                }
              else
                {
                  mysyslog ("Command returned an error...", LOG_LVL_INFO, 1,
                            1, "/var/log/myRPC.log");
                  FILE *f;
                  if (xtreme_mode == 1)
                    {
                      f = fopen (stdout_file, "r");
                    }
                  else
                    {
                      f = fopen (stderr_file, "r");
                    }
                  if (f)
                    {
                      size_t read_bytes = fread (result, 1, BSIZE, f);
                      result[read_bytes] = '\0';
                      fclose (f);
                    }
                  else
                    {
                      strcpy (result, "Error reading stderr file.");
                      mysyslog ("Error reading stderr file", LOG_LVL_ERROR, 1,
                                1, "/var/log/myRPC.log");
                    }
                }
              json_object_object_add (response_root, "result",
                                      json_object_new_string (result));
              strcpy (response, json_object_to_json_string (response_root));
              //unlink (stdout_file);
              unlink (stderr_file);
            }
          else
            {
              snprintf (response, BSIZE, "1: User '%s' is not allowed.",
                        username);
              mysyslog ("User not allowed", LOG_LVL_WARN, 1, 1,
                        "/var/log/myRPC.log");
            }

          send (connfd, response, strlen (response), 0);
          close (connfd);

        }
      else
        {
          return -1;            // REMOVE_ME
          len = sizeof (cliaddr);
          n =
            recvfrom (sockfd, buffer, BSIZE, 0, (struct sockaddr *) &cliaddr,
                      &len);
          if (n <= 0)
            {
              continue;
            }

          buffer[n] = '\0';

          mysyslog ("Client login request", LOG_LVL_INFO, 1, 1,
                    "/var/log/myRPC.log");

          char *username = strtok (buffer, ":");
          char *command = strtok (NULL, "");
          if (command)
            {
              while (*command == ' ')
                command++;
            }
          char response[BSIZE];

          if (user_allowed (username) == 0)
            {
              mysyslog ("User allowed", LOG_LVL_INFO, 1, 1,
                        "/var/log/myRPC.log");
              char stdout_file[] = "/tmp/myRPC_XXXXXX.stdout";
              char stderr_file[] = "/tmp/myRPC_XXXXXX.stderr";
              mkstemp (stdout_file);
              mkstemp (stderr_file);
              // execute_command (command, stdout_file, stderr_file);
              FILE *f = fopen (stdout_file, "r");
              if (f)
                {
                  size_t read_bytes = fread (response, 1, BSIZE, f);
                  response[read_bytes] = '\0';
                  fclose (f);
                  mysyslog ("Command executed successfully", LOG_LVL_INFO, 1,
                            1, "/var/log/myRPC.log");
                }
              else
                {
                  strcpy (response, "Error reading stdout file");
                  mysyslog ("Error reading stdout file", LOG_LVL_ERROR, 1, 1,
                            "/var/log/myRPC.log");
                }
              remove (stdout_file);
              remove (stderr_file);
            }
          else
            {
              snprintf (response, BSIZE, "1: User '%s' is not allowed",
                        username);
              mysyslog ("User not allowed", LOG_LVL_WARN, 1, 1,
                        "/var/log/myRPC.log");
            }
          sendto (sockfd, response, strlen (response), 0,
                  (struct sockaddr *) &cliaddr, len);
        }
    }
  close (sockfd);
  mysyslog ("Server stopped", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
  if (server_conf_file != NULL)
    free (server_conf_file);
  if (server_log_file != NULL)
    free (server_log_file);
  if (pid_file_name != NULL)
    free (pid_file_name);
  if (app_name != NULL)
    free (app_name);
  if (socket_type != NULL)
    free (socket_type);
  return EXIT_SUCCESS;
}
