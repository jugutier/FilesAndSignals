#ifndef __FILES_H
#define __FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define ACTION_REQ  1
#define ACTION_RESP 2
/**
* write request msg into designated file, from sender. notify server via signal
**/
int request(char * msg,	pid_t sender);
/**
* respond msg into designated file. notify receiver via signal
**/
int respond(char * msg, pid_t receiver);
/**
* get last response from file queue for sender
**/
char * getResponse(pid_t sender);
/**
* get last request from file queue for sender
**/
char * getRequest(pid_t sender);

#endif