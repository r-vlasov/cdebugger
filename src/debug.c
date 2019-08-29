#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


int main(int argc, char* argv[])
{
	char* program = argv[1];
	pid_t debug_pid = fork();
	if (debug_pid == 0)
	{
		if (ptrace(PTRACE_TRACEME, NULL, NULL, NULL) < 0)
		{
			fprintf(stderr, "ptrace error\n");
		}
		execl(program, program, NULL);
	}
	else
	{	
		if (!run_debug(debug_pid, program))
			return 0;
		
		fprintf(stderr, "Stop debug with fail!\n");
		return -1;
	}
}
