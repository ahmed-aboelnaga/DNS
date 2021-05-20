#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// Auteur :  Aboelnaga Ahmed

#define IP4 1
#define IP6 28
#define N_S 2
#define C_N 5
#define SOA 6

#define offset(B1,B2) ((B2 | (B1 << 8)) &16383)

#define QUERY 0
#define RESPONSE 1


#define MAX_QUERY_SIZE 2048

#define DNS_PORT 53

#define DNS_SERVER "9.9.9.9"


#define check(data,tobe,dis) do{if(data==tobe) {perror(dis);exit(EXIT_FAILURE);}}while(0);

// header common
 typedef struct DNS_header_s{
    unsigned short          ID; // 16 bits field  randomly generated
    unsigned char           RD:1; // 1 bit field
    unsigned char           TC:1; // 1 bit field
    unsigned char           AA:1; // 1 bit field
    unsigned char           OPCODE:4; // 4 bit field
    unsigned char           QR:1; // 1 bit field
    unsigned char           RCODE:4; // 4 bit field
    unsigned char           Z:3; // 3 bit field
    unsigned char           RA:1; // 1 bit field
    unsigned short          QDCOUNT;
    unsigned short          ANSCOUNT;
    unsigned short          AUTH_RRS;
    unsigned short          ADD_RRS;
    
   
 
}DNS_h;

typedef struct DNS_Question_s{ 

    unsigned short         QTYPE;
    unsigned short        QCLASS;


}DNS_q;

 // Answer

typedef struct DNS_ANS_s{

    unsigned short      Type;
    unsigned short      Class;
    unsigned short        Ttl;
    unsigned short        Ttl2;
    unsigned short      DA_L;
}DNS_ANS;
