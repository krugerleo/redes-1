#define  _GNU_SOURCE
#include "communication.h"
#include "commands.h"
#include "server.h"

void send_lines(char *c, Sender client, int begin,int end){
    char *text = malloc(256*sizeof(char));;
    FILE * fp;
    int count=1;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int init = 0;

    fp = fopen(c, "r");
    if (fp == NULL){
        send_msgs("File = NULL",ERROR,client.orig,client.dest);
        return ;
    }else{
        printf("openned");
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if(count >= begin && count <= end){
            snprintf(text, (strlen(line) + 3), "#%d ", count);
            text = strncat(text,line,strlen(line));
            communication(text,CONTARQ,client.orig,client.dest,init);
            init++;
        }

        count++;
    }

    fclose(fp);
    if (line)
        free(line);
    communication(NULL,EOT,client.orig,client.dest,init);
    return;
}

void send_arq(char *c, Sender client){
        char *text = malloc(256*sizeof(char));
        FILE *fp;
        int count=1;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        int init = 0;

        fp = fopen(c, "r");
        if (fp == NULL){
            send_msgs("File = NULL",ERROR,client.orig,client.dest);
            return;
        }else{
            printf("oppened");
        }
        while ((read = getline(&line, &len, fp)) != -1) {
            snprintf(text, (strlen(line) + 3), "#%d ", count);
            text = strncat(text,line,strlen(line));
            communication(text,CONTARQ,client.orig,client.dest,init);
            init++;
            count++;
        }
        printf("OUT");
        fclose(fp);
        if (line)
            free(line);
        communication(NULL,EOT,client.orig,client.dest,init);
        return;
}

