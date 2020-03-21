//--------------------------------------------------------
// explos.h
// Definiciones para las rutinas de FUEGO/EXPLOSIONES
//--------------------------------------------------------


#ifndef EXPLOS__H
#define EXPLOS__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>

//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------

void reset_explo();

void dibujar_explo(BITMAP *bmp, int x, int y);

void mover_explo();

void agregar_expl(int x, int y, int vida, int radio, int vel);

void combo_expl(int x, int y, int vida, int radio, int vel);

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

// cantidad maxima de explosiones
#define MAX_EXPLOSION          100

#endif
