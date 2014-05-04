#include <unistd.h>
#include <signal.h>
#include "../files.h"

void request_handler(int n);
int main(void){
	setpgid(getpid(), (pid_t)666);
	signal(SIGUSR1, request_handler);
	while(1);
}

void request_handler(int n){
	printf("server request\n");
	//printf("%s\n",getRequest(getpid())) ;
	//process request
	//respond(msg, receiver);
}