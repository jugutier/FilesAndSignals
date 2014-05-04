#include <string.h>
#include "files.h"

/************** PRIVATE FUNCTIONS ********************/
int notify(pid_t who, int action);
int writeMsg(char *msg, pid_t destination, int action);
char* readMsg(pid_t destination, int action);
int writeFile(char * filename, char * s);
int readFile(char * filename, char ** msg);
char * getFileName(pid_t pid, int action);
//int getline(char **lineptr, size_t *n, FILE *stream);
/************* END PRIVATE FUNCTIONS *****************/

int request(char * msg,	pid_t sender){

	if(writeMsg(msg, sender,ACTION_REQ)){
		notify(getServerPid(),ACTION_REQ);
	}
	return 1;
}

int respond(char * msg, pid_t receiver){
	if(writeMsg(msg, receiver,ACTION_RESP)){
		notify(receiver,ACTION_RESP);
	}
	return 1;
}
pid_t getServerPid(){
	char * msg;
	int ret;
	readFile("serverPid.txt",&msg);
	ret =  atoi(msg);
	//free(msg);
	return ret;
}
void setServerPid(pid_t srvrPid){
	char pidString[10];
	sprintf(pidString, "%d\n",srvrPid);
	writeFile("serverPid.txt",pidString);
}
char * getResponse(pid_t sender){
	return readMsg(sender,ACTION_RESP);
}
char * getRequest(pid_t sender){
	return readMsg(sender,ACTION_REQ);
}
int notify(pid_t who, int action){
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
	return writeFile(getFileName(destination,action),msg);
}
char* readMsg(pid_t destination, int action){
	char * msg;
	if(!readFile(getFileName(destination,action),&msg)){
		return NULL;
	}
	return msg;
}
char * getFileName(pid_t pid, int action){
	char filename[255];
 	if(action == ACTION_REQ){
	  	sprintf(filename,"request%d.txt",pid);
	}else if(action == ACTION_RESP){
	  	sprintf(filename,"response%d.txt",pid);		
	}
	return filename;
}
int writeFile(char * filename, char * s){
	FILE * fp = fopen(filename, "w");
	if(fp != NULL){
		fputs(s,fp);
		fclose(fp);
	}	
	return fp!=NULL ;
}
int readFile(char * filename, char ** msg){
	FILE *fp;
	size_t msgLenght;
	fp=fopen(filename, "r");
	if(fp != NULL){
		if(getline(msg, &msgLenght, fp)==-1){
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


int getline(char **lineptr, size_t *n, FILE *stream)
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
}*/