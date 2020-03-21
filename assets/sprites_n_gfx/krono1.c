/*    CONTIENE RUTINAS UTILES PARA ROTAR SPRITES!!! ATENTO!!!
 *    Programa creado por Kronoman
 *
 *    Carga un PCX/BMP y con A y Z se gira para derecha/izquierda
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h> // para seno, coseno, etc :D
#include "allegro.h"

// definicion de PI/180 como constante para acelerar todo...
#define KRONO_PI      0.017453292

// definicion de la relacion entre 255/360 para pasar
// de grados normales a 255 y viceversa
// angulo 0-360 * GRADOSRELAT = angulo de 0-255
// angulo 0-255 / GRADOSRELAT = angulo de 0-360
#define GRADOSRELAT   0.708333333

// Definicion de estructura usada para las coordenadas
// Tambien la usa la rutina de coordenadas de angulo
typedef struct
{
 int x1, y1; // coordenada x, y origen
 int x2, y2;    // coordenada x,y resultante
 int ang; // angulo usado
 int largo; // largo usado

} ANGULO_TYPE;


//----------------------------------------------------------- 
// Rutina que devuelve una estructura con las coordenada x,y destino
// para trazar una linea de x1, y1 en angulo ang con largo 
// el angulo va en un valor de 0 a 360
// ESTOY SEGURO QUE HAY FORMAS MEJORES DE HACERLO
// OPTIMIZAR OPTIMIZAR OPTIMIZAR OPTIMIZAR Y OPTIMIZAR!!!
// USAR MATEMATICA DE FIXED POINT (PROVISTA POR ALLEGRO) SI ES POSIBLE!
//-----------------------------------------------------------

ANGULO_TYPE dame_angc(int ang, int largo, int xx1, int yy1)
{
ANGULO_TYPE tmp; // estructura temporal

// seteo la estructura
tmp.x1 = xx1;
tmp.y1 = yy1;

tmp.ang = ang;
tmp.largo = largo;

// calculo la x2, y2

tmp.y2 = sin(tmp.ang * KRONO_PI) * tmp.largo;
tmp.y2 = tmp.y1 - tmp.y2;

tmp.x2 = cos(tmp.ang * KRONO_PI) * tmp.largo;
tmp.x2 = tmp.x2 + tmp.x1;

return tmp;
}
//----------------------------------------------------------- 

//----------------------------------------------------------- 
// Rutina que devuelve un angulo dentro del rango de 0-360
// aun si el angulo esta fuera de rango
//-----------------------------------------------------------
int arregla_ang(int angulo)
{
int i;
i = angulo - (360 * (angulo/360));
if (i>360) i -= 360;
if (i<0)   i += 360;
return i;
}
//-----------------------------------------------------------


//----------------------------------------------------------- 
// Rutina que devuelve un angulo dentro del rango de 0-255
// dandole un angulo de 0-360; util para rutinas de allegro
// Luego convertir este entero a fixed usando itofix(xxx)
//-----------------------------------------------------------
int pasa360a255(int angulo)
{
        int i;
        i = arregla_ang(angulo); // acomodo el angulo a 0-360
//        i = i * 255 / 360;
        i = i * GRADOSRELAT; // relacion de 0.70833333 etc, predefinida
        return i;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
// Rutina que devuelve un angulo dentro del rango de 0-360
// dandole un angulo de 0-255; util para rutinas de allegro
// Pasar un entero convirtiendo con fixtoi(xxx)
//-----------------------------------------------------------
int pasa255a360(int angulo)
{
        int i;
i = angulo;
// ajustar
if (i>255) i -= 255;
if (i<0)   i += 255;

//        i = i * 360 / 255;
        i = i / GRADOSRELAT; // relacion de 0.70833333 etc, predefinida
        return i;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
// Programa principal para demostracion
// Rota el poligono y marca sus vertices para comprobaciones
// Forma un poligono de 9 vertices (para mas detalle)
//-----------------------------------------------------------

int main(int argc, char *argv[])
{
   BITMAP *the_image;
   PALLETE the_pallete;
   int angulo = 0, pos_x = 0, pos_y = 0, angold;
   int agt; // temporal para uso de los angulos
   int i1; // contador para el for
ANGULO_TYPE angulotmp; // estructura temporal para recuperar la posicion
                       // en base a los angulos...


// vertices del poligono
// ver grafico:
// 5----4-----3
// |          |
// 6    1     2
// |          |
// 7----8-----9
int polx[9], poly[9];




   if (argc != 2) {
      printf("Uso: '[ejecutable.exe] archivo.[bmp|lbm|pcx|tga]' - Flechitas para girar\n");
      printf("Para mover la imagen use las letras A,S,D,W \n");
      return 1;
   }

   /* leer el bitmap */
   the_image = load_bitmap(argv[1], the_pallete);
   if (!the_image) {
      printf("Error cargando el archivo '%s'\n", argv[1]);
      return 1;
   }
    /* inicializa lo necesario */
   allegro_init();
   install_keyboard(); 
