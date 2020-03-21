//--------------------------------------------------------
// menues.c
// Menus del juego
// Menu de opciones, menu de juego nuevo, etc
// Usa las rutinas GUI de Allegro
// Esto setea MUCHAS variables globales del juego!
// OJO!!
// (c) 2001, Kronoman
//--------------------------------------------------------
#ifndef MENUES_C
#define MENUES_C


//--------------------------------------------------------
// Menu que permite las opciones de 1 o 2 jugadores
// o demo.
// Llamar antes de comenzar el juego para que
// setea las variables globales.
// Poner la pantalla en 320x240 antes!. Ojo!
//--------------------------------------------------------
void menu_init_juego()
{
int op = 0; // opcion del menu

    // esperar hasta que suelte el mouse
    // para poder entrar en el bucle principal
    do { } while (mouse_b);
    // limpia el teclado
    clear_keybuf();

    // seteo la paleta
    set_palette(krono_dato[Menu_Paleta].dat);
    // cargo la imagen de fondo
    blit(krono_dato[MNU_FONDO_GENERICO].dat, screen, 0, 0, 0, 0,320,240);

    text_mode(-1); // texto transparente

    // instrucciones abajo
    font = krono_dato[FontXM6x6].dat; // font peque¤o
    textout_centre(screen,font, "Presione 1,2 o 3 para comenzar.", SCREEN_W/2, SCREEN_H - text_height(font), 0);


    // Texto
    font = krono_dato[FONT_GRANDE].dat;
    // negro
    textout_centre(screen, font,"JUGAR" , SCREEN_W/2, 2, 0);
    textout_centre(screen, font,"Dos Jugadores" , SCREEN_W/2, (SCREEN_H/4), 0);
    textout_centre(screen, font,"Humano Vs CPU" , SCREEN_W/2, (SCREEN_H/4)*2, 0); 
    textout_centre(screen, font,"Demostracion" ,  SCREEN_W/2, (SCREEN_H/4)*3, 0); 

//    textout_centre(screen, font,"JUGAR" , SCREEN_W/2, 4, 1);
//    textout_centre(screen, font,"Dos Jugadores" , SCREEN_W/2, (SCREEN_H/4) + 2, 1);
//    textout_centre(screen, font,"Humano Vs CPU" , SCREEN_W/2, ((SCREEN_H/4)*2) + 2 , 1); 
//    textout_centre(screen, font,"Demostracion" ,  SCREEN_W/2, ((SCREEN_H/4)*3) + 2, 1); 

  // --------------------------------------------
  // menu - loop principal
  // --------------------------------------------

  // mostrar mouse
  set_mouse_sprite(krono_dato[KMousePuntero].dat);
  show_mouse(screen);

  do {
      // verificar mouse
      if (mouse_b) {
         if ( (mouse_y > (SCREEN_H/4)) && (mouse_y < (SCREEN_H/4)+ text_height(font) ) )
            {
                // opcion 1 - dos jugadores
                op = 1;
            }
         if ( (mouse_y > (SCREEN_H/4)*2) && (mouse_y < ((SCREEN_H/4)*2) + text_height(font) ) )
            {
                // opcion 2 - un jugador
                op = 2;
            }
         if ( (mouse_y > (SCREEN_H/4)*3) && (mouse_y < ((SCREEN_H/4)*3)+ text_height(font) ) )
            {
                // opcion 3 - demostracion 
                op = 3;
            }
      } // fin boton mouse

      // verificar teclado
      if (key[KEY_1]) op =1;
      if (key[KEY_2]) op =2;
      if (key[KEY_3]) op =3;

  } while (op == 0); // mientras no elija nada...

  // ocultar mouse
  set_mouse_sprite(NULL);
  show_mouse(NULL);
  // --------------------------------------------
  // Setear globales

  if (op == 1) { // 2 jugadores...
      PANT_SPLIT = TRUE;
      JUG_HUMANO[1] = TRUE;
      JUG_HUMANO[2] = TRUE;
  }

  if (op == 2) { // 1 jugador...
      PANT_SPLIT = FALSE;
      JUG_HUMANO[1] = TRUE;
      JUG_HUMANO[2] = FALSE;
  }

  if (op == 3) { // demostracion
      PANT_SPLIT = TRUE;
      JUG_HUMANO[1] = FALSE;
      JUG_HUMANO[2] = FALSE;
  }


  // --------------------------------------------
  // volver
  // esperar hasta que suelte el mouse
  // para poder volver en el bucle principal
  do { } while (mouse_b);
  // limpia el teclado
  clear_keybuf();


  fade_out(8);
  clear(screen);
  set_palette(krono_dato[Menu_Paleta].dat);

  // volver al menu
  return;

}


