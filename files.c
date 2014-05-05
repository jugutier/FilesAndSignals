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
int getLine(char **lineptr, size_t *n, FILE *stream);
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
	pid_t ret;
	//printf("&msg %p\n",&msg );
	readFile("serverPid.txt",&msg);
	//printf("server pid string %s\n",msg );
	ret = (pid_t)atoi(msg);
	free(msg);
	return ret;
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
	pid_t ret;
	//printf("&msg %p msg %p\n",&msg ,msg );
	readFile("clientPid.txt",&msg);
	//printf("client pid string %s\n",msg );
	ret = (pid_t)atoi(msg);
	free(msg);
	return ret;
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
	printf("getFileName\n");
 	if(action == ACTION_REQ){
	  	sprintf(filenameBuffer,"request%d.txt",pid);
	}else if(action == ACTION_RESP){
	  	sprintf(filenameBuffer,"response%d.txt",pid);		
	}
	//printf("getFileName return %s\n",filenameBuffer );
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
	//printf("msg %p\n",msg );
	*msg = malloc(4);
	FILE *fp;
	size_t msgLenght;
	fp=fopen(filename, "r");
	if(fp != NULL){
		if(getLine(msg, &msgLenght, fp)==-1){
			printf("error on getline\n");
		}
		fclose(fp);
	}
	return 	fp != NULL? msgLenght : -1;
}

/* Read up to (and including) a newline from STREAM into *LINEPTR
   (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'd as
   necessary.  Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.  
   src: http://lynx.isc.org/lynx-2.8.2/breakout/WWW/Library/Implementation/getline.c
*/

int getLine(char **lineptr, size_t *n, FILE *stream)
{
	static char line[256];
	char *ptr;
	unsigned int len;

   if (lineptr == NULL || n == NULL)
   {
      return -1;
   }

   if (ferror (stream))
      return -1;

   if (feof(stream))
      return -1;
     
   fgets(line,256,stream);

   ptr = strchr(line,'\n');   
   if (ptr)
      *ptr = '\0';

   len = strlen(line);
   
   if ((len+1) < 256)
   {
      ptr = realloc(*lineptr, 256);
      if (ptr == NULL)
         return(-1);
      *lineptr = ptr;
      *n = 256;
   }

   strcpy(*lineptr,line); 
   return(len);
}
