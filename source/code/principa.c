//--------------------------------------------------------
// principa.c   Kodigo principal del juego
// principa.h
//          Inicia Allegro, instala teclado, testea modos de video, sonido
//          y crea las tablas predefinidas de seno, cos, transparencia, etc
//          Ademas, contiene el manual principal de juego.-
//--------------------------------------------------------
// KRONOMAN'S BATTLE ZONE
//
// Juego al estilo del viejo BATTLE ZONE
//
// Kreado por Kronoman
// Trabaja en modo X de 320x240x256c y 320x480 (en modo de 2 jugadores)
// Empezado 31-D-2000
// Este archivo: 3-E-2001 4.30 am
//
// Codigo en lenguaje DJGPP + Allegro
// - En memoria de mi querido padre -
//--------------------------------------------------------
#ifndef PRINCIPA_C
#define PRINCIPA_C

// inclusiones standard
#include <stdlib.h>
#include <stdio.h>
// sin, cos, etc
#include <math.h>
// de aca uso las rutinas de colores para modo texto, etc...
#include <conio.h>
// para comparar las strings que puedo necesitar
#include <string.h>
// infaltable... Allegro...
#include <allegro.h>
// para el delay(msec); y eso
#include <dos.h>
// para las rutinas de azar, y rawclock() (ticks, 18.2 p/segundo)
#include <time.h>

//--------------------------------------------------------
// Inclusion del codigo propio mio...
//--------------------------------------------------------
#include "incluir.h" // aqui estan todos los includes necesarios...

//--------------------------------------------------------
// Codigo de las rutinas
//--------------------------------------------------------
//--------------------------------------------------------
// Funcion_salir
// Esta rutina es llamada automaticamente cuando el programa
// termina de forma normal; setea el modo texto y pone un mensaje
// muy bonito...
// es seteada con atexit(funcion_salir);
//--------------------------------------------------------
void funcion_salir()
 {

 // frases
char *Frases[20] = {
"El infierno esta encantador esta noche...",
"DOS + Win 3.1 = W95",
"La cumbia apesta...",
"La inspiraci¢n crece desde el ca§on de un arma...",
"No importaba la manzana, tan solo la prohibicion...",
"No es vergÅenza ser pobre, y es vergÅenza ser ladr¢n",
"Las minitas aman los payasos y la pasta de campe¢n",
"Tanquecitos = diversion!",
"Kill em' all",
"Matalos a todos. Que Dios los juzgue.",
"Yo no tengo la culpa de verte caer...",
"I wanna rock & roll all night...",
"DJGPP + Allegro = el mejor compilador del planeta",
"Micro$oft go home...",
"Larga vida al DOS!",
"Golpeala con el bate!",
"Riesgo pais: 0 puntos (marte)",
"No pises mis zapatos de gamuza azul...",
"Larga vida al rock 'n roll!" };

int C_Frases = 19;


        textcolor(WHITE); textbackground(BLACK);
        clrscr();
        textcolor(WHITE); // blanco
        textbackground(BLUE); // azul
        // linea superior con azul, etc
        gotoxy(1,1);
        cprintf("Kronoman's Battle Zone                                                         \n\r");
        cprintf("Kreado por Kronoman - (c) 2000,2001,2002 - Freeware (Gratuito)                 \n\r");
        cprintf("En memoria de mi querido padre                                                 \n\r");
        textcolor(WHITE); textbackground(BLACK);

        cprintf("'%s'\n\n\n\n\r", Frases[random()%C_Frases] );

    
}

//--------------------------------------------------------
//  Main
// inicializa todo el juego
//--------------------------------------------------------
// comienzo de main, con argumentos y toda esa mierda...

