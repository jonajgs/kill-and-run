#include <stdio.h>
#include <stdlib.h>
#include "juego.c"


int main()
{
    SDL_Surface *ventana;
    int init, exit = 0;
    struct timeval start;
    Jugador jugador1, jugador2;
    Object llave;

    init = SDL_Init( SDL_INIT_VIDEO );
    if( init >= 0 ) {
        escenario_actual = stage3;
        cargar_imagenes( imagenes, rwimagenes );
        SDL_EnableKeyRepeat(100,100);
        initObject(&llave, 10, 10);
        ventana = SDL_SetVideoMode(ANCHURA, ALTURA, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if( ventana ) {
            initPlayer( &jugador1, 1 );
            initPlayer( &jugador2, 2 );
            gettimeofday(&start, NULL);
            while ( !exit ) {
                SDL_BlitSurface( imagenes[escenario_actual], NULL, ventana, NULL );
                SDL_BlitSurface( jugador1.imagen, NULL, ventana, &jugador1.posicion );
                SDL_BlitSurface( jugador2.imagen, NULL, ventana, &jugador2.posicion );
                SDL_BlitSurface( llave.imagen, NULL, ventana, &llave.posicion );
                // if( stageAvailable != stage5 && stageAvailable != stage1){
                //     SDL_BlitSurface( antorcha, NULL, window, &lamp_1);
                //     SDL_BlitSurface( antorcha, NULL, window, &lamp_2);
                // }
                // if ( stageAvailable == stage2 || stageAvailable == stage4 ) {
                //     SDL_BlitSurface( muerto, NULL, window, &dead_1);
                // }
                SDL_Flip( ventana );
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
                                break;
                        }
                    }
                } else {
                    saltar(&jugador1);
                }
                // if( (time = passTime(start)) <= 700 ) {
                //     if( time < 200 ) {
                //         antorcha = images[lamp1];
                //         muerto = images[ dead3 ];
                //     } else if( time < 500 ) {
                //         antorcha = images[lamp2];
                //         muerto = images[ dead1 ];
                //     } else {
                //         antorcha = images[lamp3];
                //         muerto = images[ dead2 ];
                //     }
                // } else {
                //     gettimeofday( &start, NULL );
                // }
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
