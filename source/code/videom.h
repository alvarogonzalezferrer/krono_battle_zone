//--------------------------------------------------------
// videom.h
// Contiene las definiciones, variables globales, etc
// para el modo de video a utilizar durante el juego.
//
// ADEMAS, CONTIENE LAS VARIABLES PARA EL NIVEL DE DETALLE DEL JUEGO
// O SEA, QUE EFECTOS VISUALES ESTAN ON/OFF
//
// Por Kronoman - 2001
//--------------------------------------------------------


// Primero me fijo si no definio esto previamente...
#ifndef VIDEOM__H

#define VIDEOM__H

//--------------------------------------------------------
// Modo de video:
//--------------------------------------------------------
// Definicion de si debo usar pantalla partida (si juega de a 2) 320x480
// o pantalla simple (si juega de a 1) 320x240
int PANT_SPLIT = TRUE; // default:TRUE jugar de a 2: 320x480

// modos de videos soportados
// GFX_VGA: 320x200
// GFX_MODEX: 320x240; 320x480
// GFX_VESA1: 640x480

// modo de video igualado a modo de video de allegro
int MODO_VIDEO = GFX_AUTODETECT; // DEFAULT: AUTO-DETECTAR
// resolucion deseada horizontal (X) y vertical (Y)
// QUE SEA UNA VALIDA POR FAVOR
int H_RES = 320; 
int V_RES = 200; // esto se divide por 2 al usar PANT_SPLIT (automatico)


//--------------------------------------------------------
// Detalles del juego
//--------------------------------------------------------

// mostrar FPS? - Se cambia con F12 durante el juego...

int MOSTRAR_FPS = FALSE;

// mostrar efectos de iluminacion? (afecta a la luz ambiental tambien)
int VER_LUCES = TRUE; // FALSE 

// mostrar efectos de particulas?
int VER_PARTI = TRUE; // FALSE 

// mostrar explosiones?
int VER_EXPLO = TRUE; // FALSE 

// mostrar clima?
int VER_CLIMA = TRUE; // FALSE 


#endif
