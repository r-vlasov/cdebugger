#include "include/registers.h"
#include <sys/user.h>
#include <sys/ptrace.h>
#include <stdio.h>

long long get_ip(pid_t pid)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	return regs.rip;
}


void set_ip(pid_t pid, long long ip)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	regs.rip = ip;

	ptrace(PTRACE_SETREGS, pid, 0, &regs);
}


void dump_registers(pid_t pid)
{
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	fprintf(stderr, " RAX : 0x%08llx\n", regs.rax);
	fprintf(stderr, " RBX : 0x%08llx\n", regs.rbx);
	fprintf(stderr, " RCX : 0x%08llx\n", regs.rcx);
	fprintf(stderr, " RDX : 0x%08llx\n", regs.rdx);
	fprintf(stderr, " RIP : 0x%08llx\n", regs.rip);
}






