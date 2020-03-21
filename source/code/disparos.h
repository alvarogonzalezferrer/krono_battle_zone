//--------------------------------------------------------
// disparos.h
// HEADER PARA LAS RUTINAS QUE CONTIENEN EL ADMINISTRADOR DE DISPAROS
// PARA AMBOS JUGADORES
//--------------------------------------------------------

#ifndef DISPAROS__H
#define DISPAROS__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>


//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

// cantidad maxima de disparos a la vez
#define MAX_SHOOTS          25

// Definicion de las armas
// tipo de arma: 0=ametralladora, 1,2,3=ca¤ones, 4=lanza llama
// esto va en jugador[x].arma
#define A_AMETRALLADORA 0 // sprite BALA
#define A_CANON         1 // sprite BOLA (gira, frames = angulo!)
#define A_LASER         2 // sprite LASER
#define A_PLASMA        3 // sprite RAYO
#define A_LANZALLAMAS   4 // sprite FUEGO1, FUEGO2, FUEGO3
#define A_TRIPLE_SHOOT  5 // sprite TRIPLE_SHOOT

#define CANTIDAD_DE_ARMAS 6 // esto es para saber cuantas armas hay; 0..4 = 5

// Tama¤o de los sprites de los disparos de las armas (default: 10x10)
#define X_SPR_ARMA 10
#define Y_SPR_ARMA 10

// Cantidad maxima de balas
#define MAX_C_BALAS 99


//--------------------------------------------------------
// Prototipos de las funciones
//--------------------------------------------------------
void reset_disparo();
void agregar_disp(int x, int y, int vida, int dir,
                  int vel, int tipo, int indice);
void mover_disp();
void dibujar_disparos(BITMAP *bmp, int x, int y);
void check_balas(int j);
void combo_disparar(int jugador, unsigned long deciseg);


#endif
