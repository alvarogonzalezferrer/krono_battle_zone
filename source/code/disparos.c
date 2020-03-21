//--------------------------------------------------------
// disparos.c
// disparos.h
//
// RUTINAS QUE CONTIENEN EL ADMINISTRADOR DE DISPAROS
// PARA AMBOS JUGADORES
// HACEN: DISPARAR, MOVER, COMPROBAR, ETC LOS DISPAROS
//
// Por Kronoman - DJGPP
//--------------------------------------------------------

#ifndef DISPAROS_C
#define DISPAROS_C
//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE DISPAROS

// disparo libre en cada estructura (si es -1, no hay disparos disponibles,
// sino, apunta al disparo libre que haya...)
// Este metodo es mas pedorro pero mas seguro que una lista dinamica
static int disp_libre[3];

static struct {
 int   x;  // posicion x 
 int   y;  // posicion y 

 int vida; // vida del disparo, se descuenta con cada
           // llamada del timer, al llegar a 0 es liberada (muere)

 int frame; // cuadro de la imagen del disparo
            // en caso de que sea animado

 int direccion; // direcion en grados 0-255

 int vel; // velocidad en pixels * 10 (o sea, si es 100, en realidad es 10 pixeles...)

 int tipo; // tipo del disparo
           // 0 = bala
           // 1 = bola de plasma (?)
           // 2 = lanza-llamas (animado, 3 frames!)
           // 3 = laser
           // 4 = rayo

} dispa[MAX_SHOOTS][3]; // desde 0 a MAX_SHOOTS - 1 
                        // NOTAR LA DIMENSION [3] AL LADO 
                        // ESTO INDICA QUE EXISTEN TRES ESTRUCTURAS;
                        // [xx][0] <- reservado para uso futuro (enemigos, que se yo...)
                        // [xx][1] <- disparos jugador #1
                        // [xx][2] <- disparos jugador #2
                        // la 'xx' representa el numero de disparo 0 a MAX_SHOOTS-1

//--------------------------------------------------------
// Codigo de las funciones
//--------------------------------------------------------

//--------------------------------------------------------
// reset_disparo()
// resetea la estructura de los disparos
// llamar cada vez que empieza un nuevo juego
//--------------------------------------------------------

void reset_disparo()
{
int i1, i2; // para los fors

for (i1 = 0; i1 < MAX_SHOOTS ; i1++)
{
 for (i2=0; i2 < 3; i2++)
 {
    dispa[i1][i2].x = 0;
    dispa[i1][i2].y = 0;
    dispa[i1][i2].vida = 0;
    dispa[i1][i2].frame = 0;
    dispa[i1][i2].direccion = 0;
    dispa[i1][i2].vel = 0;
    dispa[i1][i2].tipo = 0;

    disp_libre[i2] = 0;
 } // fin for i2
} // fin for i1


} // fin reset_disparo
//--------------------------------------------------------


//--------------------------------------------------------
// agregar_disp(x,y,vida,dir,vel,tipo,indice);
// Agrega un disparo en el lugar deseado  x,y
// con 'vida', y 'dir'(0-255) y 'vel' de 'tipo'
// y en indice [0,1,2] (numero de jugador)
// DESCUENTA LA CANTIDAD DE BALAS!
// SOLO si hay un disparo libre...
//--------------------------------------------------------
void agregar_disp(int x, int y, int vida, int dir,
                  int vel, int tipo, int indice)
{

int i1=0, i2=0; // para alivianar la escritura... ;D

i2 = indice;
i1 = disp_libre[indice];

// ver si hay disparo disponible
if (disp_libre[i2] < 0) return;
if (disp_libre[i2] > (MAX_SHOOTS - 1) ) { disp_libre[i2] = -1; return; }

if (vel<1) vel = 1;


if (i1 < MAX_SHOOTS ) {
    dispa[i1][i2].x = x;
    dispa[i1][i2].y = y;
    dispa[i1][i2].vida = vida;
    dispa[i1][i2].frame = random() % 30; // 0..29...
    dispa[i1][i2].direccion = dir;
    dispa[i1][i2].vel = vel;
    dispa[i1][i2].tipo = tipo;

    jugador_info[indice].balas--; // descontar las balas al jugador
    }

disp_libre[i2] = -1; // todavia no se cual es el prox. disparo libre...

} // fin agregar_disp
//--------------------------------------------------------


