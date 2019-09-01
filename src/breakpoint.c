#include "include/breakpoint.h"
#include "include/registers.h"
#include "include/signals.h"

#include "ds/lists.h"
#include <sys/user.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <stdio.h>



list_t* breakpoint_list;


int breakpoint_compare(void* address, void* list_node_value)
{
	if ( (long)address == ((breakpoint_t*)list_node_value)->address )
		return 1;
	return 0;
}
	


void breakpoints_init()
{
	breakpoint_list = list_create();
}



breakpoint_t* breakpoint_create(pid_t dbpid, void* value)
{
	breakpoint_t* bp = (breakpoint_t*)malloc(sizeof(breakpoint_t));
	bp->address = (long)value;
	bp->orig_data = ptrace(PTRACE_PEEKDATA, dbpid, bp->address, 0);
	list_add(breakpoint_list, bp);
	return bp;
}



int breakpoint_enable(pid_t dbpid, long long address)
{
	list_node_t* node_bp = list_search(breakpoint_list, (void*) address, &breakpoint_compare);
	breakpoint_t* bp;

	if ( node_bp != NULL )
	{
		bp = (breakpoint_t*)node_bp->value;
	}
	else
	{
		bp = breakpoint_create(dbpid, (void*) address); 
	}

	
	if (bp->orig_data < 0)
	{
		fprintf(stderr, "ptrace peekdata error\n");
		return -1;
	}

	bp->enabled = 1;
	bp->address = address;

	if (ptrace(PTRACE_POKEDATA, dbpid, address, ((bp->orig_data & ~0xFF) | 0xCC)) < 0)
	{
		fprintf(stderr, "ptrace pokedata error\n");
		return -1;
	}


	return 0;
}



int breakpoint_disable(pid_t dbpid, long long address)
{
	long long rip = get_ip(dbpid);
	set_ip(dbpid, rip-1);



	list_node_t* node_bp = list_search(breakpoint_list, (void*) address, &breakpoint_compare);
	breakpoint_t* bp;

	if (node_bp != NULL)
	{
		bp = (breakpoint_t*)(node_bp->value);

		if (ptrace(PTRACE_POKEDATA, dbpid, address, bp->orig_data) < 0)
		{
			fprintf(stderr, "ptrace pokedata error\n");
			return -1;
		}

		if (bp->enabled)
			bp->enabled = 0;
		else
		{
			fprintf(stderr, "disable breakpoint that was disabled\n");
			return -1;
		}
	}
	else
	{
		fprintf(stderr, "can't find the breakpoint with address 0x%08lx\n", (long)address);
		return -1;
	}
	return 0;
}


void step_to_breakpoint(pid_t pid)
{
	long long cur_rip = get_ip(pid) - 1;

	list_node_t* node_bp = list_search(breakpoint_list, (void*) cur_rip, &breakpoint_compare);
	if ( node_bp != NULL )
	{
		breakpoint_disable(pid, cur_rip);
		ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
		
		handle_signal(pid);

		breakpoint_enable(pid, cur_rip);
	}
	ptrace(PTRACE_CONT, pid, 0, 0);
}


void singlestepping_start(pid_t pid)
{
	long long cur_rip = get_ip(pid) - 1;
	list_node_t* node_bp = list_search(breakpoint_list, (void*) cur_rip, &breakpoint_compare);
	breakpoint_t* bp;

	if ( node_bp != NULL )
	{
		breakpoint_disable(pid, cur_rip);
		ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
		
		handle_signal(pid);

		breakpoint_enable(pid, cur_rip);
	}
	else
	{
		ptrace(PTRACE_SINGLESTEP, pid, 0, 0);	
		handle_signal(pid);
	}

}


void remove_breakpoints()
{
	list_remove(breakpoint_list);
}
