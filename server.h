#ifndef SERVER_h
#define SERVER_h

#define  _GNU_SOURCE
#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

void send_lines(char *c, Sender client, int begin,int end);
void send_arq(char *c, Sender client);
void server();

#endif
