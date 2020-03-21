//--------------------------------------------------------
// pc_ia.h
// HEADER PARA LAS RUTINAS DE INTELIGENCIA ARTIFICIAL
//--------------------------------------------------------

#ifndef PC_IA__H
#define PC_IA__H

//--------------------------------------------------------
// inclusiones standard
#include <stdlib.h>
// infaltable... Allegro...
#include <allegro.h>

// Prototipos
int angulo_x1_y1_x2_y2(int x1, int y1, int x2, int y2);
int dame_angulo_cuerpo_tanque(int j, int ene);
int dame_angulo_enemigo(int j, int yo);
int dame_angulo_pared(int j, int ene);
void mover_IA(int j , unsigned long deciseg);
int num_0_49(int n);
void reset_IA();

//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------

int NIVEL_IA = 2;  // nivel de dificultad 0,1,2 === DEBUG: NO IMPLEMENTADO!


#endif
