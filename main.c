#define _POSIX_SOURCE

#include <stdio.h>
#include <signal.h>

void dummy(int n) { puts("SIGINT"); }

int main()
{
	int n;
	char buf[100];

	signal(SIGINT, dummy);
	printf("%d\n", n = read(0, buf, sizeof buf));
	if ( n == -1 )
		perror("read");
	printf("%d\n", n = read(0, buf, sizeof buf));
	if ( n == -1 )
		perror("read");

	return 0;
}