void server(){
    Sender client = { .orig = 2, .dest=1};
    Package_header *msg;
    Package_header *resp;
    int init;

    recieve:
    msg = recieve_msg();

    if(msg->dest != client.orig){goto recieve;}
    
    if(msg->type == CDT){
        char *aux = malloc(sizeof(char));
        if(strlen(aux)>0){
            memset(aux, 0, strlen(aux));
        }

        int seq, strSize = 0; // get fisrt number
        seq = msg->seq;
        while (msg->type != EOT)
        {
            if(msg->size > 0){
                strSize = (strlen(aux) + msg->size); // size for aux recieve string
                aux = realloc(aux,(strSize)*sizeof(char));
                strcat(aux,(char*)msg->data);
            }
            
            if(msg->seq > seq){
                send_msgs("sequency",ERROR,client.orig,client.dest);
            }

            seq = (seq + 1) % 16;
            memset(&msg, 0, sizeof(msg)); // reset msg
            msg = recieve_msg();
        }        
        printf("%s",aux);
        (changeDir(aux) != 0)?send_msgs("failed cd", ERROR,client.orig,client.dest):send_msgs(NULL, ACK,client.orig,client.dest);         
    }

    if(msg->type == LST){
        init = 0;
        struct dirent *de;  // Pointer for directory entry 
  
        // opendir() returns a pointer of DIR type.  
        DIR *dr = opendir("."); 

        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            send_msgs("open dir fail", ERROR,client.orig,client.dest);
        } 

        while ((de = readdir(dr)) != NULL) {
            communication(de->d_name,CONTLS,client.orig,client.dest,init);
            init++;
        }        
        closedir(dr);      
        communication(NULL,EOT,client.orig,client.dest,init);
        
    }
    
    if(msg->type == VERT){
        char *aux2 = malloc(sizeof(char));
        if(strlen(aux2)>0){
            memset(aux2, 0, strlen(aux2));
        }
        int seq, strSize = 0; // get fisrt number
        seq = msg->seq;
        while (msg->type != EOT)
        {
            if(msg->size > 0){
                strSize = (strlen(aux2) + msg->size); // size for aux2 recieve string
                aux2 = realloc(aux2,(strSize) * sizeof(char));
                strcat(aux2,(char*)msg->data);
            }
            
            if(msg->seq > seq){
                send_msgs("sequency",ERROR,client.orig,client.dest);
            }

            seq = (seq + 1) % 16;
            memset(&msg, 0, sizeof(msg));
            msg = recieve_msg();
        }        
        printf("%s\n",aux2);

        // open file
        send_arq(aux2,client);
    }
        
    if(msg->type == LINHAT){
        char *aux3 = malloc(sizeof(char));
        int linha;
        if(strlen(aux3)>0){
            memset(aux3, 0, strlen(aux3));
        }
        int seq, strSize = 0; // get fisrt number
        seq = msg->seq;
        while (msg->type != EOT)
        {
            if((msg->size > 0) && (msg->type == LINHAT)){
                strSize = (strlen(aux3) + msg->size); // size for aux3 recieve string
                aux3 = realloc(aux3,(strSize) * sizeof(char));
                strcat(aux3,(char*)msg->data);
            }
            if((msg->size > 0) && (msg->type == LIF)){
                linha = atoi((char*)msg->data);
            }
            if(msg->seq > seq){
                send_msgs("sequency",ERROR,client.orig,client.dest);
            }

            seq = (seq + 1) % 16;
            memset(&msg, 0, sizeof(msg));
            msg = recieve_msg();
        }        
        printf("%s e linha -> %d\n",aux3,linha);
        send_lines(aux3,client,linha,linha);
        printf("exit send lines");
    }
    
    if(msg->type == LINHAST){
        char *aux3 = malloc(sizeof(char));
        int begin,end;
        if(strlen(aux3)>0){
            memset(aux3, 0, strlen(aux3));
        }
        int seq, strSize = 0; // get fisrt number
        seq = msg->seq;
        while (msg->type != EOT)
        {
            if((msg->size > 0) && (msg->type == LINHAST)){
                strSize = (strlen(aux3) + msg->size); // size for aux3 recieve string
                aux3 = realloc(aux3,(strSize) * sizeof(char));
                strcat(aux3,(char*)msg->data);
            }
            if((msg->size > 0) && (msg->type == LIF)){
                char * token = strtok((char*)msg->data, "/");
                begin = atoi(token);
                token = strtok(NULL, " ");
                end = atoi(token);
            }
            if(msg->seq > seq){
                send_msgs("sequency",ERROR,client.orig,client.dest);
            }

            seq = (seq + 1) % 16;
            memset(&msg, 0, sizeof(msg));
            msg = recieve_msg();
        }        

        send_lines(aux3,client,begin,end);
    }    

    if(msg->type == EDITT){
        char *aux3 = malloc(sizeof(char));
        int linha;
        if(strlen(aux3)>0){
            memset(aux3, 0, strlen(aux3));
        }
        int seq, strSize = 0; // get fisrt number
        seq = msg->seq;
        char textr[512],textc[512];
        strcpy(textr, "");
        while (msg->type != EOT)
        {
            if((msg->size > 0) && (msg->type == EDITT)){
                strSize = (strlen(aux3) + msg->size); // size for aux3 recieve string
                aux3 = realloc(aux3,(strSize) * sizeof(char));
                strcat(aux3,(char*)msg->data);
            }
            if((msg->size > 0) && (msg->type == LIF)){
                linha = atoi((char*)msg->data);
            }
            if(msg->seq > seq){
                send_msgs("sequency",ERROR,client.orig,client.dest);
            }

            seq = (seq + 1) % 16;
            memset(&msg, 0, sizeof(msg));
            msg = recieve_msg();
        } 
        seq = 0;
        recieve_edit:
        resp = recieve_msg();
        if(resp->dest != client.orig){goto recieve_edit;}
        while (resp->type != EOT)
        {
            if(resp->dest != client.orig){goto recieve_edit;}
            if(resp-> seq != seq){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_edit;}
            if(check_parity(resp)){send_msgs(NULL,NACK,client.orig,client.dest);goto recieve_edit;}
            if(resp->type == CONTARQ){
                // resp->data[resp->size] = '\0';
                strcat(textr,(char*)resp->data);
                send_msgs(NULL,ACK,client.orig,client.dest);
                sleep(2);
                seq = (seq + 1 ) % 16;
                resp = recieve_msg();
            }
        }
        int i = 0;
        for(int k = 0; k < (int)strlen(textr);k++){
            if((textr[k] != 12) && (textr[k] != 34)){
                textc[i++] = textr[k];
            }
        }


        int val = replaceLine(aux3,linha,textc);
        printf("val -> %d",val);
        if(val){
            send_msgs(NULL,ACK,client.orig,client.dest);
        }else{
            send_msgs("fail to open",ERROR,client.orig,client.dest);
        }
        
        
        
    }
}
int main(){
	init_socket();
	while(1){
		server();
	}

}