#include <string.h>
#include "files.h"

pid_t serverPid = (pid_t)666;//proces group id
int notify(int action);
int writeMsg(char *msg, pid_t destination, int action);
int request(char * msg,	pid_t sender){

	if(writeMsg(msg, sender,ACTION_REQ)){
		notify(ACTION_REQ);
	}
	return 1;
}

int respond(char * msg, pid_t receiver){
	if(writeMsg(msg, receiver,ACTION_RESP)){
		notify(ACTION_RESP);
	}
	return 1;
}

char * getResponse(pid_t sender){
	return NULL;
}
char * getRequest(pid_t sender){
	return NULL;
}
int notify(int action){
	int ret;
 	if(action == ACTION_REQ){
 		//SERVER
 		ret = kill(0,SIGUSR1);
 		//if it's 0 it sends to the same gid as the caller.
	}else if(action == ACTION_RESP){
		//CLIENTS
		ret = kill(0,SIGUSR2);
	}
	return ret;
}
int writeMsg(char *msg, pid_t destination, int action){
	FILE *fp;
	char buffer [33];
	char * actionName;
  	sprintf(buffer,"%d",destination);
 	if(action == ACTION_REQ){
 		actionName = "./request";
	}else if(action == ACTION_RESP){
		actionName = "./response";
	}
	char * filename = strcat(strcat(actionName,buffer),".txt");
	fp=fopen(filename, "rw");
	fwrite(msg, sizeof(msg[0]), sizeof(msg)/sizeof(msg[0]), fp);
	fclose(fp);

	return 1;
}
int readMsg(){

	return 1;
}