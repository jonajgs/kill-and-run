#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "estructuras.c"
#include "constantes.c"

SDL_Surface *imagenes[ TOTAL_IMAGENES ];
SDL_RWops *rwimagenes[ TOTAL_IMAGENES ];
int escenario_actual;

void cargar_imagenes( SDL_Surface *images[], SDL_RWops *rwimages[] )
{
    int i;
    strcpy(routes[0], "sprites/antorcha1.png");
    strcpy(routes[1], "sprites/antorcha2.png");
    strcpy(routes[2], "sprites/antorcha3.png");
    strcpy(routes[3], "sprites/atauld1.png");
    strcpy(routes[4], "sprites/dead1.png");
    strcpy(routes[5], "sprites/dead2.png");
    strcpy(routes[6], "sprites/dead3.png");
    strcpy(routes[7], "sprites/padown_l1.png");
    strcpy(routes[8], "sprites/padown_r1.png");
    strcpy(routes[9], "sprites/pawalk_l1.png");
    strcpy(routes[10], "sprites/pawalk_l2.png");
    strcpy(routes[11], "sprites/pawalk_la.png");
    strcpy(routes[12], "sprites/pawalk_r1.png");
    strcpy(routes[13], "sprites/pawalk_r2.png");
    strcpy(routes[14], "sprites/pawalk_ra.png");
    strcpy(routes[15], "sprites/pdown_l1.png");
    strcpy(routes[16], "sprites/pdown_r1.png");
    strcpy(routes[17], "sprites/pwalk_l1.png");
    strcpy(routes[18], "sprites/pwalk_l2.png");
    strcpy(routes[19], "sprites/pwalk_r1.png");
    strcpy(routes[20], "sprites/pwalk_r2.png");
    strcpy(routes[21], "sprites/pajump_l1.png");
    strcpy(routes[22], "sprites/pajump_r1.png");
    strcpy(routes[23], "sprites/stage2.png");
    strcpy(routes[24], "sprites/stage2.png");
    strcpy(routes[25], "sprites/stage3.png");
    strcpy(routes[26], "sprites/stage4.png");
    strcpy(routes[27], "sprites/stage5.png");
    strcpy(routes[28], "sprites/window1.png");
    strcpy(routes[29], "sprites/key.png");
    for ( i=0; i<TOTAL_IMAGENES; i++ ) {
        rwimages[i] = SDL_RWFromFile( routes[i], "rb" );
        images[i] = IMG_LoadPNG_RW(rwimages[i]);
        if(!images[i]) {
            printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
        }
    }
}

void asignar_imagen(Jugador *jugador, char *sentido)
{
    if(strcmp(sentido,IZQUIERDA) != 0){
        if(jugador->posicion.x%2 == 0){
            jugador->imagen = imagenes[player_walk_knife_left2];
        } else {
            jugador->imagen = imagenes[player_walk_knife_left1];
        }
        jugador->adelante = TRUE;
    } else if(strcmp(sentido,DERECHA) != 0){
        if(jugador->posicion.x%2 == 0){
            jugador->imagen = imagenes[player_walk_knife_right2];
        } else {
            jugador->imagen = imagenes[player_walk_knife_right1];
        }
        jugador->adelante = FALSE;
    } else if(strcmp(sentido,ABAJO) != 0){
        if( jugador->adelante ){
            jugador->imagen = imagenes[player_down_knife_left];
        } else {
            jugador->imagen = imagenes[player_down_knife_right];
        }
    }
}

