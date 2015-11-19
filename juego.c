#include "juego.h"


void initPlayer( Jugador *jugador, short id, boolean key ){
    jugador->id = id;
    jugador->aceleracion = ACELERACION;
    jugador->saltar = FALSE;
    jugador->salto = SALTO;
    jugador->posicion.y = 330;
    if(id == 1){
        jugador->imagen = imagenes[ player_walk_knife_left1 ];
        jugador->posicion.x = 40;
        jugador->adelante = TRUE;
    } else {
        jugador->imagen = imagenes[ player_walk_knife_right1 ];
        jugador->posicion.x = ANCHURA -100;
        jugador->adelante = TRUE;
    }
    if(key){
        jugador->key = TRUE;
    } else {
        jugador->key = FALSE;
    }
}

void initObject(Object *obj, int x, int y, int image)
{
    obj->imagen = imagenes[image];
    obj->posicion.x = x;
    obj->posicion.y = y;
}

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
    strcpy(routes[30], "sprites/stagefinal.png");
    for ( i=0; i<TOTAL_IMAGENES; i++ ) {
        rwimages[i] = SDL_RWFromFile( routes[i], "rb" );
        images[i] = IMG_LoadPNG_RW(rwimages[i]);
        if(!images[i]) {
            printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
        }
    }
}

