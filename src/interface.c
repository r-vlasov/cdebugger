#include "include/interface.h"
#include "include/parse.h"
#include "include/breakpoint.h"
#include "include/registers.h"
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

static char* 	dbprogram_name;
static pid_t 	dbprogram_id;


static void wait_signal()
{
	int status;
	waitpid(dbprogram_id, &status, 0);
}


int continue_exec()
{
	if (ptrace(PTRACE_CONT, dbprogram_id, 0, 0))
	{
		fprintf(stderr, "ptrace continue error");
		return -1;
	}

	wait_signal();
	return 0;

}	


int singlestep_exec()
{
	if (ptrace(PTRACE_SINGLESTEP, dbprogram_id, 0, 0))
	{
		fprintf(stderr, "ptrace single step error");
		return -1;
	}

	wait_signal();

	
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, dbprogram_id, 0, &regs);
	
	
	printf( "stopped at: 0x%08x\n", regs.rip);
	

	return 0;
}


int dump_regs_exec()
{
	dump_registers(dbprogram_id);
	return 0;
}


int exec_command(char** cmd)
{
	if (!strcmp(cmd[0], "continue"))
		return continue_exec();
	
	else if (!strcmp(cmd[0], "bp"))
	{
		long address = atoi(cmd[1]);
		return breakpoint_enable(dbprogram_id, address);
	}

	else if (!strcmp(cmd[0], "dbp"))
	{
		long address = atoi(cmd[1]);
		return breakpoint_disable(dbprogram_id, address);
	}
	
	else if (!strcmp(cmd[0], "dump"))
	{
		if (!strcmp(cmd[1], "regs"))
			return dump_regs_exec();
		else
			return -1;
	}

	else if (!strcmp(cmd[0], "step"))
	{
		return singlestep_exec();
	}

	else if (!strcmp(cmd[0], "exit"))
	{
		exit(0);
	}
	else
	{
		printf("Unknown command\n");
		return 0;
	}
}


int run_debug(pid_t pid, char* program)
{
	int wait_status;
	waitpid(pid, &wait_status, 0);

	dbprogram_name = program;
	printf("Start debugging program : %s\n", program);
	dbprogram_id = pid;
	
	breakpoints_init();

	while(1)
	{
		if (exec_command(parse_tokenize_line(parse_read_line())))
			break;
	}

	return 0;
}




