#include "communication.h"

Package_header lastMessage;
int seqCounter = 0;
int sock_r;
int init = 0;
int port = 0;

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

void send_msg(char *data, int type, int orig, int dest) {
    
    Package_header *msg = (Package_header*) calloc(1,sizeof(Package_header));
    msg->mark = MARK;
    msg->orig = orig;
    msg->dest = dest;
    msg->type = type;
    // msg->parit = make_parity(msg);
    msg->par =1;

    if(data != NULL) {
        int size = strlen(data);
        do{
            memset(msg->data, 0, sizeof(msg->data));
            if(size >=15){
                printf("size > 15 = %d e data = %s",size,data);
                msg->size = 15;            
                memcpy(msg->data, data, 15);
                // printf("After memcpy dest = %s\n", dest);                  
                data+=15;
                size-=15;
            }else{
                printf("size < 15 = %d e data = %s",size,data);
                msg->size = size;            
                memcpy(msg->data, data, size);
                // printf("After memcpy dest = %s\n", dest);                  
                size-=size;
            }
            
            msg->seq  = seqCounter;
            seqCounter = (seqCounter + 1) % 255;
            if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
                perror("Send Error");
            }

            memcpy(&lastMessage,msg,sizeof(Package_header));
        } while (size > 0);        
    } else {
        msg->seq  = seqCounter;
        seqCounter = (seqCounter + 1) % 255;
        msg->size = 0;
    }
    print_msg(msg);

}

void init_socket() {
    init = 1;
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
}
Package_header * recieve_msg() {

    Package_header *msg = (Package_header *)malloc(sizeof(Package_header));
    
    // fix recieving double msg
    recv(sock_r, msg, sizeof(Package_header), 0);
    recv(sock_r, msg, sizeof(Package_header), 0);

    while(msg->mark != MARK) {
        recv(sock_r, msg, sizeof(Package_header), 0);
    }
    
    print_msg(msg);

    return msg;
}