//   set_gfx_mode(GFX_VESA1, 1024, 768, 0, 0);
    set_gfx_mode(GFX_VGA, 320,  200, 0, 0);
//    set_gfx_mode(GFX_MODEX, 320,  400, 0, 0);

   /* seleccionar la paleta del bitmap */
   set_pallete(the_pallete);

   /* copiar la imagen en pantalla, centrada */
//   blit(the_image, screen, 0, 0, (SCREEN_W-the_image->w)/2, 
//                     (SCREEN_H-the_image->h)/2, the_image->w, the_image->h);

// centra la imagen
pos_x = (SCREEN_W-the_image->w)/2;
pos_y = (SCREEN_H-the_image->h)/2;

// pos_x=0;
// pos_y=0;

// pone la imagen en posicion de usuario
//   clear(screen);
   blit(the_image, screen, 0, 0, pos_x, pos_y, the_image->w, the_image->h);

angold= -1;

   /* loop principal */
   /* notar que usa key de allegro en vez de kbhit de C */
   do {

           if (key[KEY_LEFT])
           {
           clear_keybuf(); // limpiar teclado
           angulo=angulo-8;
           }
           if (key[KEY_RIGHT])
           {
           clear_keybuf(); // limpiar teclado
           angulo=angulo+8;
           }

           if (key[KEY_A])
           {
                clear_keybuf(); // limpiar teclado
                   pos_x--;
                   angold=-1;
           }
           if (key[KEY_D])
           {
                clear_keybuf(); // limpiar teclado
                   pos_x++;
                   angold=-1;
           }
           if (key[KEY_W])
           {
                clear_keybuf(); // limpiar teclado
                   pos_y--;
                   angold=-1;
           }
           if (key[KEY_S])
           {
                clear_keybuf(); // limpiar teclado
                   pos_y++;
                   angold=-1;
           }


          if ( angulo > 255 ) angulo = angulo-255;
          if ( angulo < 0 ) angulo = angulo + 255;

//notar que el itofix es necesario para convertir el angulo!
//           rotate_sprite (screen, the_image,(SCREEN_W-the_image->w)/2,(SCREEN_H-the_image->h)/2,itofix (angulo));

if (angulo!=angold) { // redibujar
angold=angulo;
// lo situa en pos del usuario
 clear(screen);

rotate_sprite (screen, the_image,pos_x,pos_y,itofix (angulo));

// PARTE IMPORTANTE
// SETEAR LOS PUNTOS DEL POLIGONO

// centro (1)
polx[1] = pos_x + the_image->w/2;
poly[1] = pos_y + the_image->h/2;


agt = pasa255a360(angulo); // convierto a mi sistema de 0-360


// mitad derecha (2) en 0 grados
angulotmp = dame_angc(0-agt, the_image->w/2, polx[1], poly[1] );
polx[2] = angulotmp.x2;
poly[2] = angulotmp.y2;

// esquina superior (3) a 90 grados
angulotmp = dame_angc(90-agt, the_image->h/2, polx[2], poly[2] );
polx[3] = angulotmp.x2;
poly[3] = angulotmp.y2;

// mitad superior (4) a 180 grados
angulotmp = dame_angc(180-agt, the_image->w/2, polx[3], poly[3] );
polx[4] = angulotmp.x2;
poly[4] = angulotmp.y2;

// vertice superior (5) a 180 grados
angulotmp = dame_angc(180-agt, the_image->w/2, polx[4], poly[4] );
polx[5] = angulotmp.x2;
poly[5] = angulotmp.y2;

// mitad izquierda (6) a 270 grados
angulotmp = dame_angc(270-agt, the_image->h/2, polx[5], poly[5] );
polx[6] = angulotmp.x2;
poly[6] = angulotmp.y2;

// vertice inf izquierdo (7) a 270 grados
angulotmp = dame_angc(270-agt, the_image->h/2, polx[6], poly[6] );
polx[7] = angulotmp.x2;
poly[7] = angulotmp.y2;

// vertice inf mitad (8) a 0 grados
angulotmp = dame_angc(0-agt, the_image->w/2, polx[7], poly[7] ) ;
polx[8] = angulotmp.x2;
poly[8] = angulotmp.y2;

// vertice inf der (9) a 0 grados
angulotmp = dame_angc(0-agt, the_image->w/2, polx[8], poly[8] );
polx[9] = angulotmp.x2;
poly[9] = angulotmp.y2;

// dibujar los vertices en color 255
for (i1=1; i1<10; i1++)
     circlefill(screen, polx[i1], poly[i1],1, 255);
} // fin redibujar
//--------------


} while (!key[KEY_ESC]); //mientras <ESC> no sea presionado
//------------

   clear_keybuf();

   /* destruye el bitmap */
   destroy_bitmap(the_image);

   readkey(); /* espera por tecla */
   allegro_exit();

   printf("Hecho por Kronoman - Argentina \n %d \n",angulo);
   return 0; /* sale */ 
}





