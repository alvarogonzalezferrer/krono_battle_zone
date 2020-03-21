//--------------------------------------------------------
// game.c   Rutinas para JUGAR
// game.h   Contiene el MOTOR del juego
//          o sea, TODO LO QUE HACE AL LOOP PRINCIPAL
//          Y A LA LOGICA DEL JUEGO, HACE ANDAR LA COSA!
//
//          Supuestamente, el juego YA fue iniciado desde startgam.c
//
// ** NOTA ** CON <F12> PODES ACTIVAR EL CONTADOR DE FPS EN PANTALLA
//--------------------------------------------------------
// KRONOMAN'S BATTLE ZONE
// Kreado por Kronoman
// Empezado 31-D-2000
// Este archivo: 7-E-2001 21.00 pm
// Codigo en lenguaje DJGPP + Allegro
//--------------------------------------------------------

#ifndef GAME_C
#define GAME_C

// INCLUDES:
#include <values.h> // para saber el MAXLONG...

//--------------------------------------------------------

// Fisica - INERCIA
// Definir para utilizar inercia en el manejo de los tanques
// Default = FALSE, sino, el tanque se comporta 'raro'
#define USA_INERCIA FALSE

//--------------------------------------------------------
// VARIABLES Y COSAS GLOBALES
//--------------------------------------------------------
// Termino el juego?
int Fin_Juego = FALSE; // el juego termina cuando el usuario aprieta <ESC> ok?

//--------------------------------------------------------
// VARIABLES GLOBALES - DE CONTROL DE TIEMPO
// POR ESO ESTAN PRECEDIDAS DE 'volatile'
//--------------------------------------------------------
// para contar los fps en caso de debug...
volatile int frame_count, fps;

// para contar el tiempo cada 10 mls
volatile int game_time;

// TIEMPO:
// para contar el tiempo (para medir las bombas, etc)
// guarda el tiempo desde que empezo a jugar
volatile int miliseg, seg, min, hora;
// esto guarda las decimas de segundo desde que empezo a jugar
// ver que no pase de la var globlal definida en <values.h> (de C)
// como MAXLONG para evitar desbordar...
volatile unsigned long deciseg;

//--------------------------------------------------------
// RUTINAS --> LA COSA CALIENTE! WOW!!
//--------------------------------------------------------
//--------------------------------------------------------
// Rutina que es llamada cada ACTUALIZAR_GAME milisegundos
// para actualizar el juego.-
//--------------------------------------------------------
void game_timer()
{
game_time++; // aumentar el cuadro

deciseg += ACTUALIZAR_GAME / 10; // decimas de segundo

miliseg += ACTUALIZAR_GAME; // aumentar milisegs, segundos, etc
                            // todo para medir el tiempo de juego
}

END_OF_FUNCTION(game_timer); // esto ES necesario (parte de Allegro...)

//--------------------------------------------------------
// Rutina que es llamada para contar los fps del juego...
//--------------------------------------------------------

void fps_proc()
{
   fps = frame_count;
   frame_count = 0;
}

END_OF_FUNCTION(fps_proc); // esto ES necesario


//--------------------------------------------------------
// Peque¤a rutina auxiliar para corregir la 
// medicion del tiempo. Llamar seguido... ;D
//--------------------------------------------------------
void ajustar_medicion_tiempo()
{

if (deciseg > MAXLONG - 1000) deciseg=0; // por las dudas...

if (miliseg>1000)
    {
	miliseg=0;
        seg++;
        if (seg>60)
           {
           seg=0;
           min++;
           if (min>60) {
	            min=0;
                hora++;
	            }
           }
    }

} // fin ajustar tiempo
//--------------------------------------------------------

//--------------------------------------------------------
// Funcion que setea el modo de video
//
// Si el modo de video falla, devuelve error
// y sale al ODS
//--------------------------------------------------------
void seteame_el_video()
{
// SETEAR MODO DE PANTALLA AL PREFERIDO POR EL USUARIO
// SEGUN LAS VARIABLES GLOBALES... ;^)
set_color_depth(8); // 8 bits de color = 256 colores
if ( set_gfx_mode(MODO_VIDEO, H_RES, V_RES,0,0) < 0 ) // si algo falla, error
  {
     set_gfx_mode(GFX_TEXT,0,0,0,0); // modo texto
        textcolor(LIGHTGRAY); textbackground(BLACK);
        clrscr();
        textcolor(WHITE); // blanco
        textbackground(RED); // rojo
        gotoxy(1,1);
        cprintf("ERROR SETEANDO MODO DE VIDEO: %dx%d \n\r",H_RES,V_RES);
        cprintf("EL PROGRAMA SE FINALIZARA...\n\n\n\n\r");
        textcolor(WHITE); textbackground(BLACK);
        cprintf("--Presione una tecla para salir--\n\r");
        clear_keybuf();
        readkey();
        exit(1);
   }

}