int main(int argc, char *argv[])
{
// variables
int i1; //dummy para los for 

// iniciar todo lo bueno...

// seteo la rutina que debe ejecutarse al finalizar el programa
// en realidad es un peque§o mensaje con variado bla bla bla
     atexit(funcion_salir); 

// cargar configuracion

   // crea el nombre de archivo de configuracion a leer
   // con el path del ejecutable actual
   replace_filename(cfg_arch, argv[0], "krbz.cfg", sizeof(cfg_arch));

   cprintf("Archivo de configuracion: \n\r ");
   cprintf(" %s \n\r", cfg_arch);

   cprintf("Abriendo archivo de configuracion... ");

   leer_configuracion(cfg_arch);


// comprobar los parametros, a partir del 1, el 0 es el path del exe
// los parametros sobreescriben la configuracion de disco...
for (i1=1; i1<argc; i1++) {

    // parametro -debug activa el debugger...
      if (stricmp(argv[i1], "-debug") == 0) K_DEBUG = TRUE;

    //------
    // Sonido OFF
    if (stricmp(argv[i1], "-nosound") == 0) SND_DESEO_SONIDO = FALSE;

    //------
    // PANTALLA
    // parametro para desactivar pantalla partida (notar que es sobreescrito si se juega de a 2)
     if (stricmp(argv[i1], "-nosplit") == 0) PANT_SPLIT = FALSE;
    // parametro para activar pantalla partida
    if (stricmp(argv[i1], "-split") == 0) PANT_SPLIT = TRUE;

    //------
    // LUCES
    if (stricmp(argv[i1], "-noluz") == 0) VER_LUCES = FALSE;
    if (stricmp(argv[i1], "-l0") == 0) MODO_DE_LUZ = 0;
    if (stricmp(argv[i1], "-l1") == 0) MODO_DE_LUZ = 1;
    if (stricmp(argv[i1], "-l2") == 0) MODO_DE_LUZ = 2;

    //-------
    // RESOLUCIONES

    // modo standard de la VGA: 320x200 (se puede 320x100 o 160x120)
    // ULTRA RAPIDO PERO SE VE FEO
      if (stricmp(argv[i1], "-rVGA") == 0) {
             MODO_VIDEO = GFX_VGA;
             H_RES = 320; 
             V_RES = 200; 
                        }

    // modos standard de la VGA MODO-X (lento!)
      if (stricmp(argv[i1], "-r320240") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 320; 
             V_RES = 240; 
                        }

      if (stricmp(argv[i1], "-r320400") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 320; 
             V_RES = 400; 
                        }

      if (stricmp(argv[i1], "-r320480") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 320; 
             V_RES = 480; 
                        }

      if (stricmp(argv[i1], "-r320600") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 320; 
             V_RES = 600;
                        }

    // resoluciones raras de 360xXXX
      if (stricmp(argv[i1], "-r360200") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 360;
             V_RES = 200;
                        }

      if (stricmp(argv[i1], "-r360240") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 360;
             V_RES = 240;
                        }

      if (stricmp(argv[i1], "-r360360") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 360;
             V_RES = 360;
                        }

      if (stricmp(argv[i1], "-r360400") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 360;
             V_RES = 400;
                        }

      if (stricmp(argv[i1], "-r360480") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 360;
             V_RES = 480;
                        }


    // modos de video VESA 
      if (stricmp(argv[i1], "-r640480") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 640; 
             V_RES = 480; 
                        }

      if (stricmp(argv[i1], "-r800600") == 0) {
             MODO_VIDEO = GFX_AUTODETECT;
             H_RES = 800; 
             V_RES = 600; 
                        }


} // fin for 'i1': ver parametros


// COMIENZO DE LA INICIALIZACION DEL PROGRAMA
// primero, la pantalla en modo texto al estilo DOOM II  ;^P
        textcolor(WHITE); textbackground(BLACK);
        clrscr();
        textcolor(WHITE); // blanco
        textbackground(BLUE); // azul
        // linea superior con azul, etc
        gotoxy(1,1);
        cprintf("Kronoman's Battle Zone                                                          ");
        gotoxy(1,23);
        cprintf("Kreado por Kronoman - (c) 2001                                                  ");

// ventana de texto comun de la 3ra linea a casi la ultima
        window(1, 3, 80, 22);
        textcolor(LIGHTGRAY); // blanco
        textbackground(BLACK); // negro
        cprintf("Version 1.00 - (c) 2001 Kronoman\n\n\rInicializando el juego... \n\n\r");

// va llamando a las rutinas y muestra lo que hace
//----------------
// inicia Allegro
// instala teclado, mouse y timer...
   allegro_init();
cprintf ("\n\rA_Init() Ok! \n\r");

   install_keyboard();
cprintf ("I_Kb: Manejador de teclado Ok \n\r");

if  ( install_mouse() < 0 )
    cprintf("ERROR: NO HAY MOUSE INSTALADO (O FALTA SU DRIVER) \n\r");
                
cprintf ("I_Ms: Manejador del mouse Ok \n\r");
   install_timer();
cprintf ("I_Ti: Manejador de timer Ok \n\r");

//----------------
// instalar sonido
cprintf("Modulo de Sonido: ");
if (inicializar_modulo_de_sonido())
   cprintf(" OK \n\r");
   else
   cprintf(" FALLO \n\r");

//-------------------------
// chequea el sistema operativo...
cprintf("Sistema operativo:");
switch (os_type)
{
case OSTYPE_UNKNOWN:
    cprintf("DOS o similar...\n\r");
    break;
case OSTYPE_WIN3:
    cprintf("Win 3.1 o similar...\n\r");
    break;
case OSTYPE_WIN95:
    cprintf("Windows 95...\n\r");
    break;
case OSTYPE_WIN98:
    cprintf("Windows 98...\n\r");
    break;
case OSTYPE_WINNT:
    cprintf("Windows NT...\n\r");
    break;
case OSTYPE_OS2:
    cprintf("OS/2...\n\r");
    break;
case OSTYPE_WARP:
    cprintf("OS/2 Warp...\n\r");
    break;
case OSTYPE_DOSEMU:
    cprintf("Linux DOSEMU...\n\r");
    break;
case OSTYPE_OPENDOS:
    cprintf("Caldera OpenDOS...\n\r");
    break;
default:
    cprintf("Desconocido... :^( \n\r");
    break;
} // fin switch

if (K_DEBUG == TRUE) {
        cprintf ("-- Modo Debug; Presione una tecla para continuar --\n\r");
        clear_keybuf();
        i1 = readkey();
     };

// ----------fin ver sistema operativo-------


// ---------tablas precalculadas------------
// rutina para pre-calcular los senos y cosenos de 0-360 grados (no preciso pasar a radianes luego!)
cprintf ("Precalculando tabla de senos,cosenos, tangente 0-360 \n\r");
// debug info
   for(i1=0; i1<361; i1++) {
         KT_COS[i1] = cos(i1 * KRONO_PI);
         KT_SIN[i1] = sin(i1 * KRONO_PI);
         KT_TAN[i1] = tan(i1 * KRONO_PI);

    if (K_DEBUG == FALSE) cprintf("%03d de 360\r", i1);
                else
                 {
                  cprintf ("%d COS = %f -- ", i1, KT_COS[i1]);
                  cprintf ("%d SIN = %f --", i1, KT_SIN[i1]);
                  cprintf ("%d TAN = %f \n\r", i1, KT_TAN[i1]);
                }
        } // fin for i1

if (K_DEBUG == TRUE) {
//        cprintf ("Modo debug ON \n\r");
        cprintf ("Pi %f -- ", M_PI);
        cprintf ("Pi/180 %f \n\r", KRONO_PI);
}


//----------------
cprintf ("Iniciando juego...   Ok \n\r");
//----------------
// Directorio de juego, argv[0] contiene el path full al exe
// entonces, tomo el path y lo uso y lo inicializo en lo necesario...
//
cprintf("\n\r Archivo EXE en uso: \n\r ");
cprintf(">> %s \n\r", argv[0]  );

// crea el nombre de archivo
replace_filename(buf_arch, argv[0], "kronobz.dat", sizeof(buf_arch));

cprintf("\n\r Archivo DAT a utilizar (KRONOBZ.DAT) \n\r ");
cprintf(">> %s \n\r", buf_arch);

cprintf("Abriendo archivo... \n\r");
   krono_dato = load_datafile(buf_arch);
   if (!krono_dato) {
      allegro_exit();
      cprintf("Error cargando %s!\n\n\r", buf_arch);
      exit(1);
   }


cprintf("Modo de video durante el juego: [%d]x[%d]x256c \n\r",H_RES,V_RES);

cprintf("OK!... \n\r");
//----------------
// si esta en modo debug, esperar por una tecla...
if (K_DEBUG == TRUE) {
   cprintf ("-- Modo Debug; Presione una tecla para continuar --\n\r");
        clear_keybuf();
        i1 = readkey();
	       cprintf("-- Realizando FADE OUT --\n\r");
     };


// inicia el generador de numeros aleatorios
srandom(time(NULL));
cprintf("Genererador de numeros aleatorios: Semilla aproximada %d\n\r", time(0));


cprintf("--INICIANDO MODO-X : 320x240x256 colores--\n\r");

// restaura la ventana de texto
        window(1, 1, 80, 24);

// hace un fade out
   fade_out(8);
//----------------
// setea el modo de video a 320x240x256 colores
set_color_depth(8); // 8 bits de color = 256 colores
if (  set_gfx_mode(GFX_AUTODETECT, 320, 240, 0, 0)<0)
  {
   cprintf("--MODO-X 320x240x256 FALLO - ABORTANDO PROGRAMA--\n\r");
   exit(1);
   }

// -------aun no termina!!
// Ahora que estoy en modo grafico, cargo y calculo los efectos
// para las paletas de colores...

// almacena el font actual
font_original = font;

//----------------
// MAS TABLAS PRECALCULADAS (TRANSPARENCIA, ILUMINACION, RGB)

        // TOMAR PALETA DEL JUEGO
        // lee la paleta del .dat
        set_palette(krono_dato[Pal_Juego].dat);
        // la coloca en pal_game
        get_pallete(pal_game);

        // tabla RGB  del juego
           create_rgb_table(&rgb_table, pal_game, NULL);
           rgb_map = &rgb_table;

        // iluminacion del juego
           create_light_table(&light_table, pal_game, 0, 0, 0, NULL);

/*********************** NO SIRVE PARA LO QUE QUERIA
// PUEDE SERVIR PARA OTROS EFECTOS INTERESANTES
// SI SE APLICA CON COLORES EN VEZ DE GRISES
// COMO LENS FLARES PARA LAS BOMBAS, ETC...

        // paleta de 256 grises (64 en grupos de 4)
        crear256grises(pal_tmp); // de mapluz.c

        // creo mi propio mapa aditivo de colores... con los 256 grises
        // usando mi propia rutina de mapluz.c
         create_color_table(&aditivo_table,
                             pal_tmp,
                             crear_mapa_aditivo,
                             NULL);

        // tabla RGB  de la tabla de 256 grises
          create_rgb_table(&rgb_256gris, pal_tmp, NULL);

*********************************/

// ayudante que realmente hace el trabajo de iluminacion Ok
// lo guarda en el mapa greyscale_add_map;  
        crear_mapa_aditivo_luces(); // contenido en mapluz.c
//---------


        // transparencia para el juego
           create_trans_table(&trans_table, pal_game, 128, 128, 128, NULL); // transparencia del 50%
        // transparencia 
           create_trans_table(&tanque_invisible_table, pal_game, 56 , 56 , 56, NULL); // transparencia mayor para el premio de invisibilidad

        // TOMAR PALETA DEL MENU
        // lee la paleta del .dat
        set_palette(krono_dato[Menu_Paleta].dat);
        // la coloca en pal_menu
        get_pallete(pal_menu);

        // tabla RGB para el menu
           create_rgb_table(&rgb_table_menu, pal_menu, NULL);
           rgb_map = &rgb_table_menu;
        
        // transparencia del puntero del mouse del menu
           create_trans_table(&menu_l_table, pal_menu, 128, 128, 128, NULL);

// ---------------------------
// casi el final de iniciar:
// llama al menu principal
menu_principal();

// ---------------------------
// cuando vuelve el menu, terminar el programa normalmente
set_gfx_mode(GFX_TEXT,0,0,0,0); // modo texto

// SALVAR CONFIGURACION...
salvar_configuracion(cfg_arch);

return(0);
}
// fin del procedimiento main
//--------------------------------------------------------




