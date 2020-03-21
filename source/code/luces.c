//--------------------------------------------------------
// luces.c
// luces.h
// rutinas de efectos visuales 
// ILUMINACION
//
// NOTAR QUE SI LA VARIABLE BOOLEAN 'VER_LUCES' == FALSE
// ESTAS RUTINAS SON TODAS NULAS, O SEA, RETURN INMEDIATAMENTE
//
//--------------------------------------------------------

#ifndef LUCES_C
#define LUCES_C

//--------------------------------------------------------
// reset_luz()
// esto 'resetea' el sistema de iluminacion
// llamar cada vez que termina el juego
// de lo contrario habra luces al empezar a jugar de nuevo
//--------------------------------------------------------
void reset_luz()
{
int i1; // para el for

cantidad_luz = 4; 
for (i1=0; i1 < MAX_LUZ; i1++)
{
 foco_luz[i1].x = 0;
 foco_luz[i1].y = 0;
 foco_luz[i1].vida = 0;
 foco_luz[i1].radio = 0;
 foco_luz[i1].intensidad = 0;
 foco_luz[i1].crecer = 0;
}

} // fin reset_luz()
//--------------------------------------------------------

//--------------------------------------------------------
//void dibujar_luz(BITMAP *bmp, int x, int y)
//Rutina que dibuja la iluminacion ajustada a la ventana (x, y)-(x+H_RES, y+V_RES)
//Pasarle el bitmap donde se desea que lo dibuje
//
// IMPLEMENTA TRES METODOS DE DIBUJO
// ESPECIFICADOS POR EL int MODO_DE_LUZ
// 0 = MODO NORMAL, PERO LENTO (LAS LUCES SE COMBINAN Y SON SUAVES)
// 1 = MODO INTERMEDIO, LUCES SUAVES, PERO *NO* SE COMBINAN
// 3 = MODO RAPIDO, LUCES ASPERAS, SE COMBINAN (INTENSIDAD FIJA 255)
//
//-------------------------------------------------------- 
void dibujar_luz(BITMAP *bmp, int x, int y)
{
int i1, i2 ; // para el for
float salto, col1; // para calcular el salto del color de 1 al radio que debe tener
             // para poder hacer un hermoso degredee hacia la luz ambiental
BITMAP *luz; // bitmap para crear la mascara de luces, etc


// si las luces estan desactivadas, abortar.
if (VER_LUCES == FALSE) return;

// setear la tabla de iluminacion precalculada
   rgb_map = &rgb_table;
   color_map = &light_table;

switch (MODO_DE_LUZ) {

// modo normal de iluminacion (LENTO!) pero alto detalle
// las luces se funden entre si, etc
case 0:

// setar mapa aditivo creado por mi
// supuestamente ya lo calcule al iniciar el juego
// dibujar en modo transparencia
// al setear el modo aditivo, las luces se adicionaran (combinan)

luz = create_bitmap(bmp->w, bmp->h);
// limpiar con intensidad de luz ambiental
clear_to_color(luz, luz_ambiental);

// dibujar transparente
drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

// mapa aditivo de tonalidad de gris, de esa manera,
// al sobre imprimir luces, se suman...
color_map = &greyscale_add_map; // luego restaurar

for (i1=0; i1 < cantidad_luz; i1++)
    {
// ver si el foco de luz esta dentro del area que me interesa
if ((foco_luz[i1].x - x + foco_luz[i1].radio > 0) &&
    (foco_luz[i1].y - y + foco_luz[i1].radio > 0) &&
    (foco_luz[i1].x - x - foco_luz[i1].radio < bmp->w) &&
    (foco_luz[i1].y - y - foco_luz[i1].radio < bmp->h) &&
    (foco_luz[i1].vida > 0)) {
    // esta dentro, dibujar desde el centro hasta el radio actual
    // usando el degredee
    // calcular salto del 'degrade' (ABS es un macro de allegro

    // evitar div por 0
    if (foco_luz[i1].radio / detalle_luz < 1) foco_luz[i1].radio = detalle_luz;

    salto = (float)( (abs(luz_ambiental - foco_luz[i1].intensidad)) / (foco_luz[i1].radio / detalle_luz) );
    // if (salto < 1) salto = 1.0; // no permitir que sea < 1

    if (luz_ambiental > foco_luz[i1].intensidad) salto = -salto;

    col1 = (float)luz_ambiental;

    for (i2=foco_luz[i1].radio; i2 > 1; i2 -= detalle_luz) {
        circlefill(luz,
                   foco_luz[i1].x - x,
                   foco_luz[i1].y - y,
                   i2, (int)col1);
        col1 += salto;

        // control de seguridad...
        if ((int)col1 > 255.0) col1 = 255.0;
        if ((int)col1 < 0.0) col1 = 0.0;

        }  // fin for i2
     } // fin 'if' de ver si esta dentro de la pantalla
} // fin for i1

// modo solido... ;)
drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

// restaurar mapa color a mapa de luz, mapa RGB del juego
   rgb_map = &rgb_table;
   color_map = &light_table;

// colocar el bitmap  en posicion, transparente usando tabla de luz
draw_trans_sprite(bmp, luz, 0, 0);

// libera la memoria del bitmap temporal
destroy_bitmap(luz);
break;
//---------------------


// Modo INTERMEDIO: luces suaves; no se dibujan bien si se intersectan
case 1:

luz = create_bitmap(bmp->w, bmp->h);
// limpiar con intensidad de luz ambiental
clear_to_color(luz, luz_ambiental);

for (i1=0; i1 < cantidad_luz; i1++)
    {
// ver si el foco de luz esta dentro del area que me interesa
if ((foco_luz[i1].x - x + foco_luz[i1].radio > 0) &&
    (foco_luz[i1].y - y + foco_luz[i1].radio > 0) &&
    (foco_luz[i1].x - x - foco_luz[i1].radio < bmp->w) &&
    (foco_luz[i1].y - y - foco_luz[i1].radio < bmp->h) &&
    (foco_luz[i1].vida > 0)) {
    // esta dentro, dibujar desde el centro hasta el radio actual
    // usando el degredee
    // calcular salto del 'degrade' (ABS es un macro de allegro

    // evitar div por 0
    if (foco_luz[i1].radio / detalle_luz < 1) foco_luz[i1].radio = detalle_luz;

    salto = (float)( (abs(luz_ambiental - foco_luz[i1].intensidad)) / (foco_luz[i1].radio / detalle_luz) );
    // if (salto < 1) salto = 1.0; // no permitir que sea < 1

    if (luz_ambiental > foco_luz[i1].intensidad) salto = -salto;

    col1 = (float)luz_ambiental;

    for (i2=foco_luz[i1].radio; i2 > 1; i2 -= detalle_luz) {
        circlefill(luz,
                   foco_luz[i1].x - x,
                   foco_luz[i1].y - y,
                   i2, (int)col1);
        col1 += salto;

        // control de seguridad...
        if ((int)col1 > 255.0) col1 = 255.0;
        if ((int)col1 < 0.0) col1 = 0.0;

        }  // fin for i2
     } // fin 'if' de ver si esta dentro de la pantalla
} // fin for i1

// colocar el bitmap  en posicion
draw_trans_sprite(bmp, luz, 0, 0);

// libera la memoria del bitmap temporal
destroy_bitmap(luz);
break;
//---------------------

// MODO DEFAULT:
// DIBUJAR ULTRA RAPIDO CON 255 INTENSIDAD E ILUMINAR EL RESTO CON COLOR
// DE AMBIENTE, DE ESA FORMA, SE COMBINAN LAS LUCES ;D
default:

luz = create_bitmap(bmp->w, bmp->h);

// limpiar con intensidad de luz ambiental
clear_to_color(luz, luz_ambiental);

for (i1=0; i1 < cantidad_luz; i1++)
    {
// ver si el foco de luz esta dentro del area que me interesa
if ((foco_luz[i1].x - x + foco_luz[i1].radio >0) &&
    (foco_luz[i1].y - y + foco_luz[i1].radio > 0) &&
    (foco_luz[i1].x - x - foco_luz[i1].radio < bmp->w) &&
    (foco_luz[i1].y - y - foco_luz[i1].radio < bmp->h) &&
    (foco_luz[i1].vida > 0)) {

       circlefill(luz,
                   foco_luz[i1].x - x,
                   foco_luz[i1].y - y,
                   foco_luz[i1].radio,255);


     } // fin 'if' de ver si esta dentro de la pantalla
} // fin for i1

// colocar el bitmap en posicion
draw_trans_sprite(bmp, luz, 0, 0);

// libera la memoria del bitmap temporal
destroy_bitmap(luz);
break;
//---------------------

} // fin switch
//--------------------


} // fin dibujar_luz
//--------------------------------------------------------

