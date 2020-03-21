//--------------------------------------------------------
// startgam.c   Rutinas para crear el mapa del juego
// startgam.h
//          Inicia Allegro, instala teclado, testea modos de video, sonido
//          y crea las tablas predefinidas de seno, cos, transparencia, etc
//          Ademas, contiene el manual principal de juego.-
//--------------------------------------------------------
// KRONOMAN'S BATTLE ZONE
// Kreado por Kronoman
// Empezado 31-D-2000
// Este archivo: 4-E-2001 2.15 am
// Codigo en lenguaje DJGPP + Allegro
//--------------------------------------------------------

#ifndef STARGAM_C
#define STARGAM_C

//--------------------------------------------------------
// Variables Globales
//--------------------------------------------------------

// bitmap de 1000x1000 que contiene el mapa del juego
// iniciar con create_bitmap cuando sea necesario
// BITMAP *Areajuego_bmp; // definido en principa.h


//--------------------------------------------------------
// Codigo de las rutinas
//--------------------------------------------------------

//--------------------------------------------------------
// este combo devuelve el puntero al sprite del tipo de pared/piso pasado
//--------------------------------------------------------
int dame_sprite_piso(int n) {

int ti1 = 0;

   // seleccionar de acuerdo a lo que sea
switch (n) {
    //OJO QUE NO SON LO MISMO, FIJARSE QUE LAS MAYUS IMPORTAN!!!
    case TIERRA:
        ti1 = Tierra; 
        break;
    case PASTO:
        ti1 = Pasto;
        break;
    case BARRO:
        ti1 = Barro;
        break;
    case AGUA:
        ti1 = Agua;
        break;
    case LADRILLO:
        ti1 = Ladrillo;
        break;
    case MADERA:
        ti1 = Madera;
        break;
    case ROCA1:
        ti1 = Roca1;
        break;
    case ROCA2:
        ti1 = Roca2;
        break;
    case ROCA3:
        ti1 = Roca3;
        break;
    //default:
    // por las dudas :D
    //      ti1 = Tierra;
} // fin del switch

return ti1; // listo
}



//--------------------------------------------------------
// dibujar_rectangulo_mapa_1(...)
// Rutina auxiliar que coloca un solo cuadrito de la grilla
// del mapa (usado por dibujar_el_mapa y por los disparos
// para borrar paredes rotas...)
//--------------------------------------------------------
void dibujar_rectangulo_mapa_1(BITMAP *bmp, int x1,int y1)
{
    // hacer un blit del sprite de SPR_X_MAXxSPR_Y_MAX en el lugar adecuado
    // para saber la grilla la formula es:
    // x = (i2 * SPR_X_MAX); y = (i1 * SPR_Y_MAX)
int ti1=0;

if (x1 < 0) x1 = 0;
if (y1 < 0) y1 = 0;
if (x1 > 49) x1 = 49;
if (y1 > 49) y1 = 49;


   // seleccionar de acuerdo a lo que sea
ti1 = dame_sprite_piso( grilla_map[x1][y1] );

    blit(krono_dato[ti1].dat, bmp, 0, 0, x1*SPR_X_MAX, y1*SPR_Y_MAX, SPR_X_MAX, SPR_Y_MAX);
}
//--------------------------------------------------------
// dibujar_el_mapa()
// Rutina que dibuja el mapa del juego en Areajuego_bmp (1000x1000 pixels)
// Debe ser llamada luego de setear todo el mapa
//--------------------------------------------------------

void dibujar_el_mapa()
{
int i1, i2; // dummys
// DIBUJAR
// parte engorrosa, dibujar :^(
// EL BITMAP QUEDA EN *Areajuego_bmp (1000X1000 PIXELS)

destroy_bitmap(Areajuego_bmp); // por las dudas
Areajuego_bmp = create_bitmap(1000, 1000); // creo el bitmap
clear(Areajuego_bmp); // lo limpio a negro 

// recorrer la grilla colocando la imagen
for (i1=0; i1<50 ; i1++) {
    for (i2=0; i2<50 ; i2++) {

        // hacer un blit del sprite de SPR_X_MAXxSPR_Y_MAX en el lugar adecuado
        dibujar_rectangulo_mapa_1(Areajuego_bmp,  i2, i1); 

    } // fin for i2 (x)
} // fin for i1 (y)

} // fin dibujar_el_mapa()  
//--------------------------------------------------------