//--------------------------------------------------------
// Menu principal del juego
//
// Opciones: JUGAR - OPCIONES- SALIR y los CREDITOS
// Se NECESITA mouse para usarlo... oops...
//--------------------------------------------------------
void menu_principal()
{
// variables dummy para controlar el mov del mouse
int xmous_viejo = -1, ymous_viejo = -1;
int xmous_nuevo = 1 ,ymous_nuevo= 1;
int item_elegido = -1; // item seleccionado en el menu
                       // 1=creditos, 2=jugar, 3=opciones, 4=salir, 0/-1 nada
char tmpstr[40]; // string para mostrar el debug...


// bitmaps temporales
BITMAP *bmptmp1;
BITMAP *bmptmp2;

// esperar hasta que suelte el mouse
// para poder entrar en el bucle principal
do {
} while (mouse_b);
// limpia el teclado
clear_keybuf();

// entra en el bucle
// hasta que apriete el mouse o una tecla

// crea los bitmaps
bmptmp1 = create_bitmap(320, 240);

// carga la imagen del mouse
bmptmp2 = krono_dato[KMousePuntero].dat;

do { // repetir mientras no apriete nada del menu

// cargo la imagen de fondo en la pantalla
    blit(krono_dato[Menu01].dat, screen, 0, 0, 0, 0, 320, 240);

// seteo la paleta del menu
    set_palette(krono_dato[Menu_Paleta].dat);

// selecciono la transparencia del menu
    color_map = &menu_l_table;
    rgb_map = &rgb_table_menu;

// para que redibuje el cursor
    xmous_viejo = -1; ymous_viejo = -1;

    do { // repetir mientras no apriete boton o tecla

    // si el mouse se mueve, redibuja la pantalla
    // con un puntero transparente (joya!!!)

       xmous_nuevo = mouse_x ;
       ymous_nuevo = mouse_y ;

      // redibujar
    if (( xmous_nuevo !=  xmous_viejo) || ( ymous_nuevo !=  ymous_viejo))
      {

            xmous_viejo = mouse_x ;
            ymous_viejo = mouse_y ;

    // copia la imagen original del fondo
      blit(krono_dato[Menu01].dat, bmptmp1, 0, 0, 0, 0,320,240);

    // ver si debe activar alguno de los menues
    item_elegido = -1; // nada elegido

    // titulo "KRONO'S BATTLE ZONE"
    if ((xmous_nuevo >40) &&  (xmous_nuevo <290) && (ymous_nuevo >0) &&  (ymous_nuevo <70)) 
        {
          blit(krono_dato[Menu02].dat, bmptmp1,40, 0,40, 0,250,70);
          item_elegido = 1;
        }

    // titulo "JUGAR"
    if ((xmous_nuevo >80) &&  (xmous_nuevo <240) && (ymous_nuevo >80) &&  (ymous_nuevo <110)) 
        {
          blit(krono_dato[Menu02].dat, bmptmp1, 80, 80, 80, 80, 160, 50);
          item_elegido = 2;
        }

    // titulo "OPCIONES"
    if ((xmous_nuevo >45) &&  (xmous_nuevo <270) && (ymous_nuevo >125) &&  (ymous_nuevo <160)) 
        {
          blit(krono_dato[Menu02].dat, bmptmp1,45, 125,45, 125,225,50);
          item_elegido = 3;
        }

    // titulo "SALIR"
    if ((xmous_nuevo >99) &&  (xmous_nuevo <230) && (ymous_nuevo >175) &&  (ymous_nuevo <210)) 
        {
          blit(krono_dato[Menu02].dat, bmptmp1,99, 175,99,175,131,55);
          item_elegido = 4;
        }
   

    // dibuja el puntero del mouse
      draw_trans_sprite(bmptmp1, bmptmp2, xmous_nuevo, ymous_nuevo);

    // si esta en debug mode
    // mostrar la x,y
    if (K_DEBUG == TRUE) {
      text_mode(-1); // texto transparente
      font = krono_dato[FontXM6x6].dat; // font peque§o
      sprintf(tmpstr,"X,Y: %d,%d", mouse_x, mouse_y);
      textout(bmptmp1, font, tmpstr, 0, 0, 0); // en color negro...
    };

    // envia todo a pantalla
      blit(bmptmp1, screen, 0, 0, 0, 0, 320, 240);

  } // fin redibujar

    if (keypressed()) {
        // soporte para teclado - rudimentario
         if (key[KEY_ESC]) item_elegido = 4; // si apreto <ESC>, elegir salir
         if (key[KEY_UP])  item_elegido--; // si apreto <UP> subir item menu
         if (key[KEY_DOWN])  item_elegido++; // si apreto <UP> subir item menu

        if (item_elegido<1) item_elegido=4;
        if (item_elegido>4) item_elegido=1;

        position_mouse(90,90); // situar sobre jugar por las dudas...

        // posicionar mouse
        if (item_elegido==1) position_mouse(50,20);
        if (item_elegido==2) position_mouse(90,90);
        if (item_elegido==3) position_mouse(50,130);
        if (item_elegido==4) position_mouse(110,180);
        
        xmous_viejo = -1; // para que actualize la pantalla
        
        if (!key[KEY_ENTER]) clear_keybuf(); // limpiar teclado si NO es <ENTER>
    } // fin de soporte para teclado

  } while ((!key[KEY_ENTER]) && (!mouse_b)); // fin del while hasta que apriete algo
// apreto algo, interpretar


clear_keybuf(); // limpiar teclado

// meter codigo aca para interpretar la eleccion del menu
// 1=creditos
// 2=jugar
// 3=opciones
// 4=salir

// mostrar creditos
  if (item_elegido ==1) {
        xmous_viejo = -1; ymous_viejo = -1; // para que luego redibuje... ;D
        item_elegido = -1; // para que no salga del loop principal
        fade_out(8); // hace un fade out
        mostrar_mi_foto();
        }
// juego nuevo
// empezar a jugar WOWWWW WOWWWW WOWWW YEAHHH!!! AJAJAJA SON LAS 5AM!!!
  if (item_elegido ==2) {
        xmous_viejo = -1; ymous_viejo = -1; // para que luego redibuje... ;D
        item_elegido = -1; // para que no salga del loop principal
        fade_out(8); // hace un fade out
        clear(screen); // limpia pantalla

        // Menu de inicio del juego...
        menu_init_juego();

        // setear la paleta del juego
        set_palette(krono_dato[Pal_Juego].dat);

        // setear iluminacion, rgb, transparencia del juego
           rgb_map = &rgb_table;
           color_map = &light_table;   // cambiar por &trans_table cuando desee usar transparencia

        // colocar mensaje
        textout_centre(screen, font_original, "GENERANDO MAPA-ESPERE POR FAVOR", SCREEN_W / 2, SCREEN_H / 2, 255);

        iniciar_juego_azar(); // empieza el susodicho juego... (todo al azar)

        clear(screen); // evitar que los colores se pongan raros al volver...
        }

  // OPCIONES 
    if (item_elegido == 3)
    {
    item_elegido = -1;
    fade_out(8); // hace un fade out
    clear(screen); // limpia pantalla

    configurar_opciones(); 

    clear(screen);
    }

} while (item_elegido<1); // mientras no elija nada seguir...


// libera los bitmaps
destroy_bitmap(bmptmp1);
destroy_bitmap(bmptmp2);

// hace un fade out
fade_out(8);
// vuelve a main
// y supuestamente, sale del programa...

}
// fin del menu_principal
//--------------------------------------------------------

