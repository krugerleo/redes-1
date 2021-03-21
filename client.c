#include "client.h"
const int ORIGI = 01;
const int DESTI = 10;


int main () {
    char command[6];
    int line,end;
    char name[MAX],text[512];
    // Package_header *msg;
    init_socket();

    while(1){
        printf("\nescreva um comando\n");
        scanf("%s",command);
        if(!(validCommand(command))){
            if(!(strcmp("cd",command))){
                // cd <nome_dir>
                fgets(name, MAX, stdin);
                // scanf("%s",name);
                // send_msg(name,CDT,ORIGI,DESTI);
                send_msg(NULL,EOT,ORIGI,DESTI);
            }
            else if(!(strcmp("lcd",command))){
                // lcd <nome-dir>
                scanf("%s",name);
                changeDir(name);
            }
            else if(!(strcmp("ls",command))){
                // ls
                list();
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