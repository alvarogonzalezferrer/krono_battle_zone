//--------------------------------------------------------
// pc_ia.c
// pc_ia.h
//
// RUTINAS QUE CONTIENEN LA INTELIGENCIA ARTIFICIAL
// DEL JUGADOR DE LA PC
// 
// Por Kronoman - DJGPP
//--------------------------------------------------------
// NOTA UTIL:
// En el juego, los cuadrantes estan transportados asi:
//           270 
//            |    
//  180  -----+-----  0 - 360
//            |    
//            90  
//
//--------------------------------------------------------
#ifndef PC_IA_C
#define PC_IA_C
//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE IA

// Esto contiene las acciones planeadas a realizar
// para cada jugador
static struct {

    int c_mover; // cantidad de movimientos restantes a hacer
                     // antes de cambiar estrategia

    int ang_dir; // angulo del cuerpo del tanque deseado 0-255

    int ang_tor; // angulo de la torreta deseado

    int disparar; // debo disparar? FALSE / TRUE

    int c_disparar; // cuantos disparos?

    int vel; // velocidad deseada... (negativo = marcha atras)

    int energia; // energia anterior (para ver si me pegaron!)

    int contra_pared; // FALSE o TRUE, indica si estoy contra una pared [para romperla]

    // esto para evitar quedarse 'pegado'
    int estoy_pegado; // TRUE / FALSE
    // contador de cuanto ha estado pegado, al llegar a > 50, empieza a disparar y moverse al azar un rato
    int c_pegado;
    // para darse cuenta que estoy pegado
    int x_old;
    int y_old;
    int o_ang;
    int to_ang;

} IA_inf[3]; // desde 0 a 2 [1 Y 2 = JUGADOR 1, 2, 0 RESERVADO]


//--------------------------------------------------------
// Codigo de las funciones
//--------------------------------------------------------

//--------------------------------------------------------
// Inicializa la IA
//--------------------------------------------------------
void reset_IA()
{
int i;

for (i=0;i<3; i++)
    {
     IA_inf[i].c_mover = 0;
     IA_inf[i].ang_dir = 0;
     IA_inf[i].ang_tor = 0;
     IA_inf[i].disparar = FALSE;
     IA_inf[i].c_disparar =0;
     IA_inf[i].vel = 0;
     IA_inf[i].energia = 0;
     IA_inf[i].contra_pared = FALSE;
     IA_inf[i].estoy_pegado = FALSE; 
     IA_inf[i].c_pegado = 0;
     IA_inf[i].x_old = 0;
     IA_inf[i].y_old = 0;
     IA_inf[i].o_ang = 0;
     IA_inf[i].to_ang = 0;
    }

}

//--------------------------------------------------------
// Auxilar, devuelve el angulo de x1,y1 en relacion a x2,y2
// Usa la funcion atan de math.h!
// Lo devuelve en angulos 0..255
//--------------------------------------------------------
int angulo_x1_y1_x2_y2(int x1, int y1, int x2, int y2)
{

// vars necesarias
int d1;
double t;

// ver alineacion vertical
if ( (x1 > x2 - 1) &&
     (x1 < x2 + 1) ) {
     // ver arriba, abajo
     if (y1 > y2) { // enemigo abajo
            return pasa360a255(al_azar(80,100));
     } else { // enemigo arriba
            return pasa360a255(al_azar(260,280));
     }
   } // fin alineacion vertical

// ver alineacion horizontal
if ( (y1 > y2 - 1) &&
     (y1 < y2 + 1) ) {
     // ver derecha, izquierda
     if (x1 < x2) { // enemigo izquierda
            return pasa360a255(al_azar(170,190));
     } else { // enemigo derecha
            return pasa360a255(al_azar(370, 380));
     }
   } // fin alineacion horizontal

// ver cuadrantes diagonales
// usar la arcotangente
// primero saco la tangente (y2-y1) / (x2-x1)
    d1 = (x2 - x1);

    if (d1 == 0) // evitar bug con tangente indeterminada
        {
        if (y2 < y1 ) { return pasa360a255(270) ;} else {return pasa360a255(90);} // estan al reves, a proposito!
        }

    t = (y2 - y1) ;
    t = t / (double) d1;  // el cast (double) es MUY importante!
    t = atan(t) / KRONO_PI; // divido por pi/180 para pasar rad a grados

    d1 = arregla_ang((int) t); // redondea

    // DEBUG - cambiar cuadrante (90 = 270!)
    if (x1 < x2 ) d1 = arregla_ang(d1 - 180);



    return pasa360a255(d1);

}


