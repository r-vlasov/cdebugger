#include "include/interface.h"
#include "include/parse.h"
#include "include/breakpoint.h"
#include "include/signals.h"
/* c++ */
//#include "include/dwarf.hpp"
//#include <string>



#include <stdio.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

static char* 	dbprogram_name;
static pid_t 	dbprogram_id;


int db_inform_init(char*);
int set_breakpoint_at_function(int, char*);
void print_source_from_ip(char* file_n, long long ip);


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
	handle_signal(dbprogram_id);	
	return 0;

}	


static int singlestep_exec()
{
	if (ptrace(PTRACE_SINGLESTEP, dbprogram_id, 0, 0))
	{
		fprintf(stderr, "ptrace single step error\n");
		return -1;
	}

	handle_signal(dbprogram_id);

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

static int add_bp_at_address(long long address)
{
	fprintf(stderr, "\tSet breakpoint at 0x%08llx\n", address);
	return breakpoint_enable(dbprogram_id, address);
}

static int add_bp_at_function(char* name)
{
	fprintf(stderr, "\tSet breakpoint at %s\n", name);
	return set_breakpoint_at_function(dbprogram_id, name);
}

/* bug */
static int set_registers(char **cmd)
{
	if (!strcmp(cmd[0], "rip"))
	{
		set_ip(dbprogram_id, atohexi(cmd[1]));
		return 0;
	}
	return -1;
}

static int exec_command(char** cmd)
{
	if (!strcmp(cmd[0], "continue"))
		return continue_exec();
	
	else if (!strcmp(cmd[0], "breakpoint"))
	{
		if (cmd[1][0] >= '1' && cmd[1][0] <= '9')
		{
			long long address = atoi(cmd[1]);
			//return breakpoint_enable(dbprogram_id, address);
		
			return add_bp_at_address(address);
		}
		
		else if(cmd[1][0] == '0' && cmd[1][1] == 'x')
		{
			long long address = atohexi(&cmd[1][2]);
			return add_bp_at_address(address);
		}

		else if ((cmd[1][0] >= 'a' && cmd[1][0] <= 'z') || (cmd[1][0] >= 'A' && cmd[1][0] <= 'Z') || cmd[1][0] == '_')
		{
			//set_breakpoint_at_function(dbprogram_id, cmd[1]);
			//return 0;
			return add_bp_at_function(cmd[1]);

		}
		else 
			return -1;
	}
	
	else if (!strcmp(cmd[0], "dump"))
	{
		if (!strcmp(cmd[1], "regs"))
			return dump_regs_exec();
		else
			return -1;
	}
	
	else if (!strcmp(cmd[0], "setreg"))
	{
		return set_registers(&cmd[1]);
	}
	
	else if (!strcmp(cmd[0], "step"))
	{
		return singlestep_exec();
	}

	else if (!strcmp(cmd[0], "exit"))
	{
		free_cmd(cmd);
		remove_breakpoints();
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
	wait_signal();

	dbprogram_name = program;
	printf("Start debugging program : %s\n", program);
	dbprogram_id = pid;
	
	
	breakpoints_init();
	db_inform_init(program);

	while(1)
	{
		if (exec_command(parse_tokenize_line(parse_read_line())))
			break;
	}

	return 0;
}




