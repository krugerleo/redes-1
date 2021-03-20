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

typedef struct Package_header
{
    unsigned char mark : 8; // 01111110     
    unsigned char Dest : 2;
    unsigned char Orig : 2;              
    unsigned char size : 4;                
    unsigned char seq : 4;               
    unsigned char type : 4;
    unsigned char data[15];                
    unsigned char par; 
} Package_header;

typedef enum {
    CDT = 0 // 0000
  , LST     // 0001
  , VERT    // 0010
  , LINHAT  // 0011
  , LINHAST // 0100
  , EDITT   // 0101
  , NOTSET  // 0110
  , NOTSET2 // 0111
  , ACK     // 1000
  , NACK    // 1001
  , LIF     // 1010
  , CONTLS  // 1011
  , CONTARQ // 1100
  , EOT     // 1101
  , NOTSET3 // 1110
  , ERROR   // 1111
} MSG_TYPE;

typedef enum {
    FORBIDDEN   //
  , DIRNOTFOUND //
  , ARQNOTFOUND //  
  , LINENOTFOUND//
} ERROR_ENUM;


int ConexaoRawSocket(char *device);
void send_msg(char *data, int type);
void init_socket();
Package_header * recieve_msg();

#endif