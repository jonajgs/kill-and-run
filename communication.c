#include "communication.h"

bool openCommunication(bool isServer,bool *haveKey)
{
    //return true;
    unsigned char buffer[BUFFER_LEN];
    unsigned int res,len;
    struct sockaddr_in6 addrLocal;
    fd_socket = socket(AF_INET6,SOCK_DGRAM,0);
    if(fd_socket >= 0){
        if(isServer){
            addrLen = sizeof(addrLocal);
            memset(&addrLocal, 0, addrLen);
            addrLocal.sin6_port = htons(PORT);
            addrLocal.sin6_family = AF_INET6;
            addrLocal.sin6_addr = in6addr_any;
            if(bind(fd_socket,(struct sockaddr *)&addrLocal,addrLen) >= 0){
                printf("Esperando jugador...\n");
                len = recvfrom(fd_socket,buffer,sizeof(buffer),0,(struct sockaddr *)&addrContrario,&addrLen);
                res = buffer[len-1];
                buffer[len-1]= 0;
                strcpy(userNameContrario,buffer);
                if(res == COM_PETICION_CONEXION){
                    strcpy(buffer,userNameLocal);
                    len = strlen(userNameLocal);
                     if(time(NULL)%2){
                        *haveKey = true;
                        buffer [len] = COM_RESPUESTA_CONEXION_LLAVE;
                    }else{
                        *haveKey = false;
                        buffer [len] = COM_RESPUESTA_CONEXION;
                    }
                    sendto(fd_socket,buffer,len+1,0,(struct sockaddr *)&addrContrario,addrLen);
                    printf("Iniciando juego...\n");
                }else{
                    closeCommunication();
                    printf("%d: no se recibió el mensaje esperado\n",res);
                    return false;
                }
            }
        }else{
            memset(&addrContrario,0,sizeof(addrContrario));
            addrContrario.sin6_family = AF_INET6;
            inet_pton(AF_INET6, IPV6_ADDR, &addrContrario.sin6_addr);
            addrContrario.sin6_port = htons(PORT);
            strcpy(buffer,userNameLocal);
            len = strlen(userNameLocal);
            buffer[len] = COM_PETICION_CONEXION;
            sendto(fd_socket,buffer,len+1,0,(struct sockaddr *)&addrContrario,sizeof(addrContrario));
            printf("Se envio, Esperando jugador...\n");
            len=recvfrom(fd_socket,buffer,sizeof(buffer),0,(struct sockaddr *)&addrContrario,&addrLen);
            res = buffer[len-1];
            buffer[len-1]=0;
            strcpy(userNameContrario,buffer);
            if(res == COM_RESPUESTA_CONEXION) *haveKey = true;
            else if(res == COM_RESPUESTA_CONEXION_LLAVE) *haveKey = false;
            else{
                printf("%d: no se recibió el mensaje esperado\n",res);
                closeCommunication();
                return false;
            }
            printf("Iniciando juego...\n");
        }
        fcntl (fd_socket, F_SETFL, O_NONBLOCK);
//            g = 0;
//            setsockopt (fd_socket, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &g, sizeof (g));
//            g = 64;
//            setsockopt (fd_socket, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &g, sizeof (g));
//            inet_pton (AF_INET6, IPV6_ADDR, &addrLocal.sin6_addr);
//            memcpy (&mcast_req6.ipv6mr_multiaddr, &(addrLocal.sin6_addr), sizeof (struct in6_addr));
//            mcast_req6.ipv6mr_interface = 0;
//            if (setsockopt (fd_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mcast_req6, sizeof(mcast_req6)) >= 0){
                return true;
//            }else printf("setsockopt");

    }
    closeCommunication();
    return false;

}

void closeCommunication(){
    close(fd_socket);
}

void sendMsg(int action, Jugador *j){
    static int lastAcceleration=0;
    unsigned char msg[4];
    int len;
    if(lastAcceleration != j->aceleracion){
        len = 4;
        lastAcceleration=j->aceleracion;
        msg[3]=j->aceleracion;
    }else len =3;
    msg[0]=action;
    msg[1]=j->posicion.x >> 8;
    msg[2]=j->posicion.x;
    if(sendto(fd_socket,msg,len,0,(struct sockaddr *)&addrContrario,sizeof(addrContrario)) < 0)
        printf("no se envio mensaje");
}

void recvMsg(unsigned int* action, unsigned int* posX,unsigned int* acel){
    int len;
    unsigned char buffer[BUFFER_LEN];
    len=recvfrom(fd_socket,buffer,sizeof(buffer),0,0,0);
    if(len > 0){
        *action = buffer[0];
        *posX = (buffer[1] << 8) | buffer[2];
        if(len == 4) *acel = buffer[3];
    }
    else *action = 0;
}

