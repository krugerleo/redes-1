#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <string.h>
#include <arpa/inet.h>

#define MARK 126 // 01111110

typedef struct Sender{
  unsigned char dest : 2;
  unsigned char orig : 2;
} Sender;

typedef struct Package_header
{
    unsigned char mark : 8; // 01111110     
    unsigned char dest : 2;
    unsigned char orig : 2;              
    unsigned char size : 4;                
    unsigned char seq : 4;               
    unsigned char type : 4;
    unsigned char data[15];                
    unsigned char par; 
} Package_header;

typedef enum {
    CDT = 0 // 0000 0
  , LST     // 0001 1
  , VERT    // 0010 2
  , LINHAT  // 0011 3
  , LINHAST // 0100 4
  , EDITT   // 0101 5
  , NOTSET  // 0110 6
  , NOTSET2 // 0111 7
  , ACK     // 1000 8
  , NACK    // 1001 9
  , LIF     // 1010 10
  , CONTLS  // 1011 11
  , CONTARQ // 1100 12 
  , EOT     // 1101 13
  , NOTSET3 // 1110 14
  , ERROR   // 1111 15
} MSG_TYPE;

typedef enum {
    FORBIDDEN   //
  , DIRNOTFOUND //
  , ARQNOTFOUND //  
  , LINENOTFOUND//
} ERROR_ENUM;


int ConexaoRawSocket(char *device);
void send_msgs(char *data, int type, int orig, int dest);
void communication(char *data, int type, int orig, int dest, int first);
int check_parity(Package_header *msg);
int make_parity(Package_header *msg);
void init_socket();
void print_msg(Package_header *msg);
void send_last();
Package_header * recieve_msg();

#endif