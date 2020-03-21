//--------------------------------------------------------
// premios.h
//
// HEADER DE RUTINAS QUE CONTIENEN EL ADMINISTRADOR DE PREMIOS 
// PARA AMBOS JUGADORES (POWER-UPS)
//
// Por Kronoman - DJGPP
//--------------------------------------------------------

#ifndef __PREMIOS__H
#define __PREMIOS__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>


//--------------------------------------------------------
#define MAX_PREMIOS  10  // premios en pantalla a la vez

#define TIPO_MAX     7    // cantidad de premios diferentes que hay 0..x-1

// sprites que representan a los premios
#define SPR_ARMA_NUEVA  ARMA_NUEVA // cambio de arma
#define SPR_MEDIKIT     MEDIKIT    // botiquin
#define SPR_RADAR       RADAR      // radar
#define SPR_INVISIBLE   INVISIBLE_PREMIO // invisibilidad (transparencia)
#define SPR_ESCUDO      ESCUDO_PREMIO   // escudo de energia [reduce los disparos]
#define SPR_LUZ         LUZ_SPR         // mas luz
#define SPR_VELOCIDAD   VELOCIDAD_SPR  // mas velocidad

//--------------------------------------------------------
// GLOBALES
//--------------------------------------------------------

// SISTEMA DE PREMIOS
// Estructura - matriz conteniendo los power-ups
struct {
 int   x;  // posicion x 
 int   y;  // posicion y 

 int vida; // vida del premio, se descuenta con cada
           // llamada del timer, al llegar a 0 es renovado en otra posicicon

 int tipo; // tipo del premio

} premio[MAX_PREMIOS]; // desde 0 a MAX_PREMIOS - 1 




//--------------------------------------------------------
// Prototipos de las funciones
//--------------------------------------------------------
void poner_premio(int i1);
void reset_premios();
void mover_premios();
void dibujar_premios(BITMAP *bmp, int x, int y);



#endif
