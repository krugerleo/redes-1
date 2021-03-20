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
  
  memset(&ir, 0, sizeof(struct ifreq));  	/*dispositivo eth0*/
  memcpy(ir.ifr_name, device, sizeof(device));
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

void send_msg(char *data, int type) {
    Package_header *msg = (Package_header*) calloc(1,sizeof(Package_header));
    
    const int seq  = seqCounter;
    
    if(data != NULL) {
        // need to send msg while size > 0
        while{
            msg->size = strlen(data);
            memcpy(msg->data,data,strlen(data));
            seqCounter = (seqCounter + 1) % 255;
        }do
    } else {
        msg->size = 0;
    }

    msg->mark = MARK;
    msg->seq  = seq;
    msg->type = type;
    // msg->parit = make_parity(msg);
    msg->par =1;

    if((send(sock_r, msg, sizeof(Package_header), 0)) == -1) {
        perror("Send Error");
    }

    memcpy(&lastMessage,msg,sizeof(Package_header));

    printf("%u %u %u %u %s %u\n",
    msg->mark
        , msg->size
        , msg->seq
        , msg->type
        , msg->data
        , msg->par
    );
}

void init_socket() {
    init = 1;
    sock_r = ConexaoRawSocket("lo");
}

Package_header * recieve_msg() {

    Package_header *msg = (Package_header *)malloc(sizeof(Package_header));
    
    // fix recieving double msg
    recv(sock_r, msg, sizeof(Package_header), 0);
    recv(sock_r, msg, sizeof(Package_header), 0);

    while(msg->mark != MARK) {
        recv(sock_r, msg, sizeof(Package_header), 0);
    }
    
    printf("%u %u %u %u %s %u\n",
        msg->mark
        , msg->size
        , msg->seq
        , msg->type
        , msg->data
        , msg->par
    );

    return msg;
}
