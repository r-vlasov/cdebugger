#include "../ds/lists.h"
#include <sys/types.h>


typedef struct breakpoint {
	long 	orig_data;
	long 	address;
	int 	enabled;
} breakpoint_t;


list_t* breakpoint_list;


int 		breakpoint_compare(void* address, void* list_node);
void 		breakpoints_init();
breakpoint_t*	breakpoint_create(pid_t dbpid, void* value);
int 		breakpoint_enable(pid_t dbpid, long address);
int 		breakpoint_disable(pid_t dbpid, long address);
