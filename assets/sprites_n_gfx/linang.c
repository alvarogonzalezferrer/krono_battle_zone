// Prueba creada por Kronoman para DJGPP
// Intenta dibujar lineas con angulo y largo especificado
// usando el seno y coseno para averiguar su x,y
//


#include <stdlib.h>
#include <stdio.h>

// para usar M_PI, coseno, seno, etc
#include <math.h>

// para usar getch()
#include <conio.h>

#include "allegro.h"


// definicion de PI/180 como constante para acelerar todo...
#define KRONO_PI      0.017453292


// comienzo del programa
// basicamente, dibujara un circulo de lineas
// la constante M_PI contiene pi (definida por math.h)
// ver ayuda de borland c para mas info

int main(int argc, char *argv[])
{
int i,i1, x1, y1, x2, y2, lg;


   allegro_init(); 
//   install_keyboard(); 
//   install_mouse(); // no preciso mouse

// setea el modo de video a 320x200 (VGA standard 13h)
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);

x1 = 160;
y1 = 100;
lg = 50;

// hace el circulo full
// usando grados 0-360
// la constante M_PI esta en mat.h definida
   for(i=0; i<360; i += 10) {

//      y2 = sin(i * M_PI/180) * lg;
      y2 = sin(i * KRONO_PI) * lg;
      y2 = y1 - y2;
//      x2 = cos(i * M_PI/180) * lg;
      x2 = cos(i * KRONO_PI) * lg;
      x2 = x2 + x1;
            line(screen, x1, y1, x2, y2, i);

                         } // fin del for


fflush(stdin); // limpia teclado
x1 = getch(); // espera por tecla, libc
// readkey(); // espera por tecla, en caso de usar teclado de allegro


clear(screen);


// hace el circulo full
// usando la matematica 16.16, donde el circulo va de 0-255
// y usando la tabla pre-calculada de cosenos, senos, etc de allegro
// anda mucho mas rapido pero es mas inexacto... ;D
// notar que es con numeros fixed, por lo tanto los combierto con itofix
// y fixtoi

x1 = 160;
y1 = 100;
lg = 50;

   for(i=0; i<256; i++) {
      i1 = itofix(i); // convierte a algulo... 
      y2 = fixtoi(fmul(fsin(i1), itofix(lg))); // seno precalculado
      y2 = y1 - y2;
      x2 = fixtoi(fmul(fcos(i1), itofix(lg))); // coseno precalculado
      x2 = x2 + x1;
            line(screen, x1, y1, x2, y2, i);

                         } // fin del for

fflush(stdin); // limpia teclado
x1 = getch(); // espera por tecla, libc
// readkey(); // espera por tecla, en caso de usar teclado de allegro




   return 0; // fin del programa
}



