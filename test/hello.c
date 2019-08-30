#include <stdio.h>


int myfunction()
{
	return 0;
}
int main()
{
	fprintf(stderr, "FIRST");
	int a = 2;
	int c = myfunction();
	fprintf(stderr, "HELLO");
}

