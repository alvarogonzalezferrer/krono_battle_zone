// -----------------------------------------------
// tanques.c
// Contiene la rutina para dibujar los tanques.
// -----------------------------------------------
#ifndef TANQUES_C
#define TANQUES_C

void dibujar_tanques(BITMAP *bmp, int xmd, int ymd, int ipant)
{
int i2;
BITMAP *tmp1; // bitmap temporal  [para las transparencias]

// este for lo hago para que DIBUJE los 2 tanques si estan dentro de la pantalla
// de lo contrario, el 2ndo tanque es invisible en la pantalla del 1ero
for (i2=1; i2<3; i2++) {
    // Dibujar el tanque:
    // ver si el jugador esta dentro de la pantalla
    if ((jugador_info[i2].posx + JUG_X_MAX - xmd > -1) &&
        (jugador_info[i2].posy + JUG_Y_MAX - ymd > -1)) {

    if ((jugador_info[i2].posx - xmd < H_RES) &&
        (jugador_info[i2].posy - ymd < V_RES / (ipant - 1))) {

        // esta dentro, dibujar...

        // si no es transparente, dibujar solido
     if (jugador_info[i2].premio != INVISIBLE_PREMIO)
       {
        // dibuja el cuerpo rotado
             rotate_sprite (bmp,
                            krono_dato[Tanque].dat,
                            jugador_info[i2].posx - xmd,
                            jugador_info[i2].posy - ymd,
                            itofix (jugador_info[i2].ang_dir));

            if (jugador_info[i2].tiene_torreta) {
                // dibuja la torreta rotada y acomodada al lugar central
                // SUPUESTAMENTE, LA TORRETA Y EL TANQUE SON AMBOS DE 25X25
                // ASIQUE NO HAY PROBLEMAS AL GIRARLOS... OK?
                     rotate_sprite (bmp,
                                    krono_dato[Torreta].dat,
                                    jugador_info[i2].posx - xmd,
                                    jugador_info[i2].posy - ymd,
                                    itofix (jugador_info[i2].ang_tor));
            } // fin si tiene torreta

        }
        else  // es transparente...
        {
           tmp1 = create_bitmap(35,35);  // bmp temporal

           // doble limpieza por las dudas...
           clear(tmp1);
           rectfill(tmp1,0,0,35,35,0);

        // dibuja el cuerpo rotado
             rotate_sprite (tmp1,
                            krono_dato[Tanque].dat,
                            5,
                            5,
                            itofix (jugador_info[i2].ang_dir));

            if (jugador_info[i2].tiene_torreta) {
                // dibuja la torreta rotada y acomodada al lugar central
                // SUPUESTAMENTE, LA TORRETA Y EL TANQUE SON AMBOS DE 25X25
                // ASIQUE NO HAY PROBLEMAS AL GIRARLOS... OK?
                     rotate_sprite (tmp1,
                                    krono_dato[Torreta].dat,
                                    5,
                                    5,
                                    itofix (jugador_info[i2].ang_tor));
            } // fin si tiene torreta
        // copiar con transparencia
            // DEBUG:
            color_map = &tanque_invisible_table; // A VECES FALLA? P'Q'?
            draw_trans_sprite(bmp, tmp1, jugador_info[i2].posx - xmd - 5, jugador_info[i2].posy - ymd - 5);

            destroy_bitmap(tmp1); // liberar memoria
        } // fin es transparente

        // si tiene escudo, dibujar con escudo...
        if (jugador_info[i2].premio == ESCUDO_PREMIO)
        {
            color_map = &trans_table;
            draw_trans_sprite(bmp, krono_dato[ESCUDO_SPR].dat, jugador_info[i2].posx - xmd - 5, jugador_info[i2].posy - ymd - 5);
        } // fin escudo

        // si tiene el premio de velocidad, hace 'chispas' en la cola del tanque
        if (  jugador_info[i2].premio == VELOCIDAD_SPR )
                agregar_particulas(jugador_info[i2].xpol[al_azar(5,7)],
                                   jugador_info[i2].ypol[al_azar(5,7)],
                                   10 + (abs(jugador_info[i2].vel) / 5),
                                   10 + (abs(jugador_info[i2].vel) / 5) ,
                                   0,
                                   200,
                                   223);

    }
    } // fin de ver si estan dentro de la pantalla
} // fin de for i2


} // fin de la rutina
#endif