void saltar(Jugador* jugador)
{
    if( jugador->saltar == TRUE ){
        if( (jugador->posicion.y+jugador->salto++) <= ALTURA_MINIMA){
            jugador->posicion.y = jugador->posicion.y + (jugador->salto++);
            if(jugador->adelante){
                jugador->posicion.x += 5 + jugador->aceleracion;
            } else {
                jugador->posicion.x -= 5 + jugador->aceleracion;
            }
        } else {
            jugador->saltar = FALSE;
            jugador->salto = SALTO;
        }
    } else if( jugador->saltar == FALSE ){
        if ( jugador->posicion.y < ALTURA_MINIMA ) {
            if( jugador->adelante ) {
                jugador->posicion.x += ADELANTE;
                jugador->posicion.y += ADELANTE;
            } else {
                jugador->posicion.x -= ADELANTE;
                jugador->posicion.y += ADELANTE;
            }
        }
    } else if ( jugador->saltar == NEUTRO ) {
        if( (jugador->posicion.y+jugador->salto++) <= ALTURA_MINIMA){
            jugador->posicion.y = jugador->posicion.y + (jugador->salto++);
        } else {
            jugador->saltar = FALSE;
            jugador->salto = SALTO;
        }
    } else {
        if ( jugador->posicion.y < ALTURA_MINIMA ) {
            if( jugador->adelante ) {
                jugador->posicion.y += ADELANTE + GRAVEDAD;
            } else {
                jugador->posicion.y += ADELANTE;
            }
        } else {
            jugador->saltar = FALSE;
        }
        if ( jugador->posicion.y <= ALTURA_MINIMA ){
            if( !jugador->adelante ) {
                jugador->imagen = imagenes[player_attack_right];
            } else {
                jugador->imagen = imagenes[player_attack_left];
            }
        }

    }
}

void mover(Jugador *jugador)
{
    char sentido[20];
    Uint8 *keystate = SDL_GetKeyState(NULL); // para saber cuando se presionan 2 teclas al mismo tiempo
    if(jugador->teclado.key.keysym.sym == SDLK_RIGHT) {
        jugador->posicion.x = jugador->posicion.x + ADELANTE + (++jugador->aceleracion);
        strcpy(sentido,"derecha");
        asignar_imagen(jugador, sentido);
    } else if(jugador->teclado.key.keysym.sym == SDLK_LEFT){
        jugador->posicion.x = jugador->posicion.x - ADELANTE - (++jugador->aceleracion);
        strcpy(sentido,"izquierda");
        asignar_imagen(jugador, sentido);
    } else if(jugador->teclado.key.keysym.sym == SDLK_DOWN){
        strcpy(sentido,"abajo");
        asignar_imagen(jugador, sentido);
    } else if ( (keystate[SDLK_RIGHT] && keystate[SDLK_UP]) || (keystate[SDLK_LEFT] && keystate[SDLK_UP]) ) {
        jugador->saltar = TRUE;
    } else if(jugador->teclado.key.keysym.sym == SDLK_UP){
        jugador->saltar = NEUTRO;
        saltar(jugador);
    }

}

double timeval_diff(struct timeval *start, struct timeval *end){
  return
    (double)(end->tv_sec + (double)end->tv_usec/1000000) -
    (double)(start->tv_sec + (double)start->tv_usec/1000000);
}

int passTime( struct timeval start ) {
    struct timeval end;
    double seconds = 0;
    gettimeofday(&end, NULL);
    seconds = timeval_diff(&start, &end);
    return seconds*1000;

}

void initPlayer( Jugador *jugador, short id ){
    jugador->id = id;
    jugador->aceleracion = ACELERACION;
    jugador->saltar = FALSE;
    jugador->salto = SALTO;
    jugador->posicion.y = 330;
    if(id == 1){
        jugador->imagen = imagenes[ player_walk_knife_left1 ];
        jugador->posicion.x = 10;
        jugador->adelante = TRUE;
        jugador->key = TRUE;
    } else {
        jugador->imagen = imagenes[ player_walk_knife_right1 ];
        jugador->posicion.x = ANCHURA -100;
        jugador->adelante = FALSE;
    }
}

void initObject(Object *obj, int x, int y)
{
    obj->imagen = imagenes[key];
    obj->posicion.x = x;
    obj->posicion.y = y;
}
