#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "estructuras.h"

//#define IPV6_ADDR "fe80::16d6:4dff:feab:f491"  //Angel escritorio
//#define IPV6_ADDR "fe80::ea9a:8fff:fe21:ec74"   //Jonathan lap
//#define IPV6_ADDR "fe80::8200:bff:fe87:c5df"  //Angel lap
#define IPV6_ADDR "fe80::9eb7:dff:fe77:4e3c"  //Juan lap

#define PORT 1993
#define BUFFER_LEN 256

//constantes de acciones del jugador
#define COM_PETICION_CONEXION 128
#define COM_RESPUESTA_CONEXION_LLAVE 96
#define COM_RESPUESTA_CONEXION 64
#define COM_FIN_DE_JUEGO 1
#define COM_MOVIMIENTO 2
#define COM_AGACHARSE 4
#define COM_ATAQUE 8
#define COM_SALTO 16
#define COM_LLAVE 32
#define COM_SALTO_Y_MOVIMIENTO 18
#define COM_MOVIMIENTO_SALTO_Y_ATAQUE 26

typedef enum{false,true}bool;


int fd_socket;
struct sockaddr_in6 addrContrario;
socklen_t addrLen;

bool openCommunication(bool,bool*);
void closeCommunication();
void sendMsg(int, Jugador*);
void recvMsg(unsigned int*,unsigned int*,unsigned int*);

#endif // COMMUNICATION_H_INCLUDED