//--------------------------------------------------------
// mover_disp();
// Mueve los disparos de todos los jugadores
// Comprueba que no choquen paredes / jugadores
// Si chocan paredes, las rompe
// Si chocan jugadores, les saca energia al jugador tocado
// Si se les acaba la vida, los termina
// Si choca una pared / jugador, impulsa una
// explosion proporcional al tipo de arma
//--------------------------------------------------------
void mover_disp()
{
int i1, i2; // para alivianar la escritura... ;D
int xju, yju, jug; // posicion del jugador enemigo, para verificar si lo choco

ANGULO_TYPE angulotmp; // estructura temporal para recuperar la posicion
                       // en base a los angulos...


// recorrer del 1 al 2 (indice) (EL 0 NO LO TOCO PORQUE NO ES USADO TODAVIA)
for (i2=1; i2<3; i2++)
{

// por ahora, no se que disparo esta libre
disp_libre[i2] = -1;

// recorrer del 0 a MAX_SHOOTS - 1
for (i1=0; i1 < MAX_SHOOTS ; i1++)
    {

    // si el disparo esta vivo
  if ( dispa[i1][i2].vida > 0 ) {

    // ajustar las coordenadas de acuerdo a la direccion del disparo
    // mover en el angulo especificado
    // NOTAR QUE ES EL INVERSO 0-[XXXX]
    angulotmp = dame_angc360(0 - pasa255a360(dispa[i1][i2].direccion),
                             dispa[i1][i2].vel / 10,
                             dispa[i1][i2].x,
                             dispa[i1][i2].y);

    // nueva posicion
    dispa[i1][i2].x = angulotmp.x2;
    dispa[i1][i2].y = angulotmp.y2;

    // ver si se salio de pantalla, matar pasando vida a 0...
    if ((dispa[i1][i2].x<0) || (dispa[i1][i2].y<0)
       || (dispa[i1][i2].x>1000) || (dispa[i1][i2].x>1000)) dispa[i1][i2].vida = 0;


    // restar vida - 1
        dispa[i1][i2].vida--;


    //  COMPROBAR SI CHOCO CON ALGO

    // coordenadas para choque con tanque enemigo
    if (i2 == 1)
       {xju = jugador_info[2].posx; yju = jugador_info[2].posy; jug = 2; }
    else
       {xju = jugador_info[1].posx; yju = jugador_info[1].posy; jug = 1; }

    // ver si choco con un tanque
    // notar que uso xju, porque el tanque 'cuelga' de xju (xju,yju) no es el centro, sino esq. sup izq
    if ( (dispa[i1][i2].x > xju - 5) &&
         (dispa[i1][i2].x < xju + JUG_R_MAX + 5) &&
         (dispa[i1][i2].y > yju - 5) &&
         (dispa[i1][i2].y < yju + JUG_R_MAX + 5) )
    {
         dispa[i1][i2].vida = 0; // eliminar disparo
         // si el jugador tiene un escudo, restar al escudo
         if (jugador_info[jug].premio == ESCUDO_PREMIO)
         {
  
            if ( ( jugador_info[jug].c_premio -= (random()%3 + 3 +  (dispa[i1][i2].tipo*2 )) ) < 0)
                {
                jugador_info[i2].premio = 0;
                jugador_info[i2].c_premio = 0;
                }
         }
         else
         {
         // restar energia al jugador
             jugador_info[jug].energia -= random()%3 + 3 +  (dispa[i1][i2].tipo*2) ;
         }


//         if (dispa[i1][i2].tipo != A_LANZALLAMAS)
//         {
             // agregar explosion
             combo_expl(xju + random() % 20 - 10 + (JUG_X_MAX/2),
                        yju + random() % 20 - 10 + (JUG_Y_MAX/2),
                        random() % 30 + 20,
                        random() % 30 + 30, 3);
//         }
//         else
//         {
//            // incendio [lanzallamas]
//            agregar_incendio(xju + random() % 20 - 10 + (JUG_X_MAX/2),
//                             yju + random() % 20 - 10 + (JUG_Y_MAX/2),
//                             random()%10 + 10,
//                             random()%10 + 10);
//         }               

    } // fin ver si choco con tanque...


    // ver si choco con una pared...
    if ( grilla_map[dispa[i1][i2].x / SPR_X_MAX][dispa[i1][i2].y / SPR_Y_MAX] > AGUA )
        {
        dispa[i1][i2].vida = 0;

        // quemar pared
        quemar_pared(Areajuego_bmp, dispa[i1][i2].x, dispa[i1][i2].y, random()%5 + 5 + dispa[i1][i2].tipo, BALAZO);

        // restar energia correspondiente al disparo y comprobar si esta rota
      if ( ( grilla_mapE[dispa[i1][i2].x / SPR_X_MAX]
                        [dispa[i1][i2].y / SPR_Y_MAX]
                        -= random()%5 + dispa[i1][i2].tipo + 2 )  < 0 ) {

                // redibujar el piso nuevo (rutina auxiliar de stargam.c) DEBUG
                // dibujar_rectangulo_mapa_1(Areajuego_bmp,
                //                          dispa[i1][i2].x / SPR_X_MAX,
                //                          dispa[i1][i2].y / SPR_Y_MAX ); // (bmp, x,y)

                // eliminar pared en la grilla de memoria
                   grilla_map[dispa[i1][i2].x / SPR_X_MAX][dispa[i1][i2].y / SPR_Y_MAX] = info_mapa_juego.terreno_gral;

                // llamar a la rutina de quemado final
                destruir_pared(Areajuego_bmp,
                               dispa[i1][i2].x / SPR_X_MAX,
                               dispa[i1][i2].y / SPR_Y_MAX ); // (bmp, x,y)


               } // fin restar energia a la pared
        }

    // si el disparo murio, marcar como libre y agregar explosion
    if ( dispa[i1][i2].vida < 1 )
        {
        disp_libre[i2] = i1; // este disparo esta libre!
        if (disp_libre[i2] > (MAX_SHOOTS - 1) )  disp_libre[i2] = -1;
        // agregar explosion - proporcional al tipo de disparo
            switch (dispa[i1][i2].tipo) {
                   case A_AMETRALLADORA: // ametralladora
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 15 + 10,
                               random() % 15 + 5 , 2);
                   break;
                   case A_CANON        : // ca¤on (bola girando!) 
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 30 + 20,
                               random() % 30 + 30, 3);
                   break;
                   case A_LASER: // laser 
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 10 + 10,
                               random() % 40 + 20, 2);

                   break;
                   case A_PLASMA: // plasma
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 20 + 20,
                               random() % 20 + 20, 1);
                   break;

                   case A_LANZALLAMAS: // lanzallamas [dura mas la explo porque queda incendiado!]
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 40 + 40,
                               random() % 20 + 15, 3);
                      // incendiar! WOW 
//                      agregar_incendio(dispa[i1][i2].x,
//                                       dispa[i1][i2].y,
//                                       random()%10 + 10,
//                                       random()%10 + 10);
                   break;

                   case A_TRIPLE_SHOOT: // triple disparo?
                    combo_expl(dispa[i1][i2].x,
                               dispa[i1][i2].y,
                               random() % 20 + 20,
                               random() % 20 + 15, 3);
                   break;

                   }
        }
        else // si el disparo esta vivo, agregar luz donde esta el disparo
        {
            if (dispa[i1][i2].tipo != A_AMETRALLADORA) {
                agregar_luz(dispa[i1][i2].x,
                            dispa[i1][i2].y,
                            random()%5 + 5, 
                            random()%50 + 205,
                            random()%20 + 20,
                            -(random()%3) );
                        } else {
                        // ametralladora, caso especial, destellos
                agregar_luz(dispa[i1][i2].x,
                            dispa[i1][i2].y,
                            random()%5 + 5, 
                            200,
                            random()%10 + 10,
                            -(random()%3) );
                        }
        }
    // actualizar frame
        dispa[i1][i2].frame++;
        if (dispa[i1][i2].frame > 30) dispa[i1][i2].frame=0; 

  } // fin ver si esta vivo
  else  // sino esta vivo
  {
    // disparo muerto, libre
    disp_libre[i2] = i1;
    if (disp_libre[i2] > (MAX_SHOOTS - 1) )  disp_libre[i2] = -1;
  }

    } // fin for i1
} // fin for i2


} // fin mover_disp
//--------------------------------------------------------