//--------------------------------------------------------
// Instala los timers - rutina auxiliar
//--------------------------------------------------------
void instalar_timers()
{
// 'Trabo' las variables para que anden bien (funcion allegro)

// Juego timer
   LOCK_VARIABLE(game_time); // contador
   LOCK_FUNCTION(game_timer); // funcion

// Fps timer
   LOCK_VARIABLE(frame_count); // contador
   LOCK_VARIABLE(fps); // contador
   LOCK_FUNCTION(fps_proc); // funcion

// Variables de control de tiempo para las rutinas auxiliares
LOCK_VARIABLE(miliseg);
LOCK_VARIABLE(seg);
LOCK_VARIABLE(min);
LOCK_VARIABLE(hora);
LOCK_VARIABLE(deciseg);

// instala los timers

// a 1000 milisegundos (1 segundo) el contador de fps
   install_int(fps_proc, 1000);

// a ACTUALIZAR_GAME milisegundos el timer del juego
      install_int(game_timer, ACTUALIZAR_GAME);
}

//--------------------------------------------------------
// Desinstala los timers
//--------------------------------------------------------
void desinstala_timers()
{
// LIMPIAR LOS TIMERS 
   remove_int(fps_proc);
   remove_int(game_timer);
}


//--------------------------------------------------------
// Realiza una pausa del juego, poniendo cartel...
//--------------------------------------------------------
void realizar_pausa()
{
    desinstala_timers();

// mostrar pausa [240x90]
   draw_sprite(screen,
               krono_dato[PAUSA_SPR].dat,
               (SCREEN_W - 240) / 2,
               (SCREEN_H - 90 ) / 2 ); 

// esperar tecla
    // limpia el teclado
    clear_keybuf();

    do { } while (!key[KEY_ENTER] );

    // limpia el teclado
    clear_keybuf();

    instalar_timers();
    game_time = -1; // para que redibuje

    clear(screen); // oops!
}


//--------------------------------------------------------
// Rutina que es el motor del juego
// ES LA MAS IMPORTANTE
// DE AQUI SE ADMINISTRA EL JUEGO
// NOTAR QUE EL JUEGO DEBE SER PREVIAMENTE INICIALIZADO
// CON LAS RUTINAS DE STARTGAM.C (OK?)
//
//--------------------------------------------------------
void motor_juego()
{

// Establecer condicion de jugar
Fin_Juego = FALSE;

// Resetear generador de particulas
reset_particulas();

// Resetear generador de focos de iluminacion (luz)
reset_luz();

// Resetear explosiones
reset_explo();

// Resetear disparos
reset_disparo();

// resetear incendios
// reset_incendio();

// Crear clima
crear_clima();

// Ubicar premios
reset_premios();

// Resetear inteligencia artificial (IA)
reset_IA();

// setear el modo de video
seteame_el_video();

// setea la paletta a la paleta del juego
        set_palette(krono_dato[Pal_Juego].dat);

// tabla RGB para el juego
   rgb_map = &rgb_table;

// iluminacion del juego
   color_map = &light_table;   // cambiar por &trans_table cuando desee usar transparencia


// Instalar los timers
 instalar_timers();

// Setea un par de globales
   game_time = 0;
   frame_count = fps = 0;

   miliseg = seg = min = hora = deciseg = 0;

// <<------------------------------>>
// Bucle principal
// repetir mientras el juego no termine
// de la manera en que esta armado esta estructura,
// el juego saltara cuadros de animacion para mantener una velocidad constante
// Soy un capo!!!
         while (!Fin_Juego) {

            while (game_time > 0) {
                       update_game_logic(); // actualizar la logica del juego

                       ajustar_medicion_tiempo();

                       game_time--; // descontar al contador de velocidad
                 }

            // cuando cae aca, actualiza la pantalla del juego
            // es decir, cuando logra actualizar bien la logica del juego

            // para que no quede en blucle al infinito, verificar el <ESC>
            if (key[KEY_ESC]) {
                        // insertar un cuadro de dialogo AQUI
                        // preguntando si se quiere salir, bla bla bla...
                        Fin_Juego = TRUE;
                     }

            ajustar_medicion_tiempo(); // ajusta el tiempo

            // RUTINA IMPORTANTE: MUESTRA TODO EN PANTALLA... }:^)
            update_display(); // actualizar la pantalla

            frame_count++; // contar los fps

         }   // fin del while principal

// LIMPIAR LOS TIMERS PARA QUE NO SIGAN INCHANDO LUEGO DE TERMINAR EL JUEGO
    desinstala_timers();


// Devuelve el modo de video a normal del menu (320x240x256 modo X)
        clear(screen);
        set_gfx_mode(GFX_AUTODETECT, 320, 240, 0 ,0);
// GAME OVER... ;^D
// MOSTRAR PUNTOS, ETC Y VOLVER AL MENU... [solo si NO es una DEMO]
if ( (JUG_HUMANO[1] ) ||  (JUG_HUMANO[2])  ) mostrar_puntaje(info_mapa_juego.puntaje[1], info_mapa_juego.puntaje[2]);


}
//--------------------------------------------------------


