#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <sys/types.h>

#define ERROR_EXEC_CMD	-1

int run_debug(pid_t pid, char* program);

#endif
