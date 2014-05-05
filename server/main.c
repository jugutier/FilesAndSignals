#include <unistd.h>
#include <signal.h>
#include "../files.h"

void request_handler(int n);
int main(void){
	printf("Running Server\n");
	signal(SIGUSR1, request_handler);
	signal(SIGUSR2, SIG_IGN);
	setServerPid(getpid());
	while(1){
		sleep(30);
	};
}

void request_handler(int n){
	char response [255], *lastRequest;
	printf("client request arrived\n");
	pid_t lastClient;
	lastRequest = getLastRequest(&lastClient);
	printf("%s\n",lastRequest) ;
	sprintf(response,"chau %s",lastRequest);
	respond(response, lastClient);
}