//--------------------------------------------------------
// Esta rutina dibuja los disparos, transportados
// a las coordenadas x,y en el bitmap
//--------------------------------------------------------
void dibujar_disparos(BITMAP *bmp, int x, int y)
{
int i1, i2; //para los for , etc


// recorrer del 1 al 2 (indice) (EL 0 NO LO TOCO PORQUE NO ES USADO TODAVIA)
for (i2=1; i2<3; i2++)
{
// ya que estamos, ubico un disparo libre
disp_libre[i2] = -1;

for (i1 = 0; i1 < MAX_SHOOTS; i1++) {

    if (dispa[i1][i2].vida > 0) {

            // ver que grafico hay que usar y graficarlo adecuadamente
            // [notar el - (X_SPR_ARMA/2) para centrarlo en relacion a la torreta...
            switch (dispa[i1][i2].tipo) {
                   case A_AMETRALLADORA: // ametralladora
                    rotate_sprite(bmp,
                                  krono_dato[BALA].dat,
                                  dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                  dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                  itofix(dispa[i1][i2].direccion) );

                   break;
                   case A_CANON        : // ca¤on (bola girando!) 
                    rotate_sprite(bmp,
                                  krono_dato[BOLA].dat,
                                  dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                  dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                  itofix( ( dispa[i1][i2].direccion * dispa[i1][i2].frame) )
                                  );
                   break;
                   case A_LASER: // laser  
                    rotate_sprite(bmp,
                                  krono_dato[LASER].dat,
                                  dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                  dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                  itofix(dispa[i1][i2].direccion) );
                   break;
                   case A_PLASMA: // plasma  [gira de acuerdo al frame]
                    rotate_sprite(bmp,
                                  krono_dato[RAYO].dat,
                                  dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                  dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                  itofix(dispa[i1][i2].direccion) * dispa[i1][i2].frame );
                   break;

                   case A_LANZALLAMAS: // lanzallamas  
                       dispa[i1][i2].frame = random() % 30; // 0..29...  DEBUG - CORREGIR!
                       if (dispa[i1][i2].frame < 10) rotate_sprite(bmp,
                                                                    krono_dato[FUEGO1].dat,
                                                                    dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                                                    dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                                                    itofix(dispa[i1][i2].direccion) );
                       else
                       {
                       if ((dispa[i1][i2].frame > 10 ) && (dispa[i1][i2].frame < 20 ))
                            rotate_sprite(bmp,
                                          krono_dato[FUEGO2].dat,
                                          dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                          dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                          itofix(dispa[i1][i2].direccion) );
                            else
                            rotate_sprite(bmp,
                                          krono_dato[FUEGO3].dat,
                                          dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                          dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                          itofix(dispa[i1][i2].direccion) );
                      } //fin else del frame < 10
                   break;

                   case A_TRIPLE_SHOOT: // triple disparo
                    rotate_sprite(bmp,
                                  krono_dato[TRIPLE_SHOOT].dat,
                                  dispa[i1][i2].x - x - (X_SPR_ARMA/2),
                                  dispa[i1][i2].y - y - (Y_SPR_ARMA/2),
                                  itofix(dispa[i1][i2].direccion) );
                   break;


                   default: // oops, marcar con un punto...! DEBUG
                      putpixel (bmp,
                      dispa[i1][i2].x - x,
                      dispa[i1][i2].y - y,
                      248);

            } // fin switch

    } else  { disp_libre[i2] = i1; } // disparo libre

} // fin for i1
} // fin for i2

}