//--------------------------------------------------------
// Pantalla que muestra el puntaje al finalizar el juego
// Pasarle en punt1 el puntaje del jugador1,
// en punt2 el puntaje del jugador 2
//--------------------------------------------------------
void mostrar_puntaje(long int punt1, long int punt2)
{
int s = 0;

    clear(screen);

    // cargo la imagen de fondo en la pantalla
    blit(krono_dato[MNU_FONDO_GENERICO].dat, screen, 0, 0, 0, 0, 320, 240);

    // seteo la paleta del menu
    set_palette(krono_dato[Menu_Paleta].dat);
    text_mode(-1); // texto transparente

    // Texto
    font = krono_dato[FONT_GRANDE].dat;

    textprintf_centre(screen, font, 160, 20, 0,  "Fin del Juego");
    textprintf_centre(screen, font, 160, 60, 0,  "Jugador #1:[%04ld]", punt1);
    textprintf_centre(screen, font, 160, 120, 0, "Jugador #2:[%04ld]", punt2);


    // instrucciones abajo
    font = krono_dato[FontXM6x6].dat; // font peque¤o
    textout_centre(screen,font, "Presione cualquier tecla para continuar.", SCREEN_W/2, SCREEN_H - text_height(font), 0);


    // esperar hasta que suelte el mouse
    // para poder entrar en el bucle principal
    do { } while (mouse_b);
    // limpia el teclado
    clear_keybuf();




  // mostrar mouse
  set_mouse_sprite(krono_dato[KMousePuntero].dat);
  show_mouse(screen);
  do {
    if (keypressed()) s = 1;
    if (mouse_b) s = 1;
  } while (s == 0);

  // ocultar mouse
  set_mouse_sprite(NULL);
  show_mouse(NULL);

  // --------------------------------------------
  // volver
  // esperar hasta que suelte el mouse
  // para poder volver en el bucle principal
  do { } while (mouse_b);
  // limpia el teclado
  clear_keybuf();

  fade_out(8);
  clear(screen);
  set_palette(krono_dato[Menu_Paleta].dat);



return;
}



//--------------------------------------------------------
// Menu que permite las opciones del juego
// modo de video, detalle, sonido, etc
//--------------------------------------------------------
void configurar_opciones()
{
int ret; // para el valor de do_dialog


DIALOG cuadro_dialogo_1[] =
{
   // al poner en bg -1, el texto es transparente...
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)                    (d2)  (dp)              (dp2) (dp3) */
   // 0
   { d_poner_fondo_proc, 0,    0,    0,    0,    0,   -1,    0,    0,       0,                      0,    NULL,             NULL, NULL  },
   // detalle grafico 
   // 1
   { d_ctext_proc,     159,   10,   318 ,  10,   0,   -1,     0,    0,       0,                     0,    "Nivel de detalle grafico:",         NULL, NULL  },
   // 2,3,4
   { d_krono_radio_proc, 3,   23,    50 ,  10,   0,   -1,     0,    0,       1,                     0,    "Minimo",         NULL, NULL  },
   { d_krono_radio_proc,109,  23,    50 ,  10,   0,   -1,     0,    0,       1,                     0,    "Bajo  ",         NULL, NULL  },
   { d_krono_radio_proc,215,  23,    50 ,  10,   0,   -1,     0,    0,       1,                     0,    "Normal",         NULL, NULL  },

   // resolucion 
   { d_ctext_proc,     159,   70,   318 ,  10,   0,   -1,     0,    0,       0,                     0,    "Resolucion grafica",NULL, NULL  },
   // 6,7,8
   { d_krono_radio_proc,3,    83,    50 ,  10,   0,   -1,     0,    0,       2,                     0,    "320x200",         NULL, NULL  },
   { d_krono_radio_proc,109,  83,    50 ,  10,   0,   -1,     0,    0,       2,                     0,    "320x240",         NULL, NULL  },
   { d_krono_radio_proc,215,  83,    50 ,  10,   0,   -1,     0,    0,       2,                     0,    "320x480",         NULL, NULL  },

   // 9,10,11
   { d_krono_radio_proc,3,   96,     50 ,  10,   0,   -1,     0,    0,       2,                     0,    "360x480",         NULL, NULL  },
   { d_krono_radio_proc,109, 96,     50 ,  10,   0,   -1,     0,    0,       2,                     0,    "640x480",         NULL, NULL  },
   { d_krono_radio_proc,215, 96,     50 ,  10,   0,   -1,     0,    0,       2,                     0,    "800x600",         NULL, NULL  },

   // nivel de dificultad 
   { d_ctext_proc,     159,  130,   318 ,  10,   0,   -1,     0,    0,       0,                     0,    "Nivel de dificultad (Humano vs CPU)",NULL, NULL  },
   // 13,14,15
   { d_krono_radio_proc,3,   143,    50 ,  10,   0,   -1,     0,    0,       3,                     0,    "Facil  ",         NULL, NULL  },
   { d_krono_radio_proc,109, 143,    50 ,  10,   0,   -1,     0,    0,       3,                     0,    "Medio  ",         NULL, NULL  },
   { d_krono_radio_proc, 215,143,    50 ,  10,   0,   -1,     0,    0,       3,                     0,    "Normal ",         NULL, NULL  },


   // boton de OK [47x20]
   { d_krono_aceptar_button_proc,136,200,  47 ,  20,   1,           6,     0,D_EXIT,      0,        0,    "Aceptar",         NULL, NULL  },

   { NULL,               0,    0,    0,    0,    0,    0,     0,    0,       0,                     0,    NULL,              NULL, NULL  }
};

    // esperar hasta que suelte el mouse
    // para poder entrar en el bucle principal
    do { } while (mouse_b);
    // limpia el teclado
    clear_keybuf();

    // seteo la paleta
    set_palette(krono_dato[Menu_Paleta].dat);

    text_mode(-1); // texto transparente
    font = krono_dato[FontXM6x6].dat; // font peque¤o


    // mostrar mouse
    set_mouse_sprite(krono_dato[KMousePuntero].dat);
    show_mouse(screen);


