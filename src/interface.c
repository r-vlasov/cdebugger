#include "include/interface.h"
#include "include/breakpoint.h"
#include "include/parse.h"
#include "include/registers.h"
#include "include/signals.h"
//#include "include/dwarf.hpp"

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

static char* 	dbprogram_name;
static pid_t 	dbprogram_id;


static void free_cmd(char** cmd)
{
	for(int i = 0; cmd[i] != NULL; i++)
	{
		free(cmd[i]);
	}
	free(cmd);
}

static void wait_signal()
{
	int status;
	waitpid(dbprogram_id, &status, 0);
}


static int continue_exec()
{
	step_to_breakpoint(dbprogram_id);
	
	wait_signal();
	return 0;

}	


static int singlestep_exec()
{
	if (ptrace(PTRACE_SINGLESTEP, dbprogram_id, 0, 0))
	{
		fprintf(stderr, "ptrace single step error");
		return -1;
	}

	wait_signal();

	
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, dbprogram_id, 0, &regs);
	
	
	printf( "stopped at: 0x%08llx\n", regs.rip);
	

	return 0;
}


static int dump_regs_exec()
{
	dump_registers(dbprogram_id);
	return 0;
}


static int exec_command(char** cmd)
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

	free_cmd(cmd);
}


int run_debug(pid_t pid, char* program)
{
	handle_signal(pid);

	dbprogram_name = program;
	printf("Start debugging program : %s\n", program);
	dbprogram_id = pid;
	
	breakpoints_init();
//	db_inform_init(program);


	while(1)
	{
		if (exec_command(parse_tokenize_line(parse_read_line())))
			break;
	}

	return 0;
}




