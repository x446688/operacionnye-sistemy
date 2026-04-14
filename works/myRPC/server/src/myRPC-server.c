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
#include "parser/parser.h"
#include "myRPC.h"
#include <mysyslog/libmysyslog.h>

volatile sig_atomic_t stop;
config_t server_config;
char* socket_type = NULL;
const char* app_name = NULL;
int port = -1;

int get_parsed_config_options(const char* path) {
  config_option_t co;
  if ((co = read_config_file(path)) == NULL) {
      perror("read_config_file()");
      return -1;
  }
  while(1) {
      if (strcmp(co->key, "port") == 0) {
        port = atoi(co->value);
        mysyslog(co->value,LOG_LVL_INFO,1,1,"/var/log/myRPC.log");
      }
      if (strcmp(co->key, "socket_type") == 0){
        socket_type = co->value;
        mysyslog(socket_type,LOG_LVL_INFO,1,1,"/var/log/myRPC.log");
      }
      if (port >= 0 && port <= 65536 && socket_type != NULL) {
        return 0;
      }
      if (co->prev != NULL) {
          co = co->prev;
      } else {
          break;
      }
  }
  return -1;
}

void
handle_signal (int sig)
{
  stop = 1;
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
      line[strcspn(line, "\n")] = '\0';
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
execute_command (int argc, const char **argv, char *stdout_file, char *stderr_file)
{
  for (int i = 0; i < argc; ++i) {
    mysyslog(argv[i], LOG_LVL_WARN, 1, 1, "/var/log/myRPC.log");
  }
  int status;
  char cmd[BSIZE];
  int fd_stdout = open(stdout_file, O_WRONLY);
  if (fd_stdout == -1) {
    mysyslog("Failed opening stdout file",LOG_LVL_ERROR,1,1,"/var/log/myRPC.log");
  }
  int fd_stderr = open(stderr_file, O_WRONLY);
  if (fd_stderr == -1) {
    mysyslog("Failed opening stderr file",LOG_LVL_ERROR,1,1,"/var/log/myRPC.log");
  }
  pid_t pid = fork();
  if (pid == -1) {
    mysyslog("Error executing command", LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
  }
  if (pid == 0) {
    mysyslog("Child running", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
    int d2_stdout = dup2(fd_stdout, 1);
    if (d2_stdout == -1) {
      mysyslog("Failed copying file descriptor of stdout file",LOG_LVL_ERROR,1,1,"/var/log/myRPC.log");
    }
    int d2_stderr = dup2(fd_stderr, 2);
    if (d2_stderr == -1) {
      mysyslog("Failed copying file descriptor of stderr file",LOG_LVL_ERROR,1,1,"/var/log/myRPC.log");
    }
    execvp(argv[0],argv);
    exit(1);
  } else {
    waitpid(pid, &status, 0);
    mysyslog("Child executed command.",LOG_LVL_INFO,1,1,"/var/log/myRPC.log");
  }
  close(fd_stdout);
  close(fd_stderr);
  free (argv);
  return status;
}

int
main (int agrc, char *argv[])
{
  if (get_parsed_config_options ("/etc/myRPC/myRPC.conf") != 0){
    mysyslog("Error getting parsed config options", LOG_LVL_ERROR, 1, 1,"/var/log/myRPC.log");
    return -1;
  }

  mysyslog(socket_type, LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
  int use_stream = strcmp (socket_type, "stream") == 0;

  mysyslog ("Server starting...", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");

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
  servaddr.sin_port = htons(port);

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

  while (!stop)
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

          mysyslog ("Received request", LOG_LVL_INFO, 1, 1,
                    "/var/log/myRPC.log");

          int cnt_spaces = 0;
          int status;
          char *username = strtok (buffer, ":");
          char *command = strtok (NULL, ":"); 
          char *sliding_token = strtok (command, " ");
          char **cmd_args = NULL;

          while (sliding_token != NULL) {
            ++cnt_spaces;
            cmd_args = realloc(cmd_args, sizeof(char*) * (cnt_spaces)); // what if args == NULL?
            if (cmd_args == NULL){
              mysyslog("Memory allocation failed while getting bash command", LOG_LVL_CRITICAL, 1, 1, "/var/log/myRPC.log");
            }
            cmd_args[cnt_spaces-1] = sliding_token;
            sliding_token = strtok(NULL, " ");
          }
          cmd_args = realloc(cmd_args, sizeof(char*) * cnt_spaces+1);
          cmd_args[cnt_spaces] = NULL;
          char response[BSIZE];

          if (user_allowed (username) == 0)
            {
              mysyslog ("User allowed", LOG_LVL_INFO, 1, 1,
                        "/var/log/myRPC.log");
              char stdout_file[] = "/tmp/myRPC_XXXXXX.stdout";
              char stderr_file[] = "/tmp/myRPC_XXXXXX.stderr";
              int err_m_stdout = mkstemps (stdout_file,7);
              int err_m_stderr = mkstemps (stderr_file,7); // suffix
              if (err_m_stdout == -1 ) {
                printf("%s\n",strerror(errno));
                mysyslog ("Error creating /tmp/myRPC_XXXXXX.stdout", LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
              }
              if (err_m_stderr == -1) {
                printf("%s\n",strerror(errno));
                mysyslog ("Error creating /tmp/myRPC_XXXXXX.stderr", LOG_LVL_ERROR, 1, 1, "/var/log/myRPC.log");
              }
              int cmd_return_code = execute_command (cnt_spaces, cmd_args, stdout_file, stderr_file); // if cmd_return_code != 0 return stderr else stdout
              if (cmd_return_code == 0) {
                mysyslog("Command returned sucess", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
              } else {
                mysyslog("Command returned an error...", LOG_LVL_INFO, 1, 1, "/var/log/myRPC.log");
              }
              strcpy (response, cmd_return_code == 0 ? "0" : "1");
              unlink (stdout_file);
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
          return -1; // REMOVE_ME
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
  return 0;
}
