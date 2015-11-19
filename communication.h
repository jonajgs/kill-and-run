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

#define IPV6_ADDR "fe80::16d6:4dff:feab:f491"  //escritorio
//#define IPV6_ADDR "fe80::8200:bff:fe87:c5df"  //Angel lap
//#define IPV6_ADDR "FF02::224:0:0:133" //multicast
//#define IPV6_ADDR "::1"
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
struct sockaddr_in6 server_addr,client_addr;
socklen_t client_len;

bool openCommunication(bool,bool*);
void closeCommunication();
void sendMsg(int, Jugador*);
void recvMsg(int*,int*,int*);

#endif // COMMUNICATION_H_INCLUDED