//--------------------------------------------------------
// crear_mapa_juego
// Crea el bitmap del juego
// y el array bidimensional con la info del tipo de terreno
// NOTA: LOS PARAMETROS DEL MAPA (CLIMA,ETC) YA DEBEN
// ESTAR SETEADOS DE ANTEMANO EN LA ESTRUCTURA CORRESPONDIENTE!
//--------------------------------------------------------
void crear_mapa_juego()
{
int i1, i2, i3, i4, i5, i6; // contadores para los for, etc
               

// creo el array que contiene las casillas del mapa
// el mapa es de 1000x1000 pixeles
// dividido en una grilla de 20x20 pixels, o sea 50x50 grilla = 2500
// la grilla va de 0-49 -> 50 items ok?
//int grilla_map[50][50];
// grilla_mapE[50][50]; // ENERGIA
// esto se organiza asi:
// grilla_map[x][y] = tipo de terreno
// grilla_mapE[x][y] = energia que le queda para romperse en caso de ser una pared

// NOTAR QUE USO LA GRILLA DE 0 A 49, EL 50,50 LO DEJO LIBRE... 
// MEJORAR LA RUTINA DE COLOCACION DE OBSTACULOS

// TERRENO
// primero creo el terreno plano
for (i1=0; i1<50 ; i1++) {
for (i2=0; i2<50 ; i2++) {
        // setea el tipo del terreno al general
        grilla_map[i2][i1] = info_mapa_juego.terreno_gral;
        // setea la energia a 0 porque es solo terreno
        grilla_mapE[i2][i1] = 0;
} // fin for i2 (x)
} // fin for i1 (y)


// OBSTACULOS


        // setea el tipo del terreno a las paredes 4-8
        // mismo tipo de obstaculo para todo el mapa (para q' no quede choto)
           i6 = (random() % 5)+4;
        // a mi me gusta el ladrillo; sorry
//        i6 = LADRILLO;

// ahora agrego un 5 a 10 % de obstaculos sueltos
 for (i1=0; i1<((random() % 100)+100); i1++) {

 i2 = random() % 50; // posicion del obstaculo al azar
 i3 = random() % 50;

        // setea el tipo del terreno 
        grilla_map[i2][i3] = i6;

        // energia en relacion...
        grilla_mapE[i2][i3] = (random() % (grilla_map[i2][i3]*10)) + 20;

 } // fin for i1

// -----------------
// OBSTACULOS: RUTINA OPTIMIZADA
// COLOCA LINEAS VERTICALES Y HORIZONTALES Y RECTANGULOS CON OBJETOS
// DEL MISMO TIPO
for (i1=0; i1<((random() % 10)+10); i1++) {

 i2 = random() % 50; // posicion inicial del obstaculo al azar
 i3 = random() % 50;

 i4 = random() % 50; // posicion destino del obstaculo al azar
 i5 = random() % 50;

// trazar la linea... queda bastante horrible ;( 
do {
if (i4 < i2) i4++;
if (i4 > i2) i4--;

if (i5 < i3) i5++;
if (i5 < i3) i5++;
        // setea el terreno
        grilla_map[i4][i5] = i6;
        // energia en relacion...
        grilla_mapE[i4][i5] = (random() % (grilla_map[i4][i5]*10)) + 20;
    } while ((i2 != i4) && (i3 != i5));

} // fin for i1
// -----------------


// DIBUJAR
dibujar_el_mapa();

// AGREGAR UNA RUTINA PARA SUAVIZAR LOS BORDES ACA
// SI NO, QUEDA MUY 'CUADRATICO'

// listo

} // fin crear_mapa_juego()
//--------------------------------------------------------


//--------------------------------------------------------
// setear_mapa_juego
// Esto setea las condiciones iniciales del mapa del juego
// como ser clima, dia, etc
// Todo al azar. Esta joya para hacer mas interesante el duelo
//--------------------------------------------------------
void setear_mapa_juego()
{


// elegir terreno general
// del 0 al 3
info_mapa_juego.terreno_gral = random() % 4;

// DEBUG - hacer que el clima sea acorde al tipo de terreno...

// tipo de clima 0-5 (ver definiciones en .h)
info_mapa_juego.clima = (random() % 6); // numero del 0 al 5 inclusive

// hora del dia
info_mapa_juego.hora = random() % 2; // 0 o 1 (dia, noche)

info_mapa_juego.tiempo_a_jugar = -666; // infinito, hasta que se cansen

// resetear los puntos
info_mapa_juego.puntaje[0] = 0;
info_mapa_juego.puntaje[1] = 0;
info_mapa_juego.puntaje[2] = 0;


// listo...


} // fin setear_mapa_juego
//--------------------------------------------------------

