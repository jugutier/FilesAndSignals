#include "../files.h"
void response_handler(int n);
int main(void){
	setpgid(getpid(), (pid_t)666);
	signal(SIGUSR2, response_handler);
	char phrase[255];
	printf("Running client, enter some phrase\n");
	scanf("%s",phrase);
	pid_t pid = getpid();
	request(phrase,	pid);
	


}
void response_handler(int n){
	printf("client response\n");
	//printf("%s\n",getResponse(getpid())) ;
}
