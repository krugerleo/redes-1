#include <stdio.h>
#include <string.h>

int main () {
   unsigned char *src = "http://www.tutorialspoint.comasolpdkpoasjkop dik asop jds jda spijd";
   char dest[50];
   int size = strlen(src);
    do{
        memset(dest, 0, sizeof(dest));
        if(size >=15){
            memcpy(dest, src, 15);
            printf("After memcpy dest = %s\n", dest);                  
            src+=15;
            size-=15;
        }else{
            memcpy(dest, src, size);
            printf("After memcpy dest = %s\n", dest);                  
            size-=size;
        }
    } while (size > 0);
   
   
   return(0);
}