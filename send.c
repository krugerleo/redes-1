
#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	char *text;

    fp = fopen("text.txt", "r");
    if (fp == NULL)
        return 0;
    printf("#N N=numero da linha\n");
    while ((read = getline(&line, &len, fp)) != -1) {
		text = realloc(text,(strlen(line) + 10));
		snprintf(text, (strlen(line) + 3), "#%d ", count);
		text = strncat(text,line,strlen(line));
        printf("%s",text);
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}