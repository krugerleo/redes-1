#include "commands.h"
int list(){
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
  
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);     
    return 0; 
}

int changeDir(char *c){
    return chdir(c);
}
void red () {
  printf("\033[1;31m");
}

void yellow (){
  printf("\033[1;33m");
}

void reset () {
  printf("\033[0m");
}

int validCommand(char *c){
    char *commands [] = {"cd", "lcd", "ls", "lls", "ver", "linha", "linhas", "edit"};
    int length = 8;
    for(int i = 0; i < length; ++i)
    {
        if(!strcmp(commands[i], c))
        {
            return 0;
        }
    }
    red();
    reset();
    return -1;
}
void readFile(char *c){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(c, "r");
    if (fp == NULL)
        return ;
    printf("#N N=numero da linha");
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("#%d %s", count, line);
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return;
}
void readLines(int begin, int end, char *c){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(c, "r");
    if (fp == NULL)
        return ;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(count >= begin && count <= end){
            printf("#%d %s", count, line);
        }
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return;
}
void readLine(int number, char *c){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(c, "r");
    if (fp == NULL)
        return ;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(count == number){
            printf("#%d %s", count, line);
        }
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return;
}
int replaceLine(char *fn, int line, char text[512])
{
        /* File pointer to hold reference of input file */
    FILE * fPtr;
    FILE * fTemp;
    char path[100];
    
    char buffer[BUFFER_SIZE];
    char newline[BUFFER_SIZE];
    int  count;


    /* Remove extra new line character from stdin */
    strcpy(path,fn);
    strcpy(newline,text);
    // strcat(newline,"\n");

    /*  Open all required files */
    fPtr  = fopen(path, "r");
    fTemp = fopen("replace.tmp", "w"); 
    /* fopen() return NULL if unable to open file in given mode. */
    if (fPtr == NULL || fTemp == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        return 0;
    }
    /*
     * Read line from source file and write to destination 
     * file after replacing given line.
     */
    count = 0;
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        count++;
        /* If current line is line to replace */
        if (count == line)
            fputs(newline, fTemp);
        else
            fputs(buffer, fTemp);
    }
    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);
    /* Delete original source file */
    remove(path);
    /* Rename temporary file as original file */
    rename("replace.tmp", path);
    printf("\nSuccessfully replaced '%d' line with '%s'.", line, newline);
    return 1;
}