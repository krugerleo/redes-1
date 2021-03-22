#include "communication.h"

Package_header *lastMessage;
int seqCounter = 0;
int sock_r;

int timeo(int count){
    unsigned int timeout=769;
	while ((--timeout > 0)){
    }    
    return count++;
}
int make_parity(Package_header *msg){
    unsigned char p = msg->size ^ msg->seq ^ msg->type;
    for (int i = 0; i < msg->size; i++) {
        p ^= msg->data[i];
    }

    return p;
}

int check_parity(Package_header *msg){
    unsigned char p = msg->size ^msg->seq ^msg->type;
    
    for (int i = 0; i < msg->size; i++) {
        p ^= msg->data[i];
    }
    
    return p == msg->par;
}

int ConexaoRawSocket(char *device)
{
  int soquete;
  struct ifreq ir;
  struct sockaddr_ll endereco;
  struct packet_mreq mr;
  
  soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  	/*cria socket*/
  if (soquete == -1) {
    printf("Erro no Socket\n");
    exit(-1);
  }
  
  int device_size = sizeof(device);
  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, device, device_size);
  if (ioctl(soquete, SIOCGIFINDEX, &ir) == -1) {
    printf("Erro no ioctl\n");
    exit(-1);
  }
	
  
  memset(&endereco, 0, sizeof(endereco)); 	/*IP do dispositivo*/
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ir.ifr_ifindex;
  if (bind(soquete, (struct sockaddr *)&endereco, sizeof(endereco)) == -1) {
    printf("Erro no bind\n");
    exit(-1);
  }


  memset(&mr, 0, sizeof(mr));          /*Modo Promiscuo*/
  mr.mr_ifindex = ir.ifr_ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  if (setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1)	{
    printf("Erro ao fazer setsockopt\n");
    exit(-1);
  }

  return soquete;
}

void send_msgs(char *data, int type, int orig, int dest) {
    
    Package_header *msg = (Package_header*) calloc(1,sizeof(Package_header));
    msg->mark = MARK;
    msg->orig = orig;
    msg->dest = dest;
    msg->type = type;
    msg->seq = seqCounter;
    msg->par = make_parity(msg);

    if(data != NULL) {
        int size = strlen(data);
        do{
            memset(msg->data, 0, sizeof(msg->data));
            if(size >=15){
                msg->size = 15;            
                memcpy(msg->data, data, 15);
                data+=15;
                size-=15;
            }else{
                msg->size = size;            
                memcpy(msg->data, data, size);
                size-=size;
            }
            
            if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
                perror("Send Error");
            }
            msg->seq = (msg->seq + 1) % 16;
            seqCounter = (seqCounter + 1 ) %16;
            lastMessage = msg;
        } while (size > 0);        
    } else {

        msg->size = 0;
        if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
            perror("Send Error");
        }

        msg->seq = (msg->seq + 1) % 16;
        seqCounter = (seqCounter + 1 ) %16;


    }

}
void send_last(){
    if((send(sock_r, lastMessage, sizeof(Package_header), 0)) == -1) {
            perror("Send Error");
        }
}
void communication(char *data, int type, int orig, int dest, int first) {
    if(first == 0){
        seqCounter = 0;
    }
    Package_header *msg = (Package_header*) calloc(1,sizeof(Package_header));
    Package_header *resp = (Package_header*) calloc(1,sizeof(Package_header));
    msg->mark = MARK;
    msg->orig = orig;
    msg->dest = dest;
    msg->type = type;
    msg->seq = seqCounter;
    msg->par = make_parity(msg);
    if(type == CONTLS){
        char* aux = malloc(strlen(data) + 2);
        strcpy(aux, data);
        strcat(aux, "\n");
        data = aux;
    }
    if(data != NULL) {
        int size = strlen(data);
        do{
            memset(msg->data, 0, sizeof(msg->data));
            if(size >=15){
                msg->size = 15;            
                memcpy(msg->data, data, 15);
                data+=15;
                size-=15;
            }else{
                msg->size = size;            
                memcpy(msg->data, data, size);
                size-=size;
            }
            
            if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
                perror("Send Error");
            }

            msg->seq = (msg->seq + 1) % 16;
            seqCounter = (seqCounter + 1 ) %16;

            lastMessage = msg;
            wait_resp:
            resp = recieve_msg();
            if(resp->dest != orig){goto wait_resp;}
            if(resp->type == NACK){printf("NACK\n");send_last();goto wait_resp;}
            if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);}
            if(resp->type != ACK){printf("\nWaiting for ACK recieve -> %u",resp->type); goto wait_resp;};
            
        } while (size > 0);        
    } else {
        msg->size = 0;
        if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
            perror("Send Error");
        }
        if(msg->type == EOT){
            wait_ack:
            resp = recieve_msg();
            if(resp->dest != orig){goto wait_ack;}
            if(resp->type == NACK){printf("NACK\n");send_last();goto wait_ack;}
            if(resp->type == ERROR){printf("\n\t| Erro -> %s\n",resp->data);return;}
            if(resp->type != ACK){printf("\nWaiting for ACK recieve -> %u",resp->type); goto wait_ack;};
        }
        msg->seq = (msg->seq + 1) % 16;
        seqCounter = (seqCounter + 1) % 16;
    }
}

void init_socket() {
    sock_r = ConexaoRawSocket("lo");
}
void print_msg(Package_header *msg){
    printf("\n\t| Kermit Header\n");
    printf("\t| mark -> %u \n",msg->mark);
    printf("\t| size -> %u \n",msg->size);
    printf("\t| orig -> %u \n",msg->orig);
    printf("\t| dest -> %u \n",msg->dest);
    printf("\t| seq  -> %u \n",msg->seq);
    printf("\t| type -> %u \n",msg->type);
    printf("\t| data -> %s \n",msg->data);
    printf("\t| par  -> %u \n",msg->par);
    printf("\n\t| exit\n");
}
Package_header * recieve_msg() {
    Package_header *msg = (Package_header *)malloc(sizeof(Package_header));
    
    // fix recieving double msg
    recv(sock_r, msg, sizeof(Package_header), 0);
    recv(sock_r, msg, sizeof(Package_header), 0);

    while(msg->mark != MARK) {
        recv(sock_r, msg, sizeof(Package_header), 0);
    }


    return msg;
}
