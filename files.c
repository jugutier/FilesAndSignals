#include <string.h>
#include "files.h"

/************** PRIVATE FUNCTIONS ********************/
int notify(pid_t who, int action);
int writeMsg(char *msg, pid_t destination, int action);
char* readMsg(pid_t destination, int action);
int writeFile(char * filename, char * s);
int readFile(char * filename, char ** msg);
char * getFileName(pid_t pid, int action);
char filenameBuffer[20];
/************* END PRIVATE FUNCTIONS *****************/

int request(char * msg,	pid_t sender){
	printf("request\n");
	pid_t temp;
	if(writeMsg(msg, sender,ACTION_REQ)){
		setClientPid(sender);
		temp = getServerPid();
		printf("serverPid = %d\n",temp );
		notify(temp,ACTION_REQ);
		printf("finish request\n");
	}
	return 1;
}

int respond(char * msg, pid_t receiver){
	printf("respond\n");
	if(writeMsg(msg, receiver,ACTION_RESP)){
		notify(receiver,ACTION_RESP);
	}
	return 1;
}
pid_t getServerPid(){
	printf("getServerPid\n");
	char * msg;
	readFile("serverPid.txt",&msg);
	printf("server pid string %s\n",msg );
	return (pid_t)atoi(msg);
}
void setServerPid(pid_t srvrPid){
	printf("setServerPid\n");
	char pidString[10];
	sprintf(pidString, "%d\n",srvrPid);
	writeFile("serverPid.txt",pidString);
}
pid_t getClientPid(){
	printf("getClientPid\n");
	char * msg;
	readFile("clientPid.txt",&msg);
	return atoi(msg);
}
void setClientPid(pid_t srvrPid){
	printf("setClientPid\n");
	char pidString[10];
	sprintf(pidString, "%d\n",srvrPid);
	writeFile("clientPid.txt",pidString);
}
char * getResponse(pid_t sender){
	printf("getResponse\n");
	return readMsg(sender,ACTION_RESP);
}
char * getLastRequest(pid_t * sender){
	printf("getLastRequest\n");
	*sender = getClientPid();
	return readMsg(*sender,ACTION_REQ);
}
int notify(pid_t who, int action){
	printf("notify\n");
	int ret;
 	if(action == ACTION_REQ){
 		//SERVER
 		ret = kill(who,SIGUSR1);
	}else if(action == ACTION_RESP){
		//CLIENT
		ret = kill(who,SIGUSR2);
	}
	return ret;
}
int writeMsg(char *msg, pid_t destination, int action){
	printf("writeMsg\n");
	char * filename;
	int ret;
	filename = getFileName(destination,action);
	ret = writeFile(filename,msg);
	//printf("writeMsg\n");
	return ret;
}
char* readMsg(pid_t destination, int action){
	printf("readMsg\n");
	char * msg , *filename;
	filename = getFileName(destination,action);
	printf("filename readMsg = %s\n",filename);
	int error = !readFile(filename,&msg);
	return error ? NULL : msg;
}
char * getFileName(pid_t pid, int action){
 	if(action == ACTION_REQ){
	  	sprintf(filenameBuffer,"request%d.txt",pid);
	}else if(action == ACTION_RESP){
	  	sprintf(filenameBuffer,"response%d.txt",pid);		
	}
	printf("getFileName %s\n",filenameBuffer );
	return filenameBuffer;
}
int writeFile(char * filename, char * s){
	printf("writeFile %s\n",filename );
	FILE * fp = fopen(filename, "w");
	if(fp != NULL){
		fputs(s,fp);
		fclose(fp);
	}	
	return fp != NULL ;	
}
int readFile(char * filename, char ** msg){
	printf("readFile %s\n",filename );

	FILE *fp;
	size_t msgLenght;
	printf("fopen\n");
	fp=fopen(filename, "r");
	printf("pointer %p\n", fp);
	if(fp != NULL){
		printf("entro\n");
		if(fgets(*msg, 6, fp)==NULL){
			printf("error on getline\n");
		}
		printf("fclose\n");
		fclose(fp);
	}
	printf("finish readFile\n");
	return 	fp != NULL? msgLenght : -1;
}
