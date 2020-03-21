//--------------------------------------------------------
// KRONOMAN'S BATTLE ZONE
//
// sonido.c - sonido.h
//
// Modulo de sonido de Kronoman's Battle Zone
// Funciona en polifonia estereo.
// Agregado el 14/Marzo/2002
//
// (c) 2002, Kronoman
//
//--------------------------------------------------------
// Algunas notas sobre el sonido:
//--------------------------------------------------------
// Cuando se juege de a dos jugadores/demo, el paneo lo determina
// la posicion en pantalla izq/centro/der
// Cuando se juege de a un solo jugador vs cpu, el volumen
// lo determina la distancia del CPU al humano,
// siendo dentro de pantalla 100% y decreciendo hacia fuera.
// El pitch del sonido del motor es relativo a la velocidad del jugador.
// Las explosiones varian el pitch al azar para darle un efecto cool.
// El clima tiene efectos propios: viento, nieve, etc..
// Los sonidos deben estar cargados en DATAFILE *krono_dato;
//--------------------------------------------------------

#ifndef SONIDO_C
#define SONIDO_C

#include "sonido.h" // cabecera necesaria...

// Esto sirve para hacer efecto estereo,
// el sonido se panea de acuerdo al ancho de pantalla.
// Cool!
#define PAN(x)       (((x) * 256) / SCREEN_W)


//--------------------------------------------------------
// Funciones
//--------------------------------------------------------

//--------------------------------------------------------
// Inicializa el modulo de sonido
// Devuelve FALSE si falla (no sonido)
//--------------------------------------------------------
int inicializar_modulo_de_sonido()
{

remove_sound(); // remover si hubiera un subsistema de sonido anterior...

// reservar canales de voz, MIDI desabilitado...
reserve_voices(SND_CANALES, 0);

if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0) {
   // manejador de sonido nulo... pero funcional!
   install_sound(DIGI_NONE, MIDI_NONE, NULL);
   return FALSE;
   }
return TRUE;
}



#endif
