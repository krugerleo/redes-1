#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>  

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

void changeDir(char *c){
    chdir(c); 
}
int validMsg(char *c){
    
}
int main (int argc, char *argv[]) {

    char *line = NULL;  /* forces getline to allocate with malloc */
    size_t len = 0;     /* ignored when line = NULL */
    ssize_t read;

    printf ("\nEnter string below [ctrl + d] to quit\n");

    while ((read = getline(&line, &len, stdin)) != -1) {

        if (read > 0)
            printf ("\n  read %zd chars from stdin, allocated %zd bytes for line : %s\n", read, len, line);

        printf ("Enter string below [ctrl + d] to quit\n");
    }

    free (line);  /* free memory allocated by getline */

    return 0;
} 