//--------------------------------------------------------
// void mover_luz()
// Rutina que MUEVE las luces
// y libera la memoria de las luces que ya no se usan
// para permitir el uso de nuevas
// llamar cuando actualiza la logica del juego
//
// NO TOCA LAS LUCES DE 0 A 3 PORQUE SON RESERVADAS PARA LOS TANQUES!
//--------------------------------------------------------
void mover_luz()
{
int i1, i2; // para el for, etc

// si las luces estan desactivadas, abortar.
if (VER_LUCES == FALSE) return;

i2 = cantidad_luz;

for (i1=4; i1<cantidad_luz; i1++)
{

foco_luz[i1].radio += foco_luz[i1].crecer;

foco_luz[i1].vida--;

if (foco_luz[i1].vida < 1)
    {
    // luza muerta, cambiar con la ultima viva el lugar
    // y eliminar
     foco_luz[i1].x = foco_luz[i2].x;
     foco_luz[i1].y = foco_luz[i2].y;
     foco_luz[i1].vida = foco_luz[i2].vida;
     foco_luz[i1].radio = foco_luz[i2].radio;
     foco_luz[i1].intensidad = foco_luz[i2].intensidad;
     foco_luz[i1].crecer = foco_luz[i2].crecer;

     // seguridad, resetear ultima luz eliminada
     foco_luz[i2].x = 0;
     foco_luz[i2].y = 0;
     foco_luz[i2].vida = 0;
     foco_luz[i2].radio = 0;
     foco_luz[i2].intensidad = 0;
     foco_luz[i2].crecer = 0;

     i2--;
     if (i2<0) i2=0;
    }
} // fin for

cantidad_luz = i2;
// LISTO!
} // fin mover_luz
//--------------------------------------------------------