//--------------------------------------------------------
// update_game_logic()
// RUTINA QUE SE ENCARGA DE ACTUALIZAR LA LOGICA DEL JUEGO
// MUEVE LOS TANQUES, TOMA ENTRADA DEL TECLADO, CALCULA TODO
// MUEVE LOS EFECTOS ATMOSFERICOS, EN FIN, TODOS LOS CALCULOS DEL JUEGO
//--------------------------------------------------------
void update_game_logic()
{

//llama a la rutina que interpreta el teclado
Leer_Teclado();

// Mover los disparos
mover_disp();

// Mueve/actualiza los tanques
mover_tanques();

// Mover los premios
mover_premios();

// Mover los incendios
// mover_incendio();

// LUZ - solo si es de noche
if (info_mapa_juego.hora==DE_NOCHE)
{
    //actualiza la luz de los tanques
    tanque_luz();
    // Mueve/actualiza los sistemas de luz
    mover_luz();
}

// Mueve/actualiza los sistemas de particulas
mover_particulas();

// Mover/actualizar explosiones
mover_explo();

// Mover/actualizar clima
mover_clima();

} // fin update_game_logic
//--------------------------------------------------------


//--------------------------------------------------------
// update_display()
// RUTINA QUE SE ENCARGA DE ACTUALIZAR LA PANTALLA
// O SEA, DIBUJA TODO EL JUEGO EN PANTALLA
// LLAMA A LAS RUTINAS NECESARIAS PARA DIBUJAR EN ORDEN
//--------------------------------------------------------
void update_display()
{
// Variables
int xmd = 0, ymd = 0; // x, y del mapa que estamos viendo
int i1, ipant; // para los for y el numero de pantalla

BITMAP *tmpbmp666; // para hacer la pantalla en memoria y enviar luego
                // de que este todo listo
char tmpstr[80]; // string temporal para enviar texto a pantalla

//-----------
//------------DIBUJAR EN CAPAS
// Capa 0: mapa y premios encima
// Capa 1: Tanques
// Capa 2: Disparos
// Capa 3: Chispas y Explosiones
// Capa 4: Atmosfera
// Capa 5: Info (Energia, etc)
// Bucle para dibujar ambas pantallas o una sola
// si PANT_SPLIT == TRUE:2, sino 1 sola

// setea cantidad de pantallas + 1 (o sea 3=2, 2=1)
if (PANT_SPLIT)
        ipant = 3;
     else
        ipant = 2;

// crea el bitmap
tmpbmp666 = create_bitmap(H_RES, V_RES / (ipant - 1)); // temporal bitmap: buffer, al salir de la rutina lo destruyo
clear(tmpbmp666); // limpiar


for (i1 = 1; i1 < ipant; i1++) {

// calcular las x,y del mapa a mostrar para la posicion
// del tanque (centrado)
xmd = jugador_info[i1].posx;
ymd = jugador_info[i1].posy;
xmd = xmd - (H_RES - JUG_X_MAX) / 2;
ymd = ymd - ((V_RES / (ipant - 1)) - JUG_Y_MAX)/2;


if (xmd < 0) xmd = 0;
if (ymd < 0) ymd = 0;
if (xmd > 1000 - H_RES) xmd = 1000 - H_RES;
if (ymd > 1000 - (V_RES / (ipant - 1))) ymd = 1000 - (V_RES / (ipant - 1));
//--------------- dibujar terreno -----------------
        // dibujar a buffer el area de juego
        blit(Areajuego_bmp, tmpbmp666, xmd, ymd, 0, 0, H_RES, V_RES / (ipant - 1));
//--------------- fin de dibujar terreno -----------------

//--------------- dibujar premios -----------------
dibujar_premios(tmpbmp666, xmd, ymd);
//--------------- fin dibujar premios -------------

//--------------- dibujar tanques -----------------
dibujar_tanques(tmpbmp666, xmd, ymd, ipant);
//--------------- fin de dibujar tanques -----------------

//--------------- dibujar disparos -----------------
dibujar_disparos(tmpbmp666, xmd, ymd);
//--------------- fin de dibujar disparos -----------------

//--------------- dibujar particulas -----------------
dibujar_particulas(tmpbmp666, xmd, ymd);
//--------------- fin de dibujar particulas -----------------

//--------------- dibujar incendios -----------------
// dibujar_incendios(tmpbmp666, xmd, ymd);
//--------------- fin de dibujar incendios -----------------


//--------------- dibujar explosiones -----------------
dibujar_explo(tmpbmp666, xmd, ymd);
//--------------- fin de dibujar explosiones -----------------

//--------------- dibujar clima (atmosfera) -----------------
dibujar_clima(tmpbmp666, xmd, ymd, i1);
//--------------- dibujar clima -----------------


//--------------- dibujar iluminacion (luz) -----------------
// solo si el mapa es de noche
if (info_mapa_juego.hora==DE_NOCHE)
           dibujar_luz(tmpbmp666, xmd, ymd);
//--------------- fin de dibujar luz -----------------


//--------------- dibujar TABLERO -----------------
// en tablero.c
hace_tablero(tmpbmp666, i1);
//--------------- fin TABLERO -----------------


// FPS con <F12>
// DEBUG - FPS, ETC
if ((K_DEBUG) || (MOSTRAR_FPS)){
        // Mostrar los FPS abajo izquierda y coordendas, etc
        // ESTO ES DEBUG
        text_mode(-1); // texto (-1=trans, 1 solido)
        sprintf(tmpstr, "Fps[%4d]XY[%3d,%3d]-SCR:%d:E:%d:P:%d:L:%d:BT:%ld",
        fps, xmd, ymd, i1, cantidad_expl, cantidad_particulas, cantidad_luz, rawclock() );

        textout(tmpbmp666,
                krono_dato[FontXM6x6].dat,
                tmpstr, 0,
                V_RES / (ipant - 1) - 10,
                251);

        // prueba del nuevo timer
        sprintf(tmpstr,"[%3d:%2d:%2d:%4d]:[%ld]", hora, min, seg, miliseg,deciseg);
        textout(tmpbmp666,
                krono_dato[FontXM6x6].dat,
                tmpstr, 0,
                V_RES / (ipant - 1) - 20,
                251);

       // prueba de las coordenadas del jugador
       circlefill( tmpbmp666,
                   jugador_info[i1].posx - xmd + (JUG_X_MAX/2),
                   jugador_info[i1].posy - ymd + (JUG_Y_MAX/2),
                   4,
                   248);

        }

//--------------- si ambos jugadores son del PC, es una demo!
// poner el cartel apropiado...
// DEBUG - si cambia el cartel, cambiar 60x10 por las dimensiones!
if ( ( JUG_HUMANO[1] == FALSE ) &&  ( JUG_HUMANO[2] == FALSE ) )
   draw_sprite(tmpbmp666,
               krono_dato[CARTEL_DEMO_BMP].dat,
               (tmpbmp666->w - 60) / 2,
               10 ); // centrado en y => (tmpbmp666->h - 10) / 2

//--------------- enviar buffer a pantalla -----------------
// AL FIN! Enviar a pantalla el buffer del bitmap
// el ((i1-1)*240) hace que la panta se situe a la mitad si usamos dos...
// dibujo hasta 239 para que quede una linea negra entre medio de las 2 pantallas
        blit(tmpbmp666, screen,  0 , 0  ,
             0, (i1-1)*(V_RES / (ipant - 1)),
             H_RES, V_RES / (ipant - 1) - 1);

} // fin for 'i1'
//------------FIN DIBUJAR
 destroy_bitmap(tmpbmp666); 


} // fin update_display()
//--------------------------------------------------------