//--------------------------------------------------------
// Auxilar, devuelve el angulo donde esta el enemigo
// en 0..255
// Pasar en j el enemigo, en yo, el jugador mio
// NOTAR que usa un metodo de aproximacion!!!
//--------------------------------------------------------
int dame_angulo_enemigo(int j, int yo)
{
int x1, y1, x2, y2; // auxiliares

// ajustar a grilla
x1 = jugador_info[j].posx / SPR_X_MAX;
y1 = jugador_info[j].posy / SPR_Y_MAX;

x2 = jugador_info[yo].posx / SPR_X_MAX;
y2 = jugador_info[yo].posy / SPR_Y_MAX;

return angulo_x1_y1_x2_y2(x1, y1, x2, y2);

}

//--------------------------------------------------------
// Auxilar, devuelve TRUE si el numero esta entre 0..49
// y false si no...
//--------------------------------------------------------
int num_0_49(int n)
{
    if (n < 0) return FALSE;
    if (n >49) return FALSE;
    return TRUE;
}

//--------------------------------------------------------
// Auxilar, devuelve el angulo donde esta la pared proxima
// Si no hay paredes proximas, devuelve la pos del enemigo
// Pasar en j el jugador mio, pasar en ene el enemigo...
// NOTAR que usa un metodo de aproximacion!!!
// Setea  contra_pared a TRUE o FALSE...
//--------------------------------------------------------
int dame_angulo_pared(int j, int ene)
{
int x1, y1, i1; // auxiliares
int x3 = -1 , y3 = -1; // coordenadas de la pared (-1, -1 = ninguna...)
int mx[11]; // las uso para priorizar la verificacion del poligono
// 5----4----3
// |    |    |
// 6----1----2 ===> FRENTE DEL TANQUE ->
// |    |    |
// 7----8----9
// El 0 y el 10 son de la torreta
// orden de verificacion... ;^D 

mx[0] = 1;
mx[1] = 2;
mx[2] = 3;
mx[3] = 9;
mx[4] = 4;
mx[5] = 8;
mx[6] = 6;
mx[7] = 5;
mx[8] = 7;
mx[9] = 10; 
mx[10] =0;


IA_inf[j].contra_pared = TRUE; // asumo default

// verificar alrededores del perimetro del tanque...
// puntos 1 al 9, bordes del tanque...
// favorece la comprobacion de acuerdo a la posicion de la direccion del tanque
for (i1 =0 ; i1 <9 ; i1++)
{
// tomar coordenadas y ajustar a grilla
    x1 = jugador_info[j].xpol[ mx[i1] ] / SPR_X_MAX; 
    y1 = jugador_info[j].ypol[ mx[i1] ] / SPR_Y_MAX;

// priorizar segun la direccion del jugador
// dispararle primero a lo que este en el camino del jugador

// va hacia arriba 
if ( ( pasa255a360(jugador_info[j].ang_dir) > 260  ) &&  ( pasa255a360(jugador_info[j].ang_dir) < 280  ) )
   {
    // arriba
         if ((num_0_49(x1-1)) && (num_0_49(y1-1)))
           if (grilla_map[x1-1][y1-1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1)) && (num_0_49(y1-1)))
           if (grilla_map[x1][y1-1] > AGUA)
              {
              x3 = x1 ;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1+1)) && (num_0_49(y1-1)))
           if (grilla_map[x1+1][y1-1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }
    // centro
        if ((num_0_49(x1-1)) && (num_0_49(y1)))
           if (grilla_map[x1-1][y1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }


        if ((num_0_49(x1+1)) && (num_0_49(y1)))
           if (grilla_map[x1+1][y1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }
    // abajo
        if ((num_0_49(x1-1)) && (num_0_49(y1+1)))
           if (grilla_map[x1-1][y1+1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }

    if ((num_0_49(x1)) && (num_0_49(y1+1)))
       if (grilla_map[x1][y1+1] > AGUA)
          {
              x3 = x1 ;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

          }
    if ((num_0_49(x1+1)) && (num_0_49(y1+1)))
       if (grilla_map[x1+1][y1+1] > AGUA)
          {
              x3 = x1 + 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
          }
  } // fin entre 260 y 280

// va hacia abajo
if ( ( pasa255a360(jugador_info[j].ang_dir) > 80  ) &&  ( pasa255a360(jugador_info[j].ang_dir) < 100  ) )
   {
    // abajo
        if ((num_0_49(x1-1)) && (num_0_49(y1+1)))
           if (grilla_map[x1-1][y1+1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }

    if ((num_0_49(x1)) && (num_0_49(y1+1)))
       if (grilla_map[x1][y1+1] > AGUA)
          {
              x3 = x1 ;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

          }
    if ((num_0_49(x1+1)) && (num_0_49(y1+1)))
       if (grilla_map[x1+1][y1+1] > AGUA)
          {
              x3 = x1 + 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
          }
    // arriba
         if ((num_0_49(x1-1)) && (num_0_49(y1-1)))
           if (grilla_map[x1-1][y1-1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1)) && (num_0_49(y1-1)))
           if (grilla_map[x1][y1-1] > AGUA)
              {
              x3 = x1 ;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1+1)) && (num_0_49(y1-1)))
           if (grilla_map[x1+1][y1-1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }
    // centro
        if ((num_0_49(x1-1)) && (num_0_49(y1)))
           if (grilla_map[x1-1][y1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }


        if ((num_0_49(x1+1)) && (num_0_49(y1)))
           if (grilla_map[x1+1][y1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }

  } // fin entre 80 y 100

// va hacia izquierda
if ( ( pasa255a360(jugador_info[j].ang_dir) > 109  ) ||  ( pasa255a360(jugador_info[j].ang_dir) < 261 ) )
   {
         if ((num_0_49(x1-1)) && (num_0_49(y1-1)))
           if (grilla_map[x1-1][y1-1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1-1)) && (num_0_49(y1)))
           if (grilla_map[x1-1][y1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1-1)) && (num_0_49(y1+1)))
           if (grilla_map[x1-1][y1+1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }


        if ((num_0_49(x1)) && (num_0_49(y1-1)))
           if (grilla_map[x1][y1-1] > AGUA)
              {
              x3 = x1 ;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1+1)) && (num_0_49(y1-1)))
           if (grilla_map[x1+1][y1-1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }


        if ((num_0_49(x1+1)) && (num_0_49(y1)))
           if (grilla_map[x1+1][y1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }
    if ((num_0_49(x1)) && (num_0_49(y1+1)))
       if (grilla_map[x1][y1+1] > AGUA)
          {
              x3 = x1 ;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

          }
    if ((num_0_49(x1+1)) && (num_0_49(y1+1)))
       if (grilla_map[x1+1][y1+1] > AGUA)
          {
              x3 = x1 + 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
          }
  } // fin a la izquierda


// va hacia derecha
if ( ( pasa255a360(jugador_info[j].ang_dir) > 279  ) &&  ( pasa255a360(jugador_info[j].ang_dir) < 81  ) )
   {
    if ((num_0_49(x1+1)) && (num_0_49(y1+1)))
       if (grilla_map[x1+1][y1+1] > AGUA)
          {
              x3 = x1 + 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
          }

        if ((num_0_49(x1+1)) && (num_0_49(y1-1)))
           if (grilla_map[x1+1][y1-1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1+1)) && (num_0_49(y1)))
           if (grilla_map[x1+1][y1] > AGUA)
              {
              x3 = x1 + 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }


        if ((num_0_49(x1-1)) && (num_0_49(y1+1)))
           if (grilla_map[x1-1][y1+1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

              }

    if ((num_0_49(x1)) && (num_0_49(y1+1)))
       if (grilla_map[x1][y1+1] > AGUA)
          {
              x3 = x1 ;
              y3 = y1 + 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);

          }

         if ((num_0_49(x1-1)) && (num_0_49(y1-1)))
           if (grilla_map[x1-1][y1-1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

        if ((num_0_49(x1)) && (num_0_49(y1-1)))
           if (grilla_map[x1][y1-1] > AGUA)
              {
              x3 = x1 ;
              y3 = y1 - 1;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }


        if ((num_0_49(x1-1)) && (num_0_49(y1)))
           if (grilla_map[x1-1][y1] > AGUA)
              {
              x3 = x1 - 1;
              y3 = y1 ;
              return angulo_x1_y1_x2_y2((x3 * SPR_X_MAX) + (SPR_X_MAX/2),
                                        (y3 * SPR_Y_MAX) + (SPR_Y_MAX/2),
                                         jugador_info[j].xpol[ mx[i1] ],
                                         jugador_info[j].ypol[ mx[i1] ]);
              }

  } // fin entre 80 y 100



} // fin for i1



// no hay pared, devolver normal
         IA_inf[j].contra_pared = FALSE;
         return dame_angulo_enemigo(ene, j);
}

//--------------------------------------------------------
// Devuelve un angulo para el cuerpo del tanque apropiado
// a la situacion.
// Si esta lejos del enemigo, trata de acercarse.
// Si hay un premio cerca y lo precisa, lo agarra.
// NOTAR que usa un metodo de aproximacion!!!
// Pasar en j el jugador, y en ene el enemigo
//--------------------------------------------------------
int dame_angulo_cuerpo_tanque(int j, int ene)
{
int i1; // para el for
int di; //distancia a premio [estimativa]
int dm; // distancia minima [para buscar el mas cercano]
int ang; // angulo

dm = JUG_R_MAX * 6;
ang = dame_angulo_enemigo(ene, j);


// ver si tengo un premio cerca

for (i1 = 0; i1 < MAX_PREMIOS; i1++)
{
    di = abs( premio[i1].x - jugador_info[j].posx  )  + abs( premio[i1].y  - jugador_info[j].posy );
    if (di < dm)
        {
        dm = di;
        ang = angulo_x1_y1_x2_y2(premio[i1].x / SPR_X_MAX,
                                 premio[i1].y / SPR_Y_MAX,
                                 jugador_info[j].posx / SPR_X_MAX,
                                 jugador_info[j].posy / SPR_Y_MAX );
        }

} // fin for de los premios

// devolver angulo del enemigo 
return  ang;

}


//--------------------------------------------------------
// Mueve el jugador 'j' de acuerdo a la IA...
// En unsigned long deciseg pasar la var 'deciseg' definida en game.c!!!
//--------------------------------------------------------
void mover_IA(int j , unsigned long deciseg)
{
int ene; // enemigo
if (j == 1)
    ene = 2;
else
    ene = 1;

// ---------- Implementacion de la IA -------------
if (--IA_inf[j].c_mover < 1)
    {

// MOVIMIENTO DEL CUERPO
     IA_inf[j].c_mover = random()%20+ (40 - (NIVEL_IA * 10) ) ; // corregir seguido la trayectoria

     IA_inf[j].ang_dir = dame_angulo_cuerpo_tanque(j, ene);  // buscar premios, etc

     // si esta muy lejos, avanzar sin agarrar nada
     if ( (distancia_entre_jugadores() > JUG_R_MAX * 10) && jugador_info[j].energia > MAX_ENERGIA * .8 )
                IA_inf[j].ang_dir =  dame_angulo_enemigo(ene,j);

     if ( (distancia_entre_jugadores() < JUG_R_MAX * 3))
         IA_inf[j].vel = al_azar( -MAX_VEL_TA , MAX_VEL_TA);  // al azar
     else
         IA_inf[j].vel = al_azar( MAX_VEL_TA/2 , MAX_VEL_TA); // avanzar

// si esta muy cerca, alejarse
     if (distancia_entre_jugadores() < JUG_R_MAX * 2)
            IA_inf[j].vel = al_azar( -MAX_VEL_TA , -MAX_VEL_TA/2); // marcha atras



// TORRETA 
     IA_inf[j].ang_tor = dame_angulo_pared(j, ene); // esto toma el angulo de la pared, o enemigo mas cercano

     if (distancia_entre_jugadores() < JUG_R_MAX * 7)
         IA_inf[j].disparar =  TRUE; 
     else
         IA_inf[j].disparar = (al_azar(0,100) < 30) ? TRUE : FALSE;

     IA_inf[j].c_disparar = random()%25 + 15;

     // si hay una pared, disparar si o si...
     if (IA_inf[j].contra_pared)
        {
        IA_inf[j].disparar =  TRUE;
        IA_inf[j].ang_tor = dame_angulo_pared(j, ene);
        }

     // si estoy muy cerca del enemigo, disparar al enemigo e ignorar las paredes...
     if ( (distancia_entre_jugadores() < JUG_R_MAX *7) && (random()%1000>100) )
                {
                    IA_inf[j].ang_tor = dame_angulo_enemigo(ene, j) ;
                    IA_inf[j].disparar =  TRUE; 
                }

} // FIN IA

// TORRETA

    dame_angulo_pared(j, ene);

     // si hay una pared, disparar si o si...
     if (IA_inf[j].contra_pared)
        {
        IA_inf[j].disparar =  TRUE;
        IA_inf[j].ang_tor = dame_angulo_pared(j, ene);
        }

     // si estoy muy cerca del enemigo, disparar al enemigo e ignorar las paredes...
     if ( (distancia_entre_jugadores() < JUG_R_MAX * 7) && (random()%1000>100) )
                {
                    IA_inf[j].ang_tor = dame_angulo_enemigo(ene, j) ;
                    IA_inf[j].disparar =  TRUE; 
                }


    // si me estan sacando energia, contraatacar
    if (  IA_inf[j].energia > jugador_info[j].energia ) 
            {
                IA_inf[j].ang_tor = dame_angulo_enemigo(ene, j) ;
                IA_inf[j].disparar =  TRUE;
                // peque¤o truco, si la torreta mia esta cerca, la teletransporto...
                    if (abs(jugador_info[j].ang_tor -  IA_inf[j].ang_tor) < VEL_GTO * 3) jugador_info[j].ang_tor =  IA_inf[j].ang_tor;
                // rajar
                if (abs(IA_inf[j].vel) < MAX_VEL_TA/2) IA_inf[j].vel = al_azar( -MAX_VEL_TA , MAX_VEL_TA); 
            }



// -----------------------------------
// Evitar quedarse pegado
/******************* anulado
if ( ( IA_inf[j].x_old / SPR_X_MAX == jugador_info[j].posx / SPR_X_MAX ) && ( IA_inf[j].y_old / SPR_Y_MAX == jugador_info[j].posy / SPR_Y_MAX ) ) 
     {
       IA_inf[j].c_pegado++;
       if (IA_inf[j].c_pegado > 100) IA_inf[j].estoy_pegado = TRUE;
    } // fin evitar quedarse pegado
    else
       {  IA_inf[j].c_pegado--; }

// si esta pegado, agarrarse epilepsia al azar...
if (IA_inf[j].estoy_pegado == TRUE)
    {

     IA_inf[j].c_mover = random()%10+10;
     IA_inf[j].ang_dir = random()%255;
     IA_inf[j].ang_tor = random()%255;
     IA_inf[j].disparar = TRUE;
     IA_inf[j].c_disparar = random()%20+20;
     IA_inf[j].vel = al_azar( -MAX_VEL_TA , MAX_VEL_TA);
     IA_inf[j].estoy_pegado = FALSE;
     IA_inf[j].c_pegado = 0;
    } // fin estar pegado - azar
******************* anulado **********/
// -----------------------------------

// ALEJARSE DE LOS BORDES DEL MAPA
if (jugador_info[j].posx / SPR_X_MAX < 3) // borde izquierdo
   {
    // marcha atras? adelante?
    if ( (pasa255a360(jugador_info[j].ang_dir) < 270) && (pasa255a360(jugador_info[j].ang_dir) > 90) )
        // apuntando al borde, marcha atras!
        IA_inf[j].vel = -MAX_VEL_TA;
    else
        IA_inf[j].vel = MAX_VEL_TA; // apuntando al otro lado, rajar!

   } // fin borde izq

if (jugador_info[j].posx / SPR_X_MAX > 47) // borde derecho  
   {
    // marcha atras? adelante?
    if ( (pasa255a360(jugador_info[j].ang_dir) < 270) && (pasa255a360(jugador_info[j].ang_dir) > 90) )
        // marcha adelante
        IA_inf[j].vel = MAX_VEL_TA;
    else
        IA_inf[j].vel = -MAX_VEL_TA; // apuntando al borde, marcha atras!

   } // fin borde derecho

if (jugador_info[j].posy / SPR_Y_MAX < 2) // borde superior 
   {
    // marcha atras? adelante?
    if ( (pasa255a360(jugador_info[j].ang_dir) < 360) && (pasa255a360(jugador_info[j].ang_dir) > 180) )
        // apuntando al borde, marcha atras!
        IA_inf[j].vel = -MAX_VEL_TA;
    else
        IA_inf[j].vel = MAX_VEL_TA; // apuntando al otro lado, rajar!

   } // fin borde sup


if (jugador_info[j].posy / SPR_Y_MAX > 47) // borde inferior
   {
    if ( (pasa255a360(jugador_info[j].ang_dir) > 180) && (pasa255a360(jugador_info[j].ang_dir) < 0) )
        IA_inf[j].vel = -MAX_VEL_TA; // apuntando abajo, marcha atras...
    else
        IA_inf[j].vel = MAX_VEL_TA; 

   } // fin borde sup
// -----------------------------------

// -------- especifico de la IA para hacerla 'mas tonta' en caso de ser mas facil

if ( (NIVEL_IA == 1) && (!IA_inf[j].contra_pared) ) // medio
   {
    if (random()%100 < 45)
            {
                IA_inf[j].ang_tor = dame_angulo_enemigo(ene, j)  + al_azar(-16,16);
                if (random()%100 < 50)
                    {
                    IA_inf[j].disparar =  FALSE;
                    }
                    else
                    {
                    IA_inf[j].disparar =  TRUE;
                    }
            }
   }

if ( (NIVEL_IA == 0) && (!IA_inf[j].contra_pared) ) // facil
   {
    if (random()%100 < 70)
            {
                IA_inf[j].ang_tor = dame_angulo_enemigo(ene, j)  + al_azar(-64,64);
                if (random()%100 < 70)
                    {
                    IA_inf[j].disparar =  FALSE;
                    }
                    else
                    {
                    IA_inf[j].disparar =  TRUE;
                    }
            }
   }



// ---------- seguridad -----------
    // seguridad
     do {
            if (IA_inf[j].ang_tor < 0)  IA_inf[j].ang_tor += 255;
            if (IA_inf[j].ang_tor > 255)  IA_inf[j].ang_tor -= 255;
        } while ((IA_inf[j].ang_tor < 0) || (IA_inf[j].ang_tor > 255) );

    // seguridad
     do {
             if (IA_inf[j].ang_dir < 0)  IA_inf[j].ang_dir += 255;
             if (IA_inf[j].ang_dir > 255)  IA_inf[j].ang_dir -= 255;
        } while ((IA_inf[j].ang_dir < 0) || (IA_inf[j].ang_dir > 255) );


// ------------- acciones ------------
// DISPARAR 
if ((IA_inf[j].disparar == TRUE) && (IA_inf[j].c_disparar > 0))
           {
            // Agrego un disparo en la punta de la torreta
            combo_disparar(j, deciseg); 

            IA_inf[j].c_disparar--;

           }

// apagar/prender luz (ON/OFF) [util para estrategia nocturna]
// if (random()%1000 < 10) jugador_info[j].luz_prendida = !jugador_info[j].luz_prendida;


        // acelerar:
        if (IA_inf[j].vel > jugador_info[j].vel) {
             jugador_info[j].vel += ACC_TA;
             if (  jugador_info[j].premio == VELOCIDAD_SPR ) jugador_info[j].vel += ACC_TA * 2;

          }; // fin acelera

        // frenar:
        if (IA_inf[j].vel < jugador_info[j].vel) {
             jugador_info[j].vel -= ACC_TA;
             if (  jugador_info[j].premio == VELOCIDAD_SPR ) jugador_info[j].vel -= ACC_TA * 2;

          }; // fin freno


// si la pos actual - la pos a ir es < 0, ir hacia la derecha, sino a la izquierda
// de esa manera, elije el camino mas corto al doblar (supuestamente... :^P)
// solo mueve si es necesario (margen de tolerancia
if ( !( (jugador_info[j].ang_dir >  IA_inf[j].ang_dir - VEL_GTA) && (jugador_info[j].ang_dir <  IA_inf[j].ang_dir + VEL_GTA) ) )
{
    if  (jugador_info[j].ang_dir -  IA_inf[j].ang_dir < 0 )
        {
        // doblar derecha
                   jugador_info[j].ang_dir += VEL_GTA;
                  if (jugador_info[j].ang_dir >  255) jugador_info[j].ang_dir -= 255;

              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
                if (validar_posicion(j)) {
                         jugador_info[j].ang_tor += VEL_GTA; 
                         if (jugador_info[j].ang_tor >  255) jugador_info[j].ang_tor -= 255;
                                         }
                                         else
                                         {
                                         IA_inf[j].c_mover = 0;      // DEBUG!!!
                                         }
          } // fin derecha
       else // doblar izquierda
        // doblar izquierda
       {
                 jugador_info[j].ang_dir -= VEL_GTA;
                if (jugador_info[j].ang_dir < 0) jugador_info[j].ang_dir += 255;

              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
              if (validar_posicion(j)) {
                       jugador_info[j].ang_tor -= VEL_GTA; 
                        if (jugador_info[j].ang_tor < 0) jugador_info[j].ang_tor += 255;
                                       }
                                       else
                                       {
                                       IA_inf[j].c_mover = 0;      // DEBUG!!!
                                       }
          }; // fin izquierda
} // fin ver si es necesario girar

        // -- TORRETA --
if ( !( (jugador_info[j].ang_tor >  IA_inf[j].ang_tor - VEL_GTO) && (jugador_info[j].ang_tor <  IA_inf[j].ang_tor + VEL_GTO) ) )
{
    if  (jugador_info[j].ang_tor -  IA_inf[j].ang_tor < 0 )
         {
        // TORRETA MOVER DERECHA
               jugador_info[j].ang_tor += VEL_GTO; 
               if (jugador_info[j].ang_tor > 255)
               {
                   jugador_info[j].ang_tor -= 255;
                   IA_inf[j].c_mover = 0;
               }
               validar_posicion(j);
         }  // fin derecha  
        else
         {
              jugador_info[j].ang_tor -= VEL_GTO;
              if (jugador_info[j].ang_tor < 0)
              {
                jugador_info[j].ang_tor += 255;
                IA_inf[j].c_mover = 0;
              }
              validar_posicion(j);
          } // fin izquierda
} // fin ver si es necesario girar

// Nueva Informacion de estar 'pegado'
     IA_inf[j].x_old = jugador_info[j].posx;
     IA_inf[j].y_old = jugador_info[j].posy;
     IA_inf[j].o_ang = jugador_info[j].ang_dir;
     IA_inf[j].to_ang = jugador_info[j].ang_tor;
     IA_inf[j].energia = jugador_info[j].energia;
}

#endif
