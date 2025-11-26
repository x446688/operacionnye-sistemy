#include <unistd.h>
extern char **environ;
int main() {
	char* args[] = {"/bin/cat", "--help", NULL};
	execve("/bin/cat", args, environ);
	return 1;
}
