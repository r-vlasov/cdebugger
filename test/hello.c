#include <stdio.h>


int myfunction()
{
	return 0;
}
int main()
{
	fprintf(stderr, "FIRST");
	int a = 2;
	int b = a * 2000 * sizeof(int);
	int c = myfunction();
	fprintf(stderr, "HELLO");
}