//--------------------------------------------------------
// Funcion auxiliar que devuelve la distancia entre
// ambos jugadores
//--------------------------------------------------------
long distancia_entre_jugadores()
{
long i2, i3; // para calcular la distancia entre jugadores

// uso pitagoras
// notar que ABS es una macro de Allegro - abs es una de C
    i2 = abs(jugador_info[1].posx - jugador_info[2].posx);
    i3 = abs(jugador_info[1].posy - jugador_info[2].posy);
    i2 = potencia( (int)i2, 2);
    i3 = potencia( (int)i3, 2);
    i2 += i3;
    i2 = sqrt(i2); // listo
    return i2; // devolver
}

//--------------------------------------------------------
// iniciar_juego_azar()
// basicamente, llama a la rutina que setea el mapa al azar
// y luego a la rutina que crea el mapa
// y reseta y ubica los jugadores.
// si el flag de debug esta on, permite ver el mapa
// completo haciendo scroll con las flechas. (COOL!)
//--------------------------------------------------------
void iniciar_juego_azar()
{
int i1; // para evitar bucle al infinito mas abajo
long i2; // para calcular la distancia entre jugadores


// llamar al seteador de datos
setear_mapa_juego();
// llamar al creador del mapa
crear_mapa_juego();

i1 = 0;
do {
// iniciar los jugadores
setea_jug(1);
setea_jug(2);

// Ver si la distancia es muy poca entre ambos,
// situarlos mas lejos
i2 = distancia_entre_jugadores();

// si es debug, pone los tanques cerca
// si no es debug, los pone lejos...
if (K_DEBUG == TRUE)  
        { if (i2 < 100) i1 = 501; } // para que salga do
        else
        { if (i2 > 1000) i1 = 501; } // para que salga del do
i1++;
} while (i1<500); // hacer 500 intentos y si no, darse por vencido...


// si debo debuggear, llamar al mostrador de mapa por scroll
if (K_DEBUG == TRUE) {
 mostrame_mapa_para_debug();
};

// *** juego iniciado; ***
// ahora, llamar a la rutina que 'mueve' el juego...
motor_juego(); // el motor del juego incluido en game.c!

} // fin iniciar_juego_azar
//--------------------------------------------------------


//--------------------------------------------------------
// mostrame_mapa_para_debug()
// esta rutina de Debug permite ver en pantalla 320x240 (debe estar
// pre seteada y con la paleta adecuada) el mapa creado.
// Usar las flechas para hacer scroll y ESC para salir
// Soy un capo!!!
// ADEMAS, PRESENTA EL EFECTO DIA/NOCHE Y NIEBLA !!!
//--------------------------------------------------------