//--------------------------------------------------------
// Leer_Teclado()
// Rutina que interpreta el teclado
// y actualiza las variables correspondientes
// para ambos jugadores, en caso de que los controle humano;
// si los maneja el PC, llama a la rutina de Inteligencia Artificial
//
//--------------------------------------------------------
void Leer_Teclado()
{

// salir del juego
if (key[KEY_ESC]) {
        // insertar un cuadro de dialogo AQUI
        // preguntando si se quiere salir, bla bla bla...
        Fin_Juego = TRUE;
        }

// pausa: tecla pausa
if (key[KEY_PAUSE] )
   {
   key[KEY_PAUSE] = FALSE;
   realizar_pausa();
   }

// DEBUG - activar FPS con <F12>
if (key[KEY_F12])
         {
          MOSTRAR_FPS = !MOSTRAR_FPS;
          key[KEY_F12] = FALSE;
         }
// DEBUG - REMOVER!!! AL APRETAR <F11>, cambiar el arma de los tanques!
// Solo anda si se paso el parametro -debug!
if (key[KEY_F11] && K_DEBUG)
         {
          jugador_info[1].arma++;
          jugador_info[2].arma++;
          jugador_info[1].balas = random()%50 + 50;
          jugador_info[2].balas = random()%50 + 50;

          if (jugador_info[1].arma > (CANTIDAD_DE_ARMAS-1)) jugador_info[1].arma=0;
          if (jugador_info[2].arma > (CANTIDAD_DE_ARMAS-1)) jugador_info[2].arma=0;
          key[KEY_F11] = FALSE;
         }
// DEBUG - F9 cambia el jugador 2 a humano/PC
if (key[KEY_F9] && K_DEBUG)
   {
   JUG_HUMANO[2] = !JUG_HUMANO[2];
   key[KEY_F9] = FALSE;
   }

// DEBUG - F10 cambia el jugador 1 a humano/PC
if (key[KEY_F10] && K_DEBUG)
   {
   JUG_HUMANO[1] = !JUG_HUMANO[1];
   key[KEY_F10] = FALSE;
   }


// **************************
// JUGADOR #1
// FLECHAS: MOVER - INS: Dispara
// HOM, PgUp: Mover torreta - Del: Apagar/prender luz  
if (JUG_HUMANO[1] && (jugador_info[1].energia > -1)) {

// DISPARAR 
if (key[KEY_INSERT])
           {
            // Agrego un disparo en la punta de la torreta
            combo_disparar(1, deciseg);
//           key[KEY_INSERT] = FALSE;
           }

// apagar/prender luz (ON/OFF) [util para estrategia nocturna]
if (key[KEY_DEL])
          {
          key[KEY_DEL] = FALSE;
          jugador_info[1].luz_prendida = !jugador_info[1].luz_prendida;
          }
        // acelerar: si tiene el premio, es doble!
        if (key[KEY_UP]) {
	         jugador_info[1].vel += ACC_TA;
             if (  jugador_info[1].premio == VELOCIDAD_SPR ) jugador_info[1].vel += ACC_TA * 2;
          }; // fin acelera

        // frenar:
        if (key[KEY_DOWN]) {
 	         jugador_info[1].vel -= ACC_TA;
             if (  jugador_info[1].premio == VELOCIDAD_SPR ) jugador_info[1].vel -= ACC_TA * 2;
          }; // fin freno

        // doblar derecha
        if (key[KEY_RIGHT]) {
	               jugador_info[1].ang_dir += VEL_GTA;
                  if (jugador_info[1].ang_dir >  255) jugador_info[1].ang_dir -= 255;
              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
              if (validar_posicion(1)) {
                       jugador_info[1].ang_tor += VEL_GTA; 
                       if (jugador_info[1].ang_tor >  255) jugador_info[1].ang_tor -= 255;
                                       }; 
          }; // fin derecha

        // doblar izquierda
        if (key[KEY_LEFT]) {
	             jugador_info[1].ang_dir -= VEL_GTA;
                if (jugador_info[1].ang_dir < 0) jugador_info[1].ang_dir += 255;
              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
              if (validar_posicion(1)) {
                       jugador_info[1].ang_tor -= VEL_GTA; 
                       if (jugador_info[1].ang_tor < 0) jugador_info[1].ang_tor += 255;
                                       }; 
          }; // fin izquierda

        // -- TORRETA --
        // MOVER DERECHA  
        if (key[KEY_PGUP]) {
               jugador_info[1].ang_tor += VEL_GTO; 
               if (jugador_info[1].ang_tor > 255) jugador_info[1].ang_tor -= 255;
               validar_posicion(1);
          }; // fin izquierda

        // MOVER IZQUIERDA
        if (key[KEY_HOME]) {
              jugador_info[1].ang_tor -= VEL_GTO; 
              if (jugador_info[1].ang_tor < 0) jugador_info[1].ang_tor += 255;
              validar_posicion(1);
          }; // fin izquierda


}
else // si no, lo controla el PC
{
   // DEBUG
   // LLAMAR A LA RUTINA DE I.A AQUI (INTELIGENCIA ARTIFICIAL)
   // SOLO SI jugador_info[1].energia > -1
   if (jugador_info[1].energia > -1) mover_IA(1, deciseg);
} // fin de if (JUG_HUMANO[1])

// *************************************************************
// JUGADOR #2
// A,S,D,W: MOVER - U: Dispara
// I, O: Mover torreta - J: Apagar/prender luz 
if (JUG_HUMANO[2] && (jugador_info[2].energia > -1)) {

// NOTAR QUE ESTO ES TEMPORAL
// ACTUALIZAR CON EL DEL JUGADOR #1!!!!

// DISPARAR 
if (key[KEY_Q])
           {
            combo_disparar(2, deciseg);
//          key[KEY_Q] = FALSE;
           }

// apagar/prender luz (ON/OFF) [util para estrategia nocturna]
if (key[KEY_A])
          {
          key[KEY_A] = FALSE;
          jugador_info[2].luz_prendida = !jugador_info[2].luz_prendida;
          }
        // acelerar: si tiene premio, es mas rapido
        if (key[KEY_U]) {
        jugador_info[2].vel += ACC_TA;
             if (  jugador_info[2].premio == VELOCIDAD_SPR ) jugador_info[2].vel += ACC_TA * 2;

          }; // fin acelera

        // frenar:
        if (key[KEY_J]) {
        jugador_info[2].vel -= ACC_TA;
             if (  jugador_info[2].premio == VELOCIDAD_SPR ) jugador_info[2].vel -= ACC_TA * 2;
          }; // fin freno

        // doblar derecha
        if (key[KEY_K]) {
        jugador_info[2].ang_dir += VEL_GTA;
        if (jugador_info[2].ang_dir >  255) jugador_info[2].ang_dir -= 255;
              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
              if (validar_posicion(2)) {
                       jugador_info[2].ang_tor += VEL_GTA; 
                       if (jugador_info[2].ang_tor >  255) jugador_info[2].ang_tor -= 255;
                                       }; 
          }; // fin derecha

        // doblar izquierda
        if (key[KEY_H]) {
        jugador_info[2].ang_dir -= VEL_GTA;
                if (jugador_info[2].ang_dir < 0) jugador_info[2].ang_dir += 255;
              // si la nueva posicion es valida, mover la torreta
              // acompa¤ando el giro
              if (validar_posicion(2)) {
                       jugador_info[2].ang_tor -= VEL_GTA; 
                       if (jugador_info[2].ang_tor < 0) jugador_info[2].ang_tor += 255;
                                       }; 
          }; // fin izquierda

        // -- TORRETA --
        // MOVER DERECHA  
        if (key[KEY_E]) {
               jugador_info[2].ang_tor += VEL_GTO; 
               if (jugador_info[2].ang_tor > 255) jugador_info[2].ang_tor -= 255;
               validar_posicion(2);
          }; // fin izquierda

        // MOVER IZQUIERDA
        if (key[KEY_W]) {
              jugador_info[2].ang_tor -= VEL_GTO; 
              if (jugador_info[2].ang_tor < 0) jugador_info[2].ang_tor += 255;
              validar_posicion(2);
          }; // fin izquierda

}
else // si no, lo controla el PC
{
    // DEBUG
    // LLAMAR A LA RUTINA DE I.A AQUI (INTELIGENCIA ARTIFICIAL)
    // SOLO SI jugador_info[2].energia > 0

    if (jugador_info[2].energia > -1) mover_IA(2, deciseg);

} // fin de if (JUG_HUMANO[2])
// **************************
} // fin leer_teclado()
//--------------------------------------------------------


