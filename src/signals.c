#include "include/signals.h"


void handle_signal(pid_t pid)
{
	int status;
	waitpid(pid, &status, 0);
}
