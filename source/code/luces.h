//--------------------------------------------------------
// luces.h
// Definiciones para las rutinas de efectos visuales
// ILUMINACION
//--------------------------------------------------------


#ifndef LUCES__H
#define LUCES__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

// cantidad maxima de luces
#define MAX_LUZ          100

int luz_ambiental = 20; // intensidad luz ambiental 0-255 (def:16)

int detalle_luz = 8; // salto que hace para hacer la luz, cuanto >, mas rapido
                    // no pasar de 16 aprox.

// IMPLEMENTA TRES METODOS DE DIBUJO
// ESPECIFICADOS POR EL int MODO_DE_LUZ
// 0 = MODO NORMAL, PERO LENTO (LAS LUCES SE COMBINAN Y SON SUAVES)
//
// 1 = MODO INTERMEDIO, LUCES SUAVES, PERO *NO* SE COMBINAN
//
// 3 = MODO RAPIDO, LUCES ASPERAS, SE COMBINAN (INTENSIDAD FIJA 255)
int MODO_DE_LUZ = 0;


//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE ILUMINACION
static int cantidad_luz = 4; // cantidad de focos de luz actualmente (0-3)

// ******** IMPORTANTE **********
// LAS LUCES 0-3 INCLUSIVE ESTAN RESERVADAS PARA LAS 4 LUCES DE
// LOS TANQUES PARA EVITAR QUE 'PARPADEEEN'
// SE SETEAN CON LA ULTIMA RUTINA DE ABAJO CONSTANTEMENTE
// 0, 1 A JUGADOR 1
// 2, 3 A JUGADOR 2

static struct {
 int x;  // posicion x centro del foco de luz 0-1000
 int y;  // posicion y centro del foco de luz 0-1000
 int vida; // vida que le queda al foco de luz, al llegar a 0 es liberado
 int radio; // radio de la luz
 int intensidad; // intensidad  de la luz 0-255 (valor del centro del spot)
 int crecer; // aumento del radio a traves del tiempo
} foco_luz[MAX_LUZ]; // desde 0 a MAX_LUZ - 1


//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------

// resetea el sistema de iluminacion
void reset_luz();

// dibuja las luces
void dibujar_luz(BITMAP *bmp, int x, int y);

// mover
void mover_luz();

// agregar foco de luz
void agregar_luz(int x, int y, int vida,
                        int colorr, int radio, int crecer);

// crear las luces de los tanques
// llamar en cada update_game_logic()!!!;
void tanque_luz();


#endif
