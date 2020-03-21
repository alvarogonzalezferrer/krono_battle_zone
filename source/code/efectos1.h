//--------------------------------------------------------
// efectos1.h
// Definiciones para las rutinas de efectos visuales simples
// Huellas del tanque, particulas, etc
//--------------------------------------------------------


#ifndef EFECTOS1__H
#define EFECTOS1__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>

//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------

// dejar huella del tanque
void dejar_huella_t(int numjug);

// destruir una pared
void destruir_pared(BITMAP *bmp, int x, int y);

// quemar pared
void quemar_pared(BITMAP *bmp, int x, int y, int fuerza, int tipo_efecto);

// resetear particulas
void reset_particulas();

// dibujar las particulas
void dibujar_particulas(BITMAP *bmp, int x, int y);

// mover particulas
void mover_particulas();

// agregar particulas
void agregar_particulas(int x, int y, int vida,
                        int cantidad, int tipo,
                        int colorr1, int colorr2);

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

// cantidad maxima de particulas
#define MAX_PARTICULAS          300

// constantes para el tipo de pulverizacion de la pared
#define BALAZO     1
#define PULVERIZAR 2



#endif