void mostrame_mapa_para_debug()
{
int xmd = 0, ymd = 0; // x, y del mapa que estamos viendo
int xmdold = -1, ymdold = -1; // x, y posicion anterior (para redibujar, etc)
int detal = 8; // detalle, cuando mayor, menor detalle en los efectos
int i1, i2, i3; // para el for de la iluminacion, niebla, etc
int actual = 1; // tanque moviendo actual?
char tmpstr[80]; // string temporal
BITMAP *spotlight; // para hacer la iluminacion :^D
BITMAP *nieblabmp; // para hacer la niebla
                // NOTAR QUE LA PALETA TIENE QUE TENER ESCALA DE GRISES
                // DE COLOR 0 A COLOR 31 INCLUSIVE!! SIENDO 0 NEGRO Y 31 BLANCO
BITMAP *tmppreview; // para hacer el preview y mandar a pantalla


// referente a iluminacion
   spotlight = create_bitmap(320, 240); // crea el spotlight
   tmppreview = create_bitmap(320, 240); // temporal
   nieblabmp    = create_bitmap(320, 240); // crea la niebla

// tabla RGB para el juego
   rgb_map = &rgb_table;

// iluminacion del juego
   color_map = &light_table;   // cambiar por &trans_table cuando desee usar transparencia

// PRE CALCULO EL SPOTLIGHT EN EL CENTRO DE BMP SPOTLIGHT
        // dibuja la iluminacion normal (5)
         rectfill(spotlight, 0, 0, spotlight->w, spotlight->h, 5);

        // dibuja el spotlight 
        for(i1=5; i1<256; i1 += detal)
                circlefill(spotlight, SCREEN_W / 2 , SCREEN_H / 2 ,64 - i1 / 4, i1);

// PRE CALCULO LA NIEBLA    EN EL CENTRO DE BMP NIEBLABMP
        // dibuja la niebla total    (31) -> blanco
        rectfill(nieblabmp, 0, 0, nieblabmp->w, nieblabmp->h, 31);

        // dibuja el fade de la niebla de blanco total a normal (negro, 0)
        for(i1=31; i1>0 ; i1-= 2)
            circlefill(nieblabmp, SCREEN_W / 2 , SCREEN_H / 2 , (i1*2)+30, i1);



// limpiar teclado
clear_keybuf();

// loop principal
do {
// mover
    if (key[KEY_UP]) ymd-=15;
    if (key[KEY_DOWN]) ymd+=15;    
    if (key[KEY_LEFT]) xmd-=15;
    if (key[KEY_RIGHT]) xmd+=15;

// mover AMBOS tanques con A,S,D,W
    if (key[KEY_A])
    {
        validar_posicion(actual);
        jugador_info[actual].posx -= 3;
        // chequear la nueva pos (y auto crea el pol de control)
        validar_posicion(actual);
        xmdold=-1; //para que redibuje
    }
    if (key[KEY_D])
    {
        validar_posicion(actual);

        jugador_info[actual].posx += 3;
        validar_posicion(actual);
        xmdold=-1; //para que redibuje
    }
    if (key[KEY_W])
    {
        validar_posicion(actual);
        jugador_info[actual].posy -= 3;
        validar_posicion(actual);
        xmdold=-1; //para que redibuje
    }
    if (key[KEY_S])
    {
        validar_posicion(actual);
        jugador_info[actual].posy += 3;
        validar_posicion(actual);
        xmdold=-1; //para que redibuje
    }
    if (key[KEY_1])
                actual = 1;
    if (key[KEY_2])
                actual = 2;



if ((ymd != ymdold) || (xmd != xmdold)) {
// que no se salga de pantalla... ;D
        if (xmd<0) xmd=0;
        if (ymd<0) ymd=0;
        if (xmd>680) xmd=680;
        if (ymd>760) ymd=760;
        ymdold = ymd;
        xmdold = xmd;

        // dibujar a tmppreview el area de juego
        blit(Areajuego_bmp, tmppreview, xmd, ymd, 0, 0,320,240);

// dibujar los jugadores en su posicion relativa en caso de estar dentro
// de la pantalla
// para poder hacer esto, el jugador ya tiene seteado su poligono interno
// ver si esta dentro del area mostrada (el centro del jugador

for (i1=1; i1<3; i1++) // para hacer los 2 jugadores
{
// ver si el centro del jugador esta dentro de la pantalla
if ((jugador_info[i1].posx + JUG_X_MAX / 2 - xmd > -1) && (jugador_info[i1].posy + JUG_Y_MAX/2 - ymd > -1)) {
if ((jugador_info[i1].posx + JUG_X_MAX / 2 - xmd < 320) && (jugador_info[i1].posy + JUG_Y_MAX/2 - ymd < 240)) {
// esta dentro, dibujar...

// dibuja el cuerpo rotado
 rotate_sprite (tmppreview,
                krono_dato[Tanque].dat,
                jugador_info[i1].posx - xmd,
                jugador_info[i1].posy - ymd,
                itofix (jugador_info[i1].ang_dir));

if (jugador_info[i1].tiene_torreta) {
// dibuja la torreta rotada y acomodada al lugar central
// SUPUESTAMENTE, LA TORRETA Y EL TANQUE SON AMBOS DE 25X25
// ASIQUE NO HAY PROBLEMAS AL GIRARLOS... OK?
 rotate_sprite (tmppreview,
                krono_dato[Torreta].dat,
                jugador_info[i1].posx - xmd,
                jugador_info[i1].posy - ymd,
                itofix (jugador_info[i1].ang_tor));
} // fin si tiene torreta

if (K_DEBUG) { // si tiene debug on, mostrar informacion adicional
        // dibujar las coordenadas
//            circlefill(tmppreview,
//                       jugador_info[i1].posx - xmd, 
//                       jugador_info[i1].posy - ymd,
//                       2,254);
//            rect(tmppreview,
//                 jugador_info[i1].posx - xmd,
//                 jugador_info[i1].posy - ymd,
//                 jugador_info[i1].posx - xmd + JUG_X_MAX,
//                 jugador_info[i1].posy - ymd + JUG_Y_MAX,
//                 254);


        // dibujar los puntos rosas de control
        // que muestran el poligono del cuerpo y punta torreta
        for (i2=1; i2<11; i2++)
                {
                    circlefill(tmppreview,
                               jugador_info[i1].xpol[i2] - xmd, 
                               jugador_info[i1].ypol[i2] - ymd,
                               1,254);
                // le pone numero a los puntitos...
                  sprintf(tmpstr, "%d", i2);
                  textout(tmppreview, krono_dato[FontXM6x6].dat, tmpstr,
                          jugador_info[i1].xpol[i2] - xmd, 
                          jugador_info[i1].ypol[i2] - ymd, 255);

        // muestra en pantalla la info de coordenadas de los puntos
        // sin transportar y transportados a la pantalla actual
        // y el numero de la grilla sobre la que esta
          sprintf(tmpstr,"%d:XY:[%d,%d]->[%d,%d]:g:[%d]", i2,
                  jugador_info[i1].xpol[i2],
                  jugador_info[i1].ypol[i2],
                  jugador_info[i1].xpol[i2] - xmd,
                  jugador_info[i1].ypol[i2] - ymd,
                  grilla_map[jugador_info[i1].xpol[i2] / SPR_X_MAX]
                  [jugador_info[i1].ypol[i2] / SPR_Y_MAX]);

                  textout(tmppreview, krono_dato[FontXM6x6].dat, tmpstr, 0, (i2*7)+7, 255);
        } // fin for de recorrer los puntos (i2)


        // muestra en pantalla la info de coordenadas de los puntos
        // sin transportar y transportados a la pantalla actual
        // y el valor de es_pos_valida(i1) (TRUE/FALSE)
          sprintf(tmpstr,"XY-[%03d,%03d], V:[%d]",
                  jugador_info[i1].posx,
                  jugador_info[i1].posy,
                  es_pos_valida(i1));

                  textout(tmppreview, krono_dato[FontXM6x6].dat, tmpstr, 0, 90, 255);
} // fin de ver si es K_DEBUG

// gira la torreta (DEBUG)
jugador_info[i1].ang_tor += 8;
if (jugador_info[i1].ang_tor > 255) jugador_info[i1].ang_tor = 0;

// gira el tanque (DEBUG)
jugador_info[i1].ang_dir += 2;
if (jugador_info[i1].ang_dir > 255) jugador_info[i1].ang_dir = 0;

// ver si la posicion actual es correcta
validar_posicion(i1);

} // fin de los ifs para ver si el tanque esta en pantalla
} // fin de los ifs para ver si el tanque esta en pantalla
// fin dibujar jugadores                       
} // fin del for de pasar por los dos jugadores (i1)

// NIEBLA WOWOWOW
 if (info_mapa_juego.clima == NIEBLA) {

// EXPERIMENTO
// CREO LA NIEBLA DINAMICAMENTE, SEMI DESCENTRADA AL AZAR
// PARA DAR EFECTO DE NIEBLA EN MOVIMIENTO

        // dibuja la niebla total    (31) -> blanco
        rectfill(nieblabmp, 0, 0, nieblabmp->w, nieblabmp->h, 31);
        // dibuja el fade de la niebla de blanco total a normal (negro, 0)
        for(i1=31; i1>0 ; i1-= 2)
            {
            i2 = random() % 10;
            if (i2>5) i2 = -(random() % 5);
            i3 = random() % 10;
            if (i3>5) i3 = -(random() % 5);

            circlefill(nieblabmp, (SCREEN_W / 2) + i2 , (SCREEN_H / 2)+ i3 , (i1*2)+30, i1);
            }

        color_map = &trans_table;
        draw_trans_sprite(tmppreview, nieblabmp, 0, 0);

     }


// LUCES
if (info_mapa_juego.hora == DE_NOCHE) {
 // EFECTO LUMINICO EN CASO DE QUE SEA NECESARIO (O SEA, SI ES DE NOCHE)
        // dibuja la transparencia 
        color_map = &light_table;   // cambiar por &trans_table cuando desee usar transparencia
        draw_trans_sprite(tmppreview, spotlight, 0, 0);
        }

// escribir info
  text_mode(-1); // texto transparente
  sprintf(tmpstr,"Debug: X,Y: %d,%d -> %d,%d", xmd, ymd, xmd+320, ymd+240);
  textout(tmppreview, krono_dato[FontXM6x6].dat, tmpstr, 0, 0, 255);


// copiar buffer a pantalla
        blit(tmppreview, screen,  0 , 0  , 0, 0,320,240);

        // limpiar teclado
        clear_keybuf();
        }

} while (!key[KEY_ESC]); // fin loop principal

// destruye los bitmaps usados
destroy_bitmap(spotlight);
destroy_bitmap(tmppreview);
destroy_bitmap(nieblabmp);

// limpiar teclado
clear_keybuf();

} // fin mostrame_mapa_para_debug()
//--------------------------------------------------------

