#ifndef COMMANDS_H
#define COMMANDS_H

#define  _GNU_SOURCE
#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

int list();
void changeDir(char *c);
void red ();
void yellow ();
void reset ();
int validCommand(char *c);
void readFile(char *c);
void readLines(int begin, int end, char *c);
void readLine(int number, char *c);
int replaceLine();


#endif