//--------------------------------------------------------
// Rutina que mueve los tanques de acuerdo a sus parametros
// Ademas, controla la energia de los mismos...
//--------------------------------------------------------
void mover_tanques()
{
int i1; // para los for
int i2; // evita un bucle al infinito al resituar los jugadores
ANGULO_TYPE angulotmp; // estructura temporal para recuperar la posicion
                       // en base a los angulos...

//-----------------------
// MOVER LOS TANQUES
// segun su aceleracion y su inercia
for (i1=1; i1<3; i1++)
{
 // corregir angulos
 if (jugador_info[i1].ang_dir >  255) jugador_info[i1].ang_dir -= 255;
 if (jugador_info[i1].ang_dir <  0) jugador_info[i1].ang_dir += 255;
 if (jugador_info[i1].ang_tor >  255) jugador_info[i1].ang_tor -= 255;
 if (jugador_info[i1].ang_tor <  0) jugador_info[i1].ang_tor += 255;

 // corregir velocidad
 if (jugador_info[i1].vel < -MAX_VEL_TA ) jugador_info[i1].vel = -MAX_VEL_TA;
 if (jugador_info[i1].vel > MAX_VEL_TA ) jugador_info[i1].vel = MAX_VEL_TA;


 // agregar explosiones si esta muerto...
 if (jugador_info[i1].energia < 0)
    {
    // seguir descontando energia (incinerarse)
    // (al llegar a -400, oficialmente muerto... ;^P)
    if ( (jugador_info[i1].energia -= random()%5+1 ) < -401) jugador_info[i1].energia = -401;

    // explosion...
    if ((random()%100 > 70) || (jugador_info[i1].energia < -350) )
                            combo_expl(jugador_info[i1].posx + random() % 20 - 10 + (JUG_X_MAX/2),
                                      jugador_info[i1].posy + random() % 20 - 10 + (JUG_Y_MAX/2),
                                      random() % 10 + 10,
                                      random() % 10 + 10 + (abs(jugador_info[i1].energia / 10)),
                                      random()%3+1);

    // si murio bastante, apagarle las luces
    if (jugador_info[i1].energia < -200) jugador_info[i1].luz_prendida = FALSE;

    if (jugador_info[i1].energia < -399) {
                            // explotar del todo...
                            combo_expl(jugador_info[i1].posx + random() % 20 - 10 + (JUG_X_MAX/2),
                                      jugador_info[i1].posy + random() % 20 - 10 + (JUG_Y_MAX/2),
                                      random() % 30 + 30,
                                      random() % 30 + 70,
                                      1);


                            // situar al jugador en otra posicion
                            // alejado del enemigo...
                           i2 = 0; //para evitar bucle infinito
                            do {
                                setea_jug(i1);
                                if (distancia_entre_jugadores() > 1000) i2 = 501; // salir, estan lejos...
                            } while ( ++i2 < 500 ); // 500 intentos y salir igual

                            // hace un efecto de chispas como que aparecio de algun lado!
                            agregar_particulas(jugador_info[i1].posx + (JUG_X_MAX/2),
                                               jugador_info[i1].posy + (JUG_Y_MAX/2),
                                               random()%40+20,
                                               random()%40+20,
                                               2,64,79);
                            agregar_particulas(jugador_info[i1].posx + (JUG_X_MAX/2),
                                               jugador_info[i1].posy + (JUG_Y_MAX/2),
                                               random()%40+20,
                                               random()%40+20,
                                               2,112,127);
                            agregar_particulas(jugador_info[i1].posx + (JUG_X_MAX/2),
                                               jugador_info[i1].posy + (JUG_Y_MAX/2),
                                               random()%20+20,
                                               random()%20+20,
                                               2,240,254);

                            agregar_luz(jugador_info[i1].posx + (JUG_X_MAX/2),
                                        jugador_info[i1].posy + (JUG_Y_MAX/2),
                                        random()%40+40,
                                        250,
                                        random()%40+40,
                                        -3);

                            // DEBUG
                            // sumar puntajes, informar, reiniciar mapa, etc...
                            // sumar puntaje y verificar q' no se pase del maximo (desbordamiento)
                            if (i1 == 1)
                               {
                                if (info_mapa_juego.puntaje[2]++ > MAXLONG - 1000) {info_mapa_juego.puntaje[2] = MAXLONG - 1000; }
                               }
                            else
                               {
                                if (info_mapa_juego.puntaje[1]++ > MAXLONG - 1000) {info_mapa_juego.puntaje[1] = MAXLONG - 1000; }
                               }
                            // 


                            } // fin reposicionar jugador muerto

    } // fin controlar energia


 if (jugador_info[i1].vel != 0)
    {

// ajustar la inercia hacia el lugar a donde vamos
   if (jugador_info[i1].ang_inercia > jugador_info[i1].ang_dir - VEL_GTA) jugador_info[i1].ang_inercia -= VEL_INE;
   if (jugador_info[i1].ang_inercia < jugador_info[i1].ang_dir + VEL_GTA) jugador_info[i1].ang_inercia += VEL_INE;

// que la inercia sea 0<i<255
   if (jugador_info[i1].ang_inercia > 255) jugador_info[i1].ang_inercia -= 255;
   if (jugador_info[i1].ang_inercia < 0) jugador_info[i1].ang_inercia += 255;

// si ambas se cumplen, inercia = direccion
   if (jugador_info[i1].ang_inercia > jugador_info[i1].ang_dir - VEL_GTA)
   if (jugador_info[i1].ang_inercia < jugador_info[i1].ang_dir + VEL_GTA)
                                jugador_info[i1].ang_inercia = jugador_info[i1].ang_dir;

// NOTAR QUE A LA VELOCIDAD LA DIVIDO POR 10
// PARA HACER UN EFECTO DE COMA FLOTANTE
// ES DECIR: 99 ES 9.9, 100 ES 10, ETC...
// ESTA EXPLICADO EN STARTGAM.H

// mover en el angulo especificado
// NOTAR QUE ES EL INVERSO 0-[XXXX]
   angulotmp = dame_angc360(0-pasa255a360(jugador_info[i1].ang_dir),
                            jugador_info[i1].vel / 10,
                            jugador_info[i1].posx,
                            jugador_info[i1].posy);

   jugador_info[i1].posx = angulotmp.x2;
   jugador_info[i1].posy = angulotmp.y2;

// validar la nueva posicion
// si no es valida, frenar el tanque (choco con algo!)
if (!validar_posicion(i1)) jugador_info[i1].vel = 0;

if (USA_INERCIA) {
    // -----------------
    // INERCIA
    // -----------------

    // mover con la inercia
       angulotmp = dame_angc360(0-pasa255a360(jugador_info[i1].ang_inercia),
                               jugador_info[i1].vel / 10 / VEL_INE,
                               jugador_info[i1].posx,
                               jugador_info[i1].posy);

       jugador_info[i1].posx = angulotmp.x2;
       jugador_info[i1].posy = angulotmp.y2;
    // validar la nueva posicion
    // si no es valida, frenar el tanque (choco con algo!)
       if (!validar_posicion(i1)) jugador_info[i1].vel = 0;
    // -----------------
    } // fin INERCIA

// desacelerar hacia 0
 if (jugador_info[i1].vel > 0) {
     jugador_info[i1].vel -= FRICCION; // desacelerar...
     if (jugador_info[i1].vel < 0)  jugador_info[i1].vel = 0;
     }
 if (jugador_info[i1].vel < 0) {
     jugador_info[i1].vel += FRICCION; // desacelerar...
     if (jugador_info[i1].vel > 0)  jugador_info[i1].vel = 0;
     }

// CUANDO LA VELOCIDAD ES 0
// LA INERCIA ES = A LA DIRECCION
  if (jugador_info[i1].vel == 0) jugador_info[i1].ang_inercia = jugador_info[i1].ang_dir;

// dejar huella de las orugas del tanque (rutina de efectos1.c)
  dejar_huella_t(i1);

} // fin de ver si la vel es > 0

  // corregir la velocidad
  if (jugador_info[i1].vel < -MAX_VEL_TA ) jugador_info[i1].vel = -MAX_VEL_TA;
  if (jugador_info[i1].vel > MAX_VEL_TA ) jugador_info[i1].vel = MAX_VEL_TA;

} // fin for i1
// FIN MOVER LOS TANQUES
//-----------------------

} // fin mover_tanques
//--------------------------------------------------------

#endif
