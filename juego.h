#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "communication.h"

SDL_Surface *imagenes[ TOTAL_IMAGENES ];
SDL_RWops *rwimagenes[ TOTAL_IMAGENES ];
int escenario_actual;
Object llave, antorcha1, antorcha2, muerto;
struct timeval start;
bool exitGame;

void initPlayer( Jugador*, short, boolean);
void initObject(Object *, int , int , int );
void cargar_imagenes( SDL_Surface**, SDL_RWops**);
void asignar_imagen(Jugador *, const char *);
void saltar(Jugador*);
void muere(Jugador*);
void atacar(Jugador*, Jugador*);
void mover(Jugador*,Jugador*,Jugador*);
double timeval_diff(struct timeval*, struct timeval*);
int passTime( struct timeval);
void actualizaContrario(unsigned int,unsigned int,unsigned int,Jugador* ,Jugador*);

#endif // JUEGO_H_INCLUDED
