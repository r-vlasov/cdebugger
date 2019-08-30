#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	const char* s = "hello";
	*(char*) s = 'H';
	return 0;
}
