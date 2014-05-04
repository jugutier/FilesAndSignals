#include <unistd.h>
#include <signal.h>
#include "../files.h"

void request_handler(int n);
int main(void){
	printf("Running Server\n");
	signal(SIGUSR1, request_handler);
	signal(SIGUSR2, SIG_IGN);
	setServerPid(getpid());
	while(1);
}

void request_handler(int n){
	printf("client request arrived\n");
	printf("%s\n",getRequest(getpid())) ;
	//process request
	//respond(msg, receiver);
}