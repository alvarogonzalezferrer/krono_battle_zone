//--------------------------------------------------------
// poligono.c
// Rutinas para crear el poligono del jugador y su torreta
// Rutinas para comprobar ese poligono en la grilla
// Utiliza rutinas de angulo.c
// 
//--------------------------------------------------------
#ifndef POLIGONO_C
#define POLIGONO_C

//--------------------------------------------------------
// crear_poli_jug(int numjug:1 o 2)
// Para poder saber si el jugador esta sobre algun obstaculo
// o interfiriendo con otro jugador, se debe crear un poligono
// de 9 vertices que se comprueban 1 a 1 en la grilla.
// Esta rutina se encarga de crear el poligono para el jugador
// especificado, y almacenarlo en la estructura correspondiente.
// CREA EL POLIGONO DEL CUERPO
// TRANSPORTADOS A LA COORDENADA DEL JUGADOR Y TODO...
// Luego, emplear otra rutina para comprobarlo.
// Necesita las rutinas contenidas en angulo.c
//--------------------------------------------------------
void crear_poli_jug(int numjug)
{
// variables dummy
int agt, angulo, pos_x, pos_y;
ANGULO_TYPE angulotmp; // estructura temporal para recuperar la posicion
                       // en base a los angulos...
int i1; // contador para el for
// vertices del poligono
// ver grafico:
// 5----4----3
// |    |    |
// 6----1----2 ===> FRENTE DEL TANQUE ->
// |    |    |
// 7----8----9
// el 10 corresponde a la punta de la torreta
int polx[11], poly[11]; // defino 11 marcadores, del 0 al 10 [11]

// los limpio a 0
for (i1=0; i1<11; i1++)
    {
        polx[i1] = 0;
        poly[i1] = 0;
    }

// ---------- CUERPO TANQUE --------
// LLENO EL POLIGONO DEL CUERPO DEL TANQUE

// INICIAR, SETEO LAS VARS A LAS DE LA ESTRUC DEL JUGADOR
pos_x = jugador_info[numjug].posx;
pos_y = jugador_info[numjug].posy;
angulo = jugador_info[numjug].ang_dir; // ANGULO CUERPO


// PARTE IMPORTANTE
// SETEAR LOS PUNTOS DEL POLIGONO

agt = pasa255a360(angulo); // convierto a mi sistema de 0-360

// centro (1)
polx[1] = pos_x + JUG_X_MAX/2;
poly[1] = pos_y + JUG_Y_MAX/2;

// mitad derecha (2) en 0 grados
angulotmp = dame_angc360(0-agt, JUG_X_MAX/2, polx[1], poly[1] );
polx[2] = angulotmp.x2;
poly[2] = angulotmp.y2;

// esquina superior (3) a 90 grados
angulotmp = dame_angc360(90-agt, JUG_Y_MAX/2, polx[2], poly[2] );
polx[3] = angulotmp.x2;
poly[3] = angulotmp.y2;

// mitad superior (4) a 180 grados
angulotmp = dame_angc360(180-agt, JUG_X_MAX/2, polx[3], poly[3] );
polx[4] = angulotmp.x2;
poly[4] = angulotmp.y2;

// vertice superior (5) a 180 grados
angulotmp = dame_angc360(180-agt, JUG_X_MAX/2, polx[4], poly[4] );
polx[5] = angulotmp.x2;
poly[5] = angulotmp.y2;

// mitad izquierda (6) a 270 grados
angulotmp = dame_angc360(270-agt, JUG_Y_MAX/2, polx[5], poly[5] );
polx[6] = angulotmp.x2;
poly[6] = angulotmp.y2;

// vertice inf izquierdo (7) a 270 grados
angulotmp = dame_angc360(270-agt, JUG_Y_MAX/2, polx[6], poly[6] );
polx[7] = angulotmp.x2;
poly[7] = angulotmp.y2;

// vertice inf mitad (8) a 0 grados
angulotmp = dame_angc360(0-agt, JUG_X_MAX/2, polx[7], poly[7] ) ;
polx[8] = angulotmp.x2;
poly[8] = angulotmp.y2;

// vertice inf der (9) a 0 grados
angulotmp = dame_angc360(0-agt, JUG_X_MAX/2, polx[8], poly[8] );
polx[9] = angulotmp.x2;
poly[9] = angulotmp.y2;


// TORRETA:

// PARA PODER DISPARAR DESDE LA TORRETA  
// ES LA PUNTA DE LA TORRETA ALEJADA 5 PIXELES
                                       
agt = pasa255a360(jugador_info[numjug].ang_tor); // convierto a mi sistema de 0-360
angulotmp = dame_angc360(-agt, (JUG_X_MAX/2) + 5, pos_x + JUG_X_MAX/2, pos_y + JUG_Y_MAX/2);
polx[10] = angulotmp.x2;
poly[10] = angulotmp.y2;

// PARA PODER ILUMINAR DESDE LA TORRETA
// ES LA PUNTA DE LA TORRETA ALEJADO 20 PIXELES
// 'agt' VIENE DEFINIDO DE ARRIBA = ang_tor
angulotmp = dame_angc360(-agt, (JUG_X_MAX/2) + 20, pos_x + JUG_X_MAX/2, pos_y + JUG_Y_MAX/2);
polx[0] = angulotmp.x2;
poly[0] = angulotmp.y2;


// SETEO EL POLIGONO DEL CUERPO DEL TANQUE
for (i1=0; i1<11; i1++)
{
        jugador_info[numjug].xpol[i1] = polx[i1];
        jugador_info[numjug].ypol[i1] = poly[i1];
} // fin for i1


} // fin crear_poli_jug
//--------------------------------------------------------




//--------------------------------------------------------
// es_pos_valida(int numjug)
// Rutina que devuelve TRUE si el jugador esta en una
// posicion que no se choca nada
// Devuelve FALSE si el jugador esta chocando algo
// Comprueba si choca al otro jugador tambien, pero
// la colision por ahora es solo determinada si entra dentro
// del radio del tanque enemigo. ;D
// Pasarle el numero de jugador (1 o 2)
// si ocurre un error devuelve FALSE
//--------------------------------------------------------
int es_pos_valida(int numjug)
{

// i1, i3, i4 para las comprobaciones
int i1,i2, i3, i4, i5;

// ver por si hay errores
if ((numjug<1) || (numjug>2)) return FALSE;

// CODIGO PARA VERIFICAR LA COLISION
// CON EL OTRO TANQUE...
// MEJORAR!
// contra que jugador debo comparar?
if (numjug == 1)
        i1=2; // el otro jugador... :D
   else
        i1=1;
// ver si esta dentro del radio
i2 = jugador_info[numjug].posx;
i3 = jugador_info[numjug].posy;

i4 = jugador_info[i1].posx;
i5 = jugador_info[i1].posy;
// verificar los cuatro vertices
// LES DA 3 PIXELES DE MAS AL MARGEN PARA EVITAR PROBLEMAS 
// *********
// vertice sup izq
if ((i2 < i4 + JUG_X_MAX + 3) && (i3 < i5 + JUG_Y_MAX + 3)) {
if ((i2 > i4 - 3) && (i3 >i5 - 3)) {
        return FALSE;
}
} // fin vertice sup izq
// *********
// vertice sup der
if ((i2 + JUG_X_MAX < i4 + JUG_X_MAX + 3) && (i3 < i5 + JUG_Y_MAX + 3)) {
if ((i2 + JUG_X_MAX > i4 - 3) && (i3  > i5 - 3)) {
        return FALSE;
}
} // fin vertice sup der
// *********
// vertice inf izq
if ((i2  < i4 + JUG_X_MAX + 3) && (i3 + JUG_Y_MAX < i5 + JUG_Y_MAX + 3)) {
if ((i2  > i4 - 3) && (i3 + JUG_Y_MAX > i5 - 3)) {
        return FALSE;
}
} // fin vertice inf der
// *********
// vertice inf der
if ((i2 + JUG_X_MAX < i4 + JUG_X_MAX + 3) && (i3 + JUG_Y_MAX < i5 + JUG_Y_MAX + 3)) {
if ((i2 + JUG_X_MAX > i4 - 3) && (i3 + JUG_Y_MAX > i5 - 3)) {
        return FALSE;
}
} // fin vertice inf der



//----------------

// ver posicion normal primero
// deja un margen de 2 pixeles para evitar errores
// esto no devuelve nada, ni TRUE ni FALSE, ya que solo lo ajusta al mapa
// las comprobaciones grosas son mas abajo...
i3 = jugador_info[numjug].posx;
i4 = jugador_info[numjug].posy;
if (i3<3)
    {
        jugador_info[numjug].posx = 3;
  //      return FALSE;
    }

if (i4<3)
    {
        jugador_info[numjug].posy = 3;
 //       return FALSE;
    }

if (i3 > 997-JUG_R_MAX)
    {
        jugador_info[numjug].posx = 997 - JUG_R_MAX;
//        return FALSE;
    }

if (i4 > 997-JUG_R_MAX) 
    {
        jugador_info[numjug].posy = 997 - JUG_R_MAX;
//        return FALSE;
    }


// recorre todos los puntos del poligono y los ajusta a la grilla
// y de hay ve si son o no 

for (i1=1; i1<10; i1++)
{       

        i3 = jugador_info[numjug].xpol[i1];
        i4 = jugador_info[numjug].ypol[i1];

// fuera de pantalla?
if (i3<0) return FALSE;
if (i4<0) return FALSE;
if (i3>999) return FALSE;
if (i4>999) return FALSE;

// ajusta a grilla
i3 = i3 / SPR_X_MAX;
i4 = i4 / SPR_Y_MAX;

// fuera de grilla?
if (i3<0) return FALSE;
if (i4<0) return FALSE;

if (i3>49) i3=49;
if (i4>49) i4=49;

// ver sobre que esta parado?
// si es que AGUA devuelve FALSE
// caso contrario sigue
if (grilla_map[i3][i4] > AGUA)
               {
                // devolver FALSE
                return FALSE;
             }

} // fin de for


// SI LOGRO PASAR TODAS LAS COMPROBACIONES DE ARRIBA
// DEVOLVER TRUE
return TRUE;

} // fin de es_pos_valida(int numjug)
//--------------------------------------------------------


