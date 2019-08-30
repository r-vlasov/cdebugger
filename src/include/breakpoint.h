#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H


#include "../ds/lists.h"
#include <sys/types.h>


typedef struct breakpoint {
	unsigned long 	orig_data;
	long 		address;
	int 		enabled;
} breakpoint_t;



int 		breakpoint_compare(void* address, void* list_node);
void 		breakpoints_init();
breakpoint_t*	breakpoint_create(pid_t dbpid, void* value);

int 		breakpoint_enable(pid_t dbpid, long long address);
int 		breakpoint_disable(pid_t dbpid, long long address);
void 		step_to_breakpoint(pid_t pid);



#endif
