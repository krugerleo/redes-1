#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256  

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
void readFile(){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("text.txt", "r");
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
void readLines(int begin, int end){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("Makefile", "r");
    if (fp == NULL)
        return ;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(count >= begin && count <= end){
            printf("#%d %s", count, line);
            count++;
        }
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return;
}
int replaceLine()
{
    FILE *fptr1, *fptr2;
    int lno, linectr = 0;
    char str[MAX],fname[MAX];        
    char newln[MAX], temp[] = "temp.txt";
    
    printf("\n\n Replace a specific line in a text file with a new text :\n");
    printf("-------------------------------------------------------------\n"); 
    printf(" Input the file name to be opened : ");
    fgets(fname, MAX, stdin);
    fname[strlen(fname) - 1] = '\0';
    fptr1 = fopen(fname, "r");
    if (!fptr1) 
    {
            printf("Unable to open the input file!!\n");
            return 0;
    }
    fptr2 = fopen(temp, "w");
    if (!fptr2) 
    {
            printf("Unable to open a temporary file to write!!\n");
            fclose(fptr1);
            return 0;
    }
    /* get the new line from the user */
    printf(" Input the content of the new line : ");
    fgets(newln, MAX, stdin);
    /* get the line number to delete the specific line */
    printf(" Input the line no you want to replace : ");
    scanf("%d", &lno);
    lno++;
        // copy all contents to the temporary file other except specific line
    while (!feof(fptr1)) 
    {
        strcpy(str, "\0");
        fgets(str, MAX, fptr1);
        if (!feof(fptr1)) 
        {
            linectr++;
            if (linectr != lno) 
                {
                    fprintf(fptr2, "%s", str);
                } 
                else 
                {
                    fprintf(fptr2, "%s", newln);
                }
            }
    }
    fclose(fptr1);
    fclose(fptr2);
    remove(fname);
    rename(temp, fname);
    printf(" Replacement did successfully..!! \n");
    return 0;
}
int main (int argc, char *argv[]) {
    readFile();
    char *line = NULL;  /* forces getline to allocate with malloc */
    size_t len = 0;     /* ignored when line = NULL */
    ssize_t read;

    printf ("\nEntre com o comando, [ctrl + d] para sair\n");

    while ((read = getline(&line, &len, stdin)) != -1) {

        if (read > 0)
            printf ("\n  read %zd chars from stdin, allocated %zd bytes for line : %s\n", read, len, line);

        printf ("\nEntre com o comando, [ctrl + d] para sair\n");
    }

    free (line);  /* free memory allocated by getline */

    return 0;
} 