// Archivo include para principa.c
// Kodigo principal del juego
// Inicializa todo el juego
// Por Kronoman
// 3 de Enero de 2001 - 3:30am
//
// DJGPP + Allegro

// ------------------
// Variables globales inherentes a *todo* el programa


// Primero me fijo si no definio esto previamente...
#ifndef PRINCIPA__H
#define PRINCIPA__H

// Modos de video
#include "videom.h"

//--------------------------------------------------------
// Prototipos de funciones
//--------------------------------------------------------

        // main; la principal
        int main(int argc, char *argv[]);

        // menu principal del juego (JUGAR-OPCIONES-SALIR)
        void menu_principal();

        // rutina que muestra mi foto (los creditos)
        void mostrar_mi_foto();


int K_DEBUG = FALSE; // mostrar info de debug?

// Tablas pre-calculadas para efectos visuales

// RGB -> color mapping RGB. No es necesaria, pero acelera todo mucho mas
//  la primera es para el juego; la 2nda para el menu
// la 3ra es para el fundido en la iluminacion
  RGB_MAP rgb_table;
  RGB_MAP rgb_table_menu;
//   RGB_MAP rgb_256gris; // NO FUNCIONA PARA LO QUE QUIERO...

// tabla de luces
// la primera es para el juego;
// la segunda es para el menu...
 COLOR_MAP light_table;
 COLOR_MAP menu_l_table; // por ahora, la uso como transparencia, queda mejor

// 3ra, tabla aditiva para el fundido de luces
// COLOR_MAP aditivo_table;  // NO FUNCIONA PARA LO QUE QUIERO...

// 4ta tabla aditiva de grises para el fundido de luces
COLOR_MAP greyscale_add_map;

// tabla de transparencia (solo para el juego) 50%
 COLOR_MAP trans_table;

// tabla de transparencia para los tanques (invisibilidad) 90%
 COLOR_MAP tanque_invisible_table;

//--------------------------------------------------------
// Variables globales
//--------------------------------------------------------

// Puntero al archivo de datos: KRONOBZ.DAT
// donde tengo almacenado los graficos y sonido, etc del juego
DATAFILE *krono_dato;

// bufer util para el nombre de archivo
 char buf_arch[512];

// paleta del juego (256 colores)
 PALLETE pal_game;

// paleta del menu
 PALLETE pal_menu;

// paleta de usos varios [en el fundido de la luz, p/ej]
 PALLETE pal_tmp;

//font standard
 FONT *font_original;

//font de usos varios
// FONT *font_tmp;

// bitmap de 1000x1000 que contiene el mapa del juego
// iniciar con create_bitmap cuando sea necesario
BITMAP *Areajuego_bmp; 

// archivo y path del archivo de configuracion del juego
char cfg_arch[256]; 


#endif
