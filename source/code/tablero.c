//--------------------------------------------------------
// tablero.c
//
// Rutinas para dibujar el tablero del juego
// donde se ve la energia, puntos, arma, radar, etc
// Por Kronoman
//--------------------------------------------------------

#ifndef TABLERO_C
#define TABLERO_C

//--------------------------------------------------------
// hace_tablero(BITMAP *bmp, int numjug)
//
// Dibuja el tablero del jugador especificado (1 o 2)
// en el BITMAP especificado (esquina sup izquierda)
//
// NO comprueba si el numero de jugador es valido
//
//--------------------------------------------------------
void hace_tablero(BITMAP *bmp, int numjug)
{
char tmpstr[80]; // string temporal para enviar texto a pantalla


// dibuja una barra de energia en color 247,248 (rojos)

// energia
if (jugador_info[numjug].energia > 0)  rectfill(bmp, 2, 2,
                                               (jugador_info[numjug].energia / 2)+2,
                                                7, 248);
// borde barra energia
rect(bmp, 2, 2, 
     (MAX_ENERGIA/2)+2, 7, 247);


// cantidad de balas y puntaje
        text_mode(-1); // texto (-1=trans, 1 solido)
if ((jugador_info[numjug].arma > 0) && (jugador_info[numjug].balas > 0))
        sprintf(tmpstr, "[%02d][%03ld]", jugador_info[numjug].balas, info_mapa_juego.puntaje[numjug]);
else
        sprintf(tmpstr, "[--][%03ld]", info_mapa_juego.puntaje[numjug]);

        textout(bmp,krono_dato[FontXM6x6].dat,tmpstr, 0,10,251);



// Radar - Esto es un power-up
if ( (jugador_info[numjug].premio == RADAR) && (jugador_info[numjug].c_premio > 0) )
    {
        rect (bmp, bmp->w - 22, 0,
              bmp->w-1, 22, 249);

        line (bmp, bmp->w - 22, 11,
              bmp->w-1, 11, 249);

        line (bmp, bmp->w - 11, 0,
              bmp->w-11, 22, 249);

        // puntos de los jugadores
        // multiplico pos * 0.02 p'q es lo mismo que * 1000 / 20
        // o sea, convierto las coord por regla de 3, facil.
        // 1000 ---- 20 --> x = (posx * 20)/1000 = posx * 0.02
        // posx ---- x

        putpixel(bmp, bmp->w - 21 + (jugador_info[1].posx * 0.02),
                (jugador_info[1].posy * 0.02) + 1, 251);

        putpixel(bmp, bmp->w - 21 + (jugador_info[2].posx * 0.02),
                (jugador_info[2].posy * 0.02) + 1, 251);

} // -------------------------------- hasta aqui radar

}
//--------------------------------------------------------


#endif