//--------------------------------------------------------
// int  validar_posicion(int numjug)
// Rutina que, chequea la posicion actual del jugador numjug(1 o 2)
// y si es valida, actualiza la posicion nueva validandola, es decir
// los datos de la pos vieja son sobreescritos con la nueva.
// Si la posicion no es valida, regresa al jugador a la ultima posicion valida
// Devuelve TRUE si era valida, FALSE si tuvo que acomodarla
// Muy cool no?
//--------------------------------------------------------
int validar_posicion(int numjug)
{
// ver por si hay errores
if ((numjug<1) || (numjug>2)) return FALSE;

// crear nuevamente el poligono de control
crear_poli_jug(numjug);

if (es_pos_valida(numjug) == TRUE)
        {
        // posicion valida, validar...
        jugador_info[numjug].posxold = jugador_info[numjug].posx;
        jugador_info[numjug].posyold = jugador_info[numjug].posy;
        jugador_info[numjug].ang_dirold = jugador_info[numjug].ang_dir;
        jugador_info[numjug].angtor_old = jugador_info[numjug].ang_tor;
        // crear nuevamente el poligono de control
        // ya que puede haberse movido el jugador con lo de arriba...
        crear_poli_jug(numjug);
        return TRUE;
        }
else
        {
        // posicion invalida, recuperar vieja
        jugador_info[numjug].posx = jugador_info[numjug].posxold;
        jugador_info[numjug].posy = jugador_info[numjug].posyold;
        jugador_info[numjug].ang_dir = jugador_info[numjug].ang_dirold;
        jugador_info[numjug].ang_tor = jugador_info[numjug].angtor_old;
        // crear nuevamente el poligono de control
        // ya que puede haberse movido el jugador con lo de arriba...
        crear_poli_jug(numjug);
        return FALSE;
        }

return FALSE; // por si las dudas...

} // fin de validad_posicion
//--------------------------------------------------------
#endif