//--------------------------------------------------------
// situa_jug_azar(int numjug)
// Una vez que se creo el mapa, se deben situar los jugadores
// esta rutina se encarga de hacerlo, viendo que queden a
// buena distancia uno del otro, y ademas, que no esten sobre
// partes del terreno intransitables.
// Muy util si el jugador muere, o para efectos de 'teletransporte'
// Imprescindible para situar los jugadores al comienzo del juego
// pasar en numjug el numero del jugador (1 o 2)
//--------------------------------------------------------
void situa_jug_azar(int numjug)
{
int i1, i2, i3; // dummys

// ver por si hay errores
if ((numjug<1) || (numjug>2)) return;


i3 =0; // para que no caiga en bucle al infinito


do {

 i1 = random() % 50; // posicion del jugador   al azar 0-49
 i2 = random() % 50;

 // conversion a pixeles
 i1 = i1 * SPR_X_MAX;
 i2 = i2 * SPR_Y_MAX;

// seteo la parte correspondiente de la estructura
jugador_info[numjug].posx = i1;
jugador_info[numjug].posy = i2;

// creo el poligono del jugador
crear_poli_jug(numjug);

// ver si no choco con nada
// si esta arriba de algo, cambiar la posicion
// si hizo 1000 intentos, lo deja en el lugar que quedo... :(
i3++;

} while ((i3<1000) && (es_pos_valida(numjug) == FALSE));

validar_posicion(numjug); // valida la posicion...



}  // fin situa_jug_azar
//--------------------------------------------------------

