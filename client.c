#include "client.h"

int main () {
    char command[6];
    char name[MAX],text[512],begin[15],end[15];
    int seq = 0;
    Sender client = { .orig = 1, .dest=2};
    Package_header *msg;
    Package_header *resp;
    init_socket();

    while(1){
        command:
        printf("\nescreva um comando\n");
        scanf("%s",command);
        if(!(validCommand(command))){
            if(!(strcmp("cd",command))){
                // cd <nome_dir>
                scanf("%s",name);
                cd:                
                send_msgs(name,CDT,client.orig,client.dest);
                send_msgs(NULL,EOT,client.orig,client.dest);
                recieve_cd:
                msg = recieve_msg();
                if(msg->type == NACK){goto cd;}
                if(msg->type == ERROR){printf("\n\t| Erro -> %s\n",msg->data);goto command;}
                if(msg->type == ACK){printf("\n\t| Sucesso\n"); goto command;}
                if(msg->dest != client.orig){goto recieve_cd;}
            }
            else if(!(strcmp("lcd",command))){
                // lcd <nome-dir>
                scanf("%s",name);
                (changeDir(name) != 0)?printf("failed cd"):printf("\n\t| Sucesso\n");;         
            }
            else if(!(strcmp("ls",command))){
                seq = 0;
                // ls
                send_msgs(NULL,LST,client.orig,client.dest);// send LST and save it to lastmessage
                recieve_ls:
                resp = recieve_msg();
                while (resp->type != EOT)
                {
                    if(resp->type == NACK){send_last();}
                    if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);goto command;}
                    if(resp->dest != client.orig){goto recieve_ls;}
                    if(resp-> seq != seq){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_ls;}
                    if(check_parity(resp)){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_ls;}
                    if(resp->type == CONTLS){
                        printf("%s",resp->data);
                        send_msgs(NULL,ACK,client.orig,client.dest);
                        seq = (seq + 1 ) % 16;
                        resp = recieve_msg();
                    }
                }
                send_msgs(NULL,ACK,client.orig,client.dest);
            }
            else if(!(strcmp("lls",command))){
                // lls
                list();
            }
            else if(!(strcmp("ver",command))){
                seq = 0;
                // ver <nome_arq>
                scanf("%s",name);
                ver_name:
                send_msgs(name,VERT,client.orig,client.dest);
                send_msgs(NULL,EOT,client.orig,client.dest);
                sleep(4);
                recieve_ver:
                resp = recieve_msg();
                if(resp->dest != client.orig){goto recieve_ver;}
                while (resp->type != EOT)
                {
                    if(resp->type == NACK){goto ver_name;}
                    if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);goto command;}
                    if(resp->dest != client.orig){goto recieve_ver;}
                    if(resp-> seq != seq){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_ver;}
                    if(check_parity(resp)){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_ver;}
                    if(resp->type == CONTARQ){
                        printf("%s",resp->data);
                        send_msgs(NULL,ACK,client.orig,client.dest);
                        sleep(2);
                        seq = (seq + 1 ) % 16;
                        resp = recieve_msg();
                    }                    
                }
                send_msgs(NULL,ACK,client.orig,client.dest);

            }
            else if(!(strcmp("linha",command))){
                seq = 0;
                // linha <numero_linha> <nome_arq>
                scanf("%s",begin);
                scanf("%s",name);
                ver_linha:
                send_msgs(name,LINHAT,client.orig,client.dest);
                send_msgs(begin,LIF,client.orig,client.dest);
                send_msgs(NULL,EOT,client.orig,client.dest);
                recieve_linha:
                resp = recieve_msg();
                if(resp->dest != client.orig){goto recieve_linha;}
                while (resp->type != EOT)
                {
                    if(resp->type == NACK){goto ver_linha;}
                    if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);goto command;}
                    if(resp->dest != client.orig){goto recieve_linha;}
                    if(resp-> seq != seq){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_linha;}
                    if(check_parity(resp)){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_linha;}
                    if(resp->type == CONTARQ){
                        printf("%s\n ",resp->data);
                        send_msgs(NULL,ACK,client.orig,client.dest);
                        sleep(2);
                        seq = (seq + 1 ) % 16;
                        resp = recieve_msg();
                    }
                }
                send_msgs(NULL,ACK,client.orig,client.dest);
            }
            else if(!(strcmp("linhas",command))){
                seq = 0;
                // linhas <numero_linha_inicial> <numero_linha_final> <nome_arq>
                scanf("%s",begin);
                scanf("%s",end);
                scanf("%s",name);
                strcat(begin, "/");
                strcat(begin, end);
                ver_linhas:
                send_msgs(name,LINHAST,client.orig,client.dest);
                send_msgs(begin,LIF,client.orig,client.dest);
                send_msgs(NULL,EOT,client.orig,client.dest);
                recieve_linhas:
                resp = recieve_msg();
                if(resp->dest != client.orig){goto recieve_linhas;}
                while (resp->type != EOT)
                {
                    if(resp->type == NACK){goto ver_linhas;}
                    if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);goto command;}
                    if(resp->dest != client.orig){goto recieve_linhas;}
                    if(resp-> seq != seq){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_linhas;}
                    if(check_parity(resp)){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_linhas;}
                    if(resp->type == CONTARQ){
                        send_msgs(NULL,ACK,client.orig,client.dest);
                        sleep(2);
                        seq = (seq + 1 ) % 16;
                        resp = recieve_msg();
                    }
                }
                send_msgs(NULL,ACK,client.orig,client.dest);
            }
            else if(!(strcmp("edit",command))){
                // edit <numero_linha> <nome_arq> “<NOVO_TEXTO>”
                scanf("%s",begin);
                scanf("%s",name);
                fgets(text,512,stdin);
                send_msgs(name,EDITT,client.orig,client.dest);
                send_msgs(begin,LIF,client.orig,client.dest);
                send_msgs(NULL,EOT,client.orig,client.dest);
                communication(text,CONTARQ,client.orig,client.dest,0);
                communication(NULL,EOT,client.orig,client.dest,1);   
            }
        }
    }
} 