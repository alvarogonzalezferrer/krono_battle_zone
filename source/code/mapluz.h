//--------------------------------------------------------
// mapluz.h
//
// Generador de mapa de adicion de colores
// y mapa de adicion de tonalidad gris.-
//
// Por Kronoman
//
// IMPRESCINDIBLE PARA HACER LUCES SUAVES QUE SE PUEDAN
// INTERSECTAR ENTRE SI Y EFECTOS DE LENS FLARE, ETC!
//--------------------------------------------------------
#ifndef MAPLUZ_H
#define MAPLUZ_H

// prototipos
void crear_mapa_aditivo_luces();
RGB crear_mapa_aditivo(PALETTE pal, int x, int y);
void crear256grises(PALLETE tmpal);


#endif
