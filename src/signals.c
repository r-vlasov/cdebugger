#include <signal.h>
#include "include/signals.h"
#include "include/registers.h"
#include "stdio.h"
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdlib.h>


/* union sigval {
 * 	int sival_int;
 * 	void* sival_ptr;
 * };
 *
 * siginfo_t {
 * 	int sis_signo;
 * 	int si_code;
 * 	union sigval si_value;
 * 	int si_errno;
 * 	int si_pid;
 * 	int si_uid;
 * 	void *si_addr;
 * 	int si_status;
 * 	int si_band;
 * }
 */

static void handle_sigtrap(pid_t pid)
{
	fprintf(stderr, "\n\t> SIGTRAP at 0x%08llx\n", get_ip(pid));
}

static void handle_sigsegv(pid_t pid)
{
	fprintf(stderr, "\n\t> SIGSEGV at 0x%08llx\n", get_ip(pid));
}

void handle_signal(pid_t pid)
{
	int status;

	wait(&status);

	if (WIFEXITED(status) || WIFSIGNALED(status))
	{
		fprintf(stderr, "\n\t\texit\n");
		return;
	}
	else
	{
		int signum = WSTOPSIG(status);
		switch(signum)
		{
			case SIGTRAP:
				handle_sigtrap(pid);
				return;

			case SIGSEGV:
				handle_sigsegv(pid);
				return;
			default:
						
				fprintf(stderr, "\nchild stopped with signal %d\n", signum);
				return;
		}
	}
}
