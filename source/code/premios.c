//--------------------------------------------------------
// premios.c
// premios.h
//
// RUTINAS QUE CONTIENEN EL ADMINISTRADOR DE PREMIOS 
// PARA AMBOS JUGADORES (POWER-UPS)
//
// Por Kronoman - DJGPP
//--------------------------------------------------------

#ifndef PREMIOS_C
#define PREMIOS_C

//--------------------------------------------------------
// Codigo de las funciones
//--------------------------------------------------------

//--------------------------------------------------------
// poner_premio
// Rutina auxiliar, ubica el premio indice i1 en el tablero
// y ademas, impulsa un proyector de particulas para
// justificar su aparicion...
//--------------------------------------------------------
void poner_premio(int i1)
{
int i2 = 0;

     do {
     // posicion
         premio[i1].x = random() % 50;
         premio[i1].y = random() % 50;

     // vida
         premio[i1].vida = (random() % 200) + 150;

     // tipo
         premio[i1].tipo = (random() % TIPO_MAX);

     // si posicion <> pared, todo ok...
        if ( grilla_map[premio[i1].x][premio[i1].y] <= AGUA ) i2 = 1001;

        premio[i1].x *= SPR_X_MAX;
        premio[i1].y *= SPR_Y_MAX;

     } while (i2++ < 1000 );

     // impulsar proyector de particulas
     agregar_particulas(premio[i1].x + SPR_X_MAX/2,
                        premio[i1].y + SPR_Y_MAX/2,
                        random()%10+10,
                        random()%20+60,
                        5,
                        247,
                        248);
}

//--------------------------------------------------------
// reset_premios()
// Inicializa TODOS los premios...
//--------------------------------------------------------
void reset_premios()
{
int i1; // para el for

for (i1 = 0; i1 < MAX_PREMIOS; i1++) poner_premio(i1);

}

//--------------------------------------------------------
// mover_premios()
// Llamar seguido, administra los premios y los otorga
// al jugador que los agarre.
//--------------------------------------------------------

void mover_premios()
{
int i1, i2; // para el for

// DESCONTAR PREMIOS
    for (i2 = 1; i2 < 3; i2++)
    {
    // si el jugador tiene un premio, descontar cantidad...
    if (--jugador_info[i2].c_premio < 0)
        {
            jugador_info[i2].premio = 0;
            jugador_info[i2].c_premio = 0;
        }
    } // fin descontar premios

// MOVER PREMIOS
for (i1 = 0; i1 < MAX_PREMIOS; i1++)
    {
    // ver si lo agarro algun tanque
    for (i2 = 1; i2 < 3; i2++)
    {

    if ( (premio[i1].x + SPR_X_MAX/2 > jugador_info[i2].posx - 5) &&
         (premio[i1].x + SPR_X_MAX/2 < jugador_info[i2].posx + JUG_R_MAX + 5) &&
         (premio[i1].y + SPR_Y_MAX/2 > jugador_info[i2].posy - 5) &&
         (premio[i1].y + SPR_Y_MAX/2 < jugador_info[i2].posy + JUG_R_MAX + 5) )
         {
          premio[i1].vida = 0; // eliminar premio
          // otorgar premio
          switch (premio[i1].tipo)
                 {
                 case 0:  // arma nueva
                    jugador_info[i2].arma = al_azar(1, CANTIDAD_DE_ARMAS); // evito darle la ametralladora (0)

                    if (jugador_info[i2].arma > CANTIDAD_DE_ARMAS - 1) jugador_info[i2].arma = CANTIDAD_DE_ARMAS - 1;
                    if (jugador_info[i2].arma < 1) jugador_info[i2].arma = 1;

                    jugador_info[i2].balas = random()%75 + 25; // cantidad de balas al azar
                 break;

                 case 1: // botiquin - medikit
                    if ( (jugador_info[i2].energia += random()%15 + 15) > MAX_ENERGIA ) jugador_info[i2].energia = MAX_ENERGIA;
                 break; 

                 case 2: // radar - mapa
                    jugador_info[i2].premio = RADAR;
                    jugador_info[i2].c_premio = random()% 150 + 250; // duracion en ticks de juego
                 break;

                 case 3: // invisible
                    jugador_info[i2].premio = INVISIBLE_PREMIO;
                    jugador_info[i2].c_premio = random()% 300 + 300; // duracion en ticks de juego
                 break;

                 case 4: // escudo
                    jugador_info[i2].premio = ESCUDO_PREMIO;
                    jugador_info[i2].c_premio = random()% 200 + 300; // duracion en ticks de juego
                 break;
                 
                 case 5: // luz
                    jugador_info[i2].premio = LUZ_SPR;
                    jugador_info[i2].c_premio = random()% 200 + 100; // duracion en ticks de juego
                 break;

                 case 6: // velocidad
                    jugador_info[i2].premio = VELOCIDAD_SPR;
                    jugador_info[i2].c_premio = random()% 200 + 250; // duracion en ticks de juego
                 break;

                 } // fin switch
         } // fin if
    } // fin for i2


    if (--premio[i1].vida < 1)
        {
         // impulsar proyector de particulas p'q desaparece
         agregar_particulas(premio[i1].x + SPR_X_MAX/2,
                            premio[i1].y + SPR_Y_MAX/2,
                            random()%10+15,
                            random()%20+60,
                            5,
                            243,
                            245);
        poner_premio(i1);
        }
    } // fin for i1

}


//--------------------------------------------------------
// Dibuja los premios en el bmp especificado, transportados
// a las coordenadas x,y 
//--------------------------------------------------------
void dibujar_premios(BITMAP *bmp, int x, int y) {
int i1; // para el for
int spr; // sprite a dibujar

// dibujar todos los premios
for (i1 = 0; i1 < MAX_PREMIOS; i1++)
    {
    if (premio[i1].vida > 0)
       {
          switch (premio[i1].tipo)
                 {
                 case 0:  // arma nueva
                    spr = SPR_ARMA_NUEVA;
                 break;

                 case 1: // botiquin - medikit
                    spr = SPR_MEDIKIT;
                 break;

                 case 2: // radar
                    spr = SPR_RADAR;
                 break;

                 case 3: // invisible
                    spr = SPR_INVISIBLE;
                 break;

                 case 4: // escudo
                    spr = SPR_ESCUDO;
                 break;

                 case 5: // luz
                   spr = SPR_LUZ;
                 break;

                 case 6: // velocidad
                   spr = SPR_VELOCIDAD;
                 break;

                 default:
                         return; // DEBUG - ERROR HORRIBLE!

                 } // fin switch

                 // poner sprite
                 draw_sprite(bmp,
                             krono_dato[spr].dat,
                             premio[i1].x - x,
                             premio[i1].y - y);
       } // fin vida > 0
    } // fin for

}
#endif
