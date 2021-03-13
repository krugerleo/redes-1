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
void readLine(){
    FILE * fp;
    int count=0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("Makefile", "r");
    if (fp == NULL)
        return ;

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s\n", line);
        printf("%d\n",count);
        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    return;
}
int replaceLine()
{
    FILE *fileptr1, *fileptr2;
    char filechar[40];
    char c;
    int delete_line, temp = 1;
 
    printf("Enter file name: ");
    scanf("%s", filechar);
    fileptr1 = fopen(filechar, "r");
    c = getc(fileptr1);
    //print the contents of file .
    while (c != EOF)
    {
        printf("%c", c);
        c = getc(fileptr1);
    }
    printf(" \n Enter line number to be deleted and replaced");
    scanf("%d", &delete_line);
    //take fileptr1 to start point.
    rewind(fileptr1);
    //open replica.c in write mode
    fileptr2 = fopen("replica.c", "w");
    c = getc(fileptr1);
    while (c != EOF)
    {
        if (c == 'n')
        {
            temp++;
        }
        //till the line to be deleted comes,copy the content to other
        if (temp != delete_line)
        {
            putc(c, fileptr2);
        }
        else
        {
            while ((c = getc(fileptr1)) != 'n')
            {
            }
            //read and skip the line ask for new text
            printf("Enter new text");
            //flush the input stream
            fflush(stdin);
            putc('n', fileptr2);
            //put 'n' in new file
            while ((c = getchar()) != 'n')
                putc(c, fileptr2);
            //take the data from user and place it in new file
            fputs("n", fileptr2);
            temp++;
        }
        //continue this till EOF is encountered
        c = getc(fileptr1);
    }
    fclose(fileptr1);
    fclose(fileptr2);
    remove(filechar);
    rename("replica.c", filechar);
    fileptr1 = fopen(filechar, "r");
    //reads the character from file
    c = getc(fileptr1);
    //until last character of file is encountered
    while (c != EOF)
    {
        printf("%c", c);
        //all characters are printed
        c = getc(fileptr1);
    }
    fclose(fileptr1);
    return 0;
}
int main (int argc, char *argv[]) {
    readLine();
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