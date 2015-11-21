#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL.h>

char routes[ TOTAL_IMAGENES ][ TOTAL_IMAGENES ];

typedef enum{
    FALSE,
    TRUE,
    NEUTRO,
    Null
} boolean;

typedef struct{
    unsigned short id;
    unsigned short aceleracion;
    unsigned int salto;
    SDL_Rect posicion;
    SDL_Event teclado;
    SDL_Surface *imagen;
    boolean adelante;
    boolean saltar;
    boolean key;
    boolean muerto;
    boolean ganador;
}Jugador;

typedef struct{
    SDL_Surface *imagen;
    SDL_Rect posicion;
} Object;

enum {
    lamp1,
    lamp2,
    lamp3,
    coffin1,
    dead1,
    dead2,
    dead3,
    player_down_knife_left,
    player_down_knife_right,
    player_walk_knife_left1,
    player_walk_knife_left2,
    player_attack_left,
    player_walk_knife_right1,
    player_walk_knife_right2,
    player_attack_right,
    player_down_left,
    player_down_right,
    player_walk_left1,
    player_walk_left2,
    player_walk_right1,
    player_walk_right2,
    player_jump_left,
    player_jump_right,
    stage1,
    stage2,
    stage3,
    stage4,
    stage5,
    window,
    key, stageFinal
};