//--------------------------------------------------------
//void agregar_luz(int x, int y, int vida,
//                        int colorr, int radio, int crecer)
//
// Esta rutina agrega luz
// en la  posicion 'x,y' de duracion 'vida',
// de 'intensidad 'colorr' 0-255'  de 'radio', y con crecimiento 'crecer'
//
// NO TOCA LAS LUCES DE 0 A 3 PORQUE SON RESERVADAS PARA LOS TANQUES!
//
//--------------------------------------------------------

void agregar_luz(int x, int y, int vida,
                        int colorr, int radio, int crecer)
{

// si las luces estan desactivadas, abortar.
if (VER_LUCES == FALSE) return;

if (cantidad_luz<3) cantidad_luz=3; // corrige un posible bug...

// seguridad
if (cantidad_luz + 1 >MAX_LUZ-1) return;


// setear
 foco_luz[cantidad_luz].x = x;
 foco_luz[cantidad_luz].y = y;
 foco_luz[cantidad_luz].vida = vida;
 foco_luz[cantidad_luz].radio = radio;
 foco_luz[cantidad_luz].intensidad = colorr;
 foco_luz[cantidad_luz].crecer = crecer;

// agregar
cantidad_luz++;
// comprobacion de seguridad
if (cantidad_luz>MAX_LUZ-1) cantidad_luz=MAX_LUZ-1;


// LISTO!
} // fin agregar_luz 
//--------------------------------------------------------


//--------------------------------------------------------
//void tanque_luz()
//                 
// Setea las luces de ambos tanques (1 y 2)
// 0,1,2,3 a sus posiciones adecuadas SI las luces estan
// prendidas, caso contrario, las apaga...
//
// llamar en cada update_game_logic!
//--------------------------------------------------------
void tanque_luz()
{

// por las dudas, matar las 4 luces primero
foco_luz[0].vida = 0;
foco_luz[1].vida = 0;
foco_luz[2].vida = 0;
foco_luz[3].vida = 0;
foco_luz[0].crecer = 0;
foco_luz[1].crecer = 0;
foco_luz[2].crecer = 0;
foco_luz[3].crecer = 0;

if (VER_LUCES == FALSE) return;

if (jugador_info[1].luz_prendida)
    {
    // jugador 1
    // torreta
     foco_luz[0].x =  jugador_info[1].xpol[0];
     foco_luz[0].y =  jugador_info[1].ypol[0];
     foco_luz[0].vida = 1000;
     foco_luz[0].radio = 50;
     foco_luz[0].intensidad = 200;

    if (jugador_info[1].premio == LUZ_SPR)
        {
        foco_luz[0].radio = 100; // premio de luz
        foco_luz[0].intensidad = 200;
        }

     if (MODO_DE_LUZ != 1) { // si el modo de dibujar la luz admite superposicion
            // jugador 1 - foco de luz en el tanque 
             foco_luz[1].x =  jugador_info[1].xpol[2];
             foco_luz[1].y =  jugador_info[1].ypol[2];
             foco_luz[1].vida = 1000;
             foco_luz[1].radio = 40;
             foco_luz[1].intensidad = 255; 
                                          
                           } 
     }

if (jugador_info[2].luz_prendida)
    {
    // jugador 2
     foco_luz[2].x =  jugador_info[2].xpol[0];
     foco_luz[2].y =  jugador_info[2].ypol[0];
     foco_luz[2].vida = 1000;
     foco_luz[2].radio = 50;


     foco_luz[2].intensidad = 200;

    if (jugador_info[2].premio == LUZ_SPR)
        {
          foco_luz[2].radio = 100; // premio de luz
          foco_luz[2].intensidad = 200;
        }

     if (MODO_DE_LUZ != 1) { // si el modo de dibujar la luz admite superposicion
            // jugador 2 - foco de luz en el tanque 
             foco_luz[3].x =  jugador_info[2].xpol[2];
             foco_luz[3].y =  jugador_info[2].ypol[2];
             foco_luz[3].vida = 1000;
             foco_luz[3].radio = 40;
             foco_luz[3].intensidad = 255;
                                         
                           }  
    }


if (cantidad_luz<4) cantidad_luz = 4;

} // fin tanque_luz
//--------------------------------------------------------

#endif