// --------------------------------------------------------
    // setear las opciones del dialogo segun los seteos actuales
    // nivel de detalle
    if (MODO_DE_LUZ == 2)  cuadro_dialogo_1[2].flags |= D_SELECTED;
    if (MODO_DE_LUZ == 3)  cuadro_dialogo_1[3].flags |= D_SELECTED;
    if (MODO_DE_LUZ == 0)  cuadro_dialogo_1[4].flags |= D_SELECTED;

    // resolucion
    if ((H_RES==320) && (V_RES==200))  cuadro_dialogo_1[6].flags |= D_SELECTED;
    if ((H_RES==320) && (V_RES==240))  cuadro_dialogo_1[7].flags |= D_SELECTED;
    if ((H_RES==320) && (V_RES==480))  cuadro_dialogo_1[8].flags |= D_SELECTED;
    if ((H_RES==360) && (V_RES==480))  cuadro_dialogo_1[9].flags |= D_SELECTED;
    if ((H_RES==640) && (V_RES==480))  cuadro_dialogo_1[10].flags |= D_SELECTED;
    if ((H_RES==800) && (V_RES==600))  cuadro_dialogo_1[11].flags |= D_SELECTED;



    // nivel de dificultad
    if (NIVEL_IA == 0)  cuadro_dialogo_1[13].flags |= D_SELECTED;
    if (NIVEL_IA == 1)  cuadro_dialogo_1[14].flags |= D_SELECTED;
    if (NIVEL_IA == 2)  cuadro_dialogo_1[15].flags |= D_SELECTED;

// --------------------------------------------------------
    // activar el dialogo
    ret = do_dialog(cuadro_dialogo_1, -1);
// --------------------------------------------------------

    //ocultar mouse
    set_mouse_sprite(NULL);
    show_mouse(NULL);


// --------------------------------------------------------
    // ver que valores quedaron en los check boxs...

    // nivel de detalle grafico
    if (cuadro_dialogo_1[2].flags & D_SELECTED)
       {
            // mostrar efectos de iluminacion? (afecta a la luz ambiental tambien)
            VER_LUCES = FALSE;
            // mostrar efectos de particulas?
            VER_PARTI = FALSE;
            // mostrar explosiones?
            VER_EXPLO = TRUE;
            // mostrar clima?
            VER_CLIMA = FALSE;

            MODO_DE_LUZ = 2; // render pedorro!
      }
    if (cuadro_dialogo_1[3].flags & D_SELECTED)
       {
            VER_LUCES = TRUE;
            VER_PARTI = TRUE;
            VER_EXPLO = TRUE;
            VER_CLIMA = TRUE;
            // metodo de render de las luces
             MODO_DE_LUZ = 3; // render rapido
      }
    if (cuadro_dialogo_1[4].flags & D_SELECTED)
       {
            VER_LUCES = TRUE;
            VER_PARTI = TRUE;
            VER_EXPLO = TRUE;
            VER_CLIMA = TRUE;

            MODO_DE_LUZ = 0; // render suavizado
      }
    // fin detalle grafico

    // resoluciones de video
             MODO_VIDEO = GFX_AUTODETECT;

    if (cuadro_dialogo_1[6].flags & D_SELECTED)
       {
             H_RES = 320;
             V_RES = 200;
       }

    if (cuadro_dialogo_1[7].flags & D_SELECTED)
       {
             H_RES = 320;
             V_RES = 240;
       }
    if (cuadro_dialogo_1[8].flags & D_SELECTED)
       {
             H_RES = 320;
             V_RES = 480;
       }
    if (cuadro_dialogo_1[9].flags & D_SELECTED)
       {
             H_RES = 360;
             V_RES = 480;
       }
    if (cuadro_dialogo_1[10].flags & D_SELECTED)
       {
             H_RES = 640;
             V_RES = 480;
       }
    if (cuadro_dialogo_1[11].flags & D_SELECTED)
       {
             H_RES = 800;
             V_RES = 600;
       }
    // fin de resoluciones

    // nivel de dificultad
    if (cuadro_dialogo_1[13].flags & D_SELECTED) NIVEL_IA = 0;
    if (cuadro_dialogo_1[14].flags & D_SELECTED) NIVEL_IA = 1;
    if (cuadro_dialogo_1[15].flags & D_SELECTED) NIVEL_IA = 2;
    // fin nivel de dificultad


    // esperar hasta que suelte el mouse
    do { } while (mouse_b);
    // limpia el teclado
    clear_keybuf();

// --------------------------------------------------------
}
#endif
