#include <stdio.h>
#include <stdlib.h>
#include "juego.c"


int main()
{
    SDL_Surface *ventana;
    int init, exit = 0, time;
    Object llave, antorcha1, antorcha2, muerto;

    init = SDL_Init( SDL_INIT_VIDEO );
    if( init >= 0 ) {
        escenario_actual = stage3;
        cargar_imagenes( imagenes, rwimagenes );
        SDL_EnableKeyRepeat(100,100);
        initObject(&antorcha1, 100, 40, lamp1);
        initObject(&antorcha2, 1120, 40, lamp1);
        initObject(&muerto, 300, 10, dead1);
        initObject(&llave, 0, 0, key);
        ventana = SDL_SetVideoMode(ANCHURA, ALTURA, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if( ventana ) {
            initPlayer( &jugador1, 1 );
            initPlayer( &jugador2, 2 );
            gettimeofday(&start, NULL);
            while ( !exit ) {
                SDL_BlitSurface( imagenes[escenario_actual], NULL, ventana, NULL );
                if(!jugador1.muerto || passTime(start) > 3000){
                    SDL_BlitSurface( jugador1.imagen, NULL, ventana, &jugador1.posicion );
                    jugador1.muerto = FALSE;
                }
                if(!jugador2.muerto || passTime(start) > 3000){
                    SDL_BlitSurface( jugador2.imagen, NULL, ventana, &jugador2.posicion );
                    jugador2.muerto = FALSE;
                }
                SDL_BlitSurface( llave.imagen, NULL, ventana, &llave.posicion );
                if( escenario_actual != stage5 && escenario_actual != stage1){
                    SDL_BlitSurface( antorcha1.imagen, NULL, ventana, &antorcha1.posicion);
                    SDL_BlitSurface( antorcha2.imagen, NULL, ventana, &antorcha2.posicion);
                }
                if ( escenario_actual == stage2 || escenario_actual == stage4 ) {
                    SDL_BlitSurface( muerto.imagen, NULL, ventana, &muerto.posicion);
                }
                SDL_Flip( ventana );
                if( jugador2.saltar == FALSE && jugador2.posicion.y == ALTURA_MINIMA ){
                    while ( SDL_PollEvent( &jugador2.teclado )) {
                        switch ( jugador2.teclado.type ) {
                            case SDL_KEYDOWN:
                                mover( &jugador2 );
                                break;
                            case SDL_QUIT:
                                exit = true;
                                break;
                            case SDL_KEYUP:
                                if ( jugador2.teclado.key.keysym.sym == SDLK_s || jugador2.teclado.key.keysym.sym == SDLK_z) {
                                    if( jugador2.adelante ) {
                                        jugador2.imagen = imagenes[player_walk_knife_left1];
                                    } else {
                                        jugador2.imagen = imagenes[player_walk_knife_right1];
                                    }
                                }
                                jugador2.aceleracion = ACELERACION;

                                break;
                        }
                    }

                } else if(jugador2.saltar == TRUE){
                    saltar(&jugador2);
                }
                if( jugador1.saltar == FALSE && jugador1.posicion.y == ALTURA_MINIMA ){
                    while ( SDL_PollEvent( &jugador1.teclado )) {
                        switch ( jugador1.teclado.type ) {
                            case SDL_KEYDOWN:
                                mover( &jugador1 );
                                break;
                            case SDL_QUIT:
                                exit = true;
                                break;
                            case SDL_KEYUP:
                                if ( jugador1.teclado.key.keysym.sym == SDLK_DOWN || jugador1.teclado.key.keysym.sym == SDLK_SPACE ) {
                                    if( jugador1.adelante ) {
                                        jugador1.imagen = imagenes[player_walk_knife_left1];
                                    } else {
                                        jugador1.imagen = imagenes[player_walk_knife_right1];
                                    }
                                } else if(jugador1.teclado.key.keysym.sym == SDLK_RIGHT || jugador1.teclado.key.keysym.sym == SDLK_LEFT){
                                    jugador1.aceleracion = ACELERACION;
                                }
                                if ( jugador2.teclado.key.keysym.sym == SDLK_d || jugador2.teclado.key.keysym.sym == SDLK_z ) {
                                    if( jugador2.adelante ) {
                                        jugador2.imagen = imagenes[player_walk_knife_left1];
                                    } else {
                                        jugador2.imagen = imagenes[player_walk_knife_right1];
                                    }
                                } else if(jugador2.teclado.key.keysym.sym == SDLK_d || jugador2.teclado.key.keysym.sym == SDLK_a){
                                    jugador2.aceleracion = ACELERACION;
                                }
                                break;
                        }
                    }

                } else if(jugador1.saltar == TRUE) {
                    saltar(&jugador1);
                }

                if( (time = passTime(start)) <= 700 ) {
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
                } else {
                    gettimeofday( &start, NULL );
                }


            } // endwhile
        } else {
            printf("No se pudo setear el modo video\n");
        }
    } else {
        printf("No se pudo inicializar el modo video\n");
    }
    printf("Exit success\n");
    SDL_FreeSurface( ventana );
    SDL_Quit();

    return EXIT_SUCCESS;
}
