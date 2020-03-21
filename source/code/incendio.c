//--------------------------------------------------------
// incendio.c
// incendio.h
//
// RUTINAS QUE CONTIENEN EL ADMINISTRADOR DE INCENDIO
// DEBUG: ANDAN MAL!!
// Por Kronoman - DJGPP
//--------------------------------------------------------

#ifndef INCENDIO_C
#define INCENDIO_C

//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE INCENDIOS

// incendio libre en cada estructura (si es -1, no hay incendios disponibles,
// sino, apunta al incendio libre que haya...)
// Este metodo es mas pedorro pero mas seguro que una lista dinamica
static int ince_libre;

static struct {
 int   x;  // posicion x 
 int   y;  // posicion y 

 int vida; // vida del incendio, se descuenta con cada
           // llamada del timer, al llegar a 0 es liberada (muere)

 int vel; // velocidad en pixels * 10 (o sea, si es 100, en realidad es 10 pixeles...)
 int ang; // angulo 0..255
} ince[MAX_INCENDIO];  // 0..MAX_INCENDIO-1
                       
                       
                       
                       
                       
                       

//--------------------------------------------------------
// Codigo de las funciones
//--------------------------------------------------------

//--------------------------------------------------------
// reset_incendio()
// resetea la estructura de los incendios
// llamar cada vez que empieza un nuevo juego
//--------------------------------------------------------

void reset_incendio()
{
int i1; // para los fors

for (i1 = 0; i1 < MAX_INCENDIO ; i1++)
{
    ince[i1].x = 0;
    ince[i1].y = 0;
    ince[i1].vida = 0;
    ince[i1].vel = 0;
    ince[i1].ang = 0;
} // fin for i1

ince_libre = 0;

} // fin reset_incendio
//--------------------------------------------------------


//--------------------------------------------------------
// Agrega un incendio en el lugar deseado  x,y
// con 'vida', y 'vel'
// SOLO si hay un incendio libre...
//--------------------------------------------------------
void agregar_incendio(int x, int y, int vida, int vel)
{

int i1=0; // para alivianar la escritura... ;D

// ver si hay incendio disponible
if ( ince_libre < 0) return;
if ( ince_libre > (MAX_INCENDIO - 1) ) { ince_libre = -1; return; }

if (vel<1) vel = 1;

i1 = ince_libre;

if (i1 < MAX_INCENDIO ) {
    ince[i1].x = x;
    ince[i1].y = y;
    ince[i1].vida = vida;
    ince[i1].vel = vel;
    ince[i1].ang = random()%2048; // 256*8, para dar prec. decimal con un int
    }

ince_libre = -1; // todavia no se cual es el prox. incendio libre...

} 
//--------------------------------------------------------


//--------------------------------------------------------
// Mueve los incendios 
//--------------------------------------------------------
void mover_incendio()
{
int i1; // para alivianar la escritura... ;D

// por ahora, no se que incendio esta libre
ince_libre = -1;

// recorrer del 0 a MAX_INCENDIO - 1
for (i1=0; i1 < MAX_INCENDIO ; i1++)
    {

    // si el incendio esta vivo
  if ( ince[i1].vida > 0 ) {

    // restar vida - 1
        ince[i1].vida--;

    // si el incendio murio, marcar como libre
    if ( ince[i1].vida < 1 )
        {
            ince_libre = i1; // este incendio esta libre!
            if (ince_libre > (MAX_INCENDIO - 1) )  ince_libre = -1;
        }
        else // si el incendio esta vivo, agregar luz donde esta el incendio
        {
                agregar_luz(ince[i1].x,
                            ince[i1].y,
                            random()%5 + 5, 
                            random()%50 + 205,
                            random()%20 + 20,
                            -(random()%3) );
        }

  } // fin ver si esta vivo
  else  // sino esta vivo
  {
    // incendio muerto, libre
    ince_libre = i1;
    if (ince_libre > (MAX_INCENDIO - 1) )  ince_libre = -1;
  }

} // fin for i1

} // fin mover_disp
//--------------------------------------------------------


//--------------------------------------------------------
// Esta rutina dibuja los incendios, transportados
// a las coordenadas x,y en el bitmap
//--------------------------------------------------------
void dibujar_incendios(BITMAP *bmp, int x, int y)
{
int i1, x1, y1; //para los for , etc
BITMAP *tmp; // para modificar el grafico

tmp = create_bitmap(X_SPR_INCE,Y_SPR_INCE);  // bmp temporal

// ya que estamos, ubico un incendio libre
ince_libre = -1;

for (i1 = 0; i1 < MAX_INCENDIO; i1++) {

    if (ince[i1].vida > 0) {

       //  dibujar! [doble pasada con filtro!] 
       clear(tmp);
                    rotate_sprite(tmp,
                                  krono_dato[INCENDIO_SPR].dat,
                                  0,
                                  0,
                                  itofix(ince[i1].ang / 8) );

      if ( ++ince[i1].ang > 2048) ince[i1].ang = 0;

      // filtro de mezcla...
//      for (x1=0; x1<tmp->w; x1++)
//        for (y1=0; y1<tmp->h; y1++)
//            if (random()%100<50) putpixel(tmp,
//                                          x1,
//                                          y1,
//                                          0);  

      // poner con transparencia...
            color_map = &trans_table;
            draw_trans_sprite(bmp,
                              tmp, 
                              ince[i1].x - x - (X_SPR_INCE/2) + al_azar(-3, 3),
                              ince[i1].y - y - (Y_SPR_INCE/2) + al_azar(-3, 3));

    } else  { ince_libre = i1; } // incendio libre

} // fin for i1

destroy_bitmap(tmp); // liberar memoria
}

#endif