//--------------------------------------------------------
// Pantalla de los creditos
// Muestra una foto mia y pasa algunos creditos
// Bastante agradable... :P
// Apretando el mouse o el teclado volves al menu principal
//--------------------------------------------------------
//
//
//
void mostrar_mi_foto()
{
// cantidad 
int kredito_max = 20; // 1 menos que el indice
// strings con los kreditos
char *Kreditos[21] = {
"Krono's Battle Zone",
"Kreditos",
"Una Produccion De KronoCorp",
"Kreado Por Kronoman",
"(c) Diciembre 2000 - Enero 2002",
"Hecho En Argentina - Freeware",
"Saludos A:",
"Victoria",
"Yaco",
"SkaarjScout",
"Iron Chelo",
"Dj Nanga",
"Poroto",
"J Monytor",
"BloodyMary",
"El Mago Eddy",
"Super666",
"Veronica, Yunni y Claudia",
"PROGRAMA FREEWARE (GRATIS)",
"En memoria de mi padre",
"(c) 2000-2002, KronoCorp"};

// kredito actual
int kredito_actual = 0;


// esperar hasta que suelte el mouse
// para poder entrar en el bucle principal
do {
   } while (mouse_b);
// limpia el teclado
clear_keybuf();

// seteo la paleta
set_palette(krono_dato[Menu_Paleta].dat);
// cargo la imagen de fondo
blit(krono_dato[Krono_Credito].dat, screen, 0, 0, 0, 0,320,240);


// espero por una tecla o mouse...
// MIENTRAS PASO LOS CREDITOS EN PANTALLA
do {
        // ir pasando los creditos

        text_mode(-1); // texto transparente
        //  font = font_original; // font original 8x8 (grandote, grueso)

        // font rubber stamp esta joyisima de 12 puntos -> medio grande :( 
        font = krono_dato[KFont_Cool].dat;

        //sombra texto en negro
        textout_centre(screen, font, Kreditos[kredito_actual], (SCREEN_W/2)+3, (SCREEN_H/3)+3, 0); // 0=negro

        //texto 
        textout_centre(screen, font, Kreditos[kredito_actual], SCREEN_W/2, SCREEN_H/3, 1); // 1=blanco

        kredito_actual++;
        if (kredito_actual>kredito_max) kredito_actual=0;
        
        //pausa de xxx milisegundos
        delay(1500);
        // cargo la imagen de fondo de nuevo
         blit(krono_dato[Krono_Credito].dat, screen, 0, 0, 0, 0,320,240);

  } while ((!keypressed()) && (!mouse_b)); // fin del while hasta que apriete algo


// esperar hasta que suelte el mouse
// para poder volver en el bucle principal
do {
   } while (mouse_b);
// limpia el teclado
clear_keybuf();

fade_out(8);

// volver al menu
return;
}
// fin de  mostrar_mi_foto()
//--------------------------------------------------------

#endif