//--------------------------------------------------------
// Chequea si el jugador "j" tiene balas.
// Si no tiene, le devuelve la ametralladora. ;^P
//--------------------------------------------------------
void check_balas(int j)
{
// que no tenga mas balas de las permitidas
if (jugador_info[j].balas > MAX_C_BALAS) jugador_info[j].balas = MAX_C_BALAS;

// si se queda sin balas, cambiar arma a ametralladora
if (jugador_info[j].balas < 1)
   {
   jugador_info[j].balas = 0;
   jugador_info[j].arma = 0;
   }
}

//--------------------------------------------------------
// combo_disparar
// crea un disparo en la punta de la torreta del jugador 'j'
// del tipo de arma que tiene, con todas las propiedades
// del arma.
// El parametro deciseg debe ser la var deciseg de game.c!!!
// SOLO dispara si ha pasado el tiempo suficiente entre disparos
// usando la var jugador_info[j].deciseg en relacion a global deciseg de game.c
// SOLO dispara si tiene balas (excepto para la ametralladora)
// Si se quedo sin balas, devuelve la ametralladora
//--------------------------------------------------------
void combo_disparar(int j, unsigned long deciseg)
{

int vida = 20, vel = 100; // ajustar de acuerdo al arma

check_balas(j); // chequear las balas y arma a usar

            switch (jugador_info[j].arma) {
                   case A_AMETRALLADORA: // ametralladora
                        vida = 20; vel = 90;

                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) < 20) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;
                   break;
                   case A_CANON        : // ca¤on (bola girando!) 
                        vida = 25; vel = 110;
                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) < 40) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;

                   break;
                   case A_LASER: // laser 
                        vida = 30; vel = 120;
                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) < 30) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;

                   break;
                   case A_PLASMA: // plasma
                        vida = 25; vel = 130;
                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) <  30) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;

                   break;

                   case A_LANZALLAMAS: // lanzallamas 
                        vida = 15; vel = 80;
                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) < 10) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;

                   break;

                   case A_TRIPLE_SHOOT: // triple disparo
                        vida = 20; vel = 95;
                        // comprobar tiempo disparo
                        if (abs(deciseg - jugador_info[j].deciseg) < 10) return; // muy rapido, abortar...
                        jugador_info[j].deciseg = deciseg;

                   break;

            } // fin switch

            //x,y,vida,direccion (0..255), velocidad * 100, arma, jugador
            agregar_disp(jugador_info[j].xpol[10],
                         jugador_info[j].ypol[10],
                         vida,
                         jugador_info[j].ang_tor,
                         vel,
                         jugador_info[j].arma,
                         j);

            // agregar luz, y destello de disparo
            // (x,y,vida,intensidad,radio,crecimiento)
            agregar_luz(jugador_info[j].xpol[10],
                        jugador_info[j].ypol[10],
                        random()%10 + 10, 
                        random()%50 + 205,
                        random()%20 + 10,
                        -1);

            agregar_expl(jugador_info[j].xpol[10],
                         jugador_info[j].ypol[10],
                         random()%10 + 10, 
                         random()%10 + 10,
                         random()%3 + 3);
                         
            
}
#endif
