//--------------------------------------------------------
// clima.h
// Definiciones para las rutinas de CLIMA
//--------------------------------------------------------


#ifndef CLIMA__H
#define CLIMA__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>

//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------

void crear_clima();

void dibujar_clima(BITMAP *bmp, int x, int y, int j);

void crear_clima_part(int i1);

void mover_clima();

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

// cantidad maxima de particulas de clima
#define MAX_CLIMA          200

#endif
