//--------------------------------------------------------
// k_gui.c
// Rutinas de apoyo GUI para el juego
//--------------------------------------------------------
#ifndef K_GUI_C
#define K_GUI_C
//--------------------------------------------------------
// Rutinas para el tiempo de juego [usan la paleta del juego]
//--------------------------------------------------------

// DEBUG - ninguna hecha!


//--------------------------------------------------------
// Rutinas para menues.c [usan la paleta de menu generico]
//--------------------------------------------------------


//--------------------------------------------------------
// funcion que pone el fondo generico del menu
//--------------------------------------------------------
int d_poner_fondo_proc(int msg, DIALOG *d, int c)
{
    // cargo la imagen de fondo
   if (msg == MSG_DRAW)
      {
      blit(krono_dato[MNU_FONDO_GENERICO].dat, screen, 0, 0, 0, 0,320,240);
      }
   return D_O_K;
}

//--------------------------------------------------------
// funcion que hace los radio buttons graficos... ;^D
// para el menu de opciones
//--------------------------------------------------------
int d_krono_radio_proc(int msg, DIALOG *d, int c)
{
    int fg;
    if (msg == MSG_DRAW) // dibujar
       {
         fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;

         // ---------
         // DEBUG
         if (d->flags & D_GOTFOCUS) fg = 1; // cambia el color del texto si tiene foco (1=blanco)
         // ---------

         text_mode(-1); // transparente
         gui_textout(screen,
                     d->dp,
                     d->x + d->h + text_height(font),
                     d->y + (d->h - (text_height(font) - gui_font_baseline))/2,
                     fg,
                     FALSE);


         // poner la imagen
         if (d->flags & D_SELECTED)
            {
            // esta elegido
                draw_sprite(screen,
                            krono_dato[GUI_OPCION_BTN_DOWN].dat,
                            d->x,
                            d->y ); 
            }
            else
            {
            // esta libre
                draw_sprite(screen,
                            krono_dato[GUI_OPCION_BTN_UP].dat,
                            d->x,
                            d->y ); 
            }

         // listo, todo ok!
         return D_O_K;
       }

    // sobrecargar el radio button normal
      return d_radio_proc(msg, d, c);
} // fin radio button

//--------------------------------------------------------
// Boton de aceptar - sobrecarga el boton normal [lo dibuja cool!]
//--------------------------------------------------------
int d_krono_aceptar_button_proc(int msg, DIALOG *d, int c)
{
    if (msg == MSG_DRAW) // dibujar
    {
     if ( (d->flags & D_GOTFOCUS) || (d->flags & D_SELECTED) )
        {
         blit(krono_dato[GUI_BOTON_ACEPTAR_DOWN].dat,
              screen,
              0,
              0,
              d->x,
              d->y,
              d->w,
              d->h);
        }
     else
        {
         blit(krono_dato[GUI_BOTON_ACEPTAR_UP].dat,
              screen,
              0,
              0,
              d->x,
              d->y,
              d->w,
              d->h);
        }
     return D_O_K;
    }

    return d_button_proc(msg, d, c);
} // fin boton aceptar
#endif
