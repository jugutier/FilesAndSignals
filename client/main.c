#include "../files.h"
void response_handler(int n);
int main(void){
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, response_handler);
	char phrase[255];
	int i =500;
	do{
		printf("Running client, enter some phrase\n");
		scanf("%s",phrase);
		request(phrase,	getpid());
	}while(i--);
}
void response_handler(int n){
	printf("server response arrived\n");
	printf("%s\n",getResponse(getpid())) ;
}
