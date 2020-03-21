// -------------------------------------------------------- 
// sonido.h 
// -------------------------------------------------------- 
// Copyright (c) Kronoman 
// En memoria de mi querido padre 
// -------------------------------------------------------- 
// Modulo de sonido de Kronoman's Battle Zone
// -------------------------------------------------------- 

#ifndef SONIDO_H
#define SONIDO_H


//--------------------------------------------------------
// Definiciones
//--------------------------------------------------------


// Constantes de sonido
#define SND_CANALES 8   // cantidad de canales de sonido que quiero (8 = OK)

// Quiero sonido?
int SND_DESEO_SONIDO = TRUE; // SI ES FALSE DEL PRINCIPIO, NO SE INICIA SONIDO!


//--------------------------------------------------------
// Prototipos
//--------------------------------------------------------
int inicializar_modulo_de_sonido();
#endif