//--------------------------------------------------------
// setea_jug(int numjug)
// Carga los datos standard para los jugadores
// y llama a la rutina para setearlos en el mapa
// pasar el numero de jugador (1 o 2)
//--------------------------------------------------------
void setea_jug(int numjug)
{
// ver por si hay errores
if ((numjug<1) || (numjug>2)) return;

// situa el jugador en el mapa
situa_jug_azar(numjug);


jugador_info[numjug].energia = MAX_ENERGIA;
jugador_info[numjug].vel = 0;
jugador_info[numjug].ang_dir = random() % 256;
jugador_info[numjug].ang_inercia = jugador_info[numjug].ang_dir;
jugador_info[numjug].ang_tor = jugador_info[numjug].ang_dir;
jugador_info[numjug].arma = 0;
jugador_info[numjug].balas = 0;
jugador_info[numjug].premio = 0;
jugador_info[numjug].c_premio = 0;
jugador_info[numjug].tiene_torreta = TRUE;

jugador_info[numjug].ang_dirold = jugador_info[numjug].ang_dir;
jugador_info[numjug].angtor_old = jugador_info[numjug].ang_tor;

jugador_info[numjug].luz_prendida =  TRUE;


// situa el jugador en el mapa, de nuevo, con los nuevos parametros...
situa_jug_azar(numjug);

// listo

} //fin setea_jug(int numjug)
//--------------------------------------------------------
#endif