void asignar_imagen(Jugador *jugador, const char *sentido)
{
    if(strcmp(sentido,DERECHA) == 0){
        if(jugador->posicion.x%2 == 0){
            jugador->imagen = imagenes[player_walk_knife_left2];
        } else {
            jugador->imagen = imagenes[player_walk_knife_left1];
        }
        jugador->adelante = TRUE;
    } else if(strcmp(sentido,IZQUIERDA) == 0){
        if(jugador->posicion.x%2 == 0){
            jugador->imagen = imagenes[player_walk_knife_right2];
        } else {
            jugador->imagen = imagenes[player_walk_knife_right1];
        }
        jugador->adelante = FALSE;
    } else if(strcmp(sentido,ABAJO) == 0){
        if( jugador->adelante ){
            jugador->imagen = imagenes[player_down_knife_left];
        } else {
            jugador->imagen = imagenes[player_down_knife_right];
        }
    } else if(strcmp(sentido,ESPACIO) == 0){
        if( jugador->adelante ){
            jugador->imagen = imagenes[player_attack_left];
        } else {
            jugador->imagen = imagenes[player_attack_right];
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

void muere(Jugador *jugador){
    jugador->muerto = TRUE;
    initPlayer(jugador, jugador->id, FALSE);
    if(jugador->id == 1){
        llave.posicion.x = ANCHURA-100;
    } else {
        llave.posicion.x = 0;
    }
}

void atacar(Jugador *a, Jugador *b){
    asignar_imagen(a,ESPACIO);
    if(a->posicion.x >= b->posicion.x-40 && a->posicion.x <= b->posicion.x+60 && a->posicion.y == b->posicion.y){
        muere(b);
        a->key = TRUE;
        b->key = FALSE;
        gettimeofday(&start, NULL);
    }
}

void mover(Jugador *jugador1,Jugador *jugador2,Jugador *jugadorLocal)
{
    Uint8 *keystate = SDL_GetKeyState(NULL); // para saber cuando se presionan 2 teclas al mismo tiempo
    if ( jugador1->posicion.x >= ANCHURA-20  && jugador1->key == TRUE ) {
        if ( escenario_actual == stage1 ) {
            escenario_actual = stage2;
        } else if ( escenario_actual == stage2 ) {
            escenario_actual = stage3;
        } else if( escenario_actual == stage3 ) {
            escenario_actual = stage4;
        } else if( escenario_actual == stage4 ) {
            escenario_actual = stage5;
        } else if( escenario_actual == stage5 ) {
            escenario_actual = stageFinal;
        } else {
            jugador1->ganador = TRUE;
        }
        initPlayer(jugador2, jugador2->id, jugador2->key);
        initPlayer(jugador1, jugador1->id, jugador1->key);
        jugador1->muerto = FALSE;
        jugador2->muerto = FALSE;
    } else if(jugador1->posicion.x <= 10 && jugador1->key == TRUE ) {
        if(escenario_actual == stage1){
            escenario_actual = stageFinal;
        } else if(escenario_actual == stage2){
            escenario_actual = stage1;
        } else if(escenario_actual == stage3){
            escenario_actual = stage2;
        } else if(escenario_actual == stage4){
            escenario_actual = stage3;
        } else if(escenario_actual == stage5){
            escenario_actual = stage4;
        }
        initPlayer(jugador2, jugador2->id, jugador2->key);
        initPlayer(jugador1, jugador1->id, jugador1->key);
        jugador1->muerto = FALSE;
        jugador2->muerto = FALSE;
    }

    if(jugador1->teclado.key.keysym.sym == SDLK_RIGHT) {
        if( jugador1->posicion.x <= jugador2->posicion.x-25 || jugador1->posicion.x > jugador2->posicion.x+25 || jugador2->muerto ){
            if(jugador1->key || jugador1->posicion.x < ANCHURA-70){
                jugador1->posicion.x = jugador1->posicion.x + ADELANTE + (++jugador1->aceleracion);
                asignar_imagen(jugador1, DERECHA);
                if(jugador1 == jugadorLocal){//solo el jugador local manda mensajes, el contrario recibe
                    sendMsg(COM_MOVIMIENTO,jugador1);
                }
            }
        }
    } else if(jugador1->teclado.key.keysym.sym == SDLK_LEFT){
        if( jugador1->posicion.x >= jugador2->posicion.x+60 || jugador1->posicion.x <= jugador2->posicion.x|| jugador2->muerto ){
            jugador1->posicion.x = jugador1->posicion.x - ADELANTE - (++jugador1->aceleracion);
            asignar_imagen(jugador1,IZQUIERDA);
            if(jugador1 == jugadorLocal){//solo el jugador local manda mensajes, el contrario recibe
                sendMsg(COM_MOVIMIENTO,jugador1);
            }
        }
    } else if(jugador1->teclado.key.keysym.sym == SDLK_DOWN){
        asignar_imagen(jugador1,ABAJO);
        if(jugador1 == jugadorLocal){//solo el jugador local manda mensajes, el contrario recibe
            sendMsg(COM_AGACHARSE,jugador1);
        }
    } else if ( (keystate[SDLK_RIGHT] && keystate[SDLK_UP]) || (keystate[SDLK_LEFT] && keystate[SDLK_UP])) {
        //jugador1->saltar = TRUE;
    } else if(jugador1->teclado.key.keysym.sym == SDLK_UP){
        jugador1->saltar = NEUTRO;
        saltar(jugador1);
        if(jugador1 == jugadorLocal){//solo el jugador local manda mensajes, el contrario recibe
            sendMsg(COM_SALTO,jugador1);
        }
    } else if( jugador1->teclado.key.keysym.sym == SDLK_SPACE ){
        atacar(jugador1, jugador2);
        if(jugador1 == jugadorLocal){//solo el jugador local manda mensajes, el contrario recibe
            sendMsg(COM_ATAQUE,jugador1);
        }
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

void actualizaContrario(unsigned int action,unsigned int posXContrario,unsigned int acelContrario,
Jugador* jugadorLocal, Jugador* jugadorContrario){
    if(action != 0){
        switch(action){
        case COM_FIN_DE_JUEGO:
            //mostrar que se terminó el juego
            exitGame = true;
            break;
        case COM_MOVIMIENTO:
            if(posXContrario < jugadorContrario->posicion.x){
                //es menor, se movio a la izquierda
                jugadorContrario->teclado.key.keysym.sym = SDLK_LEFT;
            }else if(posXContrario > jugadorContrario->posicion.x){
                jugadorContrario->teclado.key.keysym.sym = SDLK_RIGHT;
            }
            break;
        case COM_AGACHARSE:
            jugadorContrario->teclado.key.keysym.sym = SDLK_DOWN;
            break;
        case COM_ATAQUE:
            jugadorContrario->teclado.key.keysym.sym = SDLK_SPACE;
            break;
        case COM_SALTO:
            jugadorContrario->teclado.key.keysym.sym = SDLK_UP;
            break;
        case COM_LLAVE:
            break;
        case COM_SALTO_Y_MOVIMIENTO:
            //jugadorContrario->saltar = TRUE;
            break;
        case COM_MOVIMIENTO_SALTO_Y_ATAQUE:
            break;
        default:;
        }
        mover(jugadorContrario,jugadorLocal,jugadorLocal);
        //al ser el mismo programa se supone que no debería cambiar, pero en caso de perder paquetes debemos sincronizar
        if(jugadorContrario->aceleracion != acelContrario) jugadorContrario->aceleracion = acelContrario;
        if(jugadorContrario->posicion.x != posXContrario) jugadorContrario->posicion.x = posXContrario;
    }else{
        if(jugadorContrario->teclado.key.keysym.sym == SDLK_UP && jugadorContrario->posicion.y < ALTURA_MINIMA){
            mover(jugadorContrario,jugadorLocal,jugadorLocal);
        }
        else{
            jugadorContrario->teclado.key.keysym.sym = SDLK_RIGHT;
        }
    }
    saltar(jugadorContrario);
}
