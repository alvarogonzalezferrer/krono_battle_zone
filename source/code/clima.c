//--------------------------------------------------------
// clima.c
// clima.h
// Rutinas que hacen la simulacion del clima
// como NIEVE, LLUVIA, ARENA, GRANIZO, NIEBLA
//
// NOTAR QUE SI VER_CLIMA == FALSE, NO SE VEN
//--------------------------------------------------------

#ifndef CLIMA_C
#define CLIMA_C
//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------


static struct {
 int x;  // posicion x 
 int y;  // posicion y 
 int dx; // direccion x
 int dy; // direccion y
 int tipo; // tipo: 1=lluvia, 2=nieve, 3=arena,4 granizo, 5 niebla
 int color; // color 0-255
 int radio; // radio si es un copo
} clima[MAX_CLIMA]; // desde 0 a MAX_CLIMA - 1
//--------------------------------------------------------

//--------------------------------------------------------
// crear_clima()
//
// Crea el clima para el terreno actual
// LLAMAR AL PRINCIPIO DEL JUEGO
//--------------------------------------------------------
void crear_clima()
{
int i1;

if (!VER_CLIMA) return;

for (i1=0; i1<MAX_CLIMA; i1++)
         crear_clima_part(i1);

} // fin crear_clima
//--------------------------------------------------------

//--------------------------------------------------------
// dibujar_clima(BITMAP *bmp, x , y, j)
//
// Esta rutina se encarga de DIBUJAR el clima
// en el bitmap especifico, en las coordenadas especificas
// Llamar al actualizar la pantalla
// en j pasar el numero de jugador (1 o 2)
//--------------------------------------------------------
void dibujar_clima(BITMAP *bmp, int x, int y, int j)
{
int i1;
BITMAP *nieblabmp;


// si no esta activado, salir
if (VER_CLIMA == FALSE) return;


// ------ ATMOSFERA ------
if ((info_mapa_juego.clima != NIEBLA) && (info_mapa_juego.clima != NORMAL)) {
for (i1=0; i1<MAX_CLIMA; i1++)
    {
    // dibujar
    switch (clima[i1].tipo) {
    case NIEVE:
    case GRANIZO:
       circlefill(bmp,
                  clima[i1].x,
                  clima[i1].y,
                  clima[i1].radio,
                  clima[i1].color);
    break;

    case LLUVIA:
       line(bmp,
                  clima[i1].x,
                  clima[i1].y,
                  clima[i1].x + clima[i1].dx,
                  clima[i1].y + clima[i1].dy,
                  clima[i1].color);
    break;
    case ARENA:
      putpixel(bmp,
                  clima[i1].x+ al_azar(-15,15),
                  clima[i1].y+ al_azar(-15,15),
                  clima[i1].color);
      putpixel(bmp,
                  clima[i1].x+ al_azar(-15,15),
                  clima[i1].y+ al_azar(-15,15),
                  clima[i1].color);
      putpixel(bmp,
                  clima[i1].x+ al_azar(-15,15),
                  clima[i1].y+ al_azar(-15,15),
                  clima[i1].color);
      putpixel(bmp,
                  clima[i1].x+ al_azar(-15,15),
                  clima[i1].y+ al_azar(-15,15),
                  clima[i1].color);

    break;

    } // fin switch
  } // fin for i1
} // fin ver si el clima no es NIEBLA ni NORMAL


// ----- NIEBLA -------
// ver si es niebla, dibujar niebla WOWWW
// la niebla es dibujada de manera que el tanque enemigo quede en la niebla...
// DEBUG: agregar un efecto de 'ola' usando el seno o algo asi...
if (info_mapa_juego.clima==NIEBLA) {

        nieblabmp = create_bitmap(bmp->w, bmp->h); // crea la niebla

        // dibuja la niebla total    (31) -> blanco
        rectfill(nieblabmp, 0, 0, nieblabmp->w, nieblabmp->h, 31);

        // dibuja el fade de la niebla de blanco total a normal (negro, 0)
        // en la posicion del jugador transportada a x,y
        for(i1=31; i1>0 ; i1-= 4)
             circlefill(nieblabmp,
                        jugador_info[j].xpol[1] - x,
                        jugador_info[j].ypol[1] - y,
                         (i1*2)+30, i1);
        // fin for i1...

            color_map = &trans_table;
            draw_trans_sprite(bmp, nieblabmp, 0, 0);

            // liberar memoria
            destroy_bitmap(nieblabmp);

} // fin dibujar niebla



}// fin dibujar_clima
//--------------------------------------------------------



//--------------------------------------------------------
// rutina auxiliar para setear particulas
// la hice para no repetirla 2 veces en el codigo
// pasarle el numero de particula a crear
// notar que no verifica nada... 
//--------------------------------------------------------
void crear_clima_part(int i1)
{
// posicion
clima[i1].x = random() % H_RES;
clima[i1].y = random() % (V_RES / ( (PANT_SPLIT) ? 2 : 1) ) ;


// movimiento
switch (info_mapa_juego.clima)
{
case LLUVIA:

    clima[i1].dx = (al_azar(2,5));
    clima[i1].dy = (al_azar(10, 20));
    clima[i1].radio = al_azar(3,5);
    clima[i1].tipo = LLUVIA;
    clima[i1].color = al_azar(70,79);
    break;
case NIEVE:

    clima[i1].dx = (al_azar(-10, -2));
    clima[i1].dy = (al_azar(3, 15));
    clima[i1].radio = al_azar(1,3);
    clima[i1].tipo = NIEVE;
    clima[i1].color = al_azar(28,31);
    break;
case ARENA:

    clima[i1].dx = (al_azar(10, 15));
    clima[i1].dy = 0;
    clima[i1].radio = al_azar(5,10);
    clima[i1].tipo = ARENA;
    clima[i1].color = al_azar(180,191);
    break;
case GRANIZO:

    clima[i1].dx = al_azar(-7, 7);
    clima[i1].dy = (al_azar(5, 15));
    clima[i1].radio = 1;
    clima[i1].tipo = GRANIZO;
    clima[i1].color = al_azar(70,79);
    break;

default: // si no, es niebla que no tiene particulas... ;D
    clima[i1].dx = 0;
    clima[i1].dy = 0;
    clima[i1].radio = 0;
    clima[i1].tipo = NIEBLA;
    clima[i1].color = 0;
    break;
} // fin switch


} // fin crear_clima_part(int i1)
//--------------------------------------------------------

//--------------------------------------------------------
// mover_clima()
// Rutina que mueve las particulas del clima
//--------------------------------------------------------
void mover_clima()
{
int i1;

for (i1=0; i1<MAX_CLIMA; i1++)
    {

    // mover 
    clima[i1].x +=  clima[i1].dx;
    clima[i1].y +=  clima[i1].dy;

    // verificar
    if ((clima[i1].x < 0) ||
        (clima[i1].y < 0) ||
        (clima[i1].x > H_RES) ||
        (clima[i1].y > (V_RES / ( (PANT_SPLIT) ? 2 : 1) ))) crear_clima_part(i1);
    }
}
//--------------------------------------------------------


#endif
