//--------------------------------------------------------
// explos.c
// explos.h
// Rutinas de efectos de FUEGO/EXPLOSIONES
//
// NOTAR QUE SI VER_EXPLO == FALSE, NO SE VEN LAS EXPLOSIONES/FUEGO
//--------------------------------------------------------

#ifndef EXPLOS_C
#define EXPLOS_C

//--------------------------------------------------------
// GLOBALES
// Globales pertenecientes solo a este modulo (por eso 'static')
//--------------------------------------------------------

// SISTEMA DE EXPLOSIONES
static int cantidad_expl = 0; // cantidad de explosiones

static struct {
 int   x;  // posicion x 
 int   y;  // posicion y 
 int radio; // radio *inicial* de la explosion
            // se achica al ir muriendo...

 int vida; // vida de la explosion, se descuenta con cada
           // llamada del timer, al llegar a 0 es liberada (muere)

 int v1; // esto se setea v1 = vida cuando se crea una explo
         // para calcular que tama¤o tiene el radio luego
         // a medida que muere, en relacion a que tama¤o empezo

 int frame; // cuadro de la imagen de la explosion 1,2,3,4
            // ya que esta formada por un sprite animado escalable...

 int vel; // velocidad de combustion de 1 al radio-1
} explo[MAX_EXPLOSION]; // desde 0 a MAX_EXPLOSION - 1
//--------------------------------------------------------



//--------------------------------------------------------
// reset_explo()
// esto 'resetea' el sistema de explosiones
// llamar cada vez que termina el juego
// de lo contrario habra explosiones al empezar a jugar de nuevo
//--------------------------------------------------------
void reset_explo()
{
int i1; // para el for

cantidad_expl = 0; 
for (i1=0; i1 < MAX_EXPLOSION; i1++)
    {
     explo[i1].x=0;
     explo[i1].y=0;
     explo[i1].radio=0;
     explo[i1].vida=0;
     explo[i1].v1=0;
     explo[i1].frame=0;
     explo[i1].vel=1;

    }
} // fin reset_explo()
//--------------------------------------------------------


//--------------------------------------------------------
//dibujar_explo
//
//Rutina que dibuja las explosiones ajustada a
// la ventana (x, y) dentro del bmp->w, bmp->h
//Pasarle el bitmap donde se desea que lo dibuje
//--------------------------------------------------------
void dibujar_explo(BITMAP *bmp, int x, int y)
{
int i1 = 0; // para el for
int r1 = 0 ; // para calcular el radio
int spr1 = 0; // para el frame

if (VER_EXPLO==FALSE) return; // si esta off, abortar

for (i1=0; i1 < cantidad_expl; i1++)
    {

// ver si la explosion esta viva y en posicion de ser dibujada
if ((explo[i1].x - x + explo[i1].radio > 0) &&
    (explo[i1].y - y + explo[i1].radio > 0) &&
    (explo[i1].x - x - explo[i1].radio < bmp->w) &&
    (explo[i1].y - y - explo[i1].radio < bmp->h) &&
    (explo[i1].vida > 0)) {
        // esta viva, dibujar!

// calcular el radio que tiene la explosion con
// la vida actual usando regla de 3

if (explo[i1].v1<1) explo[i1].v1=1; // evita div por 0

r1 = (explo[i1].vida * explo[i1].radio) / explo[i1].v1;
if (r1<1) r1=1;

        // elegir el cuadro adecuado
        if (explo[i1].frame<=1) spr1=EXPLO1;
        if (explo[i1].frame==2) spr1=EXPLO2;
        if (explo[i1].frame==3) spr1=EXPLO3;
        if (explo[i1].frame>=4) spr1=EXPLO4;

        // dibuja escalado
        stretch_sprite(bmp,
                       krono_dato[spr1].dat,
                       explo[i1].x - x- r1/2,
                       explo[i1].y - y - r1/2,
                       r1, r1);
    } // fin ver si esta dentro
} // fin for i1

} // fin dibujar_explo
//--------------------------------------------------------


//--------------------------------------------------------
// mover_explo()
//
// Administra, mueve, etc las explosiones
//--------------------------------------------------------
void mover_explo()
{
int i1, i2; // para el for

if (VER_EXPLO==FALSE) return; // si esta off, abortar

i2 = cantidad_expl;

for (i1=0; i1 < cantidad_expl; i1++)
    {
     explo[i1].frame++;
     if (explo[i1].frame>4) explo[i1].frame=1;

     explo[i1].vida -= explo[i1].vel;
        if (explo[i1].vida < 1)
            {
            // explosion muerta, cambiar con la ultima viva el lugar
            // y eliminar
            if (i2-1>0) explo[i1] = explo[i2-1];
            i2--;
            if (i2<0) i2=0;
            }
    } // fin for i1


if (i2<0) i2=0;
cantidad_expl = i2;

} // fin mover_explo()
//--------------------------------------------------------


//--------------------------------------------------------
// agregar_expl(x,y,vida,radio,vel);
// Agrega explosiones en el lugar deseado  x,y
// con 'vida', y 'radio' y combustion, 'vel'
//--------------------------------------------------------
void agregar_expl(int x, int y, int vida, int radio, int vel)
{

if (VER_EXPLO==FALSE) return; // si esta off, abortar

// seguridad
if (cantidad_expl + 1 >MAX_EXPLOSION-1) return;

if (vel<1) vel = 1;
if (vel>radio) vel = radio;

// comprobacion de seguridad
if (cantidad_expl>MAX_EXPLOSION-1) cantidad_expl=MAX_EXPLOSION-1;
if (cantidad_expl<0) cantidad_expl=0;

     explo[cantidad_expl].x = x;
     explo[cantidad_expl].y = y;
     explo[cantidad_expl].radio = radio;
     explo[cantidad_expl].vida = vida;
     explo[cantidad_expl].v1 = vida;
     explo[cantidad_expl].frame = random() % 4 + 1;
     explo[cantidad_expl].vel = vel;

// agregar
cantidad_expl++;

// comprobacion de seguridad
if (cantidad_expl>MAX_EXPLOSION-1) cantidad_expl=MAX_EXPLOSION-1;

} // fin agregar_expl
//--------------------------------------------------------

//--------------------------------------------------------
// combo_expl(x,y,vida,radio,vel);
//
// Casi igual a agregar_expl(x,y,vida,radio,vel);
// PERO AGREGA UNA LUZ Y PARTICULAS AMARILLAS
// PARA SIMULAR UNA EXPLOSION CONVINCENTE
//
//--------------------------------------------------------
void combo_expl(int x, int y, int vida, int radio, int vel)
{


// agrega la explosion

agregar_expl(x, y, vida, radio, vel);

// notar que divido vida / vel para el resto
// para que todo se acabe junto

// agrega las particulas
agregar_particulas(x,y, vida / 2 / vel, 30, 5, 241,242);


// agrega la luz
agregar_luz(x, y, vida / vel, 255, radio + (radio * 0.3) , -vel*1.4);


} // fin combo_expl
//--------------------------------------------------------




#endif
