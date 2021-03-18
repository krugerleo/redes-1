#define  _GNU_SOURCE
#include <stdio.h> 
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256  

int list(char *c){
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(c); 
  
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
    for(int i = 1; i <= length; ++i)
    {
        if(!strcmp(commands[i], c))
        {
            return 0;
        }
    }
    red();
    printf("\"%s\" é um comando inválido", c);
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
    printf("%s",fname);
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

int main () {
    char command[6];
    int line,end;
    char name[MAX],text[512];

    while(1){
        printf("escreva um comando\n");
        scanf("%s",command);
        if(!(validCommand(command))){
            if(!(strcmp("cd",command))){
                // cd <nome_dir>
                scanf("%s",name);
            }
            else if(!(strcmp("lcd",command))){
                // lcd <nome-dir>
                scanf("%s",name);
            }
            else if(!(strcmp("ls",command))){
                // ls
            }
            else if(!(strcmp("lls",command))){
                // lls
            }
            else if(!(strcmp("ver",command))){
                // ver <nome_arq>
                scanf("%s",name);
            }
            else if(!(strcmp("linha",command))){
                // linha <numero_linha> <nome_arq>
                scanf("%d",&line);
                scanf("%s",name);
            }
            else if(!(strcmp("linhas",command))){
                // linhas <numero_linha_inicial> <numero_linha_final> <nome_arq>
                scanf("%d",&line);
                scanf("%d",&end);
                scanf("%s",name);
            }
            else if(!(strcmp("edit",command))){
                // edit <numero_linha> <nome_arq> “<NOVO_TEXTO>”
                scanf("%d",&line);
                scanf("%s",name);
                scanf("%s",text);
            }
        }
    }
} 