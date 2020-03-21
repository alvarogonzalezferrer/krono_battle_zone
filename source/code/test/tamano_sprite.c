/* prueba para obtener el tama¤o de un sprite de un archivo dat */



// inclusiones standard
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <allegro.h>

// estos son los datos del archivo de datos generados por Grabber
#include "datahead.h"

int main()
    {
DATAFILE *krono_dato;
BITMAP *info;

   allegro_init();

   krono_dato = load_datafile("kronobz.dat");

  info = krono_dato[MNU_FONDO_GENERICO].dat; // igualo punteros, porque un cast no anda... :(
  // ahora si, se que tama¤o tiene
  printf ("XH: %d, YH: %d", info->w ,
                            info->h );



     return 0;
}
