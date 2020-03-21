//--------------------------------------------------------
// game.h   Contiene los includes del MOTOR del juego
//--------------------------------------------------------



//--------------------------------------------------------
// Primero me fijo si no definio esto previamente...
#ifndef GAME__H
#define GAME__H

// cantidad de milisegundos antes de un nuevo cuadro
// recomiendo 50 milisegundos...
#define ACTUALIZAR_GAME 45

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>
//--------------------------------------------------------

// Codigo auxiliar
// para chequear el poligono del jugador
#include "poligono.h"

// efectos visuales #1
 #include "efectos1.h"

// efectos de luz
 #include "luces.h"

//--------------------------------------------------------


//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------

// timer de 10 milisegundos para el juego
void game_timer();

// timer que cuenta los fps para debug
void fps_proc();

// ajusta la medicion del tiempo de juego
void ajustar_medicion_tiempo();

// instala los timers del juego
void instalar_timers();

// desinstala los timers
void desinstala_timers();

// pausa del juego
void realizar_pausa();

// motor del juego - bucle principal (MUEVE TODO!!)
void motor_juego();

// rutina que actualiza la logica del juego
void update_game_logic();

// rutina que actualiza la pantalla
void update_display();

// rutina que interpreta el teclado
void Leer_Teclado();

// rutina que mueve los tanques
void mover_tanques();

#endif 

