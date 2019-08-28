#ifndef __REGISTERS_H
#define __REGISTERS_H

#include <sys/types.h>

long long	get_ip(pid_t pid);
void 		set_ip (pid_t pid, long long ip);
void 		dump_registers(pid_t pid);

#endif
