#include <stdio.h>
#include <stdlib.h>
#include "juego.h"


int main() {
    Jugador jugador1, jugador2,*jugadorLocal,*jugadorContrario;
    SDL_Surface *ventana;
    int time;
    unsigned int actionContrario,posXContrario,acelContrario;
    struct timeval time_out;
    bool haveKey,isServer = true;
    printf("Nombre de usuario (Max %d caracteres): ",MAX_CARACTERES_USER_NAME-1);
    fgets(userNameLocal,MAX_CARACTERES_USER_NAME,stdin);
    if(openCommunication(isServer,&haveKey)) {
        jugador1.avance=0;
        jugador2.avance=0;
        if( SDL_Init( SDL_INIT_VIDEO ) >= 0 ) {
            escenario_actual = stageFinal;
            cargar_imagenes( imagenes, rwimagenes );
            SDL_EnableKeyRepeat(100,100);
            initObject(&antorcha1, 100, 40, lamp1);
            initObject(&antorcha2, 1120, 40, lamp1);
            initObject(&muerto, 300, 10, dead1);
            ventana = SDL_SetVideoMode(ANCHURA, ALTURA, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
            if( ventana ) {
                initPlayer( &jugador1, 1, FALSE );
                initPlayer( &jugador2, 2, TRUE );
                if(isServer) {
                    jugadorLocal = &jugador1;
                    jugadorContrario = &jugador2;
                } else {
                    jugadorLocal = &jugador2;
                    jugadorContrario = &jugador1;
                }
                gettimeofday(&start, NULL);
                gettimeofday(&time_out, NULL);
                if(jugador1.key)    initObject(&llave, 0, 0, key);
                else                initObject(&llave, ANCHURA-100, 0, key);
                while ( !exitGame ) {
                    recvMsg(&actionContrario,&posXContrario,&acelContrario);
                    actualizaContrario(actionContrario,posXContrario,acelContrario,jugadorLocal,jugadorContrario);
                    SDL_BlitSurface( imagenes[escenario_actual], NULL, ventana, NULL );
                    if(!jugadorLocal->muerto || passTime(start) > 3000) {
                        SDL_BlitSurface( jugadorLocal->imagen, NULL, ventana, &jugadorLocal->posicion );
                        jugadorLocal->muerto = FALSE;
                    }
                    if(!jugadorContrario->muerto || passTime(start) > 3000) {
                        SDL_BlitSurface( jugadorContrario->imagen, NULL, ventana, &jugadorContrario->posicion );
                        jugadorContrario->muerto = FALSE;
                    }
                    SDL_BlitSurface( llave.imagen, NULL, ventana, &llave.posicion );
                    if( escenario_actual != stage5 && escenario_actual != stage1 && escenario_actual != stageFinal) {
                        SDL_BlitSurface( antorcha1.imagen, NULL, ventana, &antorcha1.posicion);
                        SDL_BlitSurface( antorcha2.imagen, NULL, ventana, &antorcha2.posicion);
                    }
                    if ( escenario_actual == stage2 || escenario_actual == stage4 )
                        SDL_BlitSurface( muerto.imagen, NULL, ventana, &muerto.posicion);
                    SDL_Flip( ventana );
                    if( jugadorLocal->saltar == FALSE && jugadorLocal->posicion.y == ALTURA_MINIMA )
                        while ( SDL_PollEvent( &jugadorLocal->teclado )) {
                            recvMsg(&actionContrario,&posXContrario,&acelContrario);
                            actualizaContrario(actionContrario,posXContrario,acelContrario,
                                               jugadorLocal,jugadorContrario);
                            switch ( jugadorLocal->teclado.type ) {
                            case SDL_KEYDOWN:
                                mover( jugadorLocal ,jugadorContrario,jugadorLocal);
                                break;
                            case SDL_QUIT:
                                exitGame = true;
                                sendMsg(COM_FIN_DE_JUEGO,jugadorLocal);
                                break;
                            case SDL_KEYUP:
                                //si el jugador local deja de agacharse o te atacar, regresar a su posicion original
                                if ( jugadorLocal->teclado.key.keysym.sym == SDLK_DOWN ||
                                        jugadorLocal->teclado.key.keysym.sym == SDLK_SPACE )
                                    if( jugadorLocal->adelante )
                                        jugadorLocal->imagen = imagenes[player_walk_knife_left1];
                                    else jugadorLocal->imagen = imagenes[player_walk_knife_right1];
                                //si deja de avanzar, regresar a aceleracion inicial
                                else if(jugadorLocal->teclado.key.keysym.sym == SDLK_RIGHT ||
                                          jugadorLocal->teclado.key.keysym.sym == SDLK_LEFT)
                                    jugadorLocal->aceleracion = ACELERACION;
                                break;
                            }
                        }
                    else if(jugadorLocal->saltar)
                        saltar(jugadorLocal);
                    if(jugadorContrario->saltar)
                        saltar(jugadorContrario);
                    //si el jugador contrario se agachó o tiro con las espada, necesita regresar a su
                    //posicion original
                    if ( jugadorContrario->teclado.key.keysym.sym == SDLK_DOWN ||
                            jugadorContrario->teclado.key.keysym.sym == SDLK_SPACE ){
                        if( jugadorContrario->adelante )
                            jugadorContrario->imagen = imagenes[player_walk_knife_left1];
                        else jugadorContrario->imagen = imagenes[player_walk_knife_right1];
                    }
                    if( (time = passTime(time_out)) <= 700 )
                        if( time < 200 ) {
                            antorcha1.imagen = imagenes[lamp2];
                            antorcha2.imagen = imagenes[lamp3];
                            muerto.imagen = imagenes[ dead3 ];
                        } else if( time < 500 ) {
                            antorcha1.imagen = imagenes[lamp3];
                            antorcha2.imagen = imagenes[lamp2];
                            muerto.imagen = imagenes[ dead2 ];
                        } else {
                            antorcha1.imagen = imagenes[lamp3];
                            antorcha2.imagen = imagenes[lamp3];
                            muerto.imagen = imagenes[ dead2 ];
                        }
                     else gettimeofday( &time_out, NULL );
                } // endwhile
                if(jugadorLocal->ganador == TRUE)
                    printf("FELICIDADES!! GANASTE!");
                else if(jugadorContrario->ganador == TRUE)
                    printf("BUUUUU!! PERDISTE!");
                else printf("SE PERDIÓ LA CONEXIÓN!!");
            } else printf("No se pudo setear el modo video\n");
        } else printf("No se pudo inicializar el modo video\n");
        closeCommunication();
    } else printf("No se pudo comunicar con el otro jugador\n");
    SDL_FreeSurface( ventana );
    SDL_Quit();
    printf("jugador local ganador: %s",(jugadorLocal->ganador)?"true":"false");
    return EXIT_SUCCESS